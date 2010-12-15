//
// ActiveDispatcherTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/ActiveDispatcherTest.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules it contains.  It does not include source code for modules or
//    files that typically accompany the major components of the operating
//    system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//


#include "ActiveDispatcherTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"


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
	assert (!result.available());
	activeObj.cont();
	result.wait();
	assert (result.available());
	assert (result.data() == 123);
	assert (!result.failed());
}


void ActiveDispatcherTest::testWaitInterval()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(123);
	assert (!result.available());
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
	assert (result.available());
	assert (result.data() == 123);
	assert (!result.failed());
}


void ActiveDispatcherTest::testTryWait()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(123);
	assert (!result.available());
	assert (!result.tryWait(200));
	activeObj.cont();
	assert (result.tryWait(10000));
	assert (result.available());
	assert (result.data() == 123);
	assert (!result.failed());
}


void ActiveDispatcherTest::testFailure()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testMethod(100);
	result.wait();
	assert (result.available());
	assert (result.failed());
	std::string msg = result.error();
	assert (msg == "n == 100");
}


void ActiveDispatcherTest::testVoid()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoid(123);
	assert (!result.available());
	activeObj.cont();
	result.wait();
	assert (result.available());
	assert (!result.failed());
}


void ActiveDispatcherTest::testVoidInOut()
{
	ActiveObject activeObj;
	ActiveResult<void> result = activeObj.testVoidInOut();
	assert (!result.available());
	activeObj.cont();
	result.wait();
	assert (result.available());
	assert (!result.failed());
}


void ActiveDispatcherTest::testVoidIn()
{
	ActiveObject activeObj;
	ActiveResult<int> result = activeObj.testVoidIn();
	assert (!result.available());
	activeObj.cont();
	result.wait();
	assert (result.available());
	assert (!result.failed());
	assert (result.data() == 123);
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

	return pSuite;
}
