# Reindexer REST API

## Overview
**Reindexer** is an embeddable, in-memory, document-oriented database with a high-level Query builder interface.
Reindexer's goal is to provide fast search with complex queries.
The Reindexer is compact and fast. It has not heavy dependencies. Complete reindexer docker image with all libraries and web interface size is just 15MB.
Reindexer is fast.

- [Reindexer REST API](#reindexer-rest-api)
  - [Methods](#paths)
    - [List available databases](#list-available-databases)
    - [Create new database](#create-new-database)
    - [Drop database](#drop-database)
    - [List available namespaces](#list-available-namespaces)
    - [Create namespace](#create-namespace)
    - [Get namespace description](#get-namespace-description)
    - [Drop namespace](#drop-namespace)
    - [Get documents from namespace](#get-documents-from-namespace)
    - [Update documents in namespace](#update-documents-in-namespace)
    - [Insert documents to namespace](#insert-documents-to-namespace)
    - [Delete documents from namespace](#delete-documents-from-namespace)
    - [List available indexes](#list-available-indexes)
    - [Update index in namespace](#update-index-in-namespace)
    - [Add new index to namespace](#add-new-index-to-namespace)
    - [Drop index from namespace](#drop-index-from-namespace)
    - [SQL Query documents from namespace](#query-documents-from-namespace)
    - [DSL Query documents from namespace](#query-documents-from-namespace-1)
    - [Delete documents from namespace by query](#delete-documents-from-namespace-1)
    - [Get system information](#get-system-information)
    - [Get memory stats information](#get-memory-stats-information)
    - [Get performance stats information](#get-performance-stats-information)
    - [Get SELECT queries performance stats information](#get-select-queries-performance-stats-information)
    - [Update system config](#update-system-config)
  - [Definitions](#definitions)
    - [AggregationResDef](#aggregationresdef)
    - [AggregationsDef](#aggregationsdef)
    - [CacheMemStats](#cachememstats)
    - [CommonPerfStats](#commonperfstats)
    - [Database](#database)
    - [DatabaseMemStats](#databasememstats)
    - [DatabasePerfStats](#databaseperfstats)
    - [Databases](#databases)
    - [ExplainDef](#explaindef)
    - [FilterDef](#filterdef)
    - [FulltextConfig](#fulltextconfig)
    - [Index](#index)
    - [IndexCacheMemStats](#indexcachememstats)
    - [IndexMemStat](#indexmemstat)
    - [Indexes](#indexes)
    - [Items](#items)
    - [JoinCacheMemStats](#joincachememstats)
    - [JoinedDef](#joineddef)
    - [LogQueriesConfig](#logqueriesconfig)
    - [Namespace](#namespace)
    - [NamespaceMemStats](#namespacememstats)
    - [NamespacePerfStats](#namespaceperfstats)
    - [Namespaces](#namespaces)
    - [OnDef](#ondef)
    - [ProfilingConfig](#profilingconfig)
    - [QueriesPerfStats](#queriesperfstats)
    - [Query](#query)
    - [QueryCacheMemStats](#querycachememstats)
    - [QueryItems](#queryitems)
    - [QueryPerfStats](#queryperfstats)
    - [SelectPerfStats](#selectperfstats)
    - [SortDef](#sortdef)
    - [StatusResponse](#statusresponse)
    - [SysInfo](#sysinfo)
    - [SystemConfigItem](#systemconfigitem)
    - [UpdatePerfStats](#updateperfstats)
    - [UpdateResponse](#updateresponse)



### Version information
*Version* : 1.10.0


### License information
*License* : Apache 2.0  
*License URL* : http://www.apache.org/licenses/LICENSE-2.0.html  
*Terms of service* : null


### URI scheme
*BasePath* : /api/v1  
*Schemes* : HTTP


### Tags

* databases : Databases managment
* indexes : Indexes management
* items : Documents managment
* namespaces : Namespaces managment
* queries : Queries to reindexer (dsl/sql)
* system : System methods


### Produces

* `application/json`




## Paths

### List available databases
```
GET /db
```


#### Description
This operation will output list of all available databases


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Query**|**sort_order**  <br>*optional*|Sort Order|enum (asc, desc)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[Databases](#databases)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* databases


### Create new database
```
POST /db
```


#### Description
This operation will create new database. If database is already exists, then error will be returned.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Body**|**body**  <br>*required*|Database definintion|[Database](#database)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid parameters|[StatusResponse](#statusresponse)|


#### Tags

* databases


### Drop database
```
DELETE /db/{database}
```


#### Description
This operation will remove complete database from memory and disk. 
All data, including namespaces, their documents and indexes will be erased. 
Can not be undone. USE WITH CAUTION.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* databases


### List available namespaces
```
GET /db/{database}/namespaces
```


#### Description
This operation will list all availavle namespaces in specified database.
If database is not exists, then error will be returned.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Query**|**sort_order**  <br>*optional*|Sort Order|enum (asc, desc)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[Namespaces](#namespaces)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* namespaces

### Create namespace
```
POST /db/{database}/namespaces
```


#### Description
This operation will create new namespace in specified database.
If namespace is already exists, then operation do not nothing.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Body**|**body**  <br>*required*|Namespace definintion|[Namespace](#namespace)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid parameters|[StatusResponse](#statusresponse)|


#### Tags

* namespaces


### Get namespace description
```
GET /db/{database}/namespaces/{name}
```


#### Description
This operation will return specified namespace description, including options of namespace, and available indexes


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[Namespace](#namespace)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* namespaces

### Drop namespace
```
DELETE /db/{database}/namespaces/{name}
```


#### Description
This operation will delete completely namespace from memory and disk.
All documents, indexes and metadata from namespace will be removed.
Can not be undone. USE WITH CAUTION.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* namespaces


### Get documents from namespace
```
GET /db/{database}/namespaces/{name}/items
```


#### Description
This operation will select documents from namespace with specified filters, and sort them by specified sort order. Paging with limit and offset are supported.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Query**|**fields**  <br>*optional*|Comma-separated list of returned fields|string|
|**Query**|**filter**  <br>*optional*|Filter with SQL syntax, e.g: field1 = 'v1' AND field2 > 'v2'|string|
|**Query**|**limit**  <br>*optional*|Maximum count of returned items|integer|
|**Query**|**offset**  <br>*optional*|Offset of first returned item|integer|
|**Query**|**sort_field**  <br>*optional*|Sort Field|string|
|**Query**|**sort_order**  <br>*optional*|Sort Order|enum (asc, desc)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[Items](#items)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* items


### Update documents in namespace
```
PUT /db/{database}/namespaces/{name}/items
```


#### Description
This operation will UPDATE documents in namespace, by their primary keys.
Each document should be in request body as separate JSON object, e.g.
```
{"id":100, "name": "Pet"}
{"id":101, "name": "Dog"}
...
```


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Body**|**body**  <br>*required*||object|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[UpdateResponse](#updateresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* items


### Insert documents to namespace
```
POST /db/{database}/namespaces/{name}/items
```


#### Description
This operation will INSERT documents to namespace, by their primary keys.
Each document should be in request body as separate JSON object, e.g.
```
{"id":100, "name": "Pet"}
{"id":101, "name": "Dog"}
...
```


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Body**|**body**  <br>*required*||object|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[UpdateResponse](#updateresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* items


### Delete documents from namespace
```
DELETE /db/{database}/namespaces/{name}/items
```


#### Description
This operation will DELETE documents from namespace, by their primary keys.
Each document should be in request body as separate JSON object, e.g.
```
{"id":100}
{"id":101}
...
```


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Body**|**body**  <br>*required*||object|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[UpdateResponse](#updateresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* items


### List available indexes
```
GET /db/{database}/namespaces/{name}/indexes
```


#### Description
This operation will return list of available indexes, from specified database and namespace.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[Indexes](#indexes)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* indexes


### Update index in namespace
```
PUT /db/{database}/namespaces/{name}/indexes
```


#### Description
This operation will update index parameters. E.g. type of field or type of index.
Operation  is synchronious, so it can take long time, if namespace contains bunch of documents


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Body**|**body**  <br>*required*|Index definition|[Index](#index)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* indexes


### Add new index to namespace
```
POST /db/{database}/namespaces/{name}/indexes
```


#### Description
This operation will create new index. If index is already exists with the different parameters, then error will be returned.
Operation is synchronious, so it can take long time, if namespace contains bunch of documents.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|
|**Body**|**body**  <br>*required*|Index definition|[Index](#index)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* indexes


### Drop index from namespace
```
DELETE /db/{database}/namespaces/{name}/indexes/{indexname}
```


#### Description
This operation will remove index from namespace. No data will be erased.
Operation  is synchronious, so it can take long time, if namespace contains bunch of documents.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Path**|**indexname**  <br>*required*|Index name|string|
|**Path**|**name**  <br>*required*|Namespace name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[StatusResponse](#statusresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* indexes


### Query documents from namespace
```
GET /db/{database}/query
```


#### Description
This opertaion queries documents from namespace by SQL query. Query can be preced by `EXPLAIN` statement, then query execution plan will be returned with query results. 
Two level paging is supported. At first, applied normal SQL `LIMIT` and `OFFSET`,
then `limit` and `offset` from http request.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Query**|**limit**  <br>*optional*|Maximum count of returned items|integer|
|**Query**|**offset**  <br>*optional*|Offset of first returned item|integer|
|**Query**|**q**  <br>*required*|SQL query|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[QueryItems](#queryitems)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* queries

### Query documents from namespace
```
POST /db/{database}/query
```


#### Description
This opertaion queries documents from namespace by DSL query.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Body**|**body**  <br>*required*|DSL query|[Query](#query)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[QueryItems](#queryitems)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* queries


### Delete documents from namespace
```
DELETE /db/{database}/query
```


#### Description
This opertaion removes documents from namespace by DSL query.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Body**|**body**  <br>*required*|DSL query|[Query](#query)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|No Content|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* queries


### Query documents from namespace
```
POST /db/{database}/sqlquery
```


#### Description
This opertaion queries documents from namespace by SQL query. Query can be preced by `EXPLAIN` statement, then query execution plan will be returned with query results.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Body**|**q**  <br>*required*|SQL query|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[QueryItems](#queryitems)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* queries



### Get system information
```
GET /check
```


#### Description
This operation will return system informatiom about server version, uptime, and resources consumtion


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[SysInfo](#sysinfo)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* system



### Get memory stats information
```
GET /db/{database}/namespaces/%23memstats/items
```


#### Description
This operation will return detailed informatiom about database memory consumption


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[DatabaseMemStats](#databasememstats)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* system



### Get performance stats information
```
GET /db/{database}/namespaces/%23perfstats/items
```


#### Description
This operation will return detailed informatiom about database performance timings. By default performance stats is turned off.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[DatabasePerfStats](#databaseperfstats)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* system



### Get SELECT queries performance stats information
```
GET /db/{database}/namespaces/%23queriesperfstats/items
```


#### Description
This operation will return detailed informatiom about database memory consumption. By default qureis performance stat is turned off.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[QueriesPerfStats](#queriesperfstats)|
|**400**|Invalid arguments supplied|[StatusResponse](#statusresponse)|


#### Tags

* system



### Update system config
```
PUT /db/{database}/namespaces/%23config/items
```


#### Description
This operation will update system configuration:
- profiling configuration. It is used to enable recording of queries and overal performance;
- log queries configurating.


#### Parameters

|Type|Name|Description|Schema|
|---|---|---|---|
|**Path**|**database**  <br>*required*|Database name|string|
|**Body**|**body**  <br>*required*||[SystemConfigItem](#systemconfigitem)|


#### Responses

|HTTP Code|Description|Schema|
|---|---|---|
|**200**|successful operation|[UpdateResponse](#updateresponse)|
|**400**|Invalid status value|[StatusResponse](#statusresponse)|


#### Tags

* system





## Definitions


### AggregationResDef

|Name|Description|Schema|
|---|---|---|
|**facets**  <br>*optional*|Facets, calculated by aggregator|< [facets](#aggregationresdef-facets) > array|
|**value**  <br>*optional*|Value, calculated by aggregator|number|


**facets**

|Name|Description|Schema|
|---|---|---|
|**count**  <br>*optional*|Count of elemens this field value|integer|
|**value**  <br>*optional*|Facet field value|string|



### AggregationsDef

|Name|Description|Schema|
|---|---|---|
|**field**  <br>*optional*|Field or index name for aggregation function|string|
|**type**  <br>*optional*|Aggregation function|enum (SUM, AVG, MIN, MAX, FACET)|



### CacheMemStats

|Name|Description|Schema|
|---|---|---|
|**empty_count**  <br>*optional*|Count of empty elements slots in this cache|integer|
|**hit_count_limit**  <br>*optional*|Number of hits of queries, to store results in cache|integer|
|**items_count**  <br>*optional*|Count of used elements stored in this cache|integer|
|**total_size**  <br>*optional*|Total memory consumption by this cache|integer|



### CommonPerfStats

|Name|Description|Schema|
|---|---|---|
|**last_sec_avg_lock_time_us**  <br>*optional*|Average waiting time for acquiring lock to this object at last second|integer|
|**last_sec_qps**  <br>*optional*|Count of queries to this object, requested at last second|integer|
|**total_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object|integer|
|**total_avg_lock_latency_us**  <br>*optional*|Average waiting time for acquiring lock to this object|integer|
|**total_queries_count**  <br>*optional*|Total count of queries to this object|integer|
|**total_sec_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object at last second|integer|



### Database

|Name|Description|Schema|
|---|---|---|
|**name**  <br>*optional*|Name of database  <br>**Pattern** : `"^[A-Za-z0-9_\\-]*$"`|string|



### DatabaseMemStats

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*|Documents, matched specified filters|< [NamespaceMemStats](#namespacememstats) > array|
|**total_items**  <br>*optional*|Total count of documents, matched specified filters|integer|



### DatabasePerfStats

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*|Documents, matched specified filters|< [NamespacePerfStats](#namespaceperfstats) > array|
|**total_items**  <br>*optional*|Total count of documents, matched specified filters|integer|



### Databases

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*||< string > array|
|**total_items**  <br>*optional*|Total count of databases|integer|



### ExplainDef
Query execution explainings


|Name|Description|Schema|
|---|---|---|
|**indexes_us**  <br>*optional*|Indexes keys selection time|integer|
|**loop_us**  <br>*optional*|Intersection loop time|integer|
|**postprocess_us**  <br>*optional*|Query post process time|integer|
|**prepare_us**  <br>*optional*|Query prepare and optimize time|integer|
|**selectors**  <br>*optional*|Filter selectos, used to proccess query conditions|< [selectors](#explaindef-selectors) > array|
|**sort_index**  <br>*optional*|Index, which used for sort results|string|
|**total_us**  <br>*optional*|Total query execution time|integer|


**selectors**

|Name|Description|Schema|
|---|---|---|
|**comparators**  <br>*optional*|Count of comparators used, for this selector|integer|
|**cost**  <br>*optional*|Cost expectation of this selector|integer|
|**field**  <br>*optional*|Field or index name|string|
|**items**  <br>*optional*|Count of scanned documents by this selector|integer|
|**keys**  <br>*optional*|Number of uniq keys, processed by this selector (may be incorrect, in case of internal query optimization/caching|integer|
|**matched**  <br>*optional*|Count of processed documents, matched this selector|integer|
|**method**  <br>*optional*|Method, used to process condition|enum (scan, index, inner_join, left_join)|



### FilterDef

|Name|Description|Schema|
|---|---|---|
|**cond**  <br>*required*|Condition operator|enum (EQ, GT, GE, LE, LT, RANGE, SET, EMPTY)|
|**field**  <br>*required*|Field json path or index name for filter|string|
|**op**  <br>*optional*|Logic operator|enum (AND, OR, NOT)|
|**value**  <br>*optional*|Value of filter. Single integer or string for EQ, GT, GE, LE, LT condition, array of 2 elements for RANGE condition, or variable len array for SET condition|object|



### FulltextConfig
Fulltext Index configuration


|Name|Description|Schema|
|---|---|---|
|**bm25_boost**  <br>*optional*|Boost of bm25 ranking  <br>**Default** : `1.0`  <br>**Minimum value** : `0`  <br>**Maximum value** : `10`|number (float)|
|**bm25_weight**  <br>*optional*|Weight of bm25 rank in final rank 0: bm25 will not change final rank. 1: bm25 will affect to finl rank in 0 - 100% range  <br>**Default** : `0.5`  <br>**Minimum value** : `0`  <br>**Maximum value** : `1`|number (float)|
|**distance_boost**  <br>*optional*|Boost of search query term distance in found document  <br>**Default** : `1.0`  <br>**Minimum value** : `0`  <br>**Maximum value** : `10`|number (float)|
|**distance_weght**  <br>*optional*|Weight of search query terms distance in found document in final rank 0: distance will not change final rank. 1: distance will affect to final rank in 0 - 100% range  <br>**Default** : `0.5`  <br>**Minimum value** : `0`  <br>**Maximum value** : `1`|number (float)|
|**enable_kb_layout**  <br>*optional*|Enable wrong keyboard layout variants processing. e.g. term 'keynbr' will match word 'лунтик'  <br>**Default** : `true`|boolean|
|**enable_numbers_search**  <br>*optional*|Enable number variants processing. e.g. term '100' will match words one hundred  <br>**Default** : `false`|boolean|
|**enable_translit**  <br>*optional*|Enable russian translit variants processing. e.g. term 'luntik' will match word 'лунтик'  <br>**Default** : `true`|boolean|
|**extra_word_symbols**  <br>*optional*|List of symbols, which will be threated as word part, all other symbols will be thrated as wors separators  <br>**Default** : `"-/+"`|string|
|**log_level**  <br>*optional*|Log level of full text search engine  <br>**Minimum value** : `0`  <br>**Maximum value** : `4`|integer|
|**max_typo_len**  <br>*optional*|Maximum word length for building and matching variants with typos.  <br>**Minimum value** : `0`  <br>**Maximum value** : `100`|integer|
|**max_typos_in_word**  <br>*optional*|Maximum possible typos in word. 0: typos is disabled, words with typos will not match. N: words with N possible typos will match. It is not recommended to set more than 1 possible typo -It will seriously increase RAM usage, and decrease search speed  <br>**Minimum value** : `0`  <br>**Maximum value** : `2`|integer|
|**merge_limit**  <br>*optional*|Maximum documents count which will be processed in merge query results.  Increasing this value may refine ranking of queries with high frequency words, but will decrease search speed  <br>**Minimum value** : `0`  <br>**Maximum value** : `65535`|integer|
|**min_relevancy**  <br>*optional*|Minimum rank of found documents. 0: all found documents will be returned 1: only documents with relevancy >= 100% will be returned  <br>**Default** : `0.05`  <br>**Minimum value** : `0`  <br>**Maximum value** : `1`|number (float)|
|**stemmers**  <br>*optional*|List of stemmers to use|< string > array|
|**stop_words**  <br>*optional*|List of stop words. Words from this list will be ignored in documents and queries|< string > array|
|**term_len_boost**  <br>*optional*|Boost of search query term length  <br>**Default** : `1.0`  <br>**Minimum value** : `0`  <br>**Maximum value** : `10`|number (float)|
|**term_len_weght**  <br>*optional*|Weight of search query term length in final rank. 0: term length will not change final rank. 1: term length will affect to final rank in 0 - 100% range  <br>**Default** : `0.3`  <br>**Minimum value** : `0`  <br>**Maximum value** : `1`|number (float)|



### Index

|Name|Description|Schema|
|---|---|---|
|**collate_mode**  <br>*optional*|String collate mode  <br>**Default** : `"none"`|enum (none, ascii, utf8, numeric)|
|**config**  <br>*optional*||[FulltextConfig](#fulltextconfig)|
|**field_type**  <br>*required*|Field data type|enum (int, int64, double, string, bool, composite)|
|**index_type**  <br>*required*|Index structure type  <br>**Default** : `"hash"`|enum (hash, tree, text, -)|
|**is_array**  <br>*optional*|Specifies, that index is array. Array indexes can work with array fields, or work with multiple fields  <br>**Default** : `false`|boolean|
|**is_dense**  <br>*optional*|Reduces the index size. For hash and tree it will save ~8 bytes per unique key value. Useful for indexes with high selectivity, but for tree and hash indexes with low selectivity can seriously decrease update performance;  <br>**Default** : `false`|boolean|
|**is_pk**  <br>*optional*|Specifies, that index is primary key. The update opertations will checks, that PK field is unique. The namespace MUST have only 1 PK index|boolean|
|**is_sparse**  <br>*optional*|Value of index may not present in the document, and threfore, reduce data size but decreases speed operations on index  <br>**Default** : `false`|boolean|
|**json_paths**  <br>*required*|Fields path in json object, e.g 'id' or 'subobject.field'. If index is 'composite' or 'is_array', than multiple json_paths can be specified, and index will get values from all specified fields.|< string > array|
|**name**  <br>*required*|Name of index, can contains letters, digits and underscores  <br>**Default** : `"id"`  <br>**Pattern** : `"^[A-Za-z0-9_\\-]*$"`|string|
|**sort_order_letters**  <br>*optional*|Sort order letters  <br>**Default** : `""`|string|



### IndexCacheMemStats
Number of elements in idset cache. Stores merged reverse index results of SELECT field IN(...) by IN(...) keys

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**empty_count**  <br>*optional*|Count of empty elements slots in this cache|integer|
|**hit_count_limit**  <br>*optional*|Number of hits of queries, to store results in cache|integer|
|**items_count**  <br>*optional*|Count of used elements stored in this cache|integer|
|**total_size**  <br>*optional*|Total memory consumption by this cache|integer|



### IndexMemStat

|Name|Description|Schema|
|---|---|---|
|**fulltext_size**  <br>*optional*|Total memory consumption of fulltext search structures|integer|
|**idset_btree_size**  <br>*optional*|Total memory consumption of reverse index b-tree structures. For `dense` and `store` indexes always 0|integer|
|**idset_cache**  <br>*optional*||[IndexCacheMemStats](#indexcachememstats)|
|**idset_plain_size**  <br>*optional*|Total memory consumption of reverse index vectors. For `store` ndexes always 0|integer|
|**name**  <br>*optional*|Name of index. There are special index with name `-tuple`. It's stores original document's json structure with non indexe fields|string|
|**sort_orders_size**  <br>*optional*|Total memory consumption of SORT statement and `GT`, `LT` conditions optimized structures. Applicabe only to `tree` indexes|integer|
|**unique_keys_count**  <br>*optional*|Count of unique keys values stored in index|integer|



### Indexes

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*||< [Index](#index) > array|
|**total_items**  <br>*optional*|Total count of indexes|integer|



### Items

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*|Documents, matched specified filters|< object > array|
|**total_items**  <br>*optional*|Total count of documents, matched specified filters|integer|



### JoinCacheMemStats
Number of elements in join cache. Stores results of selects to right table by ON condition

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**empty_count**  <br>*optional*|Count of empty elements slots in this cache|integer|
|**hit_count_limit**  <br>*optional*|Number of hits of queries, to store results in cache|integer|
|**items_count**  <br>*optional*|Count of used elements stored in this cache|integer|
|**total_size**  <br>*optional*|Total memory consumption by this cache|integer|



### JoinedDef

|Name|Description|Schema|
|---|---|---|
|**filters**  <br>*optional*|Filter for results documents|< [FilterDef](#filterdef) > array|
|**limit**  <br>*optional*|Maximum count of returned items|integer|
|**namespace**  <br>*required*|Namespace name|string|
|**offset**  <br>*optional*|Offset of first returned item|integer|
|**op**  <br>*optional*|Logic operator|enum (AND, OR, NOT)|
|**sort**  <br>*optional*||[SortDef](#sortdef)|
|**true**  <br>*optional*|Join ON statement|< [OnDef](#ondef) > array|
|**type**  <br>*required*|Join type|enum (LEFT, INNER)|



### LogQueriesConfig

|Name|Description|Schema|
|---|---|---|
|**log_level**  <br>*optional*|Log level of queries core logger|enum (none, error, warning, info, trace)|
|**namespace**  <br>*optional*|Name of namespace, or `*` for setting to all namespaces|string|



### Namespace

|Name|Description|Schema|
|---|---|---|
|**indexes**  <br>*optional*||< [Index](#index) > array|
|**name**  <br>*optional*|Name of namespace  <br>**Pattern** : `"^[A-Za-z0-9_\\-]*$"`|string|
|**storage**  <br>*optional*||[storage](#namespace-storage)|


**storage**

|Name|Description|Schema|
|---|---|---|
|**enabled**  <br>*optional*|If true, then documents will be stored to disc storage, else all data will be lost on server shutdown|boolean|



### NamespaceMemStats

|Name|Description|Schema|
|---|---|---|
|**data_size**  <br>*optional*|Raw size of documents, stored in the namespace, except string fields|integer|
|**indexes**  <br>*optional*|Memory consumption of each namespace index|< [IndexMemStat](#indexmemstat) > array|
|**items_count**  <br>*optional*|Total count of documents in namespace|integer|
|**join_cache**  <br>*optional*||[JoinCacheMemStats](#joincachememstats)|
|**name**  <br>*optional*|Name of namespace|string|
|**query_cache**  <br>*optional*||[QueryCacheMemStats](#querycachememstats)|
|**storage_ok**  <br>*optional*|Status of disk storage|boolean|
|**storage_path**  <br>*optional*|Filesystem path to namespace storage|boolean|
|**total**  <br>*optional*|Summary of total namespace memory consumption|[total](#namespacememstats-total)|
|**updated_unix_nano**  <br>*optional*|[[deperecated]]. do not use|integer|


**total**

|Name|Description|Schema|
|---|---|---|
|**cache_size**  <br>*optional*|Total memory consumption of namespace's caches. e.g. idset and join caches|integer|
|**data_size**  <br>*optional*|Total memory size of stored documents, including system structures|integer|
|**indexes_size**  <br>*optional*|Total memory consumption of namespace's indexes|integer|



### NamespacePerfStats

|Name|Description|Schema|
|---|---|---|
|**indexes**  <br>*optional*|Memory consumption of each namespace index|< [indexes](#namespaceperfstats-indexes) > array|
|**name**  <br>*optional*|Name of namespace|string|
|**selects**  <br>*optional*||[SelectPerfStats](#selectperfstats)|
|**updates**  <br>*optional*||[UpdatePerfStats](#updateperfstats)|


**indexes**

|Name|Description|Schema|
|---|---|---|
|**name**  <br>*optional*|Name of index|string|
|**selects**  <br>*optional*||[SelectPerfStats](#selectperfstats)|
|**updates**  <br>*optional*||[UpdatePerfStats](#updateperfstats)|



### Namespaces

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*||< [items](#namespaces-items) > array|
|**total_items**  <br>*optional*|Total count of namespaces|integer|


**items**

|Name|Description|Schema|
|---|---|---|
|**name**  <br>*optional*|Name of namespace|string|
|**storage_enabled**  <br>*optional*|If true, then documents will be stored to disc storage, else all data will be lost on server shutdown|boolean|



### OnDef

|Name|Description|Schema|
|---|---|---|
|**cond**  <br>*required*|Condition operator|enum (EQ, GT, GE, LE, LT, RANGE, SET, EMPTY)|
|**left_field**  <br>*required*|Field from left namespace (main query namespace)|string|
|**op**  <br>*optional*|Logic operator|enum (AND, OR, NOT)|
|**right_field**  <br>*required*|Field from right namespace (joined query namespace)|string|



### ProfilingConfig

|Name|Description|Schema|
|---|---|---|
|**memstats**  <br>*optional*|Enables tracking memory statistics  <br>**Default** : `true`|boolean|
|**perfstats**  <br>*optional*|Enables tracking overal perofrmance statistics  <br>**Default** : `false`|boolean|
|**queries_threshold_us**  <br>*optional*|Minimum query execution time to be recoreded in #queriesperfstats namespace|integer|
|**queriesperfstats**  <br>*optional*|Enables record queries perofrmance statistics  <br>**Default** : `false`|boolean|



### QueriesPerfStats

|Name|Description|Schema|
|---|---|---|
|**items**  <br>*optional*|Documents, matched specified filters|< [QueryPerfStats](#queryperfstats) > array|
|**total_items**  <br>*optional*|Total count of documents, matched specified filters|integer|



### Query

|Name|Description|Schema|
|---|---|---|
|**aggregations**  <br>*optional*|Ask query calculate aggregation|< [AggregationsDef](#aggregationsdef) > array|
|**distinct**  <br>*optional*|Distinct field/index name. Results will contain's document with distinct field value|string|
|**explain**  <br>*optional*|Add query execution explain information  <br>**Default** : `false`|boolean|
|**filters**  <br>*optional*|Filter for results documents|< [FilterDef](#filterdef) > array|
|**join_queries**  <br>*optional*|Nested queries to be joined on right side of main query|< [JoinedDef](#joineddef) > array|
|**limit**  <br>*optional*|Maximum count of returned items|integer|
|**merge_queries**  <br>*optional*|Merged queries to be merged with main query|< [Query](#query) > array|
|**namespace**  <br>*required*|Namespace name|string|
|**offset**  <br>*optional*|Offset of first returned item|integer|
|**req_total**  <br>*optional*|Ask query to calculate total documents, match condition  <br>**Default** : `"disabled"`|enum (disabled, enabled, cached)|
|**select_filter**  <br>*optional*|Filter fields of returned document. Can be dot separated, e.g 'subobject.field'|< string > array|
|**select_functions**  <br>*optional*|Add extra select functions to query|< string > array|
|**sort**  <br>*optional*|Specifies results sorting order|< [SortDef](#sortdef) > array|



### QueryCacheMemStats
Number of elements in query cache. Stores results of SELECT COUNT(*) by Where conditions

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**empty_count**  <br>*optional*|Count of empty elements slots in this cache|integer|
|**hit_count_limit**  <br>*optional*|Number of hits of queries, to store results in cache|integer|
|**items_count**  <br>*optional*|Count of used elements stored in this cache|integer|
|**total_size**  <br>*optional*|Total memory consumption by this cache|integer|



### QueryItems

|Name|Description|Schema|
|---|---|---|
|**aggregations**  <br>*optional*|Aggregation functions results|< [AggregationResDef](#aggregationresdef) > array|
|**explain**  <br>*optional*||[ExplainDef](#explaindef)|
|**items**  <br>*optional*|Documents, matched query|< object > array|
|**query_total_items**  <br>*optional*|Total count of documents, matched query|integer|



### QueryPerfStats
Performance statistics per each query

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**last_sec_avg_lock_time_us**  <br>*optional*|Average waiting time for acquiring lock to this object at last second|integer|
|**last_sec_qps**  <br>*optional*|Count of queries to this object, requested at last second|integer|
|**query**  <br>*optional*|SQL representation of query|string|
|**total_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object|integer|
|**total_avg_lock_latency_us**  <br>*optional*|Average waiting time for acquiring lock to this object|integer|
|**total_queries_count**  <br>*optional*|Total count of queries to this object|integer|
|**total_sec_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object at last second|integer|



### SelectPerfStats
Performance statistics for select operations

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**last_sec_avg_lock_time_us**  <br>*optional*|Average waiting time for acquiring lock to this object at last second|integer|
|**last_sec_qps**  <br>*optional*|Count of queries to this object, requested at last second|integer|
|**total_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object|integer|
|**total_avg_lock_latency_us**  <br>*optional*|Average waiting time for acquiring lock to this object|integer|
|**total_queries_count**  <br>*optional*|Total count of queries to this object|integer|
|**total_sec_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object at last second|integer|


### SortDef
Specifies results sorting order


|Name|Description|Schema|
|---|---|---|
|**desc**  <br>*optional*|Descent or ascent sorting direction|boolean|
|**field**  <br>*required*|Field or index name for sorting|string|
|**values**  <br>*optional*|Optional: Documents with this values of field will be returned first|< object > array|


### StatusResponse

|Name|Description|Schema|
|---|---|---|
|**description**  <br>*optional*|Text description of error details|string|
|**response_code**  <br>*optional*|Error code:<br> * 0 - errOK<br> * 1 - errParseSQL<br> * 2 - errQueryExec<br> * 3 - errParams<br> * 4 - errLogic<br> * 5 - errParseJson<br> * 6 - errParseDSL<br> * 7 - errConflict<br> * 8 - errParseBin<br> * 9 - errForbidden<br> * 10 - errWasRelock<br> * 11 - errNotValid<br> * 12 - errNetwork<br> * 13 - errNotFound<br> * 14 - errStateInvalidated|integer|
|**success**  <br>*optional*|Status of operation|boolean|


### SysInfo

|Name|Description|Schema|
|---|---|---|
|**current_allocated_bytes**  <br>*optional*|Current inuse allocated memory size in bytes|integer|
|**heap_size**  <br>*optional*|Current heap size in bytes|integer|
|**pageheap_free**  <br>*optional*|Heap free size in bytes|integer|
|**pageheap_unmapped**  <br>*optional*|Unmapped free heap size in bytes|integer|
|**start_time**  <br>*optional*|Server start time in unix timestamp|integer|
|**uptime**  <br>*optional*|Server uptime in seconds|integer|
|**version**  <br>*optional*|Server version|string|


### SystemConfigItem

|Name|Description|Schema|
|---|---|---|
|**log_queries**  <br>*optional*||< [LogQueriesConfig](#logqueriesconfig) > array|
|**profiling**  <br>*optional*||[ProfilingConfig](#profilingconfig)|
|**type**  <br>*required*|**Default** : `"profiling"`|enum (profiling, log_queries)|


### UpdatePerfStats
Performance statistics for update operations

*Polymorphism* : Composition


|Name|Description|Schema|
|---|---|---|
|**last_sec_avg_lock_time_us**  <br>*optional*|Average waiting time for acquiring lock to this object at last second|integer|
|**last_sec_qps**  <br>*optional*|Count of queries to this object, requested at last second|integer|
|**total_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object|integer|
|**total_avg_lock_latency_us**  <br>*optional*|Average waiting time for acquiring lock to this object|integer|
|**total_queries_count**  <br>*optional*|Total count of queries to this object|integer|
|**total_sec_avg_latency_us**  <br>*optional*|Average latency (execution time) for queries to this object at last second|integer|


### UpdateResponse

|Name|Description|Schema|
|---|---|---|
|**updated**  <br>*optional*|Count of updated items|integer|





