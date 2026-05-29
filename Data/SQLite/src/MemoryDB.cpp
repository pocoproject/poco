//
// MemoryDB.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  MemoryDB
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/Session.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Thread.h"
#include "Poco/Logger.h"
#include "Poco/Exception.h"
#include <sqlite3.h>
#include <algorithm>
#include <atomic>
#include <cctype>
#include <set>

#include "SQLParser.h"
#include "sql/SQLStatement.h"
#include "sql/CreateStatement.h"
#include "sql/DropStatement.h"
#include "sql/DeleteStatement.h"
#include "sql/AlterStatement.h"


using namespace Poco::Data::Keywords;


namespace Poco::Data::SQLite {


// Bumped whenever the on-disk manifest schema changes incompatibly. Older builds
// may read newer catalogs only if every field they need is present and meaningful.
constexpr int MemoryDB_CATALOG_VERSION = 1;


namespace
{
	// Thread-local guard set by MemoryDB's own helpers when they issue SQL on _session
	// purely for internal bookkeeping (e.g. building the read-only history view). The
	// trace callback returns early when this is set, so internal DDL doesn't pollute
	// schema_log, doesn't mark the database dirty, and doesn't trigger a flush.
	thread_local bool t_inInternal = false;

	struct InternalGuard
	{
		bool prev;
		InternalGuard(): prev(t_inInternal) { t_inInternal = true; }
		~InternalGuard() { t_inInternal = prev; }
	};


	std::string quoteIdent(const std::string& s)
	{
		std::string r("\"");
		for (char c: s)
		{
			if (c == '"') r += "\"\"";
			else r += c;
		}
		r += '"';
		return r;
	}


	std::string quoteLit(const std::string& s)
	{
		std::string r("'");
		for (char c: s)
		{
			if (c == '\'') r += "''";
			else r += c;
		}
		r += '\'';
		return r;
	}


	// Returns true if the lowercased tokens [first, first+n) in `s` (starting at
	// offset `pos`, treating the next n whitespace-separated identifiers) match
	// `words`. Used to detect "create table" / "drop index" prefixes.
	bool tokenMatch(const std::string& lower, std::size_t pos, std::initializer_list<const char*> words)
	{
		for (const char* w: words)
		{
			while (pos < lower.size() && std::isspace(static_cast<unsigned char>(lower[pos]))) ++pos;
			std::size_t end = pos;
			while (end < lower.size() && std::isalpha(static_cast<unsigned char>(lower[end]))) ++end;
			std::string tok = lower.substr(pos, end - pos);
			if (tok != w) return false;
			pos = end;
		}
		return true;
	}

	// Rewrites a CREATE/DROP DDL statement to its idempotent form by inserting
	// IF NOT EXISTS (after CREATE TABLE|INDEX|VIEW|TRIGGER) or IF EXISTS (after
	// DROP TABLE|INDEX|VIEW|TRIGGER), when not already present. Other DDL
	// (ALTER, CREATE VIRTUAL TABLE, etc.) is returned unchanged. Stored in the
	// schema log so that both load() replay (against a fresh _persist) and
	// migrateShardFile() replay (against a partially-migrated shard file) are
	// safe to run more than once.
	std::string addIfExistsClause(const std::string& sql)
	{
		std::string lower;
		lower.reserve(sql.size());
		for (char c: sql) lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

		std::size_t i = 0;
		while (i < lower.size() && std::isspace(static_cast<unsigned char>(lower[i]))) ++i;
		std::size_t firstStart = i;
		while (i < lower.size() && std::isalpha(static_cast<unsigned char>(lower[i]))) ++i;
		std::string verb = lower.substr(firstStart, i - firstStart);

		const char* clause = nullptr;
		if (verb == "create") clause = "IF NOT EXISTS ";
		else if (verb == "drop") clause = "IF EXISTS ";
		else return sql;

		// Skip whitespace, then the object-type keyword (TABLE/INDEX/VIEW/TRIGGER).
		std::size_t typeStart = i;
		while (typeStart < lower.size() && std::isspace(static_cast<unsigned char>(lower[typeStart]))) ++typeStart;
		std::size_t typeEnd = typeStart;
		while (typeEnd < lower.size() && std::isalpha(static_cast<unsigned char>(lower[typeEnd]))) ++typeEnd;
		std::string type = lower.substr(typeStart, typeEnd - typeStart);
		// Only the simple object types; leave CREATE VIRTUAL TABLE / CREATE UNIQUE INDEX
		// (which would need more tokens to find the insertion point) alone for now -
		// migrateShardFile's catch-and-continue handles them.
		if (type != "table" && type != "index" && type != "view" && type != "trigger")
			return sql;

		// Already idempotent?
		if (tokenMatch(lower, typeEnd, verb == "create" ? std::initializer_list<const char*>{"if","not","exists"}
		                                                : std::initializer_list<const char*>{"if","exists"}))
			return sql;

		// Insert clause right after the object-type keyword (preserve original whitespace).
		std::string out;
		out.reserve(sql.size() + 16);
		out.append(sql, 0, typeEnd);
		out.push_back(' ');
		out.append(clause);
		std::size_t rest = typeEnd;
		while (rest < sql.size() && std::isspace(static_cast<unsigned char>(sql[rest]))) ++rest;
		out.append(sql, rest, std::string::npos);
		return out;
	}


	std::string firstKeyword(const char* sql)
	{
		std::string kw;
		const char* p = sql;
		while (*p && std::isspace(static_cast<unsigned char>(*p))) ++p;
		while (*p && (std::isalpha(static_cast<unsigned char>(*p))))
		{
			kw += static_cast<char>(std::toupper(static_cast<unsigned char>(*p)));
			++p;
		}
		return kw;
	}


	// (ColumnCopy / columnCopy moved to be a MemoryDB member so they can share
	// _columnCopyCache - see MemoryDB::columnCopy below.)
} // anonymous namespace


MemoryDB::ColumnCopy MemoryDB::columnCopy(Session& s, const std::string& table)
{
	// Schemas are stable per MemoryDB instance (all sessions see the same DDL),
	// so cache by table name. onDDL clears the cache and bumps _schemaVersion
	// on schema change. We snapshot _schemaVersion before releasing the lock
	// and only insert into the cache if no DDL happened in the meantime;
	// otherwise our pragma_table_info result reflects the pre-DDL schema and
	// installing it would resurrect a stale ColumnCopy, silently dropping
	// values of newly-added columns on the next flush.
	int snapshotSchemaVersion;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		auto it = _columnCopyCache.find(table);
		if (it != _columnCopyCache.end()) return it->second;
		snapshotSchemaVersion = _schemaVersion;
	}

	std::vector<std::string> names;
	std::vector<std::string> types;
	std::vector<int> pks;
	s << ("SELECT name, type, pk FROM pragma_table_info(" + quoteLit(table) + ")"),
		into(names), into(types), into(pks), now;

	int pkCount = 0;
	std::string pkType;
	for (std::size_t i = 0; i < pks.size(); ++i)
	{
		if (pks[i] != 0)
		{
			++pkCount;
			pkType = types[i];
		}
	}
	std::string upperPkType;
	for (char c: pkType) upperPkType += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

	ColumnCopy cc;
	if (pkCount == 1 && upperPkType == "INTEGER")
	{
		// INTEGER PRIMARY KEY is an alias for rowid; SELECT * preserves it.
		cc.selectList = "*";
		cc.insertCols.clear();
	}
	else
	{
		std::string ins("(\"rowid\"");
		std::string sel("\"rowid\"");
		for (const auto& n: names)
		{
			ins += ',' + quoteIdent(n);
			sel += ',' + quoteIdent(n);
		}
		ins += ')';
		cc.insertCols = ins;
		cc.selectList = sel;
	}

	Poco::FastMutex::ScopedLock l(_stateMutex);
	if (_schemaVersion == snapshotSchemaVersion)
		_columnCopyCache[table] = cc;
	return cc;
}


//
// construction / lifecycle
//
MemoryDB::MemoryDB(const std::string& dir):
	MemoryDB(dir, Options())
{
}


MemoryDB::MemoryDB(const std::string& dir, const Options& options):
	_dir(prepare(dir)),
	_memName(makeMemName()),
	_opts(options),
	_session(Connector::KEY, _memName),
	_persist(Connector::KEY, _memName),
	_catalog(Connector::KEY, Poco::Path(_dir, "manifest.db").toString())
{
	if (!Utility::isThreadSafe())
		throw Poco::NotImplementedException("MemoryDB requires SQLite built in thread-safe mode");

	_memHandle = Utility::dbHandle(_session);
	_persist << "PRAGMA read_uncommitted = true", now;

	open();

	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		_dirty = false;
		_lastFlush.update();
		_lastChange.update();
	}

	registerHooks();

	_timer.setStartInterval(_opts.checkIntervalMs);
	_timer.setPeriodicInterval(_opts.checkIntervalMs);
	_timer.start(Poco::TimerCallback<MemoryDB>(*this, &MemoryDB::onTimer));
}


MemoryDB::~MemoryDB()
{
	try { _timer.stop(); }
	catch (...) {}

	if (_memHandle)
	{
		sqlite3_trace_v2(_memHandle, SQLITE_TRACE_STMT, nullptr, nullptr);
		sqlite3_update_hook(_memHandle, nullptr, nullptr);
	}

	// Unconditional final flush, with allowSeal=false so we don't introduce
	// fresh shards on shutdown (a shardMaxBytes-driven seal at this point
	// would create an empty shard that the next open() would see). Two
	// reasons not to gate on dirty():
	//   1. The H2 scenario (CREATE TABLE in the sample-to-lock window) can
	//      exit a prior doFlush with _dirty==false while a new-table row is
	//      still unpersisted. A second, unconditional pass re-samples
	//      userTables and catches it.
	//   2. _rejected being set must not strand legitimately-dirty data; the
	//      onTimer gate already filters _rejected, so the dtor was the
	//      asymmetric path. doFlush still throws on _rejected, but we let
	//      the try/catch swallow it - the same outcome as before for the
	//      poisoned case, with the correct outcome for the common case.
	try { doFlush(false); }
	catch (...) {}
}


std::string MemoryDB::prepare(const std::string& dir)
{
	try { Connector::registerConnector(); }
	catch (...) {} // already registered

	std::string abs = Poco::Path(dir).absolute().toString();
	Poco::File(abs).createDirectories();
	return abs;
}


std::string MemoryDB::makeMemName()
{
	static std::atomic<Poco::UInt64> counter(0);
	return "file:poco_memdb_" + Poco::NumberFormatter::format(++counter) + "?mode=memory&cache=shared";
}


bool MemoryDB::mentionsWithoutRowid(std::string_view sql)
{
	// fast path: only CREATE TABLE can carry a WITHOUT ROWID clause
	std::size_t i = 0;
	while (i < sql.size() && std::isspace(static_cast<unsigned char>(sql[i]))) ++i;
	if (sql.size() - i < 6) return false;
	const char create[] = {'c', 'r', 'e', 'a', 't', 'e'};
	for (std::size_t k = 0; k < 6; ++k)
		if (std::tolower(static_cast<unsigned char>(sql[i + k])) != create[k]) return false;

	// full normalized (lowercased, whitespace-collapsed) scan for "without rowid"
	std::string norm;
	norm.reserve(sql.size());
	bool prevSpace = false;
	for (char c: sql)
	{
		if (std::isspace(static_cast<unsigned char>(c)))
		{
			if (!prevSpace) norm += ' ';
			prevSpace = true;
		}
		else
		{
			norm += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			prevSpace = false;
		}
	}
	return norm.find("without rowid") != std::string::npos;
}


void MemoryDB::registerHooks()
{
	sqlite3_trace_v2(_memHandle, SQLITE_TRACE_STMT, &MemoryDB::traceCallback, this);
	sqlite3_update_hook(_memHandle, &MemoryDB::updateCallback, this);
}


//
// catalog / load
//
void MemoryDB::open()
{
	_catalog << "CREATE TABLE IF NOT EXISTS meta(k TEXT PRIMARY KEY, ival INTEGER)", now;
	_catalog << "CREATE TABLE IF NOT EXISTS schema_log(version INTEGER PRIMARY KEY, ddl TEXT)", now;
	_catalog << "CREATE TABLE IF NOT EXISTS shards(id INTEGER PRIMARY KEY, filename TEXT, sealed INTEGER, "
		"created INTEGER, sealed_at INTEGER, schema_version INTEGER, bytes INTEGER)", now;
	_catalog << "CREATE TABLE IF NOT EXISTS shard_ranges(shard_id INTEGER, tbl TEXT, lo INTEGER, hi INTEGER, "
		"PRIMARY KEY(shard_id, tbl))", now;

	int shardCnt = 0;
	_catalog << "SELECT COUNT(*) FROM shards", into(shardCnt), now;
	if (shardCnt > 0) load();
	else startFresh();
}


void MemoryDB::startFresh()
{
	ShardInfo a;
	a.id = _nextShardId++;
	a.created.update();
	a.sealed = false;
	a.dirty = false;
	a.schemaVersion = 0;
	_shards.push_back(a);
	_schemaVersion = 0;
}


void MemoryDB::load()
{
	// meta
	auto readMeta = [this](const std::string& key, Poco::Int64 def) -> Poco::Int64
	{
		Poco::Int64 v = def;
		_catalog << ("SELECT ival FROM meta WHERE k=" + quoteLit(key)), into(v), now;
		return v;
	};
	Poco::Int64 catVer = readMeta("catalogVersion", 1); // default 1 for pre-versioning catalogs
	if (catVer > MemoryDB_CATALOG_VERSION)
	{
		throw Poco::NotImplementedException(
			"manifest.db is at catalogVersion " + Poco::NumberFormatter::format(catVer) +
			", this build of MemoryDB only understands up to " +
			Poco::NumberFormatter::format(MemoryDB_CATALOG_VERSION));
	}
	_schemaVersion = static_cast<int>(readMeta("schemaVersion", 0));
	_nextShardId = static_cast<Poco::UInt32>(readMeta("nextShardId", 1));

	// schema log
	_schemaLog.clear();
	_catalog << "SELECT ddl FROM schema_log ORDER BY version", into(_schemaLog), now;

	// recreate the in-memory schema by replaying the DDL log in order
	for (const auto& ddl: _schemaLog)
		_persist << ddl, now;

	// shards
	std::vector<Poco::Int64> ids, sealedv, createdv, sealedatv, svv, bytesv;
	std::vector<std::string> files;
	_catalog << "SELECT id, filename, sealed, created, sealed_at, schema_version, bytes FROM shards ORDER BY id",
		into(ids), into(files), into(sealedv), into(createdv), into(sealedatv), into(svv), into(bytesv), now;

	_shards.clear();
	for (std::size_t i = 0; i < ids.size(); ++i)
	{
		ShardInfo s;
		s.id = static_cast<Poco::UInt32>(ids[i]);
		s.file = files[i];
		s.sealed = sealedv[i] != 0;
		s.created = Poco::Timestamp(createdv[i]);
		s.sealedAt = Poco::Timestamp(sealedatv[i]);
		s.schemaVersion = static_cast<int>(svv[i]);
		s.bytes = static_cast<Poco::UInt64>(bytesv[i]);
		s.dirty = false;
		_shards.push_back(s);
	}

	// ranges
	std::vector<Poco::Int64> rsid, rlo, rhi;
	std::vector<std::string> rtbl;
	_catalog << "SELECT shard_id, tbl, lo, hi FROM shard_ranges", into(rsid), into(rtbl), into(rlo), into(rhi), now;
	for (std::size_t i = 0; i < rsid.size(); ++i)
	{
		for (auto& s: _shards)
		{
			if (s.id == static_cast<Poco::UInt32>(rsid[i]))
			{
				s.ranges[rtbl[i]] = std::make_pair(rlo[i], rhi[i]);
				break;
			}
		}
	}
	rebuildActiveLo();

	// Snapshot user-created indexes and drop them so the bulk merge below doesn't pay
	// per-row B-tree maintenance for every index. PRIMARY KEY / UNIQUE auto-indexes have
	// sql IS NULL and cannot be dropped; the uniqueness checks they do are part of
	// correctness and stay enabled. We recreate the dropped indexes once at the end,
	// which builds each as a single sort+B-tree pass over the now-populated tables.
	std::vector<std::string> ixNames, ixDdls;
	_persist << "SELECT name, sql FROM sqlite_master "
		"WHERE type='index' AND sql IS NOT NULL ORDER BY rowid",
		into(ixNames), into(ixDdls), now;
	for (const auto& n: ixNames)
		_persist << ("DROP INDEX " + quoteIdent(n)), now;

	try
	{
		// merge shard data into memory (active always; sealed only if requested)
		std::vector<std::string> mainTables = userTables(_persist);
		for (const auto& s: _shards)
		{
			bool inMemory = !s.sealed || _opts.loadArchivedShards;
			if (!inMemory || s.file.empty()) continue;

			std::string path = Poco::Path(_dir, s.file).toString();
			if (!Poco::File(path).exists())
				throw Poco::FileNotFoundException("MemoryDB shard missing", path);

			Session ss(Connector::KEY, path);
			ss << ("ATTACH DATABASE " + quoteLit(_memName) + " AS mem"), now;
			try
			{
				std::vector<std::string> shardTables = userTables(ss);
				for (const auto& t: shardTables)
				{
					bool inMain = false;
					for (const auto& mt: mainTables) if (mt == t) { inMain = true; break; }
					if (!inMain) continue;

					ColumnCopy cc = columnCopy(ss, t);
					std::string sql = "INSERT INTO mem." + quoteIdent(t);
					if (!cc.insertCols.empty()) sql += " " + cc.insertCols;
					sql += " SELECT " + cc.selectList + " FROM main." + quoteIdent(t);
					ss << sql, now;
				}
			}
			catch (...)
			{
				try { ss << "DETACH DATABASE mem", now; } catch (...) {}
				throw;
			}
			ss << "DETACH DATABASE mem", now;
		}
	}
	catch (...)
	{
		// best-effort: leave the schema consistent even if the merge failed
		for (const auto& d: ixDdls)
		{
			try { _persist << d, now; } catch (...) {}
		}
		throw;
	}

	for (const auto& d: ixDdls)
		_persist << d, now;

	cleanOrphans();
}


void MemoryDB::cleanOrphans()
{
	std::set<std::string> referenced;
	for (const auto& s: _shards)
		if (!s.file.empty()) referenced.insert(s.file);

	std::vector<std::string> entries;
	Poco::File(_dir).list(entries);
	for (const auto& e: entries)
	{
		if (e.rfind("manifest.db", 0) == 0) continue; // never touch the catalog or its journal/wal
		// remove stray temp files and unreferenced shard files
		if (referenced.find(e) == referenced.end())
		{
			try { Poco::File(Poco::Path(_dir, e).toString()).remove(); }
			catch (...) {}
		}
	}
}


//
// session-facing helpers
//
void MemoryDB::throwIfRejected() const
{
	// Fast path: noexcept atomic load. The vast majority of calls hit this
	// and never need the mutex.
	if (!_poisoned.load(std::memory_order_acquire)) return;
	Poco::FastMutex::ScopedLock l(_stateMutex);
	if (!_rejected.empty()) throw Poco::NotImplementedException(_rejected);
	// _poisoned set but _rejected empty: heap was out at the time the trace
	// hook tried to record the message. Throw with a static reason so the
	// caller still sees a clear failure.
	throw Poco::NotImplementedException(
		"MemoryDB rejected a prohibited operation (out of memory recording details)");
}


bool MemoryDB::dirty() const
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	return _dirty;
}


Poco::Timestamp MemoryDB::lastFlushTime() const
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	return _lastFlush;
}


std::size_t MemoryDB::shardCount() const
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	return _shards.size();
}


void MemoryDB::sealActive()
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	_sealRequested = true;
	_dirty = true;
}


std::vector<Poco::UInt32> MemoryDB::archivedShardIds() const
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	std::vector<Poco::UInt32> ids;
	for (const auto& s: _shards)
		if (s.sealed) ids.push_back(s.id);
	return ids;
}


std::vector<MemoryDB::ShardDescriptor> MemoryDB::shards() const
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	std::vector<ShardDescriptor> out;
	out.reserve(_shards.size());
	for (const auto& s: _shards)
	{
		ShardDescriptor d;
		d.id = s.id;
		d.sealed = s.sealed;
		d.createdAt = s.created;
		// sealedAt is only meaningful once the shard is sealed; report epoch-0 otherwise
		// (Poco::Timestamp's default constructor returns "now", which would be misleading here).
		d.sealedAt = s.sealed ? s.sealedAt : Poco::Timestamp(0);
		d.bytes = s.bytes;
		d.filename = s.file;
		out.push_back(d);
	}
	return out;
}


namespace
{
	// RAII wrapper for sqlite3_db_mutex. In SQLite's serialized threading mode
	// every sqlite3_step (and most other sqlite3_*) call on a connection
	// briefly acquires this mutex; manually holding it across a multi-step
	// operation makes that operation atomic with respect to concurrent steps
	// from other threads on the same connection. We use it to make ATTACH /
	// DETACH happen only when no other thread is mid-step on _session - SQLite
	// otherwise lets ATTACH succeed while another step is in flight, which
	// invalidates that step's prepared statement (schema cookie changes) and
	// surfaces as stale-errmsg failures ("another row available" etc.).
	struct DbMutexGuard
	{
		sqlite3_mutex* m;
		explicit DbMutexGuard(sqlite3* db): m(sqlite3_db_mutex(db)) { if (m) sqlite3_mutex_enter(m); }
		~DbMutexGuard() { if (m) sqlite3_mutex_leave(m); }
		DbMutexGuard(const DbMutexGuard&) = delete;
		DbMutexGuard& operator=(const DbMutexGuard&) = delete;
	};
}


std::string MemoryDB::attachArchived(Poco::UInt32 shardId)
{
	// Coordination layers (also see the comment block on the mutex declarations
	// in MemoryDB.h):
	//   _attachMutex (recursive) serializes our own attach/detach/historyView
	//     calls against each other AND guards _attached. _attached is
	//     reference-counted (int per shard id) so concurrent callers pair their
	//     attach/detach correctly: the SQLite ATTACH happens on the first call,
	//     the DETACH on the last matching detach.
	//   _stateMutex (FastMutex) guards _shards (the metadata we look up below
	//     to resolve the shard id to a filename and confirm it is sealed).
	//   sqlite3_db_mutex (held during the ATTACH itself) blocks any concurrent
	//     sqlite3_step on _session, so user statements never see the connection
	//     in a half-attached state.
	Poco::Mutex::ScopedLock al(_attachMutex);

	std::string alias = "arc_" + Poco::NumberFormatter::format(shardId);
	auto it = _attached.find(shardId);
	if (it != _attached.end()) { ++it->second; return alias; }

	std::string file;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		for (const auto& s: _shards)
		{
			if (s.id == shardId)
			{
				if (!s.sealed) throw Poco::InvalidArgumentException("shard is not sealed");
				file = s.file;
				break;
			}
		}
	}
	if (file.empty()) throw Poco::NotFoundException("no such sealed shard");

	std::string path = Poco::Path(_dir, file).toString();
	{
		// InternalGuard suppresses our own trace callback for the ATTACH stmt;
		// without it onStatement would try to take _stateMutex on the same
		// thread - safe (we don't currently hold it) but pointless work.
		InternalGuard guard;
		DbMutexGuard dbm(_memHandle);
		_session << ("ATTACH DATABASE " + quoteLit("file:" + path + "?mode=ro") + " AS " + alias), now;
	}
	_attached[shardId] = 1;
	return alias;
}


void MemoryDB::detachArchived(Poco::UInt32 shardId)
{
	std::string alias = "arc_" + Poco::NumberFormatter::format(shardId);
	std::string sql = "DETACH DATABASE " + alias;

	// SQLite's DETACH fails with "database <alias> is locked" if the attached
	// btree is in any transaction state (sqlite3.c:125750). This check
	// returns immediately - sqlite3_busy_timeout() does NOT retry it. Under
	// concurrent load on the same connection (constant prepare/step/finalize
	// from other threads' stmts) arc_<id>'s btree can transiently show
	// SQLITE_TXN_READ even when no one is querying arc_<id> directly. Retry
	// briefly with backoff; the window where DETACH is rejected is short.
	//
	// We RELEASE _attachMutex across the sleep so that a concurrent
	// deleteShard() waiting on _attachMutex (while holding _flushMutex) does
	// not transitively stall the flush IO loop. Each iteration re-acquires
	// _attachMutex, re-checks the refcount (another caller may have brought
	// it above 1), attempts DETACH, and either succeeds or sleeps. Bounded
	// by maxAttempts so a pathological hold cannot loop forever.
	const int maxAttempts = 100;
	for (int attempt = 0; ; ++attempt)
	{
		{
			Poco::Mutex::ScopedLock al(_attachMutex);
			auto it = _attached.find(shardId);
			if (it == _attached.end()) return;
			if (it->second > 1) { --it->second; return; }
			// refcount == 1; we are the last detacher. Run DETACH first;
			// only erase on success so an exception here leaves _attached
			// consistent with the actual SQLite state.
			try
			{
				InternalGuard guard;
				DbMutexGuard dbm(_memHandle);
				_session << sql, now;
				_attached.erase(shardId);
				return;
			}
			catch (const Poco::Exception&)
			{
				if (attempt + 1 >= maxAttempts) throw;
			}
		} // release _attachMutex before sleeping
		Poco::Thread::sleep(1);
	}
}


void MemoryDB::deleteShard(Poco::UInt32 shardId)
{
	Poco::Mutex::ScopedLock fl(_flushMutex);

	std::string file;
	std::map<std::string, std::pair<Poco::Int64, Poco::Int64> > ranges;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		auto it = std::find_if(_shards.begin(), _shards.end(),
			[shardId](const ShardInfo& s) { return s.id == shardId; });
		// Idempotent on a vanished shard: a concurrent retention sweep (or
		// another deleteShard) may have already dropped it between when the
		// caller sampled shards() and now. Silently return rather than throw
		// so user retention loops over a stale snapshot stay correct.
		if (it == _shards.end()) return;
		if (!it->sealed)
			throw Poco::InvalidArgumentException("cannot delete the active shard; seal it first");
		file = it->file;
		ranges = it->ranges;
	}

	// Force-detach if currently attached read-only. Bypass the reference count:
	// we are about to unlink the file, so any holder is going to break either
	// way; better to detach cleanly now than to leave the SQLite ATTACH
	// dangling on a deleted file. If DETACH itself fails (e.g., the shard is
	// busy with a SELECT), keep _attached consistent and propagate so the
	// caller can retry rather than silently leaving a stale alias around.
	{
		Poco::Mutex::ScopedLock al(_attachMutex);
		if (_attached.find(shardId) != _attached.end())
		{
			std::string alias = "arc_" + Poco::NumberFormatter::format(shardId);
			{
				InternalGuard guard;
				DbMutexGuard dbm(_memHandle);
				_session << ("DETACH DATABASE " + alias), now;
			}
			_attached.erase(shardId);
		}
	}

	// In loaded mode the shard's rows are also in main; remove them so the next flush
	// doesn't simply re-absorb them into the active shard. Done on _persist so the
	// _session-side update/trace hooks don't fire (no spurious dirty marks).
	if (_opts.loadArchivedShards)
	{
		for (const auto& r: ranges)
		{
			std::string sql = "DELETE FROM " + quoteIdent(r.first) +
				" WHERE rowid > " + Poco::NumberFormatter::format(r.second.first) +
				" AND rowid <= " + Poco::NumberFormatter::format(r.second.second);
			try { _persist << sql, now; }
			catch (...) {} // table may have been dropped since the shard was sealed
		}
	}

	int schemaVersion;
	std::vector<std::string> schemaLog;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		_shards.erase(std::remove_if(_shards.begin(), _shards.end(),
			[shardId](const ShardInfo& s) { return s.id == shardId; }),
			_shards.end());
		rebuildActiveLo();
		schemaVersion = _schemaVersion;
		schemaLog = _schemaLog;
	}

	writeCatalog(schemaVersion, schemaLog);

	if (!file.empty())
	{
		try { Poco::File(Poco::Path(_dir, file).toString()).remove(); }
		catch (...) {}
	}
}


void MemoryDB::writeCatalog(int schemaVersion, const std::vector<std::string>& schemaLog)
{
	// Snapshot the shard list under the lock so the catalog write itself doesn't have
	// to hold _stateMutex (which would block hook callbacks for the duration).
	std::vector<ShardInfo> snapshot;
	Poco::UInt32 nextId;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		snapshot = _shards;
		nextId = _nextShardId;
	}

	_catalog.begin();
	try
	{
		_catalog << "DELETE FROM meta", now;
		_catalog << "DELETE FROM schema_log", now;
		_catalog << "DELETE FROM shards", now;
		_catalog << "DELETE FROM shard_ranges", now;

		std::string kCat("catalogVersion"), kSchema("schemaVersion"), kNext("nextShardId");
		Poco::Int64 vCat = MemoryDB_CATALOG_VERSION;
		Poco::Int64 vSchema = schemaVersion;
		Poco::Int64 vNext = nextId;
		_catalog << "INSERT INTO meta VALUES(?,?)", use(kCat), use(vCat), now;
		_catalog << "INSERT INTO meta VALUES(?,?)", use(kSchema), use(vSchema), now;
		_catalog << "INSERT INTO meta VALUES(?,?)", use(kNext), use(vNext), now;

		for (std::size_t v = 0; v < schemaLog.size(); ++v)
		{
			Poco::Int64 ver = static_cast<Poco::Int64>(v + 1);
			std::string ddl = schemaLog[v];
			_catalog << "INSERT INTO schema_log VALUES(?,?)", use(ver), use(ddl), now;
		}

		for (const auto& s: snapshot)
		{
			Poco::Int64 id = s.id, sealed = s.sealed ? 1 : 0;
			Poco::Int64 created = s.created.epochMicroseconds();
			Poco::Int64 sealedAt = s.sealedAt.epochMicroseconds();
			Poco::Int64 sv = s.schemaVersion, bytes = static_cast<Poco::Int64>(s.bytes);
			std::string file = s.file;
			_catalog << "INSERT INTO shards VALUES(?,?,?,?,?,?,?)",
				use(id), use(file), use(sealed), use(created), use(sealedAt), use(sv), use(bytes), now;
			for (const auto& r: s.ranges)
			{
				Poco::Int64 shardId = s.id, lo = r.second.first, hi = r.second.second;
				std::string tbl = r.first;
				_catalog << "INSERT INTO shard_ranges VALUES(?,?,?,?)",
					use(shardId), use(tbl), use(lo), use(hi), now;
			}
		}
		_catalog.commit();
	}
	catch (...)
	{
		_catalog.rollback();
		throw;
	}
}


std::size_t MemoryDB::enforceRetention()
{
	Poco::Mutex::ScopedLock fl(_flushMutex);

	std::vector<ShardDescriptor> all = shards();
	std::vector<Poco::UInt32> drop;

	if (_opts.retentionMaxAge.totalMicroseconds() > 0)
	{
		Poco::Timestamp cutoff;
		cutoff -= _opts.retentionMaxAge.totalMicroseconds();
		for (const auto& d: all)
			if (d.sealed && d.sealedAt < cutoff) drop.push_back(d.id);
	}

	if (_opts.retentionMaxBytes > 0)
	{
		Poco::UInt64 total = 0;
		for (const auto& d: all) total += d.bytes;
		for (const auto& d: all)
		{
			if (total <= _opts.retentionMaxBytes) break;
			if (d.sealed) { drop.push_back(d.id); total -= d.bytes; }
		}
	}

	std::sort(drop.begin(), drop.end());
	drop.erase(std::unique(drop.begin(), drop.end()), drop.end());

	// Always-on SQLITE_LIMIT_ATTACHED backstop on total shard count.
	// attachAllArchived and wide-range historyView depend on this. The active
	// shard is `main` (no attach slot), POCO's `sys` attach takes 1 slot,
	// leaving `cap-1` slots for arc_<id> attached sealed shards - so `cap`
	// total shards fits the SQLite limit exactly. When the backstop fires,
	// log a warning so the operator can either configure age/bytes retention
	// or raise SQLITE_LIMIT_ATTACHED on session() to avoid the auto-drop.
	const std::size_t cap = static_cast<std::size_t>(
		sqlite3_limit(_memHandle, SQLITE_LIMIT_ATTACHED, -1));
	std::size_t capDropCount = 0;
	if (all.size() > drop.size() && (all.size() - drop.size()) > cap)
	{
		std::size_t excess = all.size() - drop.size() - cap;
		std::set<Poco::UInt32> already(drop.begin(), drop.end());
		for (const auto& d: all)
		{
			if (excess == 0) break;
			if (d.sealed && already.find(d.id) == already.end())
			{
				drop.push_back(d.id);
				--excess;
				++capDropCount;
			}
		}
	}

	if (capDropCount > 0)
	{
		Poco::Logger::get("Poco.Data.SQLite.MemoryDB").warning(
			"dropped %z oldest sealed shard(s) to stay within "
			"SQLITE_LIMIT_ATTACHED (%z). Raise Options::shardMaxBytes so seals "
			"fire less often, set Options::retentionMaxAge or retentionMaxBytes "
			"to drop older shards proactively, or raise SQLITE_LIMIT_ATTACHED "
			"via sqlite3_limit() on session() before adding workload.",
			capDropCount, cap);
	}

	std::sort(drop.begin(), drop.end());
	drop.erase(std::unique(drop.begin(), drop.end()), drop.end());
	for (Poco::UInt32 id: drop) deleteShard(id);
	return drop.size();
}


std::size_t MemoryDB::dropOlderThan(const Poco::Timestamp& cutoff)
{
	Poco::Mutex::ScopedLock fl(_flushMutex);
	std::vector<Poco::UInt32> drop;
	for (const auto& d: shards())
		if (d.sealed && d.sealedAt < cutoff) drop.push_back(d.id);
	for (Poco::UInt32 id: drop) deleteShard(id);
	return drop.size();
}


std::size_t MemoryDB::dropToFit(Poco::UInt64 maxTotalBytes)
{
	Poco::Mutex::ScopedLock fl(_flushMutex);
	std::vector<ShardDescriptor> all = shards();
	Poco::UInt64 total = 0;
	for (const auto& d: all) total += d.bytes;
	std::vector<Poco::UInt32> drop;
	for (const auto& d: all)
	{
		if (total <= maxTotalBytes) break;
		if (d.sealed) { drop.push_back(d.id); total -= d.bytes; }
	}
	for (Poco::UInt32 id: drop) deleteShard(id);
	return drop.size();
}


void MemoryDB::attachAllArchived()
{
	Poco::Mutex::ScopedLock al(_attachMutex);
	std::vector<Poco::UInt32> ids = archivedShardIds();
	for (auto id: ids) attachArchived(id); // attachArchived is no-op-safe if already attached
}


std::string MemoryDB::historyView(const std::string& table)
{
	Poco::Mutex::ScopedLock al(_attachMutex);
	if (_opts.loadArchivedShards)
	{
		std::string viewName = table + "_history";
		InternalGuard guard;
		DbMutexGuard dbm(_memHandle);
		_session << ("DROP VIEW IF EXISTS " + quoteIdent(viewName)), now;
		_session << ("CREATE TEMP VIEW " + quoteIdent(viewName) +
			" AS SELECT * FROM main." + quoteIdent(table)), now;
		return viewName;
	}

	std::vector<Poco::UInt32> attachedIds;
	for (const auto& kv: _attached) attachedIds.push_back(kv.first);
	std::sort(attachedIds.begin(), attachedIds.end());
	return buildHistoryView(table, attachedIds);
}


std::string MemoryDB::historyView(const std::string& table,
	const Poco::Timestamp& from, const Poco::Timestamp& to)
{
	Poco::Mutex::ScopedLock al(_attachMutex);
	if (_opts.loadArchivedShards) return historyView(table);

	std::vector<ShardDescriptor> all = shards();
	std::vector<Poco::UInt32> matching;
	for (const auto& d: all)
	{
		if (!d.sealed) continue;
		// shard data window = [createdAt, sealedAt]; intersects [from, to] iff
		// createdAt <= to && sealedAt >= from
		if (d.createdAt <= to && d.sealedAt >= from)
			matching.push_back(d.id);
	}

	int maxAttached = sqlite3_limit(_memHandle, SQLITE_LIMIT_ATTACHED, -1) - 1;
	if (maxAttached < 1) maxAttached = 1;
	if (static_cast<int>(matching.size()) > maxAttached)
	{
		throw Poco::RangeException(
			"historyView time range spans " + Poco::NumberFormatter::format(matching.size()) +
			" shards; the connection can have at most " + Poco::NumberFormatter::format(maxAttached) +
			" attached simultaneously. Narrow the range or query in batches.");
	}

	for (auto id: matching) attachArchived(id); // recursive _attachMutex -> safe

	return buildHistoryView(table, matching);
}


std::string MemoryDB::buildHistoryView(const std::string& table,
	const std::vector<Poco::UInt32>& shardIds)
{
	std::string viewName = table + "_history";
	std::string sql = "SELECT * FROM main." + quoteIdent(table);

	// Holding the SQLite connection mutex makes the whole "probe shards + drop
	// old view + create new view" sequence atomic vs concurrent user steps.
	// Without it a user's SELECT FROM <table>_history could see the view mid
	// drop-and-recreate and fail with "no such table".
	InternalGuard guard;
	DbMutexGuard dbm(_memHandle);
	for (auto id: shardIds)
	{
		std::string alias = "arc_" + Poco::NumberFormatter::format(id);
		int has = 0;
		std::string name = table;
		_session << ("SELECT count(*) FROM " + alias +
			".sqlite_master WHERE type='table' AND name=:n"),
			use(name), into(has), now;
		if (has > 0)
			sql += " UNION ALL SELECT * FROM " + alias + "." + quoteIdent(table);
	}

	_session << ("DROP VIEW IF EXISTS " + quoteIdent(viewName)), now;
	_session << ("CREATE TEMP VIEW " + quoteIdent(viewName) + " AS " + sql), now;
	return viewName;
}


void MemoryDB::detachAllArchived()
{
	// Force-detach every currently-attached shard regardless of reference
	// count. Caller is responsible for ensuring no other thread still relies
	// on these aliases (the docstring above says "useful after a burst of
	// history queries to release the per-shard SQLite page caches").
	Poco::Mutex::ScopedLock al(_attachMutex);
	std::vector<Poco::UInt32> ids;
	for (const auto& kv: _attached) ids.push_back(kv.first);
	for (auto id: ids)
	{
		std::string alias = "arc_" + Poco::NumberFormatter::format(id);
		try
		{
			{
				InternalGuard guard;
				DbMutexGuard dbm(_memHandle);
				_session << ("DETACH DATABASE " + alias), now;
			}
			_attached.erase(id);
		}
		catch (...) {} // DETACH failed; leave _attached entry alone so it stays
		               // consistent with SQLite and a future detach can retry
	}
}


//
// change detection (SQLite callbacks -> instance methods, on the user thread)
//
int MemoryDB::traceCallback(unsigned type, void* ctx, void* /*p*/, void* x)
{
	if (type == SQLITE_TRACE_STMT && ctx && x)
	{
		const char* sql = static_cast<const char*>(x);
		try { static_cast<MemoryDB*>(ctx)->onStatement(sql); }
		catch (...) {}
	}
	return 0;
}


void MemoryDB::updateCallback(void* ctx, int op, const char* /*db*/, const char* table, long long row)
{
	if (ctx && table)
	{
		try { static_cast<MemoryDB*>(ctx)->onRowChange(op, table, static_cast<Poco::Int64>(row)); }
		catch (...) {}
	}
}


void MemoryDB::onStatement(const char* sql)
{
	if (!sql || !*sql) return;
	if (t_inInternal) return;  // SQL issued by MemoryDB's own bookkeeping, not a user write

	// Skip leading whitespace and SQL comments so the keyword we sample below
	// is the actual first keyword. SQLite emits trace strings beginning with
	// "--" for trigger subprograms; the old single-char "if (sql[0] == '-')"
	// also matched any user statement that began with a "--" comment, letting
	// it bypass the WITHOUT-ROWID rejection. /* ... */ and -- ... \n forms
	// are both handled here. If the statement is comment-only, bail.
	const char* p = sql;
	while (*p)
	{
		while (*p && std::isspace(static_cast<unsigned char>(*p))) ++p;
		if (p[0] == '-' && p[1] == '-')
		{
			p += 2;
			while (*p && *p != '\n') ++p;
		}
		else if (p[0] == '/' && p[1] == '*')
		{
			p += 2;
			while (*p && !(p[0] == '*' && p[1] == '/')) ++p;
			if (*p) p += 2;
		}
		else break;
	}
	if (!*p) return;
	sql = p;

	std::string kw = firstKeyword(sql);

	// Reads + transaction control: no state change, no parse, no dirty-marking.
	if (kw == "SELECT" || kw == "PRAGMA" || kw == "EXPLAIN" || kw == "WITH" ||
		kw == "BEGIN" || kw == "COMMIT" || kw == "END" || kw == "ROLLBACK" ||
		kw == "SAVEPOINT" || kw == "RELEASE")
		return;

	// WITHOUT ROWID is prohibited; check independently of the parser (it may not accept the clause).
	// Set _poisoned first (noexcept) so that even if the string assignment that follows
	// throws bad_alloc, throwIfRejected() will still surface the failure on the next call -
	// L4 in the threading review. SQLite has already executed the statement by the time
	// the trace hook fires, so silently letting it slide would leave a corrupt shard model.
	if (kw == "CREATE" && mentionsWithoutRowid(sql))
	{
		_poisoned.store(true, std::memory_order_release);
		static const std::string kMsg = "MemoryDB does not support WITHOUT ROWID tables";
		try
		{
			Poco::FastMutex::ScopedLock l(_stateMutex);
			if (_rejected.empty()) _rejected = kMsg;
		}
		catch (...) {} // _poisoned is already set; throwIfRejected uses the static fallback
		return;
	}

	// Hot-path writes (INSERT/UPDATE/REPLACE): per-row dirtying is done by the
	// update hook with exact (table, rowid). The parser would only give us a
	// table name we don't need - skip it entirely. POCO's Statement already
	// parses every statement once; this avoids parsing a second time.
	if (kw == "INSERT" || kw == "UPDATE" || kw == "REPLACE")
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		_dirty = true;
		_lastChange.update();
		return;
	}

	bool wrote = false;
	bool classified = false;

	hsql::SQLParserResult result;
	if (hsql::SQLParser::parse(sql, &result) && result.isValid())
	{
		classified = true;
		for (std::size_t i = 0; i < result.size(); ++i)
		{
			const hsql::SQLStatement* st = result.getStatement(i);
			switch (st->type())
			{
			case hsql::kStmtInsert:
			case hsql::kStmtUpdate:
				wrote = true; // per-row dirtying handled by the update hook
				break;

			case hsql::kStmtDelete:
			{
				const auto* d = static_cast<const hsql::DeleteStatement*>(st);
				wrote = true;
				if (d->expr == nullptr && d->tableName)
				{
					// truncate optimization: the update hook does not fire
					Poco::FastMutex::ScopedLock l(_stateMutex);
					markTableDirty(d->tableName);
				}
				break;
			}

			case hsql::kStmtCreate:
			{
				const auto* c = static_cast<const hsql::CreateStatement*>(st);
				onDDL(sql, (c->type == hsql::kCreateTable) ? c->tableName : nullptr);
				wrote = true;
				break;
			}

			case hsql::kStmtDrop:
			{
				const auto* d = static_cast<const hsql::DropStatement*>(st);
				onDDL(sql, (d->type == hsql::kDropTable) ? d->name : nullptr);
				wrote = true;
				break;
			}

			case hsql::kStmtAlter:
			{
				const auto* a = static_cast<const hsql::AlterStatement*>(st);
				onDDL(sql, a->name);
				wrote = true;
				break;
			}

			default:
				break;
			}
		}
	}

	if (!classified)
	{
		// The parser could not handle this SQL (uncommon - SQLite-specific syntax it
		// doesn't yet support). Fall back to first-keyword classification so a write
		// still marks the db dirty; we just can't extract a table name from DDL.
		if (kw == "INSERT" || kw == "UPDATE" || kw == "DELETE" || kw == "REPLACE")
		{
			wrote = true;
		}
		else if (kw == "CREATE" || kw == "DROP" || kw == "ALTER")
		{
			onDDL(sql, nullptr);
			wrote = true;
		}
	}

	if (wrote)
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		_dirty = true;
		_lastChange.update();
	}
}


void MemoryDB::onDDL(const char* sql, const char* table)
{
	// Normalize CREATE/DROP to their idempotent forms so replays of _schemaLog
	// (both at load() time on a fresh _persist and at migrateShardFile() time
	// on a partially-migrated shard) are safe to run more than once.
	std::string normalized = addIfExistsClause(sql);

	Poco::FastMutex::ScopedLock l(_stateMutex);
	// push_back BEFORE ++_schemaVersion so a bad_alloc on vector growth leaves
	// _schemaVersion consistent with _schemaLog.size(). If we incremented first
	// and push_back threw, the catalog write at the next flush would persist
	// schemaVersion=N+1 with only N log entries; on restart migrateShardFile
	// would bump shard.schemaVersion past the missing DDL and lose it
	// permanently. push_back is the only operation here that can throw.
	_schemaLog.push_back(std::move(normalized));
	++_schemaVersion;
	_columnCopyCache.clear();
	if (table) markTableDirty(table);
	else if (ShardInfo* a = activeShard()) a->dirty = true;
}


void MemoryDB::onRowChange(int /*op*/, const char* table, Poco::Int64 row)
{
	Poco::FastMutex::ScopedLock l(_stateMutex);
	_dirty = true;
	_lastChange.update();
	if (ShardInfo* s = owningShard(table, row)) s->dirty = true;
}


//
// flush
//
void MemoryDB::onTimer(Poco::Timer&)
{
	if (_poisoned.load(std::memory_order_acquire)) return;
	bool go = false;
	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		if (_dirty)
		{
			Poco::Timestamp now;
			Poco::Timestamp::TimeDiff idleUs = _opts.idleInterval.totalMicroseconds();
			Poco::Timestamp::TimeDiff maxUs = _opts.maxFlushInterval.totalMicroseconds();
			bool idle = (now - _lastChange) >= idleUs;
			bool maxed = maxUs > 0 && (now - _lastFlush) >= maxUs;
			go = idle || maxed || _sealRequested;
		}
	}
	if (go)
	{
		try { doFlush(); }
		catch (...) {}
	}
}


void MemoryDB::flush()
{
	doFlush();
}


MemoryDB::ShardInfo* MemoryDB::activeShard()
{
	for (auto& s: _shards)
		if (!s.sealed) return &s;
	return nullptr;
}


MemoryDB::ShardInfo* MemoryDB::owningShard(const std::string& table, Poco::Int64 row)
{
	// Common case: insert/update of a row newer than any sealed shard's hi.
	// Cached _activeLo[table] = max(over sealed shards) ranges[table].hi, so the
	// branch resolves in O(1) without scanning shards.
	auto loIt = _activeLo.find(table);
	Poco::Int64 lo = (loIt != _activeLo.end()) ? loIt->second : 0;

	ShardInfo* active = nullptr;
	if (row > lo)
	{
		for (auto& s: _shards) if (!s.sealed) { active = &s; break; }
		return active;
	}

	// Rare case: row falls inside a sealed shard's range. Linear scan.
	for (auto& s: _shards)
	{
		if (!s.sealed) { active = &s; continue; }
		auto it = s.ranges.find(table);
		if (it != s.ranges.end() && row > it->second.first && row <= it->second.second)
			return &s;
	}
	return active;
}


void MemoryDB::rebuildActiveLo()
{
	// caller holds _stateMutex
	_activeLo.clear();
	for (const auto& s: _shards)
	{
		if (!s.sealed) continue;
		for (const auto& r: s.ranges)
		{
			auto it = _activeLo.find(r.first);
			if (it == _activeLo.end() || r.second.second > it->second)
				_activeLo[r.first] = r.second.second;
		}
	}
}


void MemoryDB::markTableDirty(const std::string& table)
{
	// Mark every in-memory shard that owns rows of this table. The active shard always
	// owns the open range; sealed shards are relevant only when held in memory.
	for (auto& s: _shards)
	{
		if (!s.sealed) { s.dirty = true; continue; }
		if (!_opts.loadArchivedShards) continue;
		if (s.ranges.find(table) != s.ranges.end()) s.dirty = true;
	}
}


Poco::Int64 MemoryDB::maxRowid(const std::string& table)
{
	Poco::Int64 mx = 0;
	_persist << ("SELECT IFNULL(MAX(rowid),0) FROM " + quoteIdent(table)), into(mx), now;
	return mx;
}


std::vector<std::string> MemoryDB::userTables(Session& s)
{
	std::vector<std::string> tables;
	s << "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'", into(tables), now;
	return tables;
}


void MemoryDB::maybeSeal(const std::vector<std::string>& tables,
	const std::map<std::string, Poco::Int64>& maxRowids)
{
	// caller holds _stateMutex; tables/maxRowids were sampled on _persist by the
	// caller WITHOUT holding _stateMutex. Issuing SQL on _persist here would
	// deadlock against the trace hook on the user thread, which acquires
	// _stateMutex while SQLite holds its shared-cache mutex (classic A->B / B->A
	// inversion confirmed by TSan).
	ShardInfo* a = activeShard();
	if (!a) return;

	Poco::Timestamp now;
	bool seal = _sealRequested;
	if (_opts.shardMaxBytes > 0 && a->bytes >= _opts.shardMaxBytes) seal = true;
	if (_opts.shardMaxAge.totalMicroseconds() > 0 &&
		(now - a->created) >= _opts.shardMaxAge.totalMicroseconds()) seal = true;
	if (!seal) return;

	// freeze the active shard's per-table ranges (a is not yet sealed -> excluded below)
	std::map<std::string, std::pair<Poco::Int64, Poco::Int64> > frozen;
	for (const auto& t: tables)
	{
		Poco::Int64 lo = 0;
		for (const auto& s: _shards)
		{
			if (s.sealed)
			{
				auto it = s.ranges.find(t);
				if (it != s.ranges.end() && it->second.second > lo) lo = it->second.second;
			}
		}
		auto mi = maxRowids.find(t);
		Poco::Int64 hi = (mi != maxRowids.end()) ? mi->second : 0;
		frozen[t] = std::make_pair(lo, hi);
	}

	a->ranges = frozen;
	a->sealed = true;
	a->sealedAt.update();
	a->dirty = true;

	// the newly-sealed shard may extend _activeLo for any of its tables
	for (const auto& r: frozen)
	{
		auto it = _activeLo.find(r.first);
		if (it == _activeLo.end() || r.second.second > it->second)
			_activeLo[r.first] = r.second.second;
	}

	ShardInfo na;
	na.id = _nextShardId++;
	na.created.update();
	na.sealed = false;
	// Mark na dirty so the current flush includes it. Writes that landed on
	// _session between the caller's maxRowids sample and lock acquisition
	// inserted rows with rowid > maxRowids[t]; those rows belong to na, not
	// to the freshly-sealed shard. Forcing na into the current flush's plan
	// makes its open-range slice (rowid > lo) capture them via SELECT
	// MAX(rowid) in heavy IO. Without this, the gap rows would not be picked
	// up until something else dirtied na on a later cycle.
	na.dirty = true;
	na.schemaVersion = _schemaVersion;
	_shards.push_back(na); // invalidates 'a'
	_sealRequested = false;
}


void MemoryDB::doFlush(bool allowSeal)
{
	// Skip-if-busy gate: if another doFlush is in progress (timer, user, or
	// destructor), bail immediately rather than queue up on _flushMutex. The
	// in-progress flush will persist everything written before it started; this
	// caller's writes (if any) will land at the next flush. flush()'s contract
	// covers this: callers who need durability loop on dirty().
	bool expected = false;
	if (!_flushing.compare_exchange_strong(expected, true))
		return;
	struct Releaser
	{
		std::atomic<bool>& flag;
		~Releaser() { flag.store(false); }
	} releaser{_flushing};

	Poco::Mutex::ScopedLock fl(_flushMutex);

	// Sample _persist BEFORE taking _stateMutex. Holding _stateMutex across
	// SQL on _persist would deadlock against the trace hook (user thread holds
	// SQLite's shared-cache mutex during sqlite3_step and calls onStatement,
	// which grabs _stateMutex). _flushMutex serializes doFlush calls; any
	// concurrent writes on _session during this sampling land in the
	// open-range slice of the new active shard (see maybeSeal note).
	std::vector<std::string> tables = userTables(_persist);
	std::map<std::string, Poco::Int64> maxRowids;
	for (const auto& t: tables) maxRowids[t] = maxRowid(t);

	struct Slice { std::string table; Poco::Int64 lo; Poco::Int64 hi; bool open; };
	struct Plan { std::size_t idx; std::string finalName; std::vector<Slice> slices; };
	std::vector<Plan> writes;
	std::vector<std::size_t> migrates;
	int schemaVersion = 0;
	std::vector<std::string> schemaLog;

	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		if (_poisoned.load(std::memory_order_acquire))
			throw Poco::NotImplementedException(_rejected.empty() ?
				std::string("MemoryDB rejected a prohibited operation") : _rejected);

		if (allowSeal) maybeSeal(tables, maxRowids);
		schemaVersion = _schemaVersion;
		schemaLog = _schemaLog;

		for (std::size_t i = 0; i < _shards.size(); ++i)
		{
			ShardInfo& s = _shards[i];
			bool inMemory = !s.sealed || _opts.loadArchivedShards;
			bool needSchema = s.schemaVersion < schemaVersion;
			bool needData = s.dirty;

			if (needData || (needSchema && inMemory))
			{
				Plan p;
				p.idx = i;
				p.finalName = s.file.empty() ? shardPath(s.id, s.created) : s.file;
				if (!s.sealed)
				{
					for (const auto& t: tables)
					{
						Poco::Int64 lo = 0;
						for (const auto& o: _shards)
						{
							if (o.sealed)
							{
								auto it = o.ranges.find(t);
								if (it != o.ranges.end() && it->second.second > lo) lo = it->second.second;
							}
						}
						p.slices.push_back({t, lo, 0, true});
					}
				}
				else
				{
					for (const auto& r: s.ranges)
					{
						bool inMain = false;
						for (const auto& t: tables) if (t == r.first) { inMain = true; break; }
						if (inMain) p.slices.push_back({r.first, r.second.first, r.second.second, false});
					}
				}
				writes.push_back(p);
				s.dirty = false;
			}
			else if (needSchema && !inMemory)
			{
				migrates.push_back(i);
			}
		}
	}

	// heavy IO without the state lock (flushes are serialized by _flushMutex)
	bool ok = true;
	std::vector<std::pair<std::size_t, std::pair<std::string, Poco::UInt64> > > written; // idx -> (file, bytes)
	std::vector<std::pair<std::size_t, std::map<std::string, std::pair<Poco::Int64, Poco::Int64> > > > newRanges;

	for (const auto& p: writes)
	{
		try
		{
			Poco::TemporaryFile tmp(_dir);
			std::string tmpPath = tmp.path();
			std::map<std::string, std::pair<Poco::Int64, Poco::Int64> > ranges;

			{
				Session ss(Connector::KEY, tmpPath);
				// read_uncommitted lets ss read attached shared-cache tables
				// (mem.<t>) without holding shared-cache read locks that would
				// block concurrent user-thread INSERTs on _session.
				ss << "PRAGMA read_uncommitted = true", now;
				ss << ("ATTACH DATABASE " + quoteLit(_memName) + " AS mem"), now;

				std::vector<std::string> ddls;
				ss << "SELECT sql FROM mem.sqlite_master WHERE type IN ('table','index') "
					"AND name NOT LIKE 'sqlite_%' AND sql IS NOT NULL ORDER BY rowid", into(ddls), now;
				for (const auto& d: ddls) ss << d, now;

				for (const auto& sl: p.slices)
				{
					ColumnCopy cc = columnCopy(ss, sl.table);
					std::string where = "rowid > " + Poco::NumberFormatter::format(sl.lo);
					if (!sl.open) where += " AND rowid <= " + Poco::NumberFormatter::format(sl.hi);

					std::string sql = "INSERT INTO " + quoteIdent(sl.table);
					if (!cc.insertCols.empty()) sql += " " + cc.insertCols;
					sql += " SELECT " + cc.selectList + " FROM mem." + quoteIdent(sl.table) + " WHERE " + where;
					ss << sql, now;

					Poco::Int64 hi = sl.hi;
					if (sl.open) ss << ("SELECT IFNULL(MAX(rowid),0) FROM mem." + quoteIdent(sl.table)), into(hi), now;
					ranges[sl.table] = std::make_pair(sl.lo, hi);
				}

				ss << "DETACH DATABASE mem", now;
			}

			Poco::File tf(tmpPath);
			Poco::Path fp(_dir, p.finalName);
			tf.renameTo(fp.toString());
			Poco::UInt64 bytes = Poco::File(fp.toString()).getSize();

			written.push_back(std::make_pair(p.idx, std::make_pair(p.finalName, bytes)));
			newRanges.push_back(std::make_pair(p.idx, ranges));
		}
		catch (...)
		{
			ok = false;
		}
	}

	for (std::size_t mi: migrates)
	{
		try { migrateShardFile(_shards[mi], schemaVersion, schemaLog); }
		catch (...) { ok = false; }
	}

	// write the catalog (single transaction = the durable commit point)
	try
	{
		// fold IO results into shard state under the lock first, so the catalog reflects it
		{
			Poco::FastMutex::ScopedLock l(_stateMutex);
			for (const auto& w: written)
			{
				_shards[w.first].file = w.second.first;
				_shards[w.first].bytes = w.second.second;
				_shards[w.first].schemaVersion = schemaVersion;
			}
			for (const auto& r: newRanges)
				_shards[r.first].ranges = r.second;
		}
		writeCatalog(schemaVersion, schemaLog);
	}
	catch (...)
	{
		ok = false;
	}

	// Detect tables that appeared between the initial userTables() sample
	// (line above, outside _stateMutex - the trace-hook A/B inversion forces
	// us to sample without it) and now. A CREATE TABLE + INSERT in that
	// window leaves the new table out of every shard's slice plan; the rows
	// are in the in-memory db but no slice was built for them. Without
	// flagging _dirty here, the recompute below would clear it and the next
	// dtor / dirty()-gated flush would skip, losing the rows on reopen.
	// We can sample userTables on _persist safely here because we are
	// outside _stateMutex (held only briefly above to fold results) and
	// _flushMutex is still held so no second flush can race.
	bool newTablesObserved = false;
	try
	{
		std::vector<std::string> tablesNow = userTables(_persist);
		if (tablesNow.size() != tables.size())
		{
			std::set<std::string> orig(tables.begin(), tables.end());
			for (const auto& t: tablesNow)
				if (orig.find(t) == orig.end()) { newTablesObserved = true; break; }
		}
	}
	catch (...) { newTablesObserved = true; } // conservative: assume yes

	{
		Poco::FastMutex::ScopedLock l(_stateMutex);
		_lastFlush.update();
		if (!ok)
		{
			// keep dirty so the next tick retries; re-mark the shards we attempted
			for (const auto& p: writes) _shards[p.idx].dirty = true;
			_dirty = true;
		}
		else
		{
			// dirty only if a concurrent write came in during the flush, a
			// new table appeared in the sampling window (H2), or a seal was
			// requested but swallowed by an in-flight flush (L1).
			_dirty = false;
			for (const auto& s: _shards) if (s.dirty) { _dirty = true; break; }
			if (newTablesObserved) _dirty = true;
			if (_sealRequested) _dirty = true;
		}
	}

	if (!ok)
		throw Poco::IOException("MemoryDB flush failed");

	// Always run enforceRetention: the SQLITE_LIMIT_ATTACHED backstop on
	// total shard count is unconditional, and age/bytes retention (if
	// configured) is checked here too. With nothing to drop, the call is a
	// cheap no-op (one shards() snapshot + a couple of size comparisons).
	// Skipped on the !allowSeal (dtor) path to keep shutdown bounded:
	// deleteShard's detach retry can take up to ~100 ms per attached shard.
	if (allowSeal) try { enforceRetention(); } catch (...) {}
}


void MemoryDB::migrateShardFile(ShardInfo& shard, int toVersion, const std::vector<std::string>& schemaLog)
{
	if (shard.file.empty()) return;
	std::string path = Poco::Path(_dir, shard.file).toString();
	if (!Poco::File(path).exists()) return;

	Session ss(Connector::KEY, path);
	for (int v = shard.schemaVersion; v < toVersion && v < static_cast<int>(schemaLog.size()); ++v)
	{
		try { ss << schemaLog[v], now; }
		catch (...) {} // best-effort: a DDL step may not apply to this shard's tables
	}
	shard.schemaVersion = toVersion;
}


std::string MemoryDB::shardPath(Poco::UInt32 id, const Poco::Timestamp& createdAt) const
{
	if (_opts.shardNamer) return _opts.shardNamer(id, createdAt);
	return "shard-" + Poco::NumberFormatter::format0(static_cast<unsigned>(id), 6) + ".db";
}


} // namespace Poco::Data::SQLite
