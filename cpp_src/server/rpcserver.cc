#include "rpcserver.h"
#include <sys/stat.h>
#include <sstream>
#include "net/cproto/cproto.h"
#include "net/cproto/serverconnection.h"
#include "net/listener.h"
#include "reindexer_version.h"

namespace reindexer_server {

RPCServer::RPCServer(DBManager &dbMgr, LoggerWrapper logger, bool allocDebug)
	: dbMgr_(dbMgr), logger_(logger), allocDebug_(allocDebug), startTs_(std::chrono::system_clock::now()) {}

RPCServer::~RPCServer() {}

Error RPCServer::Ping(cproto::Context &) {
	//
	return 0;
}

static std::atomic<int> connCounter;

Error RPCServer::Login(cproto::Context &ctx, p_string login, p_string password, p_string db) {
	if (ctx.GetClientData()) {
		return Error(errParams, "Already logged in");
	}

	auto clientData = std::make_shared<RPCClientData>();

	clientData->connID = connCounter.load();
	connCounter++;

	clientData->auth = AuthContext(login.toString(), password.toString());

	auto status = dbMgr_.Login(db.toString(), clientData->auth);
	if (!status.ok()) {
		return status;
	}
	ctx.SetClientData(clientData);
	int64_t startTs = std::chrono::duration_cast<std::chrono::seconds>(startTs_.time_since_epoch()).count();

	ctx.Return({cproto::Arg(p_string(REINDEX_VERSION)), cproto::Arg(startTs)});

	return db.length() ? OpenDatabase(ctx, db) : 0;
}

Error RPCServer::OpenDatabase(cproto::Context &ctx, p_string db) {
	auto clientData = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());
	if (clientData->auth.HaveDB()) {
		return Error(errParams, "Database already opened");
	}
	return dbMgr_.OpenDatabase(db.toString(), clientData->auth, true);
}

Error RPCServer::CloseDatabase(cproto::Context &ctx) {
	auto clientData = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());
	clientData->auth.ResetDB();
	return 0;
}
Error RPCServer::DropDatabase(cproto::Context &ctx) {
	auto clientData = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());
	return dbMgr_.DropDatabase(clientData->auth);
}

Error RPCServer::CheckAuth(cproto::Context &ctx) {
	auto clientData = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());

	if (ctx.call->cmd == cproto::kCmdLogin || ctx.call->cmd == cproto::kCmdPing) {
		return 0;
	}

	if (!clientData) {
		return Error(errForbidden, "You should login");
	}

	return 0;
}

void RPCServer::OnClose(cproto::Context &ctx, const Error &err) {
	(void)ctx;
	(void)err;

	logger_.info("RPC: Client disconnected");
}

void RPCServer::Logger(cproto::Context &ctx, const Error &err, const cproto::Args &ret) {
	auto clientData = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());
	WrSerializer ser;

	if (clientData) {
		ser << "c='" << clientData->connID << "' db='" << clientData->auth.Login() << "@" << clientData->auth.DBName() << "' ";
	} else {
		ser << "- - ";
	}

	if (ctx.call) {
		ser << cproto::CmdName(ctx.call->cmd) << " ";
		ctx.call->args.Dump(ser);
	} else {
		ser << '-';
	}

	ser << " -> " << (err.ok() ? "OK" : err.what());
	if (ret.size()) {
		ser << ' ';
		ret.Dump(ser);
	}

	if (allocDebug_) {
		Stat statDiff = Stat() - ctx.stat;

		ser << " | elapsed: " << statDiff.GetTimeElapsed() << "us, allocs: " << statDiff.GetAllocsCnt()
			<< ", allocated: " << statDiff.GetAllocsBytes() << " byte(s)";
	}

	logger_.info("{0}", ser.Slice());
}

Error RPCServer::OpenNamespace(cproto::Context &ctx, p_string nsDefJson) {
	NamespaceDef nsDef;

	string json = nsDefJson.toString();

	nsDef.FromJSON(const_cast<char *>(json.c_str()));
	if (!nsDef.indexes.empty()) {
		return getDB(ctx, kRoleDataRead)->AddNamespace(nsDef);
	}
	return getDB(ctx, kRoleDataRead)->OpenNamespace(nsDef.name, nsDef.storage, nsDef.cacheMode);
}

Error RPCServer::DropNamespace(cproto::Context &ctx, p_string ns) {
	//
	return getDB(ctx, kRoleDBAdmin)->DropNamespace(ns.toString());
}

Error RPCServer::CloseNamespace(cproto::Context &ctx, p_string ns) {
	// Do not close.
	// TODO: add reference counters
	// return getDB(ctx, kRoleDataRead)->CloseNamespace(ns.toString());
	return getDB(ctx, kRoleDataRead)->Commit(ns.toString());
}

Error RPCServer::EnumNamespaces(cproto::Context &ctx) {
	vector<NamespaceDef> nsDefs;
	auto err = getDB(ctx, kRoleDataRead)->EnumNamespaces(nsDefs, true);
	if (!err.ok()) {
		return err;
	}
	WrSerializer ser;
	ser << "{\"items\":[";
	for (unsigned i = 0; i < nsDefs.size(); i++) {
		if (i != 0) ser << ',';
		nsDefs[i].GetJSON(ser);
	}
	ser << "]}";
	auto resSlice = ser.Slice();

	ctx.Return({cproto::Arg(p_string(&resSlice))});
	return 0;
}

Error RPCServer::AddIndex(cproto::Context &ctx, p_string ns, p_string indexDef) {
	IndexDef iDef;
	auto err = iDef.FromJSON(const_cast<char *>(indexDef.toString().c_str()));
	if (!err.ok()) {
		return err;
	}
	return getDB(ctx, kRoleDBAdmin)->AddIndex(ns.toString(), iDef);
}

Error RPCServer::UpdateIndex(cproto::Context &ctx, p_string ns, p_string indexDef) {
	IndexDef iDef;
	auto err = iDef.FromJSON(const_cast<char *>(indexDef.toString().c_str()));
	if (!err.ok()) {
		return err;
	}
	return getDB(ctx, kRoleDBAdmin)->UpdateIndex(ns.toString(), iDef);
}

Error RPCServer::DropIndex(cproto::Context &ctx, p_string ns, p_string index) {
	return getDB(ctx, kRoleDBAdmin)->DropIndex(ns.toString(), index.toString());
}

Error RPCServer::ModifyItem(cproto::Context &ctx, p_string nsName, int format, p_string itemData, int mode, p_string perceptsPack,
							int stateToken, int /*txID*/) {
	auto db = getDB(ctx, kRoleDataWrite);
	string ns = nsName.toString();
	auto item = Item(db->NewItem(ns));
	bool tmUpdated = false;
	Error err;
	if (!item.Status().ok()) {
		return item.Status();
	}

	switch (format) {
		case FormatJson:
			err = item.Unsafe().FromJSON(itemData, nullptr, mode == ModeDelete);
			break;
		case FormatCJson:
			if (item.GetStateToken() != stateToken) {
				err = Error(errStateInvalidated, "stateToken mismatch:  %08X, need %08X. Can't process item", stateToken,
							item.GetStateToken());
			} else {
				err = item.Unsafe().FromCJSON(itemData, mode == ModeDelete);
			}
			break;
		default:
			err = Error(-1, "Invalid source item format %d", format);
	}
	if (!err.ok()) {
		return err;
	}
	tmUpdated = item.IsTagsUpdated();

	if (perceptsPack.length()) {
		Serializer ser(perceptsPack);
		unsigned preceptsCount = ser.GetVarUint();
		vector<string> precepts;
		for (unsigned prIndex = 0; prIndex < preceptsCount; prIndex++) {
			string precept = ser.GetVString().ToString();
			precepts.push_back(precept);
		}
		item.SetPrecepts(precepts);
	}
	switch (mode) {
		case ModeUpsert:
			err = db->Upsert(ns, item);
			break;
		case ModeInsert:
			err = db->Insert(ns, item);
			break;
		case ModeUpdate:
			err = db->Update(ns, item);
			break;
		case ModeDelete:
			err = db->Delete(ns, item);
			break;
	}
	if (!err.ok()) {
		return err;
	}
	QueryResults qres;
	qres.AddItem(item);
	int32_t ptVers = -1;
	ResultFetchOpts opts;
	if (tmUpdated) {
		opts = ResultFetchOpts{kResultsWithItemID | kResultsWithPayloadTypes, span<int32_t>(&ptVers, 1), 0, INT_MAX};
	} else {
		opts = ResultFetchOpts{kResultsWithItemID, {}, 0, INT_MAX};
	}

	return sendResults(ctx, qres, -1, opts);
}

Error RPCServer::DeleteQuery(cproto::Context &ctx, p_string queryBin) {
	Query query;
	Serializer ser(queryBin.data(), queryBin.size());
	query.Deserialize(ser);

	QueryResults qres;
	auto err = getDB(ctx, kRoleDataWrite)->Delete(query, qres);
	if (!err.ok()) {
		return err;
	}
	ResultFetchOpts opts{0, {}, 0, INT_MAX};
	return sendResults(ctx, qres, -1, opts);
}

shared_ptr<Reindexer> RPCServer::getDB(cproto::Context &ctx, UserRole role) {
	auto clientData = ctx.GetClientData().get();
	if (clientData) {
		shared_ptr<Reindexer> db;
		auto status = dynamic_cast<RPCClientData *>(clientData)->auth.GetDB(role, &db);
		if (!status.ok()) {
			throw status;
		}
		if (db != nullptr) return db;
	}
	throw Error(errParams, "Database is not openeded, you should open it first");
}

Error RPCServer::sendResults(cproto::Context &ctx, QueryResults &qres, int reqId, const ResultFetchOpts &opts) {
	WrResultSerializer rser(opts);

	bool doClose = rser.PutResults(&qres);

	if (doClose && reqId >= 0) {
		freeQueryResults(ctx, reqId);
		reqId = -1;
	}

	string_view resSlice = rser.Slice();
	ctx.Return({cproto::Arg(p_string(&resSlice)), cproto::Arg(int(reqId))});
	return 0;
}

QueryResults &RPCServer::getQueryResults(cproto::Context &ctx, int &id) {
	auto data = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());

	if (id < 0) {
		for (id = 0; id < int(data->results.size()); id++) {
			if (!data->results[id].second) {
				data->results[id] = {QueryResults(), true};
				return data->results[id].first;
			}
		}

		if (data->results.size() > cproto::kMaxConcurentQueries) throw Error(errLogic, "Too many paralell queries");
		id = data->results.size();
		data->results.push_back({QueryResults(), true});
	}

	if (id >= int(data->results.size())) {
		throw Error(errLogic, "Invalid query id");
	}
	return data->results[id].first;
}

void RPCServer::freeQueryResults(cproto::Context &ctx, int id) {
	auto data = dynamic_cast<RPCClientData *>(ctx.GetClientData().get());
	if (id >= int(data->results.size()) || id < 0) {
		throw Error(errLogic, "Invalid query id");
	}
	data->results[id] = {QueryResults(), false};
}

static h_vector<int32_t, 4> pack2vec(p_string pack) {
	// Get array of payload Type Versions
	Serializer ser(pack.data(), pack.size());
	h_vector<int32_t, 4> vec;
	int cnt = ser.GetVarUint();
	for (int i = 0; i < cnt; i++) vec.push_back(ser.GetVarUint());
	return vec;
}

Error RPCServer::Select(cproto::Context &ctx, p_string queryBin, int flags, int limit, p_string ptVersionsPck) {
	Query query;
	Serializer ser(queryBin);
	query.Deserialize(ser);

	int id = -1;
	QueryResults &qres = getQueryResults(ctx, id);

	auto ret = getDB(ctx, kRoleDataRead)->Select(query, qres);
	if (!ret.ok()) {
		freeQueryResults(ctx, id);
		return ret;
	}
	auto ptVersions = pack2vec(ptVersionsPck);
	ResultFetchOpts opts{flags, ptVersions, 0, unsigned(limit)};

	return fetchResults(ctx, id, opts);
}

Error RPCServer::SelectSQL(cproto::Context &ctx, p_string querySql, int flags, int limit, p_string ptVersionsPck) {
	int id = -1;
	QueryResults &qres = getQueryResults(ctx, id);
	auto ret = getDB(ctx, kRoleDataRead)->Select(querySql, qres);
	if (!ret.ok()) {
		freeQueryResults(ctx, id);
		return ret;
	}
	auto ptVersions = pack2vec(ptVersionsPck);
	ResultFetchOpts opts{flags, ptVersions, 0, unsigned(limit)};

	return fetchResults(ctx, id, opts);
}

Error RPCServer::FetchResults(cproto::Context &ctx, int reqId, int flags, int offset, int limit) {
	flags &= ~kResultsWithPayloadTypes;

	ResultFetchOpts opts = {flags, {}, unsigned(offset), unsigned(limit)};
	return fetchResults(ctx, reqId, opts);
}

Error RPCServer::CloseResults(cproto::Context &ctx, int reqId) {
	freeQueryResults(ctx, reqId);
	return errOK;
}

Error RPCServer::fetchResults(cproto::Context &ctx, int reqId, const ResultFetchOpts &opts) {
	cproto::Args ret;
	QueryResults &qres = getQueryResults(ctx, reqId);

	return sendResults(ctx, qres, reqId, opts);
}

Error RPCServer::Commit(cproto::Context &ctx, p_string ns) {
	//
	return getDB(ctx, kRoleDataWrite)->Commit(ns.toString());
}

Error RPCServer::GetMeta(cproto::Context &ctx, p_string ns, p_string key) {
	string data;
	auto err = getDB(ctx, kRoleDataRead)->GetMeta(ns.toString(), key.toString(), data);
	if (!err.ok()) {
		return err;
	}

	ctx.Return({cproto::Arg(data)});
	return 0;
}

Error RPCServer::PutMeta(cproto::Context &ctx, p_string ns, p_string key, p_string data) {
	return getDB(ctx, kRoleDataWrite)->PutMeta(ns.toString(), key.toString(), data);
}

Error RPCServer::EnumMeta(cproto::Context &ctx, p_string ns) {
	vector<string> keys;
	auto err = getDB(ctx, kRoleDataWrite)->EnumMeta(ns.toString(), keys);
	if (!err.ok()) {
		return err;
	}
	cproto::Args ret;
	for (auto &key : keys) {
		ret.push_back(cproto::Arg(key));
	}
	ctx.Return(ret);
	return errOK;
}

Error RPCServer::SubscribeUpdates(cproto::Context &ctx, int flag) {
	//
	return getDB(ctx, kRoleDataRead)->SubscribeUpdates(this, flag);
}

bool RPCServer::Start(const string &addr, ev::dynamic_loop &loop) {
	dispatcher.Register(cproto::kCmdPing, this, &RPCServer::Ping);
	dispatcher.Register(cproto::kCmdLogin, this, &RPCServer::Login);
	dispatcher.Register(cproto::kCmdOpenDatabase, this, &RPCServer::OpenDatabase);
	dispatcher.Register(cproto::kCmdCloseDatabase, this, &RPCServer::CloseDatabase);
	dispatcher.Register(cproto::kCmdDropDatabase, this, &RPCServer::DropDatabase);
	dispatcher.Register(cproto::kCmdOpenNamespace, this, &RPCServer::OpenNamespace);
	dispatcher.Register(cproto::kCmdDropNamespace, this, &RPCServer::DropNamespace);
	dispatcher.Register(cproto::kCmdCloseNamespace, this, &RPCServer::CloseNamespace);
	dispatcher.Register(cproto::kCmdEnumNamespaces, this, &RPCServer::EnumNamespaces);

	dispatcher.Register(cproto::kCmdAddIndex, this, &RPCServer::AddIndex);
	dispatcher.Register(cproto::kCmdUpdateIndex, this, &RPCServer::UpdateIndex);
	dispatcher.Register(cproto::kCmdDropIndex, this, &RPCServer::DropIndex);
	dispatcher.Register(cproto::kCmdCommit, this, &RPCServer::Commit);

	dispatcher.Register(cproto::kCmdModifyItem, this, &RPCServer::ModifyItem);
	dispatcher.Register(cproto::kCmdDeleteQuery, this, &RPCServer::DeleteQuery);

	dispatcher.Register(cproto::kCmdSelect, this, &RPCServer::Select);
	dispatcher.Register(cproto::kCmdSelectSQL, this, &RPCServer::SelectSQL);
	dispatcher.Register(cproto::kCmdFetchResults, this, &RPCServer::FetchResults);
	dispatcher.Register(cproto::kCmdCloseResults, this, &RPCServer::CloseResults);

	dispatcher.Register(cproto::kCmdGetMeta, this, &RPCServer::GetMeta);
	dispatcher.Register(cproto::kCmdPutMeta, this, &RPCServer::PutMeta);
	dispatcher.Register(cproto::kCmdEnumMeta, this, &RPCServer::EnumMeta);
	dispatcher.Register(cproto::kCmdSubscribeUpdates, this, &RPCServer::SubscribeUpdates);
	dispatcher.Middleware(this, &RPCServer::CheckAuth);
	dispatcher.OnClose(this, &RPCServer::OnClose);

	if (logger_) {
		dispatcher.Logger(this, &RPCServer::Logger);
	}

	listener_.reset(new Listener(loop, cproto::ServerConnection::NewFactory(dispatcher)));
	return listener_->Bind(addr);
}

Error RPCServer::OnModifyItem(const string &nsName, ItemImpl *item, int modifyMode) {
	(void)nsName;
	(void)item;
	(void)modifyMode;

	return errOK;
}

Error RPCServer::OnNewNamespace(const string &nsName) {
	(void)nsName;
	return errOK;
}
Error RPCServer::OnModifyIndex(const string &nsName, const IndexDef &idx, int modifyMode) {
	(void)nsName;
	(void)idx;
	(void)modifyMode;
	return errOK;
}

Error RPCServer::OnDropIndex(const string &nsName, const string &indexName) {
	(void)nsName;
	(void)indexName;
	return errOK;
}

Error RPCServer::OnDropNamespace(const string &nsName) {
	(void)nsName;
	return errOK;
}
Error RPCServer::OnPutMeta(const string &nsName, const string &key, const string &data) {
	(void)nsName;
	(void)key;
	(void)data;
	return errOK;
}

}  // namespace reindexer_server
