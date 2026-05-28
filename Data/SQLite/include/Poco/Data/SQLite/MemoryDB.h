//
// MemoryDB.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  MemoryDB
//
// Definition of the MemoryDB class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLite_MemoryDB_INCLUDED
#define SQLite_MemoryDB_INCLUDED


#ifdef POCO_DATA_NO_SQL_PARSER
#error "MemoryDB requires the bundled SQL parser; rebuild without POCO_DATA_NO_SQL_PARSER."
#endif


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Timer.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/Mutex.h"
#include "Poco/Types.h"
#include "Poco/Exception.h"
#include <atomic>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>


extern "C"
{
	typedef struct sqlite3 sqlite3;
}


namespace Poco::Data::SQLite {


class SQLite_API MemoryDB
	/// An in-memory SQLite database with robust, sharded persistence to disk.
	///
	/// MemoryDB keeps the whole dataset in an in-memory SQLite database (fast, no
	/// per-statement disk I/O) and persists it to a directory of shard files so the data
	/// survives process restarts. It is used like a regular Poco::Data::Session:
	///
	///     MemoryDB db("/var/lib/myapp/db");
	///     db << "CREATE TABLE t(id INTEGER PRIMARY KEY, name TEXT)", now;
	///     db << "INSERT INTO t(name) VALUES(:n)", use(name), now;
	///     int n = 0;
	///     db << "SELECT count(*) FROM t", into(n), now;
	///
	/// Persistence is incremental and bounded in file size: data is partitioned into
	/// disjoint shard files. The *active* shard receives new rows and is the only file
	/// rewritten on a normal flush; once it reaches a configurable size or age it is
	/// *sealed* (made immutable) and a new active shard is started. The full dataset is
	/// the union of all shards. A flush happens when the database has been idle for a
	/// configurable interval (default 30s), at a maximum interval regardless of activity,
	/// on destruction, and on demand via flush().
	///
	/// Robustness: each shard is written to a temp file in the same directory and
	/// atomically renamed over the live shard file, then a single transaction on the
	/// catalog database (manifest.db) commits the new shard metadata. A crash
	/// therefore always leaves the catalog and the files it references mutually
	/// consistent; worst-case data loss is bounded by the idle/max-flush window.
	///
	/// Change detection uses the SQLite statement trace plus the bundled SQL parser, and
	/// the SQLite update hook, so it observes every statement on the connection regardless
	/// of whether it was issued through this object or the underlying Session.
	///
	/// Limitations:
	///   * WITHOUT ROWID tables are not supported. The sharding model partitions rows by
	///     rowid range, so a table that has no rowid cannot be persisted under this
	///     scheme. Any statement passed through operator<< that contains a CREATE TABLE
	///     ... WITHOUT ROWID clause is rejected up front with Poco::NotImplementedException
	///     — the underlying in-memory database is never touched, so the MemoryDB
	///     instance stays usable; just rewrite the table as a normal rowid table. As a
	///     backstop, statements that bypass operator<< (e.g. by going through session()
	///     directly) are caught reactively via the SQLite trace hook and poison the
	///     instance: every subsequent call throws until the instance is discarded.
	///   * The sharding scheme is most efficient when older data stops changing; correctness
	///     is preserved if it does not, but modifying rows in many sealed shards causes those
	///     shards to be rewritten.
	///   * Do not run the VACUUM command against the database (see PRAGMA notes in the docs);
	///     it can renumber rowids and break shard ownership.
	///   * Total shard count is capped at sqlite3_limit(SQLITE_LIMIT_ATTACHED) on the
	///     in-memory connection (SQLite default 10, hard ceiling 125). This is the same
	///     limit that bounds attachAllArchived() and wide-range historyView(); rather
	///     than letting sealed shards pile up past it and break those calls, every flush
	///     drops the oldest sealed shard(s) to stay within the cap and logs a warning to
	///     "Poco.Data.SQLite.MemoryDB". To avoid the auto-drop, raise Options::shardMaxBytes
	///     so seals fire less often, set Options::retentionMaxAge or retentionMaxBytes to
	///     drop older shards proactively (by your policy rather than SQLite's), or raise
	///     SQLITE_LIMIT_ATTACHED via sqlite3_limit() on session() before adding workload.
{
public:
	struct Options
	{
		Poco::Timespan idleInterval     = Poco::Timespan(30, 0);
			/// Flush after the database has been idle (no writes) for this long.

		Poco::Timespan maxFlushInterval = Poco::Timespan(300, 0);
			/// Force a flush at least this often even under continuous writes. Zero disables.

		Poco::UInt64   shardMaxBytes    = 256ULL * 1024 * 1024;
			/// Seal the active shard once its on-disk size reaches this many bytes. Zero disables.

		Poco::Timespan shardMaxAge      = Poco::Timespan(3600, 0);
			/// Seal the active shard once it is this old. Zero disables.

		long           checkIntervalMs  = 1000;
			/// Granularity of the background flush timer, in milliseconds.

		bool           loadArchivedShards = false;
			/// If false (the default), only the active shard is RAM-resident; sealed shards
			/// stay on disk and are read on demand (the SQLite page cache handles paging
			/// transparently when they are attached via attachArchived()/historyView()).
			/// This keeps memory bounded regardless of how much history accumulates and is
			/// the right choice for any workload where data grows over time.
			/// Set to true to merge every sealed shard into memory on open, so plain
			/// SELECTs see the full dataset without attaching anything. Only safe when the
			/// total dataset comfortably fits in RAM.

		std::function<std::string(Poco::UInt32 shardId, const Poco::Timestamp& createdAt)> shardNamer;
			/// Optional shard file naming policy. If empty, files are named "shard-%06u.db".
			/// The created-time is supplied so a policy can embed a timestamp. The catalog
			/// records the resulting filename, so reload never depends on this policy.

		Poco::Timespan retentionMaxAge = Poco::Timespan(0);
			/// Drop sealed shards older than this. Zero disables the age policy.

		Poco::UInt64 retentionMaxBytes = 0;
			/// Drop oldest sealed shards until the total size of all shard files is at most
			/// this many bytes. Zero disables the size policy.
			///
			/// Note: total shard count is always also capped at sqlite3_limit(
			/// SQLITE_LIMIT_ATTACHED) on session() (SQLite default 10). See the class-level
			/// "Limitations" note; that backstop is independent of this knob.
	};

	explicit MemoryDB(const std::string& dir);
		/// Creates (or opens) a MemoryDB persisted in directory dir using default options.

	MemoryDB(const std::string& dir, const Options& options);
		/// Creates (or opens) a MemoryDB persisted in directory dir. If the directory contains
		/// a catalog, its shards are loaded; otherwise an empty database is created.
		/// Throws if SQLite is not built thread-safe, or if an existing catalog/shard is corrupt.

	~MemoryDB();
		/// Stops the flush timer, performs a final flush if there are unpersisted changes,
		/// and closes the database.

	MemoryDB(const MemoryDB&) = delete;
	MemoryDB& operator = (const MemoryDB&) = delete;

	template <typename T>
	Statement operator << (const T& t)
		/// Creates a Statement, forwarding to the underlying in-memory Session. This enables
		/// the usual db << "SQL", into(x), use(y), now; syntax. Statements containing a
		/// CREATE TABLE ... WITHOUT ROWID clause are rejected up front (the in-memory DB
		/// is left untouched, so this instance stays usable).
	{
		throwIfRejected();
		if constexpr (std::is_convertible_v<const T&, std::string_view>)
		{
			if (mentionsWithoutRowid(std::string_view(t)))
				throw Poco::NotImplementedException("MemoryDB does not support WITHOUT ROWID tables");
		}
		return _session << t;
	}

	Session& session();
		/// Returns the underlying in-memory Session, for APIs that require a Session&
		/// (e.g. Poco::Data::Transaction, RecordSet).

	operator Session& ();
		/// Implicit conversion to the underlying in-memory Session.

	void flush();
		/// Attempts to persist all unpersisted changes to disk.
		///
		/// Best-effort under concurrency: if another flush (timer-driven, user-called,
		/// or destructor-triggered) is already running, this call returns immediately
		/// without waiting and without doing anything. Use dirty() to check whether
		/// a follow-up call is needed for full durability:
		///
		///     while (db.dirty()) db.flush();   // or sleep+retry in a hot loop
		///
		/// The skip-on-busy semantic avoids cascading waits on _flushMutex when many
		/// threads attempt to flush in parallel.

	void sealActive();
		/// Forces the active shard to be sealed at the next flush, starting a new active shard.

	bool dirty() const;
		/// Returns true if there are changes that have not yet been persisted.

	std::size_t shardCount() const;
		/// Returns the total number of shards (sealed plus the active one).

	const std::string& directory() const;
		/// Returns the persistence directory.

	struct ShardDescriptor
		/// A read-only snapshot of one shard's metadata. Returned by shards(); used for
		/// inspection and for policy decisions (e.g. retention by sealedAt).
	{
		Poco::UInt32    id;          // shard id (also the order in which shards were created)
		bool            sealed;      // true if sealed and immutable; false for the active shard
		Poco::Timestamp createdAt;   // when the shard was first opened in memory
		Poco::Timestamp sealedAt;    // when the shard was sealed; epoch 0 if !sealed
		Poco::UInt64    bytes;       // on-disk size as of the last write (0 if never written)
		std::string     filename;    // basename of the shard file in the persistence directory
	};

	std::vector<ShardDescriptor> shards() const;
		/// Returns a snapshot of every shard's metadata, ordered by id. Useful for
		/// retention policies, e.g.
		///
		///     for (const auto& d : db.shards())
		///         if (d.sealed && d.sealedAt < cutoff) db.deleteShard(d.id);

	std::vector<Poco::UInt32> archivedShardIds() const;
		/// Returns the ids of sealed shards. Relevant when Options::loadArchivedShards is false.

	std::string attachArchived(Poco::UInt32 shardId);
		/// Attaches the given sealed shard read-only to the in-memory connection and returns the
		/// schema alias under which it can be queried (e.g. "arc_7" -> "SELECT * FROM arc_7.t").
		/// No-op-safe if already attached. Relevant when Options::loadArchivedShards is false.
		/// Thread-safe: concurrent attach/detach/historyView calls serialize via an internal mutex.

	void detachArchived(Poco::UInt32 shardId);
		/// Detaches a previously attached sealed shard. Thread-safe (see attachArchived).

	void attachAllArchived();
		/// Attaches every sealed shard read-only that is not already attached. Convenient
		/// when history queries become needed in a session; sealed shards remain on disk
		/// and are demand-paged by SQLite. Thread-safe (see attachArchived).

	void deleteShard(Poco::UInt32 shardId);
		/// Permanently removes the given sealed shard from the catalog and deletes its
		/// file from disk. Throws Poco::InvalidArgumentException if the shard is the
		/// active one (seal it first if you really want to drop in-flight data) and
		/// Poco::NotFoundException if no such shard exists. If the shard is currently
		/// attached read-only via attachArchived(), it is detached first. In
		/// loadArchivedShards=true mode the shard's rows are also in main; they are
		/// deleted from the in-memory tables so the drop actually removes the data,
		/// not just the disk copy. The removal is crash-safe: the manifest transaction
		/// commits before the file is unlinked, so a crash in the window leaves an
		/// orphan file that cleanOrphans() removes on the next open.

	std::size_t enforceRetention();
		/// Applies the Options-driven retention policy (retentionMaxAge, retentionMaxBytes)
		/// plus the always-on SQLITE_LIMIT_ATTACHED backstop on total shard count.
		/// Called automatically at the end of every successful flush; safe to call
		/// manually at any time. Returns the number of shards dropped. When the backstop
		/// fires, logs a warning to the "Poco.Data.SQLite.MemoryDB" logger naming the
		/// cap and the four ways to avoid the auto-drop.

	std::size_t dropOlderThan(const Poco::Timestamp& cutoff);
		/// Drops every sealed shard whose sealedAt is strictly before cutoff. Returns the
		/// number of shards dropped.

	std::size_t dropToFit(Poco::UInt64 maxTotalBytes);
		/// Drops oldest sealed shards until the sum of all shard files' bytes is at most
		/// maxTotalBytes, or no more sealed shards remain. Returns the number dropped.

	std::string historyView(const std::string& table);
		/// Creates (or refreshes) a read-only TEMP VIEW spanning the in-memory base table
		/// and every currently-attached sealed shard's same-named table via UNION ALL.
		/// Returns the view name ("<table>_history"). The view is read-only by virtue of
		/// being a SQLite view without INSTEAD OF triggers, so writes to it fail with a
		/// clear SQLite error. If Options::loadArchivedShards is true, sealed data is
		/// already merged into main, so the view simply aliases the base table to keep
		/// the call site uniform across modes. Thread-safe (see attachArchived).

	std::string historyView(const std::string& table,
		const Poco::Timestamp& from, const Poco::Timestamp& to);
		/// Same as historyView(table), but auto-attaches only the sealed shards whose
		/// time window [createdAt, sealedAt] intersects [from, to] before building the
		/// view, and unions only those. Useful for "show me a specific window in
		/// history" queries. Thread-safe (see attachArchived).
		///
		/// Throws Poco::RangeException if the range requires more sealed shards than
		/// the connection can have attached simultaneously (sqlite3_limit(
		/// SQLITE_LIMIT_ATTACHED) minus one for the POCO-attached "sys" database, which
		/// is 9 in the bundled build). Narrow the range or query shards in batches.
		///
		/// When Options::loadArchivedShards is true, sealed data is already merged into
		/// main; the time range is silently ignored and the returned view aliases the
		/// base table. Filter by your time column in the query.
		///
		/// Note: the [createdAt, sealedAt] window approximates the data's time span
		/// accurately for real-time ingestion. If your inserts use back-dated
		/// timestamps, filter further with a WHERE clause on your time column.

	void detachAllArchived();
		/// Detaches every sealed shard currently attached read-only. Useful after a
		/// burst of history queries to release the per-shard SQLite page caches.
		/// Thread-safe (see attachArchived).

private:
	struct ShardInfo
	{
		Poco::UInt32    id = 0;
		std::string     file;        // current filename ("" until first written)
		bool            sealed = false;
		Poco::Timestamp created;
		Poco::Timestamp sealedAt;
		int             schemaVersion = 0;
		Poco::UInt64    bytes = 0;    // size of the file as last written
		bool            dirty = false;
		std::map<std::string, std::pair<Poco::Int64, Poco::Int64> > ranges; // table -> (lo, hi]
	};

	// SQLite C callbacks (trampolines -> instance methods)
	static int  traceCallback(unsigned type, void* ctx, void* p, void* x);
	static void updateCallback(void* ctx, int op, const char* db, const char* table, long long row);

	void onStatement(const char* sql);                  // user thread
	void onRowChange(int op, const char* table, Poco::Int64 row); // user thread
	void onDDL(const char* sql, const char* table);     // user thread
	void onTimer(Poco::Timer& timer);                   // timer thread

	static std::string prepare(const std::string& dir); // register connector + create dir, returns absolute dir
	static std::string makeMemName();                   // unique shared-cache in-memory URI
	static bool mentionsWithoutRowid(std::string_view sql);

	struct ColumnCopy
	{
		std::string insertCols; // "" means use the default (all) columns
		std::string selectList; // "*" or "rowid","c1",...
	};

	void open();
	void load();
	void startFresh();
	void registerHooks();
	void doFlush();
	void writeCatalog(int schemaVersion, const std::vector<std::string>& schemaLog);
	std::string buildHistoryView(const std::string& table, const std::vector<Poco::UInt32>& shardIds);
	void maybeSeal(const std::vector<std::string>& tables,
		const std::map<std::string, Poco::Int64>& maxRowids); // caller holds _stateMutex
	void migrateShardFile(ShardInfo& shard, int toVersion, const std::vector<std::string>& schemaLog);
	std::vector<std::string> userTables(Session& s);
	Poco::Int64 maxRowid(const std::string& table);
	ColumnCopy columnCopy(Session& s, const std::string& table); // cached by table name
	void markTableDirty(const std::string& table);      // caller holds _stateMutex
	ShardInfo* activeShard();                            // caller holds _stateMutex
	ShardInfo* owningShard(const std::string& table, Poco::Int64 row); // caller holds _stateMutex
	void rebuildActiveLo();                              // caller holds _stateMutex
	void throwIfRejected() const;
	std::string shardPath(Poco::UInt32 id, const Poco::Timestamp& createdAt) const;
	void cleanOrphans();

	std::string                _dir;
	std::string                _memName;     // shared-cache in-memory URI name
	Options                    _opts;
	Session                    _session;      // user-facing in-memory connection
	Session                    _persist;      // second connection to the same in-memory db (flush/IO)
	Session                    _catalog;      // on-disk manifest.db
	sqlite3*                   _memHandle = nullptr;

	std::vector<ShardInfo>     _shards;
	int                        _schemaVersion = 0;
	std::vector<std::string>   _schemaLog;    // ddl statements, index == version-1
	Poco::UInt32               _nextShardId = 1;
	bool                       _dirty = false;
	bool                       _sealRequested = false;
	Poco::Timestamp            _lastChange;
	Poco::Timestamp            _lastFlush;

	std::string                _rejected;     // non-empty => a prohibited operation occurred
	std::map<Poco::UInt32, int> _attached;    // sealed shards attached read-only,
	                                          // value = reference count (matched attach calls)

	// O(1) common-case for owningShard: per-table max sealed-shard hi (=lo of the
	// active shard's range). Recomputed in load()/maybeSeal()/deleteShard().
	std::unordered_map<std::string, Poco::Int64> _activeLo;

	// Cache of ColumnCopy results keyed by table name. Schemas are stable per
	// MemoryDB instance, so this is safe to share across sessions; cleared on
	// every DDL in onDDL().
	std::unordered_map<std::string, ColumnCopy> _columnCopyCache;

	Poco::Timer                _timer;
	mutable Poco::FastMutex    _stateMutex;   // guards shard bookkeeping / dirty / timestamps
	Poco::Mutex                _flushMutex;   // serializes flushes
	Poco::Mutex                _attachMutex;  // serializes attach/detach/historyView
	std::atomic<bool>          _flushing{false}; // CAS-bail gate on doFlush re-entry
};


//
// inlines
//
inline Session& MemoryDB::session()
{
	return _session;
}


inline MemoryDB::operator Session& ()
{
	return _session;
}


inline const std::string& MemoryDB::directory() const
{
	return _dir;
}


} // namespace Poco::Data::SQLite


#endif // SQLite_MemoryDB_INCLUDED
