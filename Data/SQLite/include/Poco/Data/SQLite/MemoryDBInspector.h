//
// MemoryDBInspector.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  MemoryDBInspector
//
// Definition of the MemoryDBInspector class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLite_MemoryDBInspector_INCLUDED
#define SQLite_MemoryDBInspector_INCLUDED


#ifdef POCO_DATA_NO_SQL_PARSER
#error "MemoryDBInspector requires MemoryDB; rebuild without POCO_DATA_NO_SQL_PARSER."
#endif


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/Data/Statement.h"
#include "Poco/Timestamp.h"
#include "Poco/Types.h"
#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>


namespace Poco::Data::SQLite {


class SQLite_API MemoryDBInspector
	/// Introspection, ad-hoc query and management facade over a MemoryDB, plus a
	/// JSON projection of everything it reports.
	///
	/// It answers the three questions a management UI asks of a store - "what does
	/// the persistence layer look like right now" (status()), "what is in there"
	/// (schema()) and "run this for me" (query()) - as plain C++ structs, and
	/// serializes those structs to a std::ostream as JSON. The structs are the
	/// single source of truth: every writeJSON() overload is a pure function of one
	/// of them and touches no database.
	///
	/// Nothing here pulls in the JSON library. String escaping goes through
	/// Poco::toJSON() from Foundation, the same way Data/src/JSONRowFormatter.cpp
	/// does, so PocoDataSQLite keeps its two-library dependency set (PocoData plus
	/// PocoFoundation).
	///
	/// Threading
	/// ---------
	/// The inspector holds a reference to a MemoryDB it does not own, and owns no
	/// lock of its own. That is deliberate: the owner of a MemoryDB necessarily
	/// already serializes access to MemoryDB::session() with a lock of its own
	/// choosing (a Poco::Data::Session is not thread-safe), and a second lock in
	/// here would be a *different* lock - it would order inspector calls against
	/// each other while leaving the real race, inspector-read versus owner-write,
	/// wide open behind an API that looked synchronized. So the contract is
	/// explicit instead, and every method belongs to exactly one of three groups.
	///
	/// Group A - no external synchronization required.
	///   Reaches only MemoryDB's own thread-safe API and the persistence directory.
	///   Runs no SQL. Call it from any thread at any time.
	///
	/// Group B - the caller MUST hold its own Session lock across the call.
	///   Builds a Poco::Data::Statement/RecordSet on MemoryDB::session() and steps
	///   it many times. MemoryDB's internal mutexes do not cover that: they make
	///   each of MemoryDB's *own* statements atomic against concurrent steps (see
	///   the DbMutexGuard comment in MemoryDB.cpp), they do not protect a third
	///   party's multi-step result set. Wrap these in exactly the same lock the
	///   owner uses for its own session() access. The returned struct is a value -
	///   release the lock before serializing it, which is what the static
	///   writeJSON() overloads are for.
	///
	/// Group C - the caller MUST NOT hold its Session lock across the call.
	///   flush() (and perform("flush"/"seal")) hold MemoryDB's _flushMutex across
	///   seconds of file IO; running that under the owner's write lock stalls the
	///   writer thread for the duration. The attach/detach/history/drop verbs are
	///   individually thread-safe but can perturb queries already in flight on
	///   other threads: an ATTACH between two steps of somebody else's RecordSet
	///   changes the schema cookie and surfaces as "no such table: arc_N", "not an
	///   error" or "another row available". Treat those as transient and retry.
	///
	/// A typical owner therefore looks like:
	///
	///     QueryResult r;
	///     { std::lock_guard<std::mutex> lk(_dbMutex); r = _inspector.query(sql); }
	///     MemoryDBInspector::writeJSON(out, r);   // outside the lock
	///
	/// SQLPolicy is a guard rail on this class's entry points, not a sandbox. The
	/// caller holds the MemoryDB and can always bypass it; the point is that a UI
	/// routed through here cannot destroy a store by accident. In particular this
	/// class NEVER issues PRAGMA query_only: the connection is shared with the
	/// owner, query_only is connection-scoped and sticky, and setting it would
	/// break the owner's writes for as long as it stayed on.
{
public:
	enum class SQLPolicy
	{
		READ_ONLY,
			/// query() accepts only statements that parse as SELECT, and the
			/// destructive verbs of perform() (seal, drop, enforceRetention) are
			/// refused.

		ALLOW_WRITES
			/// query() runs anything MemoryDB::operator << accepts, and every verb
			/// of perform() is available.
	};

	enum class RowCounts
	{
		OMIT,
			/// schema() reports no row counts (TableInfo::rowsKnown stays false).
			/// The default: a full count(*) per table is by far the most expensive
			/// thing schema() can do, and on a store whose largest table holds tens
			/// of millions of rows it would hold the caller's Session lock for
			/// seconds. Count on demand with rowCount() instead.

		EXACT
			/// schema() issues one count(*) per table and view.
	};

	struct FileInfo
		/// One file in the persistence directory, as seen by a plain directory walk.
	{
		std::string     name;
		Poco::UInt64    bytes = 0;
		Poco::Timestamp modified;
	};

	struct ShardStatus
		/// MemoryDB::ShardDescriptor plus whether the shard is attached read-only.
	{
		Poco::UInt32    id = 0;
		bool            sealed = false;
		Poco::Timestamp createdAt;
		Poco::Timestamp sealedAt;    // epoch 0 when !sealed
		Poco::UInt64    bytes = 0;
		std::string     filename;
		bool            attached = false;
		std::string     alias;       // "arc_<id>" when attached, otherwise empty
	};

	struct Status
		/// Everything the persistence layer reports without running a query.
	{
		std::string              directory;
		bool                     readOnly = true;   // mirrors SQLPolicy
		bool                     dirty = false;
		Poco::Timestamp          lastFlush;
		std::size_t              shardCount = 0;
		std::vector<ShardStatus> shards;
		Poco::UInt64             shardBytes = 0;    // sum of shards[].bytes: the catalog's view
		std::vector<FileInfo>    files;
		Poco::UInt64             diskBytes = 0;     // sum of files[].bytes: the directory's view
	};

	struct ColumnInfo
		/// One row of PRAGMA <database>.table_info(<table>).
	{
		std::string name;
		std::string type;            // declared type, verbatim
		bool        notNull = false;
		std::string defaultValue;
		bool        hasDefault = false;
		bool        primaryKey = false;
	};

	struct IndexInfo
	{
		std::string name;
		std::string sql;             // CREATE INDEX ...; empty for implicit indexes
	};

	struct TableInfo
		/// A table or a view; type says which.
	{
		std::string             name;
		std::string             type;        // "table" or "view"
		std::string             sql;         // DDL from sqlite_master
		std::vector<ColumnInfo> columns;
		std::vector<IndexInfo>  indexes;
		Poco::Int64             rows = 0;
		bool                    rowsKnown = false;
			/// False when RowCounts::OMIT was requested, and also when the count
			/// itself failed - a view over a detached shard alias cannot be counted.
	};

	struct DatabaseInfo
		/// One schema on the connection: "main", "temp", or an attached sealed
		/// shard's "arc_<id>".
	{
		std::string            name;
		std::string            file;         // empty for in-memory schemas
		std::vector<TableInfo> objects;
	};

	struct Schema
	{
		std::vector<DatabaseInfo> databases;
	};

	struct RowCount
		/// Result of a single on-demand count(*).
	{
		std::string database;
		std::string table;
		Poco::Int64 rows = 0;
		bool        known = false;
	};

	struct ResultColumn
	{
		std::string name;
		std::string type;            // MetaColumn type name: "int64", "string", ...
	};

	struct Cell
		/// One grid cell. NULL is a distinct state; everything else is rendered as
		/// text, including numbers - a 64-bit rowid does not survive a JSON number
		/// round-trip through a browser, and a BLOB does not survive at all (it is
		/// replaced by a "<blob, N bytes>" marker).
	{
		bool        isNull = true;
		std::string text;
	};

	struct QueryResult
		/// The outcome of one query(). A failing statement is a normal outcome
		/// here, not an exception: ok is false and error carries the message, so a
		/// UI can render it next to the editor. Always check ok before reading
		/// columns and rows.
	{
		bool                            ok = false;
		std::string                     error;
		std::vector<ResultColumn>       columns;
		std::vector<std::vector<Cell> > rows;
		Poco::UInt64                    rowCount = 0;
			/// Rows extracted, NOT the number of rows the query would produce:
			/// extraction stops just past maxRows. See truncated.
		bool                            truncated = false;
		Poco::UInt64                    maxRows = 0;
		Poco::UInt64                    affected = 0;
		double                          elapsedMs = 0.0;
	};

	struct ActionResult
		/// The outcome of one perform(). Same convention as QueryResult: refusals
		/// and SQLite errors come back as ok == false rather than as exceptions.
	{
		bool         ok = false;
		std::string  message;        // human-readable, set when ok
		std::string  error;          // set when !ok
		std::string  detail;         // attach -> alias, historyView -> view name
		Poco::UInt64 count = 0;      // enforceRetention -> shards dropped
	};

	static constexpr std::size_t DEFAULT_MAX_ROWS = 500;

	MemoryDBInspector(MemoryDB& db, SQLPolicy policy = SQLPolicy::READ_ONLY);
		/// Creates an inspector over db, which must outlive the inspector.

	~MemoryDBInspector();

	MemoryDBInspector(const MemoryDBInspector&) = delete;
	MemoryDBInspector& operator = (const MemoryDBInspector&) = delete;

	SQLPolicy policy() const;
		/// Returns the SQL policy this inspector was constructed with.

	std::size_t maxRows() const;
		/// Returns the default row cap applied by query(const std::string&).

	void setMaxRows(std::size_t maxRows);
		/// Sets the default row cap (DEFAULT_MAX_ROWS initially). Not synchronized;
		/// set it once, at construction time.

	const std::string& directory() const;
		/// Returns the MemoryDB's persistence directory.

	//
	// Group A - no external synchronization required
	//

	Status status();
		/// Returns a snapshot of the persistence layer: shard metadata with attach
		/// state, the directory listing, and the two byte totals. Runs no SQL.
		///
		/// The snapshot is stale the moment it returns - a concurrent flush, seal
		/// or retention sweep can change any of it - which is the same caveat
		/// MemoryDB::shards() carries.
		///
		/// Cost is one MemoryDB::shards() call plus a directory walk. On a slow or
		/// networked filesystem that walk dominates; it is deliberately kept out of
		/// any lock.

	void writeStatusJSON(std::ostream& os);
		/// status() followed by writeJSON(). Group A, like status().

	//
	// Group B - the caller MUST hold its own Session lock across these
	//

	Schema schema(RowCounts rowCounts = RowCounts::OMIT);
		/// Walks sqlite_master on every schema PRAGMA database_list reports - main,
		/// temp, and every attached "arc_<id>" - and returns tables and views with
		/// their DDL, columns and indexes.
		///
		/// Every sub-step swallows its own failures: a schema that vanishes
		/// mid-walk, because a concurrent deleteShard() force-detached it,
		/// contributes nothing rather than failing the whole document. Partial
		/// results are preferred to no results.

	RowCount rowCount(const std::string& database, const std::string& table);
		/// SELECT count(*) FROM <database>.<table>, for filling in one cell of a
		/// schema view on demand. known is false if the count failed - a view whose
		/// UNION references a detached shard cannot be counted.
		///
		/// This is an exact count and it scans. On a table with tens of millions of
		/// rows it holds the caller's lock for a noticeable time, which is why
		/// schema() does not do it by default.

	QueryResult query(const std::string& sql);
		/// query(sql, maxRows()).

	QueryResult query(const std::string& sql, std::size_t maxRows);
		/// Runs sql and returns the result set, capped at maxRows rows.
		///
		/// Under SQLPolicy::READ_ONLY the statement is checked first (see
		/// checkSQLAllowed) and a refusal is reported as ok == false, not thrown.
		/// SQLite errors are reported the same way. This method does not throw for
		/// any input.
		///
		/// The statement is issued through MemoryDB::operator <<, not through
		/// session() directly, so the WITHOUT ROWID rejection and the poisoned-
		/// instance check apply to it exactly as they do to the owner's own writes.
		///
		/// Extraction is bounded: the statement carries a row limit of maxRows + 1,
		/// so a SELECT over a huge table cannot pull the whole table into memory
		/// before the cap is applied. QueryResult::rowCount therefore reports what
		/// was extracted and truncated says whether more rows exist; neither is a
		/// total.

	void checkSQLAllowed(const std::string& sql);
		/// Throws Poco::InvalidAccessException if sql is not permitted under this
		/// inspector's SQLPolicy. Under ALLOW_WRITES it never throws.
		///
		/// Classification uses Poco::Data::Parser, the same SQL parser MemoryDB
		/// itself uses to decide what counts as a write, so the two agree by
		/// construction. A statement the parser cannot parse is treated as a write:
		/// on a live store the safe reading of "I do not know what this is" is
		/// "assume it changes something". A consequence worth knowing is that
		/// PRAGMA and EXPLAIN do not parse and are therefore refused under
		/// READ_ONLY; status(), schema() and rowCount() already expose what those
		/// would be used for.
		///
		/// query() applies this check itself and reports the refusal through
		/// QueryResult::error; this entry point exists for callers that want the
		/// exception, and so the policy can be tested in isolation.

	void writeSchemaJSON(std::ostream& os, RowCounts rowCounts = RowCounts::OMIT);
	void writeRowCountJSON(std::ostream& os, const std::string& database, const std::string& table);
	void writeQueryJSON(std::ostream& os, const std::string& sql);
	void writeQueryJSON(std::ostream& os, const std::string& sql, std::size_t maxRows);
		/// Gather-and-serialize convenience: equivalent to calling the matching
		/// getter and passing the result to writeJSON().
		///
		/// Prefer the two-step form when the caller holds a contended lock: gather
		/// under the lock, release it, then serialize. Formatting a 500-row grid is
		/// pure string work and has no business happening under the owner's write
		/// lock.

	//
	// Group C - the caller MUST NOT hold its Session lock across these
	//

	bool flush();
		/// Asks MemoryDB to persist unflushed changes and returns whether a flush
		/// actually completed. MemoryDB::flush() is skip-on-busy and returns void,
		/// so this samples lastFlushTime() around the call rather than inferring
		/// from dirty(), which is unreliable under a concurrent writer.
		///
		/// Holds MemoryDB's _flushMutex across the whole of the file IO. Never call
		/// it while holding a lock the writer thread needs.

	ActionResult perform(const std::string& action,
		Poco::UInt32 shardId = 0, const std::string& table = std::string());
		/// Dispatches one named management verb, for HTTP-shaped callers.
		/// Exceptions and policy refusals come back as ok == false.
		///
		///   "flush"            - flush()
		///   "seal"             - sealActive() then flush()                [writes]
		///   "attach"           - attachArchived(shardId); detail = alias
		///   "detach"           - detachArchived(shardId)
		///   "attachAll"        - attachAllArchived()
		///   "detachAll"        - detachAllArchived()
		///   "drop"             - deleteShard(shardId)                     [writes]
		///   "enforceRetention" - enforceRetention(); count = dropped      [writes]
		///   "historyView"      - attachAllArchived() then historyView(table);
		///                        detail = view name
		///
		/// The verbs marked [writes] are refused under SQLPolicy::READ_ONLY. The
		/// others change only transient connection state, not the store, and are
		/// always available.
		///
		/// "attachAll" can push the store to sqlite3_limit(SQLITE_LIMIT_ATTACHED),
		/// whose always-on backstop in enforceRetention() then drops the oldest
		/// sealed shards. status() afterwards is the source of truth for what is
		/// actually attached.
		///
		/// "historyView" is expensive: it holds the SQLite connection mutex across
		/// N ATTACHes plus the view rebuild, so concurrent statements serialize
		/// behind it. Cache the returned view name and re-query it rather than
		/// rebuilding.
		///
		/// Typed callers should skip this and call MemoryDB directly - it is right
		/// there. This exists so the string-to-verb dispatch is written once.

	void writeActionJSON(std::ostream& os, const std::string& action,
		Poco::UInt32 shardId = 0, const std::string& table = std::string());
		/// perform() followed by writeJSON().

	//
	// Pure serializers - no database access, no synchronization
	//

	static void writeJSON(std::ostream& os, const Status& status);
	static void writeJSON(std::ostream& os, const Schema& schema);
	static void writeJSON(std::ostream& os, const RowCount& rowCount);
	static void writeJSON(std::ostream& os, const QueryResult& result);
	static void writeJSON(std::ostream& os, const ActionResult& result);
		/// Writes one complete JSON object to os. Safe to call from any thread on a
		/// struct the caller owns; nothing here reads the MemoryDB.

private:
	std::vector<std::pair<std::string, std::string> > databaseNames();
	std::vector<ColumnInfo> tableColumns(const std::string& database, const std::string& table);
	std::vector<TableInfo> schemaObjects(const std::string& database, RowCounts rowCounts);
	std::vector<FileInfo> listFiles(Poco::UInt64& totalBytes) const;
	void fillQueryResult(QueryResult& result, Statement& stmt, std::size_t maxRows);

	MemoryDB&   _db;
	SQLPolicy   _policy;
	std::size_t _maxRows;
};


//
// inlines
//
inline MemoryDBInspector::SQLPolicy MemoryDBInspector::policy() const
{
	return _policy;
}


inline std::size_t MemoryDBInspector::maxRows() const
{
	return _maxRows;
}


inline void MemoryDBInspector::setMaxRows(std::size_t maxRows)
{
	_maxRows = maxRows;
}


inline const std::string& MemoryDBInspector::directory() const
{
	return _db.directory();
}


inline MemoryDBInspector::QueryResult MemoryDBInspector::query(const std::string& sql)
{
	return query(sql, _maxRows);
}


} // namespace Poco::Data::SQLite


#endif // SQLite_MemoryDBInspector_INCLUDED
