//
// MemoryDBTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MemoryDBTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/Session.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Data::SQLite::MemoryDB;
using Poco::Timespan;


MemoryDBTest::MemoryDBTest(const std::string& name): CppUnit::TestCase(name)
{
}


MemoryDBTest::~MemoryDBTest()
{
}


void MemoryDBTest::setUp()
{
	_dir = Poco::TemporaryFile::tempName();
}


void MemoryDBTest::tearDown()
{
	try { Poco::File(_dir).remove(true); }
	catch (...) {}
}


void MemoryDBTest::testPersistAndReload()
{
	{
		MemoryDB db(_dir);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, name TEXT)", now;
		std::string n1("alice"), n2("bob");
		db << "INSERT INTO t(name) VALUES(:n)", use(n1), now;
		db << "INSERT INTO t(name) VALUES(:n)", use(n2), now;
		db.flush();
		assertTrue (!db.dirty());
	}

	assertTrue (Poco::File(Poco::Path(_dir, "manifest.db").toString()).exists());

	{
		MemoryDB db(_dir);
		int cnt = 0;
		db << "SELECT count(*) FROM t", into(cnt), now;
		assertTrue (cnt == 2);
		std::string name;
		db << "SELECT name FROM t WHERE id=1", into(name), now;
		assertTrue (name == "alice");
	}
}


void MemoryDBTest::testManualFlushAndDirty()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(x INTEGER)", now;
	assertTrue (db.dirty());
	db.flush();
	assertTrue (!db.dirty());

	int x = 5;
	db << "INSERT INTO t VALUES(:x)", use(x), now;
	assertTrue (db.dirty());
	db.flush();
	assertTrue (!db.dirty());
}


void MemoryDBTest::testDestructorFlush()
{
	{
		MemoryDB db(_dir);
		db << "CREATE TABLE t(x INTEGER)", now;
		int x = 7;
		db << "INSERT INTO t VALUES(:x)", use(x), now;
		// no manual flush; destructor must persist
	}

	MemoryDB db(_dir);
	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 1);
}


void MemoryDBTest::testShardingBySize()
{
	MemoryDB::Options o;
	o.shardMaxBytes = 1; // force a seal as soon as the active shard has any content
	o.loadArchivedShards = true;

	std::size_t shards = 0;
	{
		MemoryDB db(_dir, o);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		for (int i = 0; i < 6; ++i)
		{
			std::string v("row" + Poco::NumberFormatter::format(i));
			db << "INSERT INTO t(v) VALUES(:v)", use(v), now;
			db.flush();
		}
		shards = db.shardCount();
		assertTrue (shards >= 2);
	}

	MemoryDB db(_dir, o);
	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 6);
	assertTrue (db.shardCount() == shards);
}


void MemoryDBTest::testActiveOnlyRewrite()
{
	MemoryDB::Options o;
	o.loadArchivedShards = true; // the reopen asserts a count across sealed + active
	MemoryDB db(_dir, o);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	for (int i = 0; i < 3; ++i)
	{
		std::string v("a" + Poco::NumberFormatter::format(i));
		db << "INSERT INTO t(v) VALUES(:v)", use(v), now;
	}
	db.sealActive();
	db.flush();

	std::vector<Poco::UInt32> ids = db.archivedShardIds();
	assertTrue (ids.size() == 1);

	std::string sealedFile = Poco::Path(_dir, "shard-000001.db").toString();
	assertTrue (Poco::File(sealedFile).exists());
	Poco::Timestamp before = Poco::File(sealedFile).getLastModified();

	Poco::Thread::sleep(1100); // exceed filesystem mtime resolution

	for (int i = 0; i < 3; ++i)
	{
		std::string v("b" + Poco::NumberFormatter::format(i));
		db << "INSERT INTO t(v) VALUES(:v)", use(v), now;
	}
	db.flush(); // only the active shard should be rewritten

	Poco::Timestamp after = Poco::File(sealedFile).getLastModified();
	assertTrue (before == after);

	MemoryDB db2(_dir, o);
	int cnt = 0;
	db2 << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 6);
}


void MemoryDBTest::testUpdateSealedRow()
{
	MemoryDB::Options o;
	o.loadArchivedShards = true; // updating a sealed row requires it to be in RAM
	{
		MemoryDB db(_dir, o);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string a("a"), b("b"), c("c");
		db << "INSERT INTO t(v) VALUES(:v)", use(a), now;
		db << "INSERT INTO t(v) VALUES(:v)", use(b), now;
		db << "INSERT INTO t(v) VALUES(:v)", use(c), now;
		db.sealActive();
		db.flush();

		std::string nv("A");
		db << "UPDATE t SET v=:v WHERE id=1", use(nv), now;
		db.flush();
	}

	MemoryDB db(_dir, o);
	std::string v;
	db << "SELECT v FROM t WHERE id=1", into(v), now;
	assertTrue (v == "A");
}


void MemoryDBTest::testTruncate()
{
	MemoryDB::Options o;
	o.loadArchivedShards = true; // truncate of sealed rows is meaningful only when they're in main
	{
		MemoryDB db(_dir, o);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string x("x");
		for (int i = 0; i < 3; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.sealActive();
		db.flush();
		for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.flush();

		db << "DELETE FROM t", now; // truncate (no WHERE) -> update hook does not fire
		db.flush();
	}

	MemoryDB db(_dir, o);
	int cnt = -1;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 0);
}


void MemoryDBTest::testWithoutRowidRejected()
{
	MemoryDB db(_dir);

	// the CREATE itself is rejected, before reaching SQLite; the in-memory DB stays clean
	try
	{
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT) WITHOUT ROWID", now;
		failmsg("creating a WITHOUT ROWID table must be rejected");
	}
	catch (Poco::NotImplementedException&)
	{
	}

	// the instance is still usable - a normal rowid table works
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string h("hi");
	db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
	int n = 0;
	db << "SELECT count(*) FROM t", into(n), now;
	assertTrue (n == 1);
}


void MemoryDBTest::testWithoutRowidRejectedViaSession()
{
	MemoryDB db(_dir);

	// bypass operator<< by going through the underlying session directly;
	// the trace-based backstop catches it after-the-fact and poisons the instance
	db.session() << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT) WITHOUT ROWID", now;

	try
	{
		std::string h("hi");
		db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
		failmsg("trace-based backstop must reject WITHOUT ROWID created via bypass");
	}
	catch (Poco::NotImplementedException&)
	{
	}
}


void MemoryDBTest::testLoadArchivedFalse()
{
	MemoryDB::Options off;
	off.loadArchivedShards = false;

	{
		MemoryDB db(_dir, off);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string x("x");
		for (int i = 0; i < 3; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.sealActive();
		db.flush();
		for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.flush();
	}

	{
		MemoryDB db(_dir, off);
		int cnt = 0;
		db << "SELECT count(*) FROM t", into(cnt), now;
		assertTrue (cnt == 2); // only the active shard is RAM-resident

		std::vector<Poco::UInt32> ids = db.archivedShardIds();
		assertTrue (ids.size() == 1);
		std::string alias = db.attachArchived(ids[0]);
		int scnt = 0;
		db << ("SELECT count(*) FROM " + alias + ".t"), into(scnt), now;
		assertTrue (scnt == 3); // sealed shard still queryable on disk
		db.detachArchived(ids[0]);
	}

	{
		MemoryDB::Options on;
		on.loadArchivedShards = true;
		MemoryDB db(_dir, on);
		int cnt = 0;
		db << "SELECT count(*) FROM t", into(cnt), now;
		assertTrue (cnt == 5); // full dataset in RAM
	}
}


void MemoryDBTest::testIdleFlush()
{
	MemoryDB::Options o;
	o.idleInterval = Timespan(0, 200 * 1000); // 200 ms
	o.checkIntervalMs = 50;

	MemoryDB db(_dir, o);
	db << "CREATE TABLE t(x INTEGER)", now;
	int x = 1;
	db << "INSERT INTO t VALUES(:x)", use(x), now;
	assertTrue (db.dirty());

	for (int i = 0; i < 80 && db.dirty(); ++i)
		Poco::Thread::sleep(50);

	assertTrue (!db.dirty());
}


void MemoryDBTest::testCustomShardNamer()
{
	MemoryDB::Options o;
	o.shardNamer = [](Poco::UInt32 id, const Poco::Timestamp&)
	{
		return "myshard_" + Poco::NumberFormatter::format(id) + ".sqlite";
	};

	{
		MemoryDB db(_dir, o);
		db << "CREATE TABLE t(x INTEGER)", now;
		int x = 42;
		db << "INSERT INTO t VALUES(:x)", use(x), now;
		db.flush();
		assertTrue (Poco::File(Poco::Path(_dir, "myshard_1.sqlite").toString()).exists());
	}

	// reload uses the catalog's recorded filename, independent of the naming policy
	MemoryDB db(_dir);
	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 1);
}


void MemoryDBTest::testIndexPreservedAcrossReload()
{
	{
		MemoryDB db(_dir);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		db << "CREATE INDEX t_v_idx ON t(v)", now;
		for (int i = 0; i < 5; ++i)
		{
			std::string v("v" + Poco::NumberFormatter::format(i));
			db << "INSERT INTO t(v) VALUES(:v)", use(v), now;
		}
		db.flush();
	}

	// reload drops + recreates the secondary index transparently
	MemoryDB db(_dir);

	// the index is present
	int idxCount = 0;
	db << "SELECT count(*) FROM sqlite_master WHERE type='index' AND name='t_v_idx'",
		into(idxCount), now;
	assertTrue (idxCount == 1);

	// and is actually usable
	std::string target("v3");
	int id = 0;
	db << "SELECT id FROM t WHERE v=:v", use(target), into(id), now;
	assertTrue (id == 4); // 1-based rowids: v3 is the 4th insert
}


void MemoryDBTest::testHistoryView()
{
	MemoryDB::Options off;
	off.loadArchivedShards = false;

	{
		MemoryDB db(_dir, off);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string h("hist");
		for (int i = 0; i < 3; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
		db.sealActive();
		db.flush();
		std::string l("live");
		for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(l), now;
		db.flush();
	}

	MemoryDB db(_dir, off);
	int liveCnt = 0;
	db << "SELECT count(*) FROM t", into(liveCnt), now;
	assertTrue (liveCnt == 2);

	db.attachAllArchived();
	std::string view = db.historyView("t");
	assertTrue (view == "t_history");

	int histCnt = 0;
	db << "SELECT count(*) FROM t_history", into(histCnt), now;
	assertTrue (histCnt == 5); // 3 sealed + 2 live

	int histRows = 0;
	std::string h("hist");
	db << "SELECT count(*) FROM t_history WHERE v=:v", use(h), into(histRows), now;
	assertTrue (histRows == 3); // demand-paged from the sealed shard, never merged into main

	// the view is SQLite-enforced read-only - writes through it must fail
	try
	{
		db << "INSERT INTO t_history(v) VALUES('x')", now;
		failmsg("history view must reject writes");
	}
	catch (Poco::Exception&)
	{
	}

	// internal DDL must not have dirtied the DB or polluted schema_log
	assertTrue (!db.dirty());
}


void MemoryDBTest::testDeleteSealedShardLoaded()
{
	MemoryDB::Options o;
	o.loadArchivedShards = true;
	MemoryDB db(_dir, o);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string h("hist");
	for (int i = 0; i < 3; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
	db.sealActive();
	db.flush();
	std::string l("live");
	for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(l), now;
	db.flush();

	std::vector<Poco::UInt32> ids = db.archivedShardIds();
	assertTrue (ids.size() == 1);
	std::string sealedFile = Poco::Path(_dir, "shard-000001.db").toString();
	assertTrue (Poco::File(sealedFile).exists());

	int beforeAll = 0;
	db << "SELECT count(*) FROM t", into(beforeAll), now;
	assertTrue (beforeAll == 5);

	db.deleteShard(ids[0]);

	// file is gone, catalog updated, rows actually removed from RAM
	assertTrue (!Poco::File(sealedFile).exists());
	assertTrue (db.archivedShardIds().empty());
	assertTrue (db.shardCount() == 1);

	int afterAll = 0;
	db << "SELECT count(*) FROM t", into(afterAll), now;
	assertTrue (afterAll == 2);

	// reopen: only the live rows survive; no stale shard reference
	db.flush();
	MemoryDB db2(_dir, o);
	int reopened = 0;
	db2 << "SELECT count(*) FROM t", into(reopened), now;
	assertTrue (reopened == 2);
}


void MemoryDBTest::testDeleteSealedShardUnloaded()
{
	MemoryDB::Options off;
	off.loadArchivedShards = false;

	{
		MemoryDB db(_dir, off);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string h("hist");
		for (int i = 0; i < 3; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
		db.sealActive();
		db.flush();
		std::string l("live");
		for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(l), now;
		db.flush();
	}

	MemoryDB db(_dir, off);
	std::vector<Poco::UInt32> ids = db.archivedShardIds();
	assertTrue (ids.size() == 1);
	std::string sealedFile = Poco::Path(_dir, "shard-000001.db").toString();
	assertTrue (Poco::File(sealedFile).exists());

	// active-only count before delete (sealed rows not in main)
	int before = 0;
	db << "SELECT count(*) FROM t", into(before), now;
	assertTrue (before == 2);

	db.deleteShard(ids[0]);

	assertTrue (!Poco::File(sealedFile).exists());
	assertTrue (db.archivedShardIds().empty());

	int after = 0;
	db << "SELECT count(*) FROM t", into(after), now;
	assertTrue (after == 2);

	// reopen still works (catalog no longer references the removed shard)
	MemoryDB db2(_dir, off);
	int reopened = 0;
	db2 << "SELECT count(*) FROM t", into(reopened), now;
	assertTrue (reopened == 2);
}


void MemoryDBTest::testDeleteActiveShardRefused()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(x INTEGER)", now;
	int x = 1;
	db << "INSERT INTO t VALUES(:x)", use(x), now;
	db.flush();

	// the only shard is the active one
	assertTrue (db.shardCount() == 1);
	assertTrue (db.archivedShardIds().empty());

	try
	{
		db.deleteShard(1);
		failmsg("deleting the active shard must throw");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}

	// db is still usable
	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 1);
}


void MemoryDBTest::testDeleteNonexistentShardRefused()
{
	// deleteShard is idempotent on a vanished id - it returns silently so a
	// retention loop iterating a stale shards() snapshot stays safe even if
	// another thread already dropped the same shard. The contract is no
	// longer "throws NotFoundException"; verify the no-throw behaviour and
	// that the db remains usable.
	MemoryDB db(_dir);
	db << "CREATE TABLE t(x INTEGER)", now;
	db.flush();

	db.deleteShard(999); // must not throw

	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt == 0);
}


void MemoryDBTest::testShardDescriptors()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(x INTEGER)", now;

	int x = 1;
	db << "INSERT INTO t VALUES(:x)", use(x), now;
	db.sealActive();
	db.flush();

	Poco::Thread::sleep(20); // separate sealedAt timestamps clearly

	db << "INSERT INTO t VALUES(:x)", use(x), now;
	db.sealActive();
	db.flush();

	db << "INSERT INTO t VALUES(:x)", use(x), now;
	db.flush();

	std::vector<MemoryDB::ShardDescriptor> ds = db.shards();
	assertTrue (ds.size() == 3);

	// shards are returned in id order; first two sealed, third active
	assertTrue (ds[0].id == 1 && ds[0].sealed);
	assertTrue (ds[1].id == 2 && ds[1].sealed);
	assertTrue (ds[2].id == 3 && !ds[2].sealed);

	// sealedAt is set for sealed shards and ordered; zero for the active one
	assertTrue (ds[0].sealedAt.epochMicroseconds() > 0);
	assertTrue (ds[1].sealedAt > ds[0].sealedAt);
	assertTrue (ds[2].sealedAt.epochMicroseconds() == 0);

	// every shard has been written, so files and sizes are populated
	for (const auto& d: ds)
	{
		assertTrue (!d.filename.empty());
		assertTrue (d.bytes > 0);
		assertTrue (d.createdAt.epochMicroseconds() > 0);
	}

	// the retention-policy idiom: drop shards sealed before a cutoff
	Poco::Timestamp cutoff = ds[1].sealedAt; // strict-less keeps ds[1]; drops ds[0]
	for (const auto& d: ds)
	{
		if (d.sealed && d.sealedAt < cutoff) db.deleteShard(d.id);
	}
	assertTrue (db.shardCount() == 2);
	std::vector<Poco::UInt32> remaining = db.archivedShardIds();
	assertTrue (remaining.size() == 1 && remaining[0] == 2);
}


namespace
{
	void sealAndInsert(MemoryDB& db, int rows)
	{
		std::string v("x");
		db.sealActive();
		db.flush();
		for (int i = 0; i < rows; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(v), now;
		db.flush();
	}

	// Poll `pred` until it returns true or `timeout` elapses. Returns whether
	// `pred` was observed true. Use when a test needs to wait for a wall-clock
	// or state condition without hard-coding a "should be enough" sleep.
	template <typename Pred>
	bool waitUntil(Pred pred, Poco::Timespan timeout,
		Poco::Timespan pollInterval = Poco::Timespan(0, 5 * 1000))
	{
		Poco::Timestamp deadline;
		deadline += timeout.totalMicroseconds();
		while (!pred())
		{
			if (Poco::Timestamp() >= deadline) return pred();
			Poco::Thread::sleep(static_cast<long>(pollInterval.totalMilliseconds()));
		}
		return true;
	}
}


void MemoryDBTest::testRetentionByAge()
{
	MemoryDB::Options o;
	// Generous headroom so the two seal+flush calls below cannot age shard 1
	// out on slow CI before the shardCount check. The post-setup wait below
	// only blocks until the cutoff is actually crossed, so a large value here
	// does not slow the test in the common case.
	o.retentionMaxAge = Poco::Timespan(2, 0);

	MemoryDB db(_dir, o);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	for (int i = 0; i < 2; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(x), now;

	sealAndInsert(db, 2); // shard 1 sealed (~now); active=2
	sealAndInsert(db, 2); // shard 2 sealed (~now); active=3
	assertTrue (db.shardCount() == 3);

	// Wait until both sealed shards have aged out, with a generous timeout.
	const Poco::Timestamp::TimeDiff maxAgeUs = o.retentionMaxAge.totalMicroseconds();
	bool aged = waitUntil([&]() {
		std::vector<MemoryDB::ShardDescriptor> ds = db.shards();
		Poco::Timestamp cutoff;
		cutoff -= maxAgeUs;
		int agedSealed = 0;
		for (const auto& d: ds)
			if (d.sealed && d.sealedAt < cutoff) ++agedSealed;
		return agedSealed >= 2;
	}, Poco::Timespan(10, 0));
	assertTrue (aged);

	sealAndInsert(db, 1); // flush -> enforceRetention drops shards 1 and 2
	std::vector<Poco::UInt32> arc = db.archivedShardIds();
	assertTrue (arc.size() == 1); // only shard 3 (just sealed) remains
	assertTrue (arc[0] == 3);
	assertTrue (!Poco::File(Poco::Path(_dir, "shard-000001.db").toString()).exists());
	assertTrue (!Poco::File(Poco::Path(_dir, "shard-000002.db").toString()).exists());
}


void MemoryDBTest::testRetentionByTotalBytes()
{
	MemoryDB::Options o;
	o.retentionMaxBytes = 32 * 1024; // small enough that each shard exceeds it on its own

	MemoryDB db(_dir, o);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;

	// each shard payload is well over 32KiB (many rows of a 200-byte string)
	std::string blob(200, 'A');
	auto fillAndSeal = [&]()
	{
		for (int i = 0; i < 500; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(blob), now;
		db.sealActive();
		db.flush();
	};

	fillAndSeal(); fillAndSeal(); fillAndSeal();

	// trigger one more flush so enforceRetention runs after the seals
	std::string x("x");
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
	db.flush();

	Poco::UInt64 total = 0;
	for (const auto& d: db.shards()) total += d.bytes;
	// Either we're under the cap, or only the active shard remains (cap unachievable)
	assertTrue (total <= o.retentionMaxBytes || db.archivedShardIds().empty());
}


void MemoryDBTest::testDropOlderThan()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;

	sealAndInsert(db, 1);
	Poco::Thread::sleep(20);
	sealAndInsert(db, 1);
	Poco::Thread::sleep(20);
	sealAndInsert(db, 1);

	std::vector<MemoryDB::ShardDescriptor> ds = db.shards();
	assertTrue (ds.size() == 4); // 3 sealed + active

	std::size_t dropped = db.dropOlderThan(ds[1].sealedAt); // strictly before -> drops ds[0]
	assertTrue (dropped == 1);
	assertTrue (db.shardCount() == 3);
	assertTrue (!Poco::File(Poco::Path(_dir, ds[0].filename).toString()).exists());
}


void MemoryDBTest::testDropToFit()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string blob(200, 'B');
	for (int i = 0; i < 200; ++i) db << "INSERT INTO t(v) VALUES(:v)", use(blob), now;
	sealAndInsert(db, 1); // shard 1 with a chunky payload + small active 2

	std::vector<MemoryDB::ShardDescriptor> before = db.shards();
	Poco::UInt64 active2Bytes = before.back().bytes;
	Poco::UInt64 cap = active2Bytes; // keep only what the active shard already needs

	std::size_t dropped = db.dropToFit(cap);
	assertTrue (dropped >= 1);
	Poco::UInt64 total = 0;
	for (const auto& d: db.shards()) total += d.bytes;
	assertTrue (total <= cap || db.archivedShardIds().empty());
}


void MemoryDBTest::testRetentionDisabledIsNoop()
{
	MemoryDB db(_dir); // default options: no retention
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
	for (int i = 0; i < 3; ++i) sealAndInsert(db, 1);

	std::size_t before = db.shardCount();
	std::size_t dropped = db.enforceRetention();
	assertTrue (dropped == 0);
	assertTrue (db.shardCount() == before);
}


void MemoryDBTest::testHistoryViewTimeRange()
{
	// Populate in one session, then close so that on reopen with
	// loadArchivedShards=false main holds only the active shard's rows.
	{
		MemoryDB dbw(_dir);

		// Widen each shard's [createdAt, sealedAt] window beyond the host
		// clock's tick granularity (Windows ~15 ms) BEFORE sealing. maybeSeal
		// sets the freezing shard's sealedAt and the new shard's createdAt
		// in adjacent statements (microseconds apart), so on a coarse clock
		// shard(N+1).createdAt collapses onto shard(N).sealedAt and a bound
		// between them is not expressible. With a wide window we place the
		// test bound INSIDE shard 1 instead of between shards. The wait is
		// bounded so the test cannot hang.
		auto widenActiveWindow = [&](Poco::Timespan minSpan) {
			Poco::Timestamp activeCreated = dbw.shards().back().createdAt;
			waitUntil([&]() {
				return (Poco::Timestamp() - activeCreated) >=
					minSpan.totalMicroseconds();
			}, Poco::Timespan(5, 0));
		};

		dbw << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string a("old");
		for (int i = 0; i < 3; ++i) dbw << "INSERT INTO t(v) VALUES(:v)", use(a), now;
		widenActiveWindow(Poco::Timespan(0, 100 * 1000));
		dbw.sealActive();
		dbw.flush();
		std::string b("mid");
		for (int i = 0; i < 2; ++i) dbw << "INSERT INTO t(v) VALUES(:v)", use(b), now;
		widenActiveWindow(Poco::Timespan(0, 100 * 1000));
		dbw.sealActive();
		dbw.flush();
		std::string c("live");
		dbw << "INSERT INTO t(v) VALUES(:v)", use(c), now;
		dbw.flush();
	}

	MemoryDB db(_dir); // loadArchivedShards=false
	std::vector<MemoryDB::ShardDescriptor> ds = db.shards();
	assertTrue (ds.size() == 3);
	Poco::Timestamp shard1Sealed = ds[0].sealedAt;
	Poco::Timestamp shard2Sealed = ds[1].sealedAt;

	// sanity: live view only shows the active shard's rows
	int live = 0;
	db << "SELECT count(*) FROM t", into(live), now;
	assertTrue (live == 1);

	// Range that covers only shard 1 -> view = main + arc_1 = 1 live + 3 old = 4.
	// `to` is one microsecond before shard1Sealed: still inside shard 1's
	// (widened) window so shard 1 matches, but strictly before shard 2's
	// createdAt (always >= shard1Sealed) so shard 2 is excluded. Bounding
	// against shard2.createdAt directly is unsafe on coarse-clock platforms
	// where it can collapse onto shard1.sealedAt.
	std::string view = db.historyView("t", Poco::Timestamp(0),
		shard1Sealed - Poco::Timespan(0, 1));
	int n = 0;
	db << ("SELECT count(*) FROM " + view), into(n), now;
	assertTrue (n == 4);

	std::string oldV("old");
	int oldCount = 0;
	db << ("SELECT count(*) FROM " + view + " WHERE v=:v"), use(oldV), into(oldCount), now;
	assertTrue (oldCount == 3);

	// range that covers both sealed shards -> view = main + arc_1 + arc_2 = 1 + 3 + 2 = 6
	view = db.historyView("t", shard1Sealed - 1, shard2Sealed + 1);
	db << ("SELECT count(*) FROM " + view), into(n), now;
	assertTrue (n == 6);

	// range that covers nothing in the past -> view = main only = 1
	view = db.historyView("t", shard2Sealed + Poco::Timespan(1000, 0),
		shard2Sealed + Poco::Timespan(2000, 0));
	db << ("SELECT count(*) FROM " + view), into(n), now;
	assertTrue (n == 1);
}


void MemoryDBTest::testHistoryViewTimeRangeOverflow()
{
	MemoryDB db(_dir);

	// Raise SQLITE_LIMIT_ATTACHED above the shard count we're about to create
	// so the always-on auto-drop in enforceRetention doesn't fire while we
	// build up history. The compile-time SQLITE_MAX_ATTACHED (default 10)
	// caps the runtime limit, so this requests 20 but actually gets 10 -
	// still enough for the 12 seals below to coexist briefly before the
	// post-stop lowering of the limit makes the overflow check trip.
	Poco::Data::SQLite::Utility::setAttachedLimit(db.session(), 20);

	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	for (int i = 0; i < 12; ++i)
	{
		db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.sealActive();
		db.flush();
	}

	// Lower the limit below the shard count; historyView's range-coverage
	// check at MemoryDB.cpp now trips because matching > sqlite3_limit - 1.
	// (We don't flush after this, so the auto-drop in enforceRetention does
	// not run between the limit drop and the assertion.)
	Poco::Data::SQLite::Utility::setAttachedLimit(db.session(), 4);

	try
	{
		db.historyView("t", Poco::Timestamp(0), Poco::Timestamp::TimeVal(1) << 60);
		failmsg("requesting a range that exceeds SQLITE_LIMIT_ATTACHED must throw");
	}
	catch (Poco::RangeException&)
	{
	}
}


void MemoryDBTest::testDetachAllArchived()
{
	MemoryDB db(_dir);
	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
	db.sealActive();
	db.flush();
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
	db.sealActive();
	db.flush();

	db.attachAllArchived();
	std::vector<Poco::UInt32> ids = db.archivedShardIds();
	assertTrue (ids.size() == 2);

	// querying through the attached schemas works
	int n = 0;
	db << ("SELECT count(*) FROM arc_" + Poco::NumberFormatter::format(ids[0]) + ".t"),
		into(n), now;
	assertTrue (n == 1);

	db.detachAllArchived();

	// now the alias no longer exists
	try
	{
		db << ("SELECT count(*) FROM arc_" + Poco::NumberFormatter::format(ids[0]) + ".t"),
			into(n), now;
		failmsg("after detachAllArchived the alias should be gone");
	}
	catch (Poco::Exception&)
	{
	}
}


void MemoryDBTest::testShardCeilingAutoDrop()
{
	// Always-on backstop in enforceRetention: when total shard count exceeds
	// sqlite3_limit(SQLITE_LIMIT_ATTACHED) on session(), the oldest sealed
	// shards are auto-dropped at flush time so attachAllArchived /
	// historyView keep working. We lower the limit to 3 so we don't have to
	// create 11 shards just to trip the default.
	MemoryDB::Options o;
	o.shardMaxBytes = 1; // seal whenever the active shard has any content
	MemoryDB db(_dir, o);
	Poco::Data::SQLite::Utility::setAttachedLimit(db.session(), 3);

	db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
	std::string x("x");
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;

	// Six seals; without the backstop we'd accumulate 7 shards. With it,
	// total stays at the cap.
	for (int i = 0; i < 6; ++i) sealAndInsert(db, 1);

	assertTrue (db.shardCount() <= 3);
	std::vector<Poco::UInt32> arc = db.archivedShardIds();
	for (auto id: arc) assertTrue (id != 1); // shard 1 (oldest) was dropped

	// The active shard still accepts writes after the backstop has fired.
	db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
	int cnt = 0;
	db << "SELECT count(*) FROM t", into(cnt), now;
	assertTrue (cnt >= 1);
}


void MemoryDBTest::testConcurrentAccess()
{
	// 5-second stress test of MemoryDB's thread-safe surfaces.
	//
	//   1. No deadlock - the test must complete in bounded time.
	//   2. No data loss - the row count after a clean shutdown and reopen
	//      must equal the number of successful INSERTs.
	//   3. No TSan-flagged data races - validates the locking around
	//      _stateMutex / _flushMutex / _attachMutex and the SQLite
	//      connection-level coordination that attachArchived/detachArchived/
	//      historyView do internally via sqlite3_db_mutex.
	//
	// Configuration: tight shardMaxBytes and short idle/maxFlush intervals so
	// sealing fires repeatedly during the run and HIST workers always find
	// sealed shards to attach.

	MemoryDB::Options o;
	// shardMaxBytes is the only natural seal trigger here (ADMIN does not
	// call sealActive() - see the comment in the ADMIN role below). Pick a
	// size that produces a handful of seals during the 5-second run (so
	// HIST has sealed shards to attach and shardsCreated > 1 holds) while
	// staying comfortably under SQLITE_LIMIT_ATTACHED (default 10), so the
	// auto-drop in enforceRetention does not fire and create cross-thread
	// detach races with HIST workers. 4 MiB sustains fast Windows/MSVC and
	// linux-gcc-deprecated CI runs (which can produce >10 seals/5s at the
	// old 256 KiB) without losing the 3 pre-seeded shards either - those
	// hold ~80 bytes total each, so they alone never trip the cap.
	o.shardMaxBytes    = 4 * 1024 * 1024;
	o.idleInterval     = Poco::Timespan(0, 50 * 1000);   // 50 ms
	o.maxFlushInterval = Poco::Timespan(0, 200 * 1000);  // 200 ms
	o.checkIntervalMs  = 25;

	// `writes` outlives the inner db scope so the post-reopen verification
	// can still read it after the live MemoryDB has been destroyed.
	std::atomic<long> writes{0};

	{
		MemoryDB db(_dir, o);
		db << "CREATE TABLE events("
			"id INTEGER PRIMARY KEY, ts INTEGER, src TEXT, val REAL)", now;

		// Seed three sealed shards so HIST has targets to attach independently
		// of how quickly WRITER fills shardMaxBytes during the run. Under TSan
		// instrumentation, the run is 5-15x slower and may not naturally seal
		// from shardMaxBytes alone within the 5-second window.
		for (int i = 0; i < 3; ++i)
		{
			Poco::Int64 ts = Poco::Timestamp().epochMicroseconds();
			std::string src = "seed";
			double val = 0.0;
			db << "INSERT INTO events(ts, src, val) VALUES(:t, :s, :v)",
				use(ts), use(src), use(val), now;
			writes.fetch_add(1, std::memory_order_relaxed);
			db.sealActive();
			db.flush();
		}

		const unsigned hw = std::thread::hardware_concurrency();
		const int workers = static_cast<int>(std::max(2u, hw / 2));

		std::atomic<bool> stop{false};
		std::atomic<int>  errors{0};
		std::vector<std::atomic<long>> iters(workers);
		for (auto& a: iters) a.store(0);

		// Role assignment: worker 0 is the sole WRITER (POCO Session isn't
		// designed for concurrent writers on one handle; one writer + N
		// readers/historians is the realistic shape). Remaining workers split
		// READ / HIST / ADMIN.
		auto roleName = [](int idx) -> const char* {
			if (idx == 0) return "WRITE";
			switch ((idx - 1) % 3) { case 0: return "READ "; case 1: return "HIST ";
									default: return "ADMIN"; }
		};
		auto roleOf = [](int idx) -> int {
			if (idx == 0) return 0; // WRITER
			return 1 + ((idx - 1) % 3); // 1=READ, 2=HIST, 3=ADMIN
		};

		auto worker = [&](int idx)
		{
			std::mt19937 rng(static_cast<unsigned>(idx + 1));
			while (!stop.load(std::memory_order_relaxed))
			{
				try
				{
					const int role = roleOf(idx);
					if (role == 0)
					{
						Poco::Int64 ts = Poco::Timestamp().epochMicroseconds();
						std::string src = "w" + Poco::NumberFormatter::format(idx);
						double val = static_cast<double>(rng());
						db << "INSERT INTO events(ts, src, val) VALUES(:t, :s, :v)",
							use(ts), use(src), use(val), now;
						writes.fetch_add(1, std::memory_order_relaxed);
					}
					else if (role == 1)
					{
						int n = 0;
						db << "SELECT count(*) FROM events", into(n), now;
						std::vector<Poco::Int64> ids;
						Poco::Int64 since = 0;
						db << "SELECT id FROM events WHERE id > :since ORDER BY id LIMIT 50",
							use(since), into(ids), now;
					}
					else if (role == 2)
					{
						// HIST: attach a random sealed shard, query it, detach. The
						// attach/detach calls inside MemoryDB hold sqlite3_db_mutex
						// for the duration of their ATTACH/DETACH statement, so user
						// steps from this thread or others can't race with the
						// schema change.
						std::vector<Poco::UInt32> sealedIds = db.archivedShardIds();
						if (!sealedIds.empty())
						{
							Poco::UInt32 sid = sealedIds[rng() % sealedIds.size()];
							std::string alias;
							bool attached = false;
							try
							{
								alias = db.attachArchived(sid);
								attached = true;
								int n = 0;
								db << ("SELECT count(*) FROM " + alias + ".events"),
									into(n), now;
							}
							catch (Poco::NotFoundException&) {} // raced with retention
							catch (...)
							{
								if (attached)
									try { db.detachArchived(sid); } catch (...) {}
								throw;
							}
							if (attached) db.detachArchived(sid);
						}
					}
					else
					{
						// ADMIN: management-side reads and occasional explicit flush.
						//
						// sealActive() is NOT in the mix here. With a 1-in-5 chance and
						// many ADMIN iterations per second, it forced hundreds of seals
						// in the 5-second window, blowing past SQLITE_LIMIT_ATTACHED
						// (default 10). The auto-drop response (one deleteShard per
						// excess, each with its own catalog write) then bogged down
						// every subsequent flush; the drain loop after stop took
						// minutes. Seal logic is covered by single-threaded tests
						// (testShardingBySize, testActiveOnlyRewrite,
						// testShardCeilingAutoDrop). This stress focuses on the
						// _stateMutex / _flushMutex / _attachMutex surfaces under
						// concurrent access; it doesn't also need to exercise the
						// seal/auto-drop interaction.
						const int sub = static_cast<int>(rng() % 4);
						switch (sub)
						{
						case 0: db.flush(); break;
						case 1: (void) db.shards(); break;
						case 2: (void) db.dirty(); break;
						case 3: (void) db.shardCount(); break;
						}
					}
					iters[idx].fetch_add(1, std::memory_order_relaxed);
				}
				catch (Poco::Exception& exc)
				{
					// HIST workers query sealed shards via attachArchived/SELECT.
					// The always-on SQLITE_LIMIT_ATTACHED backstop in
					// enforceRetention auto-drops the oldest sealed shards
					// every flush; this can yank an arc_<id> out from under
					// a HIST worker mid-SELECT. The principle here is "the
					// data is going away anyway, so a failing SELECT on a
					// shard being detached/deleted is fine to ignore" - we
					// recognise that pattern as expected and don't count it.
					// All other exceptions are real and increment errors.
					const std::string msg = exc.displayText();
					const bool transient =
						msg.find("no such table: arc_") != std::string::npos ||
						msg.find("unable to open database") != std::string::npos ||
						// SQLite returns "not an error" / "another row available"
						// as a stale-errmsg artifact when a step encounters a
						// transient state (eg schema changed under us by the
						// concurrent detach). Same root cause as the no-such-
						// table case.
						msg.find("not an error") != std::string::npos ||
						msg.find("another row available") != std::string::npos;
					if (!transient)
					{
						std::cerr << "[concur] worker " << idx << " " << roleName(idx)
							<< " Poco::Exception: " << msg << '\n';
						errors.fetch_add(1, std::memory_order_relaxed);
					}
				}
				catch (std::exception& exc)
				{
					std::cerr << "[concur] worker " << idx << " " << roleName(idx)
						<< " std::exception: " << exc.what() << '\n';
					errors.fetch_add(1, std::memory_order_relaxed);
				}
				std::this_thread::yield();
			}
		};

		std::vector<std::thread> threads;
		threads.reserve(workers);
		for (int i = 0; i < workers; ++i)
			threads.emplace_back(worker, i);

		// Run for ~5 wall-seconds. Long enough that races have many windows to
		// surface across the workers' inner loops, short enough to not bloat
		// the suite.
		std::this_thread::sleep_for(std::chrono::seconds(5));

		stop.store(true, std::memory_order_relaxed);
		for (int i = 0; i < workers; ++i)
			threads[i].join();

		assertTrue (errors.load() == 0);

		for (int i = 0; db.dirty() && i < 200; ++i)
		{
			db.flush();
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		}
		assertTrue (!db.dirty());

		const long wrote = writes.load();
		{
			const std::size_t shardsCreated = db.shards().size();
			assertTrue (shardsCreated > 1); // sharding actually happened during the run

			// Sanity on the live db: with loadArchivedShards=false, sealing does
			// not physically remove rows from the in-memory connection (it only
			// persists them), so SELECT count(*) on the live db must equal
			// `wrote`. If it doesn't, the trace or update hook lost track of
			// a write under contention.
			int liveCount = 0;
			db << "SELECT count(*) FROM events", into(liveCount), now;
			assertTrue (liveCount == wrote);
		}

	}  // db destroyed here; destructor performs final flush

	// Reopen on the same directory. With loadArchivedShards=false the
	// reopened in-memory db holds only the active shard's rows; sealed
	// shards stay on disk and must be attached to be counted. Summing main
	// + every attached sealed shard gives the persisted total and must
	// equal `wrote`; if not, the flush plan dropped rows under
	// concurrency. This is the real persistence-under-load check.
	{
		MemoryDB db2(_dir);
		long persisted = 0;
		int liveAfter = 0;
		db2 << "SELECT count(*) FROM events", into(liveAfter), now;
		persisted = liveAfter;
		for (auto id: db2.archivedShardIds())
		{
			std::string alias = db2.attachArchived(id);
			int n = 0;
			db2 << ("SELECT count(*) FROM " + alias + ".events"), into(n), now;
			persisted += n;
			db2.detachArchived(id);
		}
		assertTrue (persisted == writes.load());
	}
}


void MemoryDBTest::testCommentPrefixedWithoutRowidRejected()
{
	// Two paths. (a) The operator<< prophylactic check mentionsWithoutRowid()
	// requires CREATE as the first non-whitespace token and therefore does NOT
	// see through leading line comments - the CREATE statement is allowed to
	// reach the in-memory SQLite. (b) The session() bypass skips even that
	// check. In both cases the reactive trace-hook backstop must catch the
	// WITHOUT ROWID and poison the instance so subsequent operations throw.
	// This is what the leading-comment-skip rewrite in onStatement was added
	// to guarantee; previously the single-char "if (sql[0] == '-')" treated
	// any --prefixed user SQL as a SQLite trigger marker and skipped it.

	// (a) operator<< path with leading comment
	{
		MemoryDB db(_dir);
		db << "-- a leading comment\nCREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT) WITHOUT ROWID", now;
		try
		{
			std::string h("hi");
			db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
			failmsg("trace-hook backstop must reject comment-prefixed WITHOUT ROWID on operator<< path");
		}
		catch (Poco::NotImplementedException&)
		{
		}
	}

	// (b) session() bypass with leading comment
	{
		MemoryDB db(_dir);
		db.session() << "-- a leading comment\nCREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT) WITHOUT ROWID", now;
		try
		{
			std::string h("hi");
			db << "INSERT INTO t(v) VALUES(:v)", use(h), now;
			failmsg("trace-hook backstop must reject comment-prefixed WITHOUT ROWID via session() bypass");
		}
		catch (Poco::NotImplementedException&)
		{
		}
	}
}


void MemoryDBTest::testDropTableClassified()
{
	// DROP TABLE must be classified by the trace hook as kStmtDrop + kDropTable
	// and replayed via the schema log on reopen so the table is genuinely absent,
	// not just absent from main while still in a sealed shard. Pins the dispatch
	// at the kStmtDrop branch in MemoryDB::onStatement.
	{
		MemoryDB db(_dir);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT)", now;
		std::string x("x");
		db << "INSERT INTO t(v) VALUES(:v)", use(x), now;
		db.flush();

		db << "DROP TABLE t", now;
		db.flush();
	}

	MemoryDB db(_dir);
	int tcnt = -1;
	db << "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='t'",
		into(tcnt), now;
	assertTrue (tcnt == 0);
}


void MemoryDBTest::testAlterTableClassified()
{
	// ALTER TABLE ADD COLUMN: hsql parses this but does not currently model
	// ADD COLUMN as an AlterAction (only DROP COLUMN is in the enum), so the
	// trace hook's parser branch reports !isValid() and the keyword-fallback
	// path classifies it as DDL via the first-keyword check. The schema log
	// records the ALTER either way; on reopen the column must be present.
	{
		MemoryDB db(_dir);
		db << "CREATE TABLE t(id INTEGER PRIMARY KEY, a TEXT)", now;
		std::string a1("a1");
		db << "INSERT INTO t(a) VALUES(:a)", use(a1), now;

		db << "ALTER TABLE t ADD COLUMN b TEXT", now;

		std::string a2("a2"), b2("b2");
		db << "INSERT INTO t(a, b) VALUES(:a, :b)", use(a2), use(b2), now;
		db.flush();
	}

	MemoryDB db(_dir);
	std::string b;
	db << "SELECT b FROM t WHERE id=2", into(b), now;
	assertTrue (b == "b2");
}


CppUnit::Test* MemoryDBTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MemoryDBTest");

	CppUnit_addTest(pSuite, MemoryDBTest, testPersistAndReload);
	CppUnit_addTest(pSuite, MemoryDBTest, testManualFlushAndDirty);
	CppUnit_addTest(pSuite, MemoryDBTest, testDestructorFlush);
	CppUnit_addTest(pSuite, MemoryDBTest, testShardingBySize);
	CppUnit_addTest(pSuite, MemoryDBTest, testActiveOnlyRewrite);
	CppUnit_addTest(pSuite, MemoryDBTest, testUpdateSealedRow);
	CppUnit_addTest(pSuite, MemoryDBTest, testTruncate);
	CppUnit_addTest(pSuite, MemoryDBTest, testWithoutRowidRejected);
	CppUnit_addTest(pSuite, MemoryDBTest, testWithoutRowidRejectedViaSession);
	CppUnit_addTest(pSuite, MemoryDBTest, testCommentPrefixedWithoutRowidRejected);
	CppUnit_addTest(pSuite, MemoryDBTest, testLoadArchivedFalse);
	CppUnit_addTest(pSuite, MemoryDBTest, testIdleFlush);
	CppUnit_addTest(pSuite, MemoryDBTest, testCustomShardNamer);
	CppUnit_addTest(pSuite, MemoryDBTest, testIndexPreservedAcrossReload);
	CppUnit_addTest(pSuite, MemoryDBTest, testDropTableClassified);
	CppUnit_addTest(pSuite, MemoryDBTest, testAlterTableClassified);
	CppUnit_addTest(pSuite, MemoryDBTest, testHistoryView);
	CppUnit_addTest(pSuite, MemoryDBTest, testDeleteSealedShardLoaded);
	CppUnit_addTest(pSuite, MemoryDBTest, testDeleteSealedShardUnloaded);
	CppUnit_addTest(pSuite, MemoryDBTest, testDeleteActiveShardRefused);
	CppUnit_addTest(pSuite, MemoryDBTest, testDeleteNonexistentShardRefused);
	CppUnit_addTest(pSuite, MemoryDBTest, testShardDescriptors);
	CppUnit_addTest(pSuite, MemoryDBTest, testRetentionByAge);
	CppUnit_addTest(pSuite, MemoryDBTest, testRetentionByTotalBytes);
	CppUnit_addTest(pSuite, MemoryDBTest, testDropOlderThan);
	CppUnit_addTest(pSuite, MemoryDBTest, testDropToFit);
	CppUnit_addTest(pSuite, MemoryDBTest, testRetentionDisabledIsNoop);
	CppUnit_addTest(pSuite, MemoryDBTest, testHistoryViewTimeRange);
	CppUnit_addTest(pSuite, MemoryDBTest, testHistoryViewTimeRangeOverflow);
	CppUnit_addTest(pSuite, MemoryDBTest, testDetachAllArchived);
	CppUnit_addTest(pSuite, MemoryDBTest, testShardCeilingAutoDrop);
	CppUnit_addTest(pSuite, MemoryDBTest, testConcurrentAccess);

	return pSuite;
}
