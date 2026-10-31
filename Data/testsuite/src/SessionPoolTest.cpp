//
// SessionPoolTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SessionPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SessionPoolContainer.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Thread.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Event.h"
#include "Connector.h"
#include "SessionImpl.h"
#include <array>
#include <atomic>
#include <chrono>
#include <functional>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Thread;
using Poco::AutoPtr;
using Poco::ErrorHandler;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::NullPointerException;
using Poco::Data::PooledSessionImpl;
using Poco::Data::Session;
using Poco::Data::SessionImpl;
using Poco::Data::SessionPool;
using Poco::Data::SessionPoolContainer;
using Poco::Data::SessionPoolExhaustedException;
using Poco::Data::SessionPoolExistsException;
using Poco::Data::SessionUnavailableException;
using Poco::Data::Transaction;


namespace
{
	class MovingSessionPool: public SessionPool
		/// A SessionPool whose customizeSession() moves from the
		/// session it is given, leaving it without an implementation.
	{
	public:
		MovingSessionPool(const std::string& connector, const std::string& connectionString):
			SessionPool(connector, connectionString)
		{
		}

	protected:
		void customizeSession(Session& session) override
		{
			Session moved(std::move(session));
		}
	};


	class BlockingSessionPool: public SessionPool
		/// A SessionPool whose customizeSession() waits until unblock() is called,
		/// so that a test can keep a call of get() in progress.
	{
	public:
		BlockingSessionPool(const std::string& connector, const std::string& connectionString):
			SessionPool(connector, connectionString)
		{
		}

		[[nodiscard]] bool waitUntilBlocked(long milliseconds)
			/// Waits until a call of get() waits in customizeSession().
			/// Returns false if none does within the given time.
		{
			return _blocked.tryWait(milliseconds);
		}

		void unblock()
			/// Lets the waiting call of get() go on; later calls do not wait.
		{
			_released.set();
		}

	protected:
		void customizeSession(Session&) override
		{
			_blocked.set();
			_released.wait();
		}

	private:
		Poco::Event _blocked;
		Poco::Event _released{Poco::Event::EVENT_MANUALRESET};
	};


	class RecordingSessionPool: public SessionPool
		/// A SessionPool that keeps a reference to each session it creates,
		/// so that a test can look at a session the pool has let go of.
	{
	public:
		RecordingSessionPool(const std::string& connector, const std::string& connectionString):
			SessionPool(connector, connectionString)
		{
		}

		[[nodiscard]] AutoPtr<SessionImpl> created(std::size_t index) const
			/// Returns the session created as the index-th one.
		{
			return _created.at(index);
		}

	protected:
		void customizeSession(Session& session) override
		{
			_created.push_back(AutoPtr<SessionImpl>(session.impl(), true));
		}

	private:
		std::vector<AutoPtr<SessionImpl>> _created;
	};


	class CountingErrorHandler: public ErrorHandler
		/// Counts the exceptions reported to it.
	{
	public:
		void exception(const Poco::Exception&) override
		{
			++_reports;
		}

		void exception(const std::exception&) override
		{
			++_reports;
		}

		void exception() override
		{
			++_reports;
		}

		[[nodiscard]] int reports() const
		{
			return _reports;
		}

	private:
		std::atomic<int> _reports{0};
	};


	class ErrorHandlerGuard
		/// Installs an ErrorHandler for the time of a test.
	{
	public:
		POCO_NODISCARD_CTOR explicit ErrorHandlerGuard(ErrorHandler& handler):
			_pPrevious(ErrorHandler::set(&handler))
		{
		}

		~ErrorHandlerGuard()
		{
			try
			{
				ErrorHandler::set(_pPrevious);
			}
			catch (...)
			{
				poco_unexpected();
			}
		}

		ErrorHandlerGuard(const ErrorHandlerGuard&) = delete;
		ErrorHandlerGuard(ErrorHandlerGuard&&) = delete;
		ErrorHandlerGuard& operator = (const ErrorHandlerGuard&) = delete;
		ErrorHandlerGuard& operator = (ErrorHandlerGuard&&) = delete;

	private:
		ErrorHandler* _pPrevious;
	};
}


SessionPoolTest::SessionPoolTest(const std::string& name): CppUnit::TestCase(name)
{
	Poco::Data::Test::Connector::addToFactory();
}


SessionPoolTest::~SessionPoolTest()
{
	Poco::Data::Test::Connector::removeFromFactory();
}


void SessionPoolTest::testSessionPool()
{
	SessionPool pool("test", "cs", 1, 4, 2, 10);

	pool.setFeature("f1", true);
	assertTrue (pool.getFeature("f1"));
	try { (void) pool.getFeature("g1"); fail ("must fail"); }
	catch ( Poco::NotFoundException& ) { }

	pool.setProperty("p1", 1);
	assertTrue (1 == Poco::AnyCast<int>(pool.getProperty("p1")));
	try { (void) pool.getProperty("r1"); fail ("must fail"); }
	catch ( Poco::NotFoundException& ) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 4);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());
	Session s1(pool.get());

	assertTrue (s1.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s1.getProperty("p1")));

	try { pool.setFeature("f1", true); fail ("must fail"); }
	catch ( Poco::InvalidAccessException& ) { }

	try { pool.setProperty("p1", 1); fail ("must fail"); }
	catch ( Poco::InvalidAccessException& ) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 1);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 3);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s2(pool.get("f1", false));
	assertTrue (!s2.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s2.getProperty("p1")));

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	{
		Session s3(pool.get("p1", 2));
		assertTrue (s3.getFeature("f1"));
		assertTrue (2 == Poco::AnyCast<int>(s3.getProperty("p1")));

		assertTrue (pool.capacity() == 4);
		assertTrue (pool.allocated() == 3);
		assertTrue (pool.idle() == 0);
		assertTrue (pool.connTimeout() == 10);
		assertTrue (pool.available() == 1);
		assertTrue (pool.dead() == 0);
		assertTrue (pool.allocated() == pool.used() + pool.idle());
	}

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 1);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s4(pool.get());
	assertTrue (s4.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s4.getProperty("p1")));

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s5(pool.get());

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 0);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	try
	{
		Session s6(pool.get());
		fail("pool exhausted - must throw");
	}
	catch (SessionPoolExhaustedException&) { }

	s5.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 1);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	try
	{
		s5 << "DROP TABLE IF EXISTS Test", now;
		fail("session unusable - must throw");
	}
	catch (SessionUnavailableException&) { }

	s4.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 2);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Thread::sleep(5000); // time to clean up idle sessions

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s6(pool.get());

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	s6.setFeature("connected", false);

	s6.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	assertTrue (pool.isActive());
	pool.shutdown();
	assertTrue (!pool.isActive());
	try
	{
		Session s7(pool.get());
		fail("pool shut down - must throw");
	}
	catch (InvalidAccessException&) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 0);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());
}


void SessionPoolTest::testSessionPoolContainer()
{
	SessionPoolContainer spc;
	AutoPtr<SessionPool> pPool = new SessionPool("TeSt", "Cs");
	spc.add(pPool);
	assertTrue (pPool->isActive());
	assertTrue (spc.isActive("test", "cs"));
	assertTrue (spc.isActive("test:///cs"));
	assertTrue (spc.has("test:///cs"));
	assertTrue (1 == spc.count());

	Poco::Data::Session sess = spc.get("test:///cs");
	assertTrue ("test" == sess.impl()->connectorName());
	assertTrue ("Cs" == sess.impl()->connectionString());
	assertTrue ("test:///Cs" == sess.uri());

	try { spc.add(pPool); fail ("must fail"); }
	catch (SessionPoolExistsException&) { }
	pPool->shutdown();
	assertTrue (!pPool->isActive());
	assertTrue (!spc.isActive("test", "cs"));
	assertTrue (!spc.isActive("test:///cs"));
	spc.remove(pPool->name());
	assertTrue (!spc.has("test:///cs"));
	assertTrue (!spc.isActive("test", "cs"));
	assertTrue (!spc.isActive("test:///cs"));
	assertTrue (0 == spc.count());
	try { (void) spc.get("test"); fail ("must fail"); }
	catch (NotFoundException&) { }

	spc.add("tEsT", "cs");
	spc.add("TeSt", "cs");//duplicate request, must be silently ignored
	assertTrue (1 == spc.count());
	spc.remove("TesT:///cs");
	assertTrue (0 == spc.count());
	try { (void) spc.get("test"); fail ("must fail"); }
	catch (NotFoundException&) { }
}


void SessionPoolTest::testSessionPoolMovedFromSession()
{
	MovingSessionPool pool("test", "cs");

	try
	{
		Session s(pool.get());
		fail("customized session has no implementation - must throw");
	}
	catch (NullPointerException&) { }

	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.used() == 0);
	assertTrue (pool.available() == pool.capacity());
}


void SessionPoolTest::testSessionPoolShutdown()
{
	RecordingSessionPool pool("test", "cs");
	Session s1(pool.get());
	Session s2(pool.get());
	AutoPtr<SessionImpl> pInUse = pool.created(0);
	AutoPtr<SessionImpl> pIdle = pool.created(1);
	s2.close();
	assertTrue (pool.used() == 1);
	assertTrue (pool.idle() == 1);

	pool.shutdown();
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.used() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (!pIdle->isConnected());

	// a session still in use stays usable until it is released
	assertTrue (pInUse->isConnected());
	assertTrue (s1.isConnected());
	s1 << "DROP TABLE IF EXISTS Test", now;

	// once released, it is closed instead of being returned to the pool
	s1.close();
	assertTrue (!pInUse->isConnected());
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
}


void SessionPoolTest::testSessionPoolDestroyed()
{
	AutoPtr<RecordingSessionPool> pPool = new RecordingSessionPool("test", "cs");
	Session s(pPool->get());
	AutoPtr<SessionImpl> pImpl = pPool->created(0);

	// the pool goes away while the session is still in use
	pPool.reset();
	assertTrue (pImpl->isConnected());
	assertTrue (s.isConnected());

	// releasing the session must not touch the destroyed pool
	s.close();
	assertTrue (!pImpl->isConnected());
	assertTrue (!s.isTransaction());
}


void SessionPoolTest::testSessionPoolPutBackFailure()
{
	RecordingSessionPool pool("test", "cs");
	pool.setFeature("emptyStringIsNull", true);
	Session s(pool.get());
	AutoPtr<SessionImpl> pImpl = pool.created(0);

	// with forceEmptyString on, the pool can not re-apply emptyStringIsNull
	s.setFeature("emptyStringIsNull", false);
	s.setFeature("forceEmptyString", true);

	CountingErrorHandler errorHandler;
	{
		ErrorHandlerGuard guard(errorHandler);
		s.close();
	}

	// the session is discarded and the error reported, not thrown
	assertTrue (errorHandler.reports() == 1);
	assertTrue (!pImpl->isConnected());
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.used() == 0);
	assertTrue (pool.idle() == 0);
}


void SessionPoolTest::testSessionPoolSettingsRestored()
{
	SessionPool pool("test", "cs", 1, 1);
	{
		Session s(pool.get("f1", true));
		assertTrue (s.getFeature("f1"));
	}
	{
		Session s(pool.get("p3", 2));
		assertTrue (2 == Poco::AnyCast<int>(s.getProperty("p3")));
	}
	{
		Session s(pool.get());
		assertTrue (s.getFeature("autoCommit"));
		s.setFeature("autoCommit", false);
	}

	// the session comes back from the pool as the pool created it
	Session s(pool.get());
	assertTrue (!s.getFeature("f1"));
	assertTrue (s.getProperty("p3").empty());
	assertTrue (s.getFeature("autoCommit"));
}


void SessionPoolTest::testSessionPoolTransaction()
{
	SessionPool pool("test", "cs", 1, 1);
	{
		Session s(pool.get());
		Transaction trans(s);
		assertTrue (s.isTransaction());
		assertTrue (!s.getFeature("autoCommit"));

		// returning the session to the pool rolls the transaction back
		s.close();
		assertTrue (!s.isTransaction());
		assertTrue (!trans.isActive());
	}

	// the next user gets the session in auto-commit mode
	Session s(pool.get());
	assertTrue (!s.isTransaction());
	assertTrue (s.getFeature("autoCommit"));
}


void SessionPoolTest::testSessionPoolManualCommit()
{
	RecordingSessionPool pool("test", "cs");
	CountingErrorHandler errorHandler;

	// a session left in manual-commit mode, with no transaction begun
	// through it, is rolled back and kept by the pool
	Session s1(pool.get());
	s1.setFeature("autoCommit", false);
	assertTrue (!s1.isTransaction());
	{
		ErrorHandlerGuard guard(errorHandler);
		s1.close();
	}
	assertTrue (errorHandler.reports() == 0);
	assertTrue (pool.idle() == 1);

	// the rollback comes before the pool switches auto-commit back on,
	// here by reverting get(name, value): a failing rollback shows it,
	// as the session is then discarded and the error reported
	Session s2(pool.get("autoCommit", false));
	assertTrue (!s2.isTransaction());
	s2.setFeature("throwOnRollback", true);
	{
		ErrorHandlerGuard guard(errorHandler);
		s2.close();
	}
	assertTrue (errorHandler.reports() == 1);
	assertTrue (!pool.created(0)->isConnected());
	assertTrue (pool.allocated() == 0);

	// and here by restoring the auto-commit mode the session was created with
	Session s3(pool.get());
	s3.setFeature("autoCommit", false);
	s3.setFeature("throwOnRollback", true);
	{
		ErrorHandlerGuard guard(errorHandler);
		s3.close();
	}
	assertTrue (errorHandler.reports() == 2);
	assertTrue (!pool.created(1)->isConnected());
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.used() == 0);
	assertTrue (pool.idle() == 0);
}


void SessionPoolTest::testSessionPoolRollbackUnlocked()
{
	using TestSessionImpl = Poco::Data::Test::SessionImpl;

	SessionPool pool("test", "cs", 1, 2);
	Session s(pool.get("autoCommit", false));

	// returning a session in manual-commit mode rolls it back; the pool stays
	// usable meanwhile, so a second session can be taken
	TestSessionImpl::holdNextRollback();
	Thread closer;
	closer.startFunc([&s]() { s.close(); });
	const bool held = TestSessionImpl::waitUntilRollbackHeld(10000);

	std::atomic<bool> got(false);
	Thread getter;
	getter.startFunc([&pool, &got]()
	{
		Session s2(pool.get());
		got = true;
	});
	const bool gotWhileHeld = held && getter.tryJoin(10000) && got;

	TestSessionImpl::releaseRollback();
	closer.join();
	getter.join();

	assertTrue (held);
	assertTrue (gotWhileHeld);
	assertTrue (pool.used() == 0);
	assertTrue (pool.idle() == 2);
}


void SessionPoolTest::testSessionPoolContainerWaitsForAdd()
{
	SessionPoolContainer spc;
	AutoPtr<BlockingSessionPool> pPool = new BlockingSessionPool("test", "cs");
	spc.add(pPool);

	// add() gets the session from the pool with the container locked;
	// the pool keeps it there until it is released
	Thread adder;
	adder.startFunc([&spc]()
	{
		Session s(spc.add("test", "cs"));
	});
	const bool blocked = pPool->waitUntilBlocked(10000);

	// meanwhile, each of these calls waits for the container; remove() is given
	// a name the container does not hold, so that shutdown() still finds the pool
	const std::array<std::function<void()>, 5> calls =
	{
		[&spc]() { (void) spc.has("test:///cs"); },
		[&spc]() { (void) spc.count(); },
		[&spc]() { (void) spc.isActive("test", "cs"); },
		[&spc]() { spc.remove("test:///none"); },
		[&spc]() { spc.shutdown(); }
	};
	std::array<Thread, 5> callers;
	std::atomic<bool> released(false);
	std::atomic<int> started(0);
	std::atomic<int> returned(0);
	std::atomic<int> returnedLocked(0);
	Poco::Event progress;
	bool activeLocked = false;
	if (blocked)
	{
		for (std::size_t i = 0; i < calls.size(); ++i)
		{
			const std::function<void()>& call = calls[i];
			callers[i].startFunc([&call, &released, &started, &returned, &returnedLocked, &progress]()
			{
				++started;
				progress.set();
				call();
				if (!released) ++returnedLocked;
				++returned;
				progress.set();
			});
		}

		// once all have started, give them the time to return,
		// which they must not do while the container is locked
		const int callCount = static_cast<int>(calls.size());
		auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
		while (started < callCount)
		{
			const auto remaining = std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
			if (remaining.count() <= 0) break;
			progress.tryWait(static_cast<long>(remaining.count()));
		}
		deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(200);
		while (returned < callCount)
		{
			const auto remaining = std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
			if (remaining.count() <= 0) break;
			progress.tryWait(static_cast<long>(remaining.count()));
		}

		// nor has shutdown() reached the pool
		activeLocked = pPool->isActive();
	}

	released = true;
	pPool->unblock();
	adder.join();
	for (Thread& caller: callers) caller.join();

	assertTrue (blocked);
	assertEqual (0, returnedLocked.load());
	assertTrue (activeLocked);
	assertEqual (static_cast<int>(calls.size()), returned.load());
	assertTrue (!pPool->isActive());
}


void SessionPoolTest::testPooledSessionNullHolder()
{
	try
	{
		AutoPtr<PooledSessionImpl> pImpl(new PooledSessionImpl(nullptr));
		fail("no holder - must throw");
	}
	catch (NullPointerException&) { }
}


void SessionPoolTest::setUp()
{
}


void SessionPoolTest::tearDown()
{
}


CppUnit::Test* SessionPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SessionPoolTest");

	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPool);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolContainer);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolMovedFromSession);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolShutdown);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolDestroyed);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolPutBackFailure);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolSettingsRestored);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolTransaction);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolManualCommit);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolRollbackUnlocked);
	CppUnit_addTest(pSuite, SessionPoolTest, testPooledSessionNullHolder);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolContainerWaitsForAdd);

	return pSuite;
}
