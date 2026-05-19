# Poco::MongoDB

C++ client library for MongoDB. Speaks the OP_MSG wire protocol over
`Poco::Net::StreamSocket` and exposes BSON documents, CRUD operations,
cursors, indexes, and replica-set topology discovery.

Minimum server version: MongoDB 3.6. Legacy opcodes (`OP_QUERY`,
`OP_INSERT`, etc.) were removed in 1.14 (GH #4781); only `OP_MSG` and
`OP_COMPRESSED` are sent on the wire. Servers from 3.6 through 8.x are
supported at the protocol level. The "Server feature compatibility" section
below details which features are exposed at the API level.

Replica-set support (SDAM, automatic failover, read preferences, topology
change notifications) is documented in [README-ReplicaSet.md](README-ReplicaSet.md).

## Quick start

```cpp
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/OpMsgMessage.h"

using namespace Poco::MongoDB;

Connection connection("localhost", 27017);
Database db("test");

auto request = db.createOpMsgMessage("users");
request->setCommandName(OpMsgMessage::CMD_INSERT);

Document::Ptr user = new Document;
user->add("name", "Alice").add("age", 30);
request->documents().push_back(user);

OpMsgMessage response;
connection.sendRequest(*request, response);
```

For `mongodb://` URI connections (including authentication), use the
`Connection::connect(uri, socketFactory)` overload with a user-provided
`SocketFactory`.

## Server feature compatibility

The tables below list MongoDB 6.0 / 7.0 / 8.0 server features and their
status in Poco::MongoDB:

- **Supported**: dedicated API.
- **Partial**: command-level access via `OpMsgMessage` (caller assembles
  the BSON body); no typed helper.
- **Missing**: no API and no helper; out of reach without library changes.
- **Added 1.15.x**: introduced by the patch series that ships this document.

Any command can be sent via `OpMsgMessage` with a hand-built body, so
"Missing" means "no typed helper", not "unreachable".

### A. Driver baseline (pre-6.0 specs)

Capabilities expected of any modern MongoDB driver.

| Capability | Status | Notes |
|---|---|---|
| Stable API (`apiVersion: "1"`, since 5.0) | Missing | Set `apiVersion` manually on each command. |
| SCRAM-SHA-256 (server default since 4.0) | Added 1.15.x | New `AUTH_SCRAM_SHA256`; default for `authenticate()`. |
| SCRAM-SHA-1 | Supported | Pass `AUTH_SCRAM_SHA1` explicitly. |
| x.509 / PLAIN / GSSAPI / MONGODB-AWS / MONGODB-OIDC | Missing | |
| TLS / SSL | Partial | Default `SocketFactory` throws for secure; user must supply a `SocketFactory` returning a `Poco::Net::SecureStreamSocket`. |
| `mongodb://` URI | Supported | |
| `mongodb+srv://` DNS seedlist | Missing | Resolve the host list out of band. |
| `tls=` URI option | Added 1.15.x | Alias for the historical `ssl=`. |
| Retryable reads / writes | Missing | Transient network errors are not retried. |
| Network compression (`zstd` / `zlib` / `snappy`) | Missing | `OP_COMPRESSED` opcode present; no codec or handshake. |
| ClientSession, causal consistency, snapshot reads | Missing | No multi-document transactions. |

### B. MongoDB 6.0

| Feature | Status | Notes |
|---|---|---|
| OP_MSG-only wire protocol | Supported | Cleaned up in 1.14 (GH #4781). |
| `$densify`, `$fill`, `$documents`, `$maxN`/`$minN`/`$lastN`/`$topN`/`$bottomN`, `$sortArray` | Partial | Hand-rolled aggregation pipeline. |
| Change streams (`$changeStream`) with `fullDocumentBeforeChange` and DDL events | Missing | No `watch()`, no resume tokens. |
| Clustered collections (`clusteredIndex`) | Partial | Raw `create` command. |
| Time-series collections; secondary / compound indexes on them | Partial | Raw `create` command; new `createIndex(extraOptions)` covers the index side. |
| **Partial indexes (`partialFilterExpression`, since 3.2)** | Added 1.15.x | Pass `partialFilterExpression` in the new `extraOptions` overload. |
| Hidden indexes (since 4.4) | Added 1.15.x | New `INDEX_HIDDEN` flag. |
| Text / 2dsphere / hashed / wildcard indexes | Partial | Reachable via `extraOptions`; wildcard via `"$**"` field name. |
| Collation on indexes | Added 1.15.x | Pass `collation` in `extraOptions`. |
| Queryable Encryption (preview) | Missing | No `ClientEncryption` class. |

### C. MongoDB 7.0

| Feature | Status | Notes |
|---|---|---|
| Queryable Encryption (GA) | Missing | No `ClientEncryption` class, no key-vault helper. |
| MONGODB-OIDC authentication | Missing | |
| Compound wildcard indexes | Partial | Reachable via `extraOptions` and a `"$**"` field. |
| `$median`, `$percentile`, `$bitAnd` / `$bitOr` / `$bitXor` / `$bitNot` | Partial | Hand-rolled aggregation pipeline. |
| `$changeStreamSplitLargeEvent` | Missing | No change-stream helper. |
| `analyzeShardKey`, `configureQueryAnalyzer` | Partial | Raw admin command. |
| Time-series delete relaxations | Supported | Transparent on the wire. |

### D. MongoDB 8.0

| Feature | Status | Notes |
|---|---|---|
| Cross-collection `bulkWrite` | Missing | Per-collection batched writes already work via `OpMsgMessage`. |
| Queryable Encryption range queries | Missing | Depends on QE. |
| `moveCollection`, `unshardCollection`, config-shard transitions | Partial | Raw admin command. |
| `setQuerySettings`, `removeQuerySettings` | Partial | Raw admin command. |
| `$convert` to BinData, `$toUUID`, `$toBinData` | Partial | Hand-rolled aggregation pipeline; UUID binary subtype already supported. |
| Majority write concern oplog optimization | Supported | Server-side only. |

### E. BSON type coverage

| Type | TypeId | Status |
|---|---|---|
| Double | 0x01 | Supported |
| String | 0x02 | Supported |
| Document | 0x03 | Supported |
| Array | 0x04 | Supported |
| Binary (incl. UUID subtype 0x04) | 0x05 | Supported |
| ObjectId | 0x07 | Supported |
| Boolean | 0x08 | Supported |
| UTC DateTime | 0x09 | Supported |
| Null | 0x0A | Supported |
| Regex | 0x0B | Supported |
| JavaScript | 0x0D | Supported |
| Int32 | 0x10 | Supported |
| Timestamp | 0x11 | Supported |
| Int64 | 0x12 | Supported |
| **Decimal128** | 0x13 | Added 1.15.x |
| **MinKey** | 0xFF | Added 1.15.x |
| **MaxKey** | 0x7F | Added 1.15.x |
| Code-with-scope | 0x0F | Missing |
| DBPointer (deprecated) | 0x0C | Missing |
| Symbol (deprecated) | 0x0E | Missing |

### F. Features obsolete in recent MongoDB

These work against current servers but the listed replacement is preferred.
Nothing has been removed.

| Surface | Status | Replacement |
|---|---|---|
| `OpMsgMessage::CMD_MAP_REDUCE` | `POCO_DEPRECATED` (server-deprecated since 5.0). | Aggregation pipeline with `$accumulator`, `$function`, `$out`, `$merge`. |
| `OpMsgMessage::CMD_COUNT` | Outside Stable API v1; `Database::count()` no longer issues it directly. | Aggregation `$count` (used internally by `Database::count()`). |
| `Database::INDEX_BACKGROUND` | `POCO_DEPRECATED` (server no-op since 4.2). | Drop the flag; index builds are online by default. |
| `Database::INDEX_SPARSE` | Superseded by `partialFilterExpression` since 3.2. | Pass `partialFilterExpression` in `createIndex(extraOptions)`. |
| `Database::createIndex(..., int version)` | Explicit `v=1` is incompatible with several modern index types. | Leave at 0; server picks v=2 (default since 3.4). |
| Connection URI option `ssl=true` | Replaced by `tls=true` (canonical since 4.2). | Either accepted. |

### Top blockers

The largest remaining gaps for users targeting MongoDB 6.0 / 7.0 / 8.0:

  1. `mongodb+srv://` Atlas connection strings.
  2. Transactions and `ClientSession`.
  3. Change streams.
  4. Queryable Encryption.
  5. MONGODB-OIDC and other modern auth mechanisms.
  6. Retryable reads / writes.
  7. Built-in TLS factory.
  8. Stable API opt-in.
  9. Cross-collection `bulkWrite` command.
  10. Aggregation pipeline builder.
