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
#include "Poco/Data/SQLite/Notifier.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Delegate.h"
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
using Poco::Data::SQLite::Notifier;
using Poco::Data::SQLite::Utility;
using namespace Poco::Data::Keywords;


namespace
{
	// Long enough for TSan to observe many interleavings (inner loops issue
	// thousands of SQL statements per second per thread) without bloating
	// the suite.
	constexpr int kRunMs = 1000;


	class CloseDuringCallback
		/// An insert handler that closes the session on another thread, which then
		/// waits for the connection mutex that SQLite holds for the handler. A third
		/// thread reads the handle and the "autoCommit" feature of the session
		/// meanwhile. The handler waits for these reads under a deadline, so that it
		/// returns, and the threads end, also if the reads wait for close().
	{
	public:
		CloseDuringCallback() = delete;
		explicit CloseDuringCallback(Session& session);
			/// Creates the handler for the given session.

		~CloseDuringCallback() = default;
			/// Destroys the handler.

		CloseDuringCallback(const CloseDuringCallback&) = delete;
		CloseDuringCallback& operator = (const CloseDuringCallback&) = delete;
		CloseDuringCallback(CloseDuringCallback&&) = delete;
		CloseDuringCallback& operator = (CloseDuringCallback&&) = delete;

		void onInsert(const void*);
			/// Starts the closing and the reading thread, and waits for the read under a deadline.

		void join();
			/// Waits until the closing and the reading thread have ended.

		[[nodiscard]] bool readWhileHeld() const;
			/// Returns true if the reading thread has read the session while the
			/// handler ran.

		[[nodiscard]] bool autoCommit() const;
			/// Returns true if the reading thread and the handler have both read
			/// the "autoCommit" feature as true.

	private:
		Session& _session;
		std::thread _closer;
		std::thread _reader;
		std::atomic<bool> _read{false};
		bool _readerAutoCommit = false;
		bool _readWhileHeld = false;
		bool _handlerAutoCommit = false;
	};


	CloseDuringCallback::CloseDuringCallback(Session& session):
		_session(session)
	{
	}


	void CloseDuringCallback::onInsert(const void*)
	{
		const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
		_closer = std::thread([this]()
		{
			try
			{
				_session.close();
			}
			catch (...)
			{
			}
		});
		_reader = std::thread([this, deadline]()
		{
			try
			{
				// close() takes the handle away before it waits for the connection mutex
				while (Utility::dbHandle(_session) != nullptr)
				{
					if (std::chrono::steady_clock::now() >= deadline) return;
					std::this_thread::yield();
				}
				_readerAutoCommit = _session.getFeature("autoCommit");
				_read = true;
			}
			catch (...)
			{
			}
		});
		while (!_read && std::chrono::steady_clock::now() < deadline)
			std::this_thread::yield();
		_readWhileHeld = _read;
		// what the reader could read, the handler can read too
		if (_readWhileHeld) _handlerAutoCommit = _session.getFeature("autoCommit");
	}


	void CloseDuringCallback::join()
	{
		if (_closer.joinable()) _closer.join();
		if (_reader.joinable()) _reader.join();
	}


	bool CloseDuringCallback::readWhileHeld() const
	{
		return _readWhileHeld;
	}


	bool CloseDuringCallback::autoCommit() const
	{
		return _readerAutoCommit && _handlerAutoCommit;
	}
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


void SQLiteThreadSafetyTest::testRaceHookRegistration()
{
	constexpr int THREAD_COUNT = 4;
	constexpr int ROUNDS = 500;
	std::atomic<int> failures{0};
	std::vector<std::thread> threads;
	for (int t = 0; t < THREAD_COUNT; ++t)
	{
		threads.emplace_back([&failures](int n)
		{
			for (int i = 0; i < n; ++i)
			{
				try
				{
					Session session(Connector::KEY, ":memory:");
					Notifier notifier(session);
					if (!notifier.updateEnabled() || !notifier.commitEnabled() || !notifier.rollbackEnabled())
						++failures;
					if (!notifier.disableAll())
						++failures;
				}
				catch (...)
				{
					++failures;
				}
			}
		}, ROUNDS);
	}
	for (auto& thread: threads) thread.join();
	assertTrue (failures.load() == 0);
}


void SQLiteThreadSafetyTest::testCloseWhileCallbackReadsSession()
{
	Session session(Connector::KEY, _dbFile);
	session << "CREATE TABLE t(i INTEGER)", now;

	CloseDuringCallback handler(session);
	{
		Notifier notifier(session, Notifier::SQLITE_NOTIFY_UPDATE);
		notifier.insert += Poco::delegate(&handler, &CloseDuringCallback::onInsert);
		session << "INSERT INTO t VALUES (1)", now;
		// the Notifier disables its callbacks only after close() has ended
		handler.join();
		notifier.insert -= Poco::delegate(&handler, &CloseDuringCallback::onInsert);
	}
	assertTrue (handler.readWhileHeld());
	assertTrue (handler.autoCommit());
	assertTrue (!session.isConnected());
}


void SQLiteThreadSafetyTest::testRaceReadSessionVsReopen()
{
	constexpr int READER_COUNT = 4;
	constexpr int ROUNDS = 200;
	Session session(Connector::KEY, _dbFile);
	std::atomic<bool> stop{false};
	std::atomic<int> failures{0};
	std::vector<std::thread> readers;
	for (int t = 0; t < READER_COUNT; ++t)
	{
		readers.emplace_back([&session, &stop, &failures]()
		{
			while (!stop)
			{
				try
				{
					// no transaction is ever open, also not while the session is closed
					if (!session.getFeature("autoCommit")) ++failures;
					static_cast<void>(Utility::dbHandle(session));
				}
				catch (...)
				{
					++failures;
				}
			}
		});
	}
	for (int i = 0; i < ROUNDS; ++i)
	{
		session.close();
		session.open();
	}
	stop = true;
	for (auto& reader: readers) reader.join();
	assertTrue (failures.load() == 0);
	assertTrue (session.isConnected());
}


CppUnit::Test* SQLiteThreadSafetyTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLiteThreadSafetyTest");

	CppUnit_addTest(pSuite, SQLiteThreadSafetyTest, testRaceHasNextVsAttach);
	CppUnit_addTest(pSuite, SQLiteThreadSafetyTest, testRaceHookRegistration);
	CppUnit_addTest(pSuite, SQLiteThreadSafetyTest, testCloseWhileCallbackReadsSession);
	CppUnit_addTest(pSuite, SQLiteThreadSafetyTest, testRaceReadSessionVsReopen);

	return pSuite;
}
