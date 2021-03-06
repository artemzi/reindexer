# Version 1.10.3 (02.12.2018)

## Core 

- [fix] Potential crash on delete from store string index
- [fix] Slow cache cleanup
- [fix] Print precision of double values decreased to actual double range

## Reindexer tool
- [fix] Invalid http redirects, if compiled with -DLINK_RESOURCES

## Reindexer tool
- [fix] Unhandled exception in case trying of create output file in unexisting directory
- [fix] RPC client optimizations and races fixes
- [fea] \bench command added


# Version 1.10.2 (20.11.2018)

## Core

- [fea] Indexes rebuilding now is non blocking background task, concurrent R-W queries performance increased
- [fix] Fulltext index incremental rebuild memory grow fixed

## Reindexer server

- [fix] Logger buffer increased, and will not block on errors
- [fix] DELETE Query method fixed
- [fix] urldecode of database names fixed

## Reindexer tool

- [fix] Pretty print fixed

# Version 1.10.1 (06.11.2018)

## Core

- [fea] Incremental fulltext search index rebuild 
- [fea] Async C++ RPC client  
- [fix] Fixed incorrect behaviour with non indexed field conditions
- [fix] Extra non indexed fields tests added
- [fix] Json parser memleak fixed

## Reindexer server

- [fea] REST API documentation improoved
- [fea] Optimized performance

## Reindexer tool

- [fea] Operation speed is improoved


# Version 1.10.0 (29.10.2018)

## Core

- [fea] Added explain query feature
- [fea] Added min, max and facets aggregation functions
- [fea] Added EqualPosition query function
- [fea] In SQL query parser/printer added is NULL condition
- [fea] Native bool data type support
- [fea] Filter nested fields in SelectFilter statement
- [fix] Fix crash on comparator with Join queries
- [fix] Incorrect sort order using with Join queries in join cache optimization step 
- [ref] Composite indexes now using json_paths for list of fields
- [ref] Storage format has been changed, and incompatible with this version. To migrate date from previous version use [reindexer_tool](cpp_src/cmd/reindexer_tool/readme.md)

## Go connector

- [fea] reindexer.Status method added, to check connector status after initialization
- [fea] OpenNamespace now register namespace <-> struct mapping without server connection requiriment
- [fix] int type is now converted to int32/int64 depends on architecture

## Python connector

- [fea] [Python connector](connectors/py_reindexer) v.0.0.1 is released.

## Reindexer server

- [fea] Added fields filter to method GET /api/v1/:db/:namespace:/items mathed
- [fea] Added method DELETE /api/v1/:db/query
- [fea] Added poll loop backend (osx,bsd)
- [ref] `json_path` renamed to `json_paths`, and now array
- [ref] Binary cproto protocol has been optimized and changed. Please consider to update clients
- [fix] EnumMeta method has been fixed
- [fix] ModifyItem will retry attempt if tags <-> names state is invalidated

## Face

- [fea] Added EXPLAIN query stats view
- [fea] Added tags selector component for json_paths to index create/modify form

# Version 1.9.7 (28.09.2018)

## Core
- [fea] Storing index configuration in storage
- [fea] Concurent R-W queries performance optimization
- [fea] Added indexes runtime performance statistics
- [fix] Incorrect NOT behaviour on queries with only comparators
- [fix] Race condition on shutdown
- [fix] Type conversion on queries without indexes

## Go connector
- [fea] Added public AddIndex/DropIndex/UpdateIndex methods
- [ref] ConfigureIndex method deprecated
- [ref] `appendable` logic has been moved from C++ core to golang connector
- [fix] Multiple database support in `embeded` mode.

## Reindexer tool
- [fix] Fixed restoring namespaces with index names non equal to json paths

# Version 1.9.6 (03.09.2018)

## Core
- [fea] Merge with Join queries support
- [fea] Sort by multiple columns/indexes
- [fix] Case insensivity for index/namespaces names
- [fix] Sparse indexes behavior fixed
- [fix] Full text index - correct calculatuon of distance between words
- [fix] Race condition on concurent ConfigureIndex requests

## Reindexer server
- [fea] Added modify index method

## Go connector
- [fea] New builtinserver binding: builtin mode for go application + bundled server for external clients
- [fea] Improoved validation of go struct `reindex` tags

# Version 1.9.5 (04.08.2018)

## Core

- [fea] Sparse indexes
- [fix] Fixed errors on conditions to unindexed fields
- [fix] Fulltext terms relevancy, then query contains 2 terms included to single word
- [fea] Customizable symbols set of "words" symbols for fulltext
- [fix] Incorrect behavior on addition index with duplicated json path of another index

## Reindexer tool

- [fix] Fixed deadlock on linux
- [fix] Fixed \dump command namespace list parsing

## Reindexer server

- [fea] Added method /api/v1/check with system information about memory consumption, uptime and version
- [fea] Passing to RPC client information about version and uptime
- [fea] Optional embed web resources to server binary

## Face

- [fix] Incorrect urlencode for document update API url
- [fix] Namespace view layout updated, jsonPath added to table

## Go connector

- [fix] Fixed error after closing connection by timeout
- [fix] Caches invalidation on server restart

# Version 1.9.4 (25.07.2018)

## Core

- [fea] Conditions to any fields, even not indexed
- [fea] cproto network client added 
- [fix] Query execution plan optimizator fixes. 

## Reindexer tool

- [fea] Command line editor. tool has been mostly rewritten at all
- [fea] Interopertion with standalone server

## Reindexer server

- [fea] Pagination in the GET sqlquery HTTP method
- [fea] Filtration in the GET items HTTP method

## Face

- [fea] Table view of items page
- [fea] Filtration in the items page
- [fea] SQL queries history

# Version 1.9.3 (20.06.2018)

## Core

- [fea] Added system namespaces #memstats #profstats #queriesstats #namespaces with executuin and profiling statistics
- [fea] Added system namespace #config with runtime profiling configuration
- [fix] Join cache memory limitation
- [fix] Fixed bug with cjson parsing in nested objects on delete
- [fix] 32 bit reference counter for records instead of 16 bit
- [fix] Crash on load namespaces from storage with custom sort order
- [fix] Crash on select from composite index, after delete last shared string copy for sub part of composite index
- [fix] Full text build threads limited to 8

## Reindexer Server

- [fea] Load data in multiple threads on startup
- [fea] Auto rebalance connection between worker threads
- [fix] "Authorization" http header case insensitivity lookup
- [fix] Unexpected exit on SIGPIPE
- [fix] Namespaces names are now url decoded

## Go connector

- [fea] Ability to pass extra options to bindings
- [ref] cgo buffers cleanup optimization

# Version 1.9.2 (04.06.2018)

## Core
- [fea] SQL parser performance improved
- [fea] Caching of intermediate join results
- [fix] Assert on error cjson parser disabled: server will not crash on application errors
- [fix] Relaxed check of fields type on index addition and loading from storage
- [fix] Potential field data corruption on runtime index addition

## Reindexer server
- [fix] Correct grow of input buffer for http and cproto connections
- [ref] HTTP parser and router refactored.
- [fix] gprof handlers fixes
- [fix] packages dependencies fixed, installation errors on centos fixes, path to webroot fixed
- [ci] added sanity tests of packages installations

## Go connector
- [fea] Checking for duplicate names in `json` struct's tags on OpenNamespace
- [fea] Checking DeepCopy interface for correct return value on OpenNamespace
- [fix] Fixed error with sync payload types, on json queries
- [fix] Local imports of ./repo in benchmarks package broke gb

# Version 1.9.1 (17.05.2018)

## Reindexer server
- [fix] path to queries API

## Face
- [fix] error on create indexes
- [fix] show 'new database windows 'in case of start without any databases 

# Version 1.9.0 (16.05.2018)

## C++ core

- [fea] Search digits by words representation (now only for Russian language) in ft1
- [fix] Forced flush all batched writes to disk on exit
- [fix] Incorrect total count for queries without where clause
- [fea] custom collate now accepts sequence of letters
- [fix] Build issues with clang 3.9
- [fix] Cmake build dependencies, leveldb and snappy optionally added as ExternalProject
- [port] Windows and BSD build
- [port] 32 bit build

## Reindexer tool
- [fea] Ability to input/output dump from stdin/stdout

## Reindexer server
- [ref] Flags `create_if_missed` and `drop_on_format_error` are removed from namespaces API
- [fix] Correct behavior on logging different loggers to the same file
- [fix] CPU profiler error in case of 
- [port] Ability to run as Windows service and cmake Windows NSIS installer

## Go connector
- [fea] Removed cproto connector dependency on cgo
- [fea] cproto connector are compiled by default

# Version 1.8.1 (26.04.2018)

## C++ core

- [fix] Fix KeyValue::operator == for strings (IdSetCache isn't worked due this)
- [fix] Describe for composite indexes will return name alias
- [fix] Check for invalid PK indexes: Throw error on '-' (scan) PK
- [fix] Add missed comma in IndexDef JSON serializer 
- [fix] Relevancy in case when first term from query found in non boosted field first
- [opt] Optimize AND performance in ft1
- [fea] Add extra debug output to ft1


## Go connector
- [fix] Golang SQL query pseudo pre-parser accept all whitespace chars, not only ' ' 
- [fix] Correct composite PK handling in QueryTest
- [fix] Support of multiply index options in struct tag


# Version 1.8.0 (17.04.2018)

## C++ core

- [fea] Support of custom letters order set for string collates
- [fea] Full text indexing memory consumption optimization
- [fea] Thread russian letter `ё` as `е` in full text index
- [fix] Fixed incorrect behavior of full text search with term `*<stop-word>`
- [fix] Fixed full text behavior with FtDSL started with `+`
- [fix] Fix conflict of with leveldb's and reindexer's tcmalloc library

## Reindexer server
- [fea] Added daemonize mode to reindexer_server
- [fix] init.d script fixes
- [fea] Added CPU profiling mode to built-in HTTP pprof server

# Version 1.7.0 (05.04.2018)

## C++ core

- [fea] Support join, marge, aggregations in json DSL & SQL queris
- [fea] Added multiline form and comments in SQL query
- [fix] Last symbol of documents was not used by fulltext indexer
- [fix] Potential data corruption after removing index

## Go connector
- [fea] Batching of free c-buffers calls
- [fea] Added cgo limiter for deleteQuery call

## Reindexer server beta
- [fea] POST /api/v1/:db/sqlquery method
- [fea] Added sort_order to GET /api/v1/db method

# Version 1.6.0 (18.03.2018)

## C++ core
- [fea] Composite indexes direct queries support
- [fea] Fulltext support functions `snippet` and `highlight`
- [fix] Added utf8 strings validation
- [fix] WrSerializer::Printf incorrect buffer maxlen calculation
- [ref] utf8 collates does not allocate extra memory
- [ref] sort by unordered indexes optimizations
- [ref] Join queries optimizations

## Go connector
- [ref] Raise panic on Query reuse in Join
- [fea] Composite indexes direct queries support

## Reindexer server beta
- [fea] Write components logs to separate files
- [ref] Protocol breaking changes

# Version 1.5.0 (02.03.2018)

## Reindexer server beta released:
- [fea] Added cmake package target for RPM & DEB based systems
- [fea] sysv5 initscript added
- [fea] Binary cproto RPC protocol introduced
- [fea] Graceful server shutdown on SIGTERM and SIGINT
- [fea] Multiply databases support was implemented
- [fea] HTTP REST API changed: db name added to path, optional HTTP basic authorization (disabled by default)
- [fea] HTTP REST API for add/drop indexes added
- [fea] Configuration moved to separate .yml file

## C++ Core
- [ref] Item and Reindexer classes API has been redesigned. C++ tests was refactored to run with new API
- [fix] Potential race conditions and memory leak was fixed
- [fix] Fixed Query optimization bug. Queries with Join and precondition with GT/LT condition can return incorrect results
- [fea] Add/Drop indexes without reloading data
- [ref] Redundant Makefiles removed
- [del] Ugly RenameNamespace & CloneNamespace API was removed
- [fix] Fixed unnecessary copy results of precondition Join Query

## Go connector
- [fea] cproto connector to standalone server
- [fix] Potential races was fixed
- [del] Tx now alias to Batch. Old update logic was deprecated
- [fix] Limit cgo execution to 2K goroutines to avoid exceed of OS threads limit 
- [ref] EnableStorage method was deprecated
- [fix] Query builder did not reset opOR after InnerJoin

## Misc

