//
// FIFOEventTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FIFOEventTest.h"
#include "DummyDelegate.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Delegate.h"
#include "Poco/Expire.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include "Poco/Stopwatch.h"
#include <iostream>
#include <numeric>


using namespace Poco;


#define LARGEINC 100


FIFOEventTest::FIFOEventTest(const std::string& name): CppUnit::TestCase(name)
{
}


FIFOEventTest::~FIFOEventTest()
{
}


void FIFOEventTest::testNoDelegate()
{
	int tmp = 0;
	EventArgs args;

	assertTrue (_count == 0);
	Void.notify(this);
	assertTrue (_count == 0);

	Void += delegate(this, &FIFOEventTest::onVoid);
	Void -= delegate(this, &FIFOEventTest::onVoid);
	Void.notify(this);
	assertTrue (_count == 0);

	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	ConstSimple += delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple -= delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assertTrue (_count == 0);

	//Note: passing &args will not work due to &
	EventArgs* pArgs = &args;
	Complex += delegate(this, &FIFOEventTest::onComplex);
	Complex -= delegate(this, &FIFOEventTest::onComplex);
	Complex.notify(this, pArgs);
	assertTrue (_count == 0);

	Complex2 += delegate(this, &FIFOEventTest::onComplex2);
	Complex2 -= delegate(this, &FIFOEventTest::onComplex2);
	Complex2.notify(this, args);
	assertTrue (_count == 0);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex -= delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assertTrue (_count == 0);

	Const2Complex += delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex -= delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 0);
}


void FIFOEventTest::testSingleDelegate()
{
	int tmp = 0;
	EventArgs args;

	assertTrue (_count == 0);

	Void += delegate(this, &FIFOEventTest::onVoid);
	Void.notify(this);
	assertTrue (_count == 1);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);

	ConstSimple += delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assertTrue (_count == 3);

	EventArgs* pArgs = &args;
	Complex += delegate(this, &FIFOEventTest::onComplex);
	Complex.notify(this, pArgs);
	assertTrue (_count == 4);

	Complex2 += delegate(this, &FIFOEventTest::onComplex2);
	Complex2.notify(this, args);
	assertTrue (_count == 5);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assertTrue (_count == 6);

	Const2Complex += delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 7);
	// check if 2nd notify also works
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 8);
}


void FIFOEventTest::testDuplicateRegister()
{
	int tmp = 0;

	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
}


void FIFOEventTest::testDuplicateUnregister()
{
	// duplicate unregister shouldn't give an error,
	int tmp = 0;

	assertTrue (_count == 0);

	Simple -= delegate(this, &FIFOEventTest::onSimple); // should work
	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
}


void FIFOEventTest::testDisabling()
{
	int tmp = 0;

	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.disable();
	Simple.notify(this, tmp);
	assertTrue (_count == 0);
	Simple.enable();
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	// unregister should also work with disabled event
	Simple.disable();
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.enable();
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
}


void FIFOEventTest::testFIFOOrder()
{
	DummyDelegate o1;
	DummyDelegate o2;

	assertTrue (_count == 0);

	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o2, &DummyDelegate::onSimple2);
	int tmp = 0;
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);

	Simple -= delegate(&o1, &DummyDelegate::onSimple);
	Simple -= delegate(&o2, &DummyDelegate::onSimple2);

	// now try with the wrong order
	Simple += delegate(&o2, &DummyDelegate::onSimple2);
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	try
	{
		tmp = 0;
		Simple.notify(this, tmp);
		failmsg ("Notify should not work");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void FIFOEventTest::testFIFOOrderExpire()
{
	// expire must not break order!
	DummyDelegate o1;
	DummyDelegate o2;

	assertTrue (_count == 0);

	Simple += delegate(&o1, &DummyDelegate::onSimple, 5000);
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	int tmp = 0;
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);

	// both ways of unregistering should work
	Simple -= delegate(&o1, &DummyDelegate::onSimple, 6000);
	Simple -= delegate(&o2, &DummyDelegate::onSimple2);
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);

	// now start mixing of expire and non expire
	tmp = 0;
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);

	Simple -= delegate(&o2, &DummyDelegate::onSimple2);
	// it is not forbidden to unregister a non expiring event with an expire decorator (it is just stupid ;-))
	Simple -= delegate(&o1, &DummyDelegate::onSimple, 6000);
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);

	// now try with the wrong order
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	try
	{
		tmp = 0;
		Simple.notify(this, tmp);
		failmsg ("Notify should not work");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void FIFOEventTest::testExpire()
{
	int tmp = 0;

	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
	Poco::Thread::sleep(700);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
}


void FIFOEventTest::testExpireReRegister()
{
	int tmp = 0;

	assertTrue (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
	Poco::Thread::sleep(200);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);
	// renew registration
	Simple += delegate(this, &FIFOEventTest::onSimple, 600);
	Poco::Thread::sleep(400);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
	Poco::Thread::sleep(300);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
}


void FIFOEventTest::testReturnParams()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	int tmp = 0;
	Simple.notify(this, tmp);
	assertTrue (tmp == 1);
}


void FIFOEventTest::testOverwriteDelegate()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o1, &DummyDelegate::onSimple2);

	int tmp = 0; // onsimple requires 0 as input
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);
}


void FIFOEventTest::testAsyncNotify()
{
	Poco::FIFOEvent<int> simple;
	simple += delegate(this, &FIFOEventTest::onAsync);
	assertTrue (_count == 0);
	int tmp = 0;
	Poco::ActiveResult<int>retArg = simple.notifyAsync(this, tmp);
	assertTrue (_count == 0);
	retArg.wait();
	assertTrue (retArg.data() == tmp);
	assertTrue (_count == LARGEINC);
}

void FIFOEventTest::testAsyncNotifyBenchmark()
{
	Poco::FIFOEvent<int> simple;
	simple += delegate(this, &FIFOEventTest::onAsyncBench);
	assertTrue (_count == 0);
	const int cnt = 10000;
	int runCount = 1000;
	const Poco::Int64 allCount = cnt * runCount;
	std::vector<int> times;
	times.reserve(allCount);
	std::vector<Poco::ActiveResult<int>> vresult;
	vresult.reserve(cnt);
	Poco::Stopwatch sw;
	while (runCount-- > 0)
	{
		sw.restart();
		for (int i = 0; i < cnt; ++i)
		{
			vresult.push_back(simple.notifyAsync(this, i));
		}

		for (int i = 0; i < cnt; ++i)
		{
			vresult[i].wait();
			assertTrue (vresult[i].data() == (i*2));
		}
		sw.stop();
		times.push_back(static_cast<int>(sw.elapsed()/1000));
		vresult.clear();
	}

	Poco::UInt64 totTime = std::accumulate(times.begin(), times.end(), 0);
	double avgTime = static_cast<double>(totTime)/times.size();
	std::cout << "Total notify/wait time for " << allCount << " runs of "
		<< cnt << " tasks = " << totTime << "ms (avg/run=" << avgTime << "ms)";
	assertTrue (_count == allCount);
}


void FIFOEventTest::onVoid(const void* pSender)
{
	_count++;
}


void FIFOEventTest::onSimple(const void* pSender, int& i)
{
	_count++;
}


void FIFOEventTest::onSimpleOther(const void* pSender, int& i)
{
	_count+=100;
}


void FIFOEventTest::onConstSimple(const void* pSender, const int& i)
{
	_count++;
}


void FIFOEventTest::onComplex(const void* pSender, Poco::EventArgs* & i)
{
	_count++;
}


void FIFOEventTest::onComplex2(const void* pSender, Poco::EventArgs & i)
{
	_count++;
}


void FIFOEventTest::onConstComplex(const void* pSender, const Poco::EventArgs*& i)
{
	_count++;
}


void FIFOEventTest::onConst2Complex(const void* pSender, const Poco::EventArgs * const & i)
{
	_count++;
}


void FIFOEventTest::onAsync(const void* pSender, int& i)
{
	Poco::Thread::sleep(700);
	_count += LARGEINC ;
}

void FIFOEventTest::onAsyncBench(const void* pSender, int& i)
{
	++_count;
	i *= 2;
}

int FIFOEventTest::getCount() const
{
	return static_cast<int>(_count.load());
}


void FIFOEventTest::setUp()
{
	_count = 0;
	// must clear events, otherwise repeating test executions will fail
	// because tests are only created once, only setup is called before
	// each test run
	Void.clear();
	Simple.clear();
	ConstSimple.clear();
	Complex.clear();
	Complex2.clear();
	ConstComplex.clear();
	Const2Complex.clear();
}


void FIFOEventTest::tearDown()
{
}


CppUnit::Test* FIFOEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FIFOEventTest");

	CppUnit_addTest(pSuite, FIFOEventTest, testNoDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testSingleDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testReturnParams);
	CppUnit_addTest(pSuite, FIFOEventTest, testDuplicateRegister);
	CppUnit_addTest(pSuite, FIFOEventTest, testDuplicateUnregister);
	CppUnit_addTest(pSuite, FIFOEventTest, testDisabling);
	CppUnit_addTest(pSuite, FIFOEventTest, testFIFOOrder);
	CppUnit_addTest(pSuite, FIFOEventTest, testFIFOOrderExpire);
	CppUnit_addTest(pSuite, FIFOEventTest, testExpire);
	CppUnit_addTest(pSuite, FIFOEventTest, testExpireReRegister);
	CppUnit_addTest(pSuite, FIFOEventTest, testOverwriteDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testAsyncNotify);
	//CppUnit_addTest(pSuite, FIFOEventTest, testAsyncNotifyBenchmark);
	return pSuite;
}
