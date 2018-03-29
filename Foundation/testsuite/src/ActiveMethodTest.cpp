//
// ActiveMethodTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ActiveMethodTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"


using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::Thread;
using Poco::Event;
using Poco::Exception;


namespace
{
	class ActiveObject
	{
	public:
		typedef ActiveMethod<int, int, ActiveObject>   IntIntType;
		typedef ActiveMethod<void, int, ActiveObject>  VoidIntType;
		typedef ActiveMethod<void, void, ActiveObject> VoidVoidType;
		typedef ActiveMethod<int, void, ActiveObject>  IntVoidType;

		ActiveObject():
			testMethod(this, &ActiveObject::testMethodImpl),
			testVoid(this,&ActiveObject::testVoidOutImpl),
			testVoidInOut(this,&ActiveObject::testVoidInOutImpl),
			testVoidIn(this,&ActiveObject::testVoidInImpl)
		{
		}
		
		~ActiveObject()
		{
		}
		
		IntIntType testMethod;

		VoidIntType testVoid;

		VoidVoidType testVoidInOut;

		IntVoidType testVoidIn;
		
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

		void testVoidOutImpl(const int& n)
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


ActiveMethodTest::ActiveMethodTest(const std::string& name): CppUnit::TestCase(name)
{
}


ActiveMethodTest::~ActiveMethodTest()
{
}


void ActiveMethodTest::testWait()
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


void ActiveMethodTest::testCopy()
{
	ActiveObject activeObj;

	ActiveObject::IntIntType ii = activeObj.testMethod;
	ActiveResult<int> rii = ii(123);
	assertTrue (!rii.available());
	activeObj.cont();
	rii.wait();
	assertTrue (rii.available());
	assertTrue (rii.data() == 123);
	assertTrue (!rii.failed());

	ActiveObject::VoidIntType  vi = activeObj.testVoid;
	ActiveResult<void> rvi = vi(123);
	assertTrue (!rvi.available());
	activeObj.cont();
	rvi.wait();
	assertTrue (rvi.available());
	assertTrue (!rvi.failed());

	ActiveObject::VoidVoidType vv = activeObj.testVoidInOut;
	ActiveResult<void> rvv = vv();
	assertTrue (!rvv.available());
	activeObj.cont();
	rvv.wait();
	assertTrue (rvv.available());
	assertTrue (!rvv.failed());

	ActiveObject::IntVoidType  iv = activeObj.testVoidIn;
	ActiveResult<int> riv = iv();
	assertTrue (!riv.available());
	activeObj.cont();
	riv.wait();
	assertTrue (riv.available());
	assertTrue (riv.data() == 123);
	assertTrue (!riv.failed());
}


void ActiveMethodTest::testWaitInterval()
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


void ActiveMethodTest::testTryWait()
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


void ActiveMethodTest::testFailure()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(100);
	result.wait();
	assertTrue (result.available());
	assertTrue (result.failed());
	std::string msg = result.error();
	assertTrue (msg == "n == 100");
}


void ActiveMethodTest::testVoidOut()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoid(101);
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
}


void ActiveMethodTest::testVoidInOut()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoidInOut();
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
}


void ActiveMethodTest::testVoidIn()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testVoidIn();
	activeObj.cont();
	result.wait();
	assertTrue (result.available());
	assertTrue (!result.failed());
	assertTrue (result.data() == 123);
}


void ActiveMethodTest::setUp()
{
}


void ActiveMethodTest::tearDown()
{
}


CppUnit::Test* ActiveMethodTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ActiveMethodTest");

	CppUnit_addTest(pSuite, ActiveMethodTest, testWait);
	CppUnit_addTest(pSuite, ActiveMethodTest, testCopy);
	CppUnit_addTest(pSuite, ActiveMethodTest, testWaitInterval);
	CppUnit_addTest(pSuite, ActiveMethodTest, testTryWait);
	CppUnit_addTest(pSuite, ActiveMethodTest, testFailure);
	CppUnit_addTest(pSuite, ActiveMethodTest, testVoidOut);
	CppUnit_addTest(pSuite, ActiveMethodTest, testVoidIn);
	CppUnit_addTest(pSuite, ActiveMethodTest, testVoidInOut);

	return pSuite;
}
