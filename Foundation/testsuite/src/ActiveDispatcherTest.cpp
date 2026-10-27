//
// ActiveDispatcherTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActiveDispatcherTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include <atomic>
#include <thread>
#include <vector>


using Poco::ActiveDispatcher;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::ActiveStarter;
using Poco::Thread;
using Poco::Event;
using Poco::Exception;


namespace
{
	class ActiveObject: public ActiveDispatcher
	{
	public:
		ActiveObject():
			testMethod(this, &ActiveObject::testMethodImpl),
			testVoid(this, &ActiveObject::testVoidImpl),
			testVoidInOut(this, &ActiveObject::testVoidInOutImpl),
			testVoidIn(this, &ActiveObject::testVoidInImpl)
		{
		}

		~ActiveObject() override = default;

		ActiveMethod<int, int, ActiveObject, ActiveStarter<ActiveDispatcher> > testMethod;

		ActiveMethod<void, int, ActiveObject, ActiveStarter<ActiveDispatcher> > testVoid;

		ActiveMethod<void, void, ActiveObject, ActiveStarter<ActiveDispatcher> > testVoidInOut;

		ActiveMethod<int, void, ActiveObject, ActiveStarter<ActiveDispatcher> > testVoidIn;

		void cont()
		{
			_continue.set();
		}

	protected:
		int testMethodImpl(const int& n)
		{
			if (n == 100) throw Exception("n == 100");
			_continue.wait();
			return n;
		}

		void testVoidImpl(const int& n)
		{
			if (n == 100) throw Exception("n == 100");
			_continue.wait();
		}

		void testVoidInOutImpl()
		{
			_continue.wait();
		}

		int testVoidInImpl()
		{
			_continue.wait();
			return 123;
		}

	private:
		Event _continue;
	};


	constexpr long RESULT_TIMEOUT = 10000;


	class SerialActiveObject: public ActiveDispatcher
		/// An active object that records whether a call of its active method
		/// started while another call was running.
	{
	public:
		SerialActiveObject();
			/// Creates the active object.

		~SerialActiveObject() override = default;
			/// Destroys the active object.

		ActiveMethod<int, int, SerialActiveObject, ActiveStarter<ActiveDispatcher>> echo;
			/// Returns its argument.

		[[nodiscard]] bool overlapped() const;
			/// Returns true if two calls of echo ran at the same time.

	private:
		int echoImpl(const int& n);

		std::atomic<int> _inProgress{0};
		std::atomic<bool> _overlapped{false};
	};


	SerialActiveObject::SerialActiveObject():
		echo(this, &SerialActiveObject::echoImpl)
	{
	}


	bool SerialActiveObject::overlapped() const
	{
		return _overlapped;
	}


	int SerialActiveObject::echoImpl(const int& n)
	{
		if (++_inProgress > 1) _overlapped = true;
		// Gives a second dispatcher thread, if there were one, the chance to
		// run a call at the same time.
		Thread::yield();
		--_inProgress;
		return n;
	}
}


ActiveDispatcherTest::ActiveDispatcherTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveDispatcherTest::~ActiveDispatcherTest() = default;


void ActiveDispatcherTest::testWait()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(123);
	assertTrue (!result.available());
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (result.data() == 123);
	assertTrue (!result.failed());
}


void ActiveDispatcherTest::testWaitInterval()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(123);
	assertTrue (!result.available());
	try
	{
		result.wait(100);
		fail("wait must fail");
	}
	catch (Exception&)
	{
	}
	activeObj.cont();
	result.wait(10000);
	assertTrue (result.available());
	assertTrue (result.data() == 123);
	assertTrue (!result.failed());
}


void ActiveDispatcherTest::testTryWait()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(123);
	assertTrue (!result.available());
	assertTrue (!result.tryWait(200));
	activeObj.cont();
	assertTrue (result.tryWait(10000));
	assertTrue (result.available());
	assertTrue (result.data() == 123);
	assertTrue (!result.failed());
}


void ActiveDispatcherTest::testFailure()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(100);
	result.wait();
	assertTrue (result.available());
	assertTrue (result.failed());
#ifndef POCO_ENABLE_TRACE
	std::string msg = result.error();
	assertEqual ("n == 100", msg);
#endif
}


void ActiveDispatcherTest::testVoid()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoid(123);
	assertTrue (!result.available());
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
}


void ActiveDispatcherTest::testVoidInOut()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoidInOut();
	assertTrue (!result.available());
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
}


void ActiveDispatcherTest::testVoidIn()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testVoidIn();
	assertTrue (!result.available());
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
	assertTrue (result.data() == 123);
}


void ActiveDispatcherTest::testConcurrentFirstCalls()
{
	constexpr int CALLER_COUNT = 8;
	constexpr int NO_RESULT = -1;

	SerialActiveObject activeObj;
	std::vector<int> results(CALLER_COUNT, NO_RESULT);
	Event start(Event::EVENT_MANUALRESET);
	std::vector<std::thread> callers;
	callers.reserve(CALLER_COUNT);
	for (int i = 0; i < CALLER_COUNT; ++i)
	{
		callers.emplace_back([&, i]()
		{
			start.wait();
			ActiveResult<int> result = activeObj.echo(i);
			if (result.tryWait(RESULT_TIMEOUT) && !result.failed())
				results[i] = result.data();
		});
	}
	start.set();
	for (auto& caller: callers)
	{
		caller.join();
	}

	for (int i = 0; i < CALLER_COUNT; ++i)
	{
		assertEqual (i, results[i]);
	}
	assertTrue (!activeObj.overlapped());
}


void ActiveDispatcherTest::setUp()
{
}


void ActiveDispatcherTest::tearDown()
{
}


CppUnit::Test* ActiveDispatcherTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActiveDispatcherTest");

	CppUnit_addTest(pSuite, ActiveDispatcherTest, testWait);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testWaitInterval);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testTryWait);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testFailure);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoid);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoidIn);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoidInOut);
	CppUnit_addTest(pSuite, ActiveDispatcherTest, testConcurrentFirstCalls);

	return pSuite;
}
