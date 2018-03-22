//
// ThreadLocalTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ThreadLocalTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/ThreadLocal.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"


using Poco::ThreadLocal;
using Poco::Thread;
using Poco::Runnable;


class TLTestRunnable: public Runnable
{
public:
	TLTestRunnable(int n): _n(n)
	{
	}

	void run()
	{
		*_count = 0;
		for (int i = 0; i < _n; ++i)
			++(*_count);
		_result = *_count;
	}
	
	int result()
	{
		return _result;
	}
	
private:
	int _n;
	int _result;
	static ThreadLocal<int> _count;
};


struct TLTestStruct
{
	int i;
	std::string s;
};


ThreadLocal<int> TLTestRunnable::_count;


ThreadLocalTest::ThreadLocalTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


ThreadLocalTest::~ThreadLocalTest()
{
}


void ThreadLocalTest::testLocality()
{
	TLTestRunnable r1(5000);
	TLTestRunnable r2(7500);
	TLTestRunnable r3(6000);
	Thread t1;
	Thread t2;
	Thread t3;
	t1.start(r1);
	t2.start(r2);
	t3.start(r3);
	t1.join();
	t2.join();
	t3.join();
	
	assertTrue (r1.result() == 5000);
	assertTrue (r2.result() == 7500);
	assertTrue (r3.result() == 6000);
}


void ThreadLocalTest::testAccessors()
{
	ThreadLocal<TLTestStruct> ts;
	ts->i = 100;
	ts->s = "foo";
	assertTrue ((*ts).i == 100);
	assertTrue ((*ts).s == "foo");
	assertTrue (ts.get().i == 100);
	assertTrue (ts.get().s == "foo");
}


void ThreadLocalTest::setUp()
{
}


void ThreadLocalTest::tearDown()
{
}


CppUnit::Test* ThreadLocalTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ThreadLocalTest");

	CppUnit_addTest(pSuite, ThreadLocalTest, testLocality);
	CppUnit_addTest(pSuite, ThreadLocalTest, testAccessors);

	return pSuite;
}
