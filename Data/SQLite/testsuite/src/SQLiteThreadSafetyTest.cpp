//
// SQLiteThreadSafetyTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLiteThreadSafetyTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/NumberFormatter.h"
#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <vector>


using Poco::Data::Session;
using Poco::Data::SQLite::Connector;
using namespace Poco::Data::Keywords;


namespace
{
	// Long enough for TSan to observe many interleavings (inner loops issue
	// thousands of SQL statements per second per thread) without bloating
	// the suite.
	constexpr int kRunMs = 1000;
}


SQLiteThreadSafetyTest::SQLiteThreadSafetyTest(const std::string& name):
	CppUnit::TestCase(name)
{
	Connector::registerConnector();
}


SQLiteThreadSafetyTest::~SQLiteThreadSafetyTest()
{
	Connector::unregisterConnector();
}


void SQLiteThreadSafetyTest::setUp()
{
	_dbFile = Poco::TemporaryFile::tempName() + ".db";
}


void SQLiteThreadSafetyTest::tearDown()
{
	try { Poco::File(_dbFile).remove(); } catch (...) {}
	// Match the SQLiteTest tearDown pattern so any test that flips shared
	// cache mode doesn't leak that into the next test.
	Connector::enableSharedCache(false);
}


void SQLiteThreadSafetyTest::testRaceHasNextVsAttach()
{
	// Pair: SQLiteStatementImpl.cpp:249 sqlite3_stmt_readonly /
	// sqlite3_changes (bare reads of Vdbe / connection state when the fix
	// wrap is absent) against ATTACH/DETACH-driven writes to Vdbe::expired
	// via sqlite3ExpirePreparedStatements.
	//
	// Verified calibration: after temporarily reverting the SQLiteMutex
	// wrap at SQLiteStatementImpl.cpp:249, this test surfaces a
	// "WARNING: ThreadSanitizer: data race" pointing at sqlite3.c:93062
	// (sqlite3ExpirePreparedStatements). With the wrap in place, no race.
	Session s(Connector::KEY, _dbFile);
	s << "CREATE TABLE t(id INTEGER PRIMARY KEY, name TEXT, val REAL)", now;
	for (int i = 0; i < 50; ++i)
	{
		std::string name = "row" + Poco::NumberFormatter::format(i);
		double val = static_cast<double>(i) * 1.5;
		s << "INSERT INTO t(name, val) VALUES(:n, :v)",
			use(name), use(val), now;
	}

	std::atomic<bool> stop{false};

	std::thread reader([&]()
	{
		while (!stop.load(std::memory_order_relaxed))
		{
			std::vector<int>         ids;
			std::vector<std::string> names;
			std::vector<double>      vals;
			try
			{
				s << "SELECT id, name, val FROM t",
					into(ids), into(names), into(vals), now;
			}
			catch (...) {} // ATTACH/DETACH can transiently invalidate stmts
		}
	});

	std::thread attacher([&]()
	{
		while (!stop.load(std::memory_order_relaxed))
		{
			try
			{
				s << "ATTACH ':memory:' AS aux", now;
				s << "DETACH aux", now;
			}
			catch (...) {}
		}
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(kRunMs));
	stop.store(true, std::memory_order_relaxed);
	reader.join();
	attacher.join();

	// No correctness assertion: the data race we want to surface doesn't
	// produce visible incorrect behavior in release mode (the bytes torn
	// aren't semantically meaningful for the stmt lifetime). The test
	// "passes" if neither thread crashes. Under TSan the race itself is
	// the failure signal.
}


CppUnit::Test* SQLiteThreadSafetyTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLiteThreadSafetyTest");

	CppUnit_addTest(pSuite, SQLiteThreadSafetyTest, testRaceHasNextVsAttach);

	return pSuite;
}
