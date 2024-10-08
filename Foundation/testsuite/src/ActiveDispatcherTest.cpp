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
#include "Poco/Environment.h"
#include <iostream>


using Poco::ActiveDispatcher;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::ActiveStarter;
using Poco::Thread;
using Poco::Event;
using Poco::Exception;
using Poco::Environment;


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

		~ActiveObject()
		{
		}

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
}


ActiveDispatcherTest::ActiveDispatcherTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveDispatcherTest::~ActiveDispatcherTest()
{
}


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


void ActiveDispatcherTest::testActiveDispatcher()
{
	std::cout << "(disabled on TSAN runs)";
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

	// see https://github.com/pocoproject/poco/pull/3617
	if (!Environment::has("TSAN_OPTIONS"))
	{
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testWait);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testWaitInterval);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testTryWait);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testFailure);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoid);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoidIn);
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testVoidInOut);
	}
	else
		CppUnit_addTest(pSuite, ActiveDispatcherTest, testActiveDispatcher);

	return pSuite;
}
