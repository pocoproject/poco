//
// BasicEventTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BasicEventTest.h"
#include "DummyDelegate.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Expire.h"
#include "Poco/Delegate.h"
#include "Poco/FunctionDelegate.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include "Poco/StdFunctionDelegate.h"


using namespace Poco;


#define LARGEINC 100


BasicEventTest::BasicEventTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


BasicEventTest::~BasicEventTest()
{
}


void BasicEventTest::testNoDelegate()
{
	int tmp = 0;
	EventArgs args;

	assertTrue (_count == 0);
	assertTrue (Void.empty());
	Void.notify(this);
	assertTrue (_count == 0);

	Void += delegate(this, &BasicEventTest::onVoid);
	assertTrue (!Void.empty());
	Void -= delegate(this, &BasicEventTest::onVoid);
	assertTrue (Void.empty());
	Void.notify(this);

	assertTrue (_count == 0);
	assertTrue (Simple.empty());
	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple);
	assertTrue (!Simple.empty());
	Simple -= delegate(this, &BasicEventTest::onSimple);
	assertTrue (Simple.empty());
	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimpleNoSender);
	Simple -= delegate(this, &BasicEventTest::onSimpleNoSender);
	Simple.notify(this, tmp);
	assertTrue (_count == 0);
	
	ConstSimple += delegate(this, &BasicEventTest::onConstSimple);
	ConstSimple -= delegate(this, &BasicEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assertTrue (_count == 0);
	
	//Note: passing &args will not work due to &
	EventArgs* pArgs = &args;
	Complex += delegate(this, &BasicEventTest::onComplex);
	Complex -= delegate(this, &BasicEventTest::onComplex);
	Complex.notify(this, pArgs);
	assertTrue (_count == 0);

	Complex2 += delegate(this, &BasicEventTest::onComplex2);
	Complex2 -= delegate(this, &BasicEventTest::onComplex2);
	Complex2.notify(this, args);
	assertTrue (_count == 0);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &BasicEventTest::onConstComplex);
	ConstComplex -= delegate(this, &BasicEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assertTrue (_count == 0);

	Const2Complex += delegate(this, &BasicEventTest::onConst2Complex);
	Const2Complex -= delegate(this, &BasicEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 0);

	Simple += delegate(&BasicEventTest::onStaticSimple);
	Simple += delegate(&BasicEventTest::onStaticSimple);
	Simple += delegate(&BasicEventTest::onStaticSimple2);
	Simple += delegate(&BasicEventTest::onStaticSimple3);
	
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
	Simple -= delegate(BasicEventTest::onStaticSimple);

	Void += delegate(&BasicEventTest::onStaticVoid);
	Void += delegate(&BasicEventTest::onStaticVoid);

	Void.notify(this);
	assertTrue (_count == 5);
	Void -= delegate(BasicEventTest::onStaticVoid);
}


void BasicEventTest::testSingleDelegate()
{
	int tmp = 0;
	std::string tmpStr;
	EventArgs args;

	assertTrue (_count == 0);

	Void += delegate(this, &BasicEventTest::onVoid);
	Void.notify(this);
	assertTrue (_count == 1);

	Simple += delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);
	
	ConstSimple += delegate(this, &BasicEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assertTrue (_count == 3);

	tmpStr = "str";
	SimpleString += delegate(this, &BasicEventTest::onString);
	SimpleString.notify(this, tmpStr);
	assertTrue (_str == "str");

	tmpStr = "strConst";
	ConstString += delegate(this, &BasicEventTest::onConstString);
	ConstString.notify(this, tmpStr);
	assertTrue (_str == "strConst");

	EventArgs* pArgs = &args;
	Complex += delegate(this, &BasicEventTest::onComplex);
	Complex.notify(this, pArgs);
	assertTrue (_count == 4);

	Complex2 += delegate(this, &BasicEventTest::onComplex2);
	Complex2.notify(this, args);
	assertTrue (_count == 5);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &BasicEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assertTrue (_count == 6);

	Const2Complex += delegate(this, &BasicEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 7);
	// check if 2nd notify also works
	Const2Complex.notify(this, pArgs);
	assertTrue (_count == 8);
	
}


void BasicEventTest::testDuplicateRegister()
{
	int tmp = 0;
	
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple);
	Simple += delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);
	Simple -= delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
}


void BasicEventTest::testNullMutex()
{
	Poco::BasicEvent<int, NullMutex> ev;
	int tmp = 0;
	
	assertTrue (_count == 0);

	ev += delegate(this, &BasicEventTest::onSimple);
	ev += delegate(this, &BasicEventTest::onSimple);
	ev.notify(this, tmp);
	assertTrue (_count == 2);
	ev -= delegate(this, &BasicEventTest::onSimple);
	ev.notify(this, tmp);
	assertTrue (_count == 3);
}


void BasicEventTest::testDuplicateUnregister()
{
	// duplicate unregister shouldn't give an error,
	int tmp = 0;
	
	assertTrue (_count == 0);

	Simple -= delegate(this, &BasicEventTest::onSimple); // should work
	Simple.notify(this, tmp);
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	Simple -= delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	Simple -= delegate(this, &BasicEventTest::onSimple);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
}


void BasicEventTest::testDisabling()
{
	int tmp = 0;
	
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple);
	Simple.disable();
	Simple.notify(this, tmp);
	assertTrue (_count == 0);
	Simple.enable();
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	// unregister should also work with disabled event
	Simple.disable();
	Simple -= delegate(this, &BasicEventTest::onSimple);
	Simple.enable();
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
}


void BasicEventTest::testExpire()
{
	int tmp = 0;
	
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
	Poco::Thread::sleep(700);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);

	Simple += delegate(&BasicEventTest::onStaticSimple, 400);
	Simple += delegate(&BasicEventTest::onStaticSimple, 400);
	Simple += delegate(&BasicEventTest::onStaticSimple2, 400);
	Simple += delegate(&BasicEventTest::onStaticSimple3, 400);
	Simple.notify(this, tmp);
	assertTrue (_count == 4);
	Poco::Thread::sleep(700);
	Simple.notify(this, tmp);
	assertTrue (_count == 4);
}


void BasicEventTest::testExpireReRegister()
{
	int tmp = 0;
	
	assertTrue (_count == 0);

	Simple += delegate(this, &BasicEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assertTrue (_count == 1);
	Poco::Thread::sleep(200);
	Simple.notify(this, tmp);
	assertTrue (_count == 2);
	// renew registration
	Simple += delegate(this, &BasicEventTest::onSimple, 600);
	Poco::Thread::sleep(400);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
	Poco::Thread::sleep(300);
	Simple.notify(this, tmp);
	assertTrue (_count == 3);
}


void BasicEventTest::testReturnParams()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	int tmp = 0;
	Simple.notify(this, tmp);
	assertTrue (tmp == 1);
}


void BasicEventTest::testOverwriteDelegate()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o1, &DummyDelegate::onSimple2);

	int tmp = 0; // onsimple requires 0 as input
	Simple.notify(this, tmp);
	assertTrue (tmp == 2);
}


void BasicEventTest::testAsyncNotify()
{
	Poco::BasicEvent<int>* pSimple= new Poco::BasicEvent<int>();
	(*pSimple) += delegate(this, &BasicEventTest::onAsync);
	assertTrue (_count == 0);
	int tmp = 0;
	Poco::ActiveResult<int>retArg = pSimple->notifyAsync(this, tmp);
	delete pSimple; // must work even when the event got deleted!
	pSimple = NULL;
	assertTrue (_count == 0);
	retArg.wait();
	assertTrue (retArg.data() == tmp);
	assertTrue (_count == LARGEINC);
}


void BasicEventTest::testLambda()
{
	int count = 0;
	auto f = StdFunctionDelegate<int>([&](const void *, int &args) { count += args; });

	Simple += f;
	int cparam = 1;
	Simple.notify(this, cparam);
	assertTrue (count == 1);

	Simple -= f;
	assertTrue (Simple.empty());
}


void BasicEventTest::onStaticVoid(const void* pSender)
{
	BasicEventTest* p = const_cast<BasicEventTest*>(reinterpret_cast<const BasicEventTest*>(pSender));
	p->_count++;
}


void BasicEventTest::onVoid(const void* pSender)
{
	_count++;
}


void BasicEventTest::onSimpleNoSender(int& i)
{
	_count++;
}


void BasicEventTest::onSimple(const void* pSender, int& i)
{
	_count++;
}


void BasicEventTest::onStaticSimple(const void* pSender, int& i)
{
	BasicEventTest* p = const_cast<BasicEventTest*>(reinterpret_cast<const BasicEventTest*>(pSender));
	p->_count++;
}


void BasicEventTest::onStaticSimple2(void* pSender, int& i)
{
	BasicEventTest* p = reinterpret_cast<BasicEventTest*>(pSender);
	p->_count++;
}


void BasicEventTest::onStaticSimple3(int& i)
{
}


void BasicEventTest::onSimpleOther(const void* pSender, int& i)
{
	_count+=100;
}


void BasicEventTest::onConstSimple(const void* pSender, const int& i)
{
	_count++;
}


void BasicEventTest::onString(const void* pSender, std::string& str)
{
	_str = str;
}


void BasicEventTest::onConstString(const void* pSender, const std::string& str)
{
	_str = str;
}


void BasicEventTest::onComplex(const void* pSender, Poco::EventArgs* & i)
{
	_count++;
}


void BasicEventTest::onComplex2(const void* pSender, Poco::EventArgs & i)
{
	_count++;
}


void BasicEventTest::onConstComplex(const void* pSender, const Poco::EventArgs*& i)
{
	_count++;
}


void BasicEventTest::onConst2Complex(const void* pSender, const Poco::EventArgs * const & i)
{
	_count++;
}


void BasicEventTest::onAsync(const void* pSender, int& i)
{
	Poco::Thread::sleep(700);
	_count += LARGEINC ;
}


int BasicEventTest::getCount() const
{
	return _count;
}


void BasicEventTest::setUp()
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


void BasicEventTest::tearDown()
{
}


CppUnit::Test* BasicEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BasicEventTest");

	CppUnit_addTest(pSuite, BasicEventTest, testNoDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testSingleDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testReturnParams);
	CppUnit_addTest(pSuite, BasicEventTest, testDuplicateRegister);
	CppUnit_addTest(pSuite, BasicEventTest, testDuplicateUnregister);
	CppUnit_addTest(pSuite, BasicEventTest, testDisabling);
	CppUnit_addTest(pSuite, BasicEventTest, testExpire);
	CppUnit_addTest(pSuite, BasicEventTest, testExpireReRegister);
	CppUnit_addTest(pSuite, BasicEventTest, testOverwriteDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testAsyncNotify);
	CppUnit_addTest(pSuite, BasicEventTest, testNullMutex);
	CppUnit_addTest(pSuite, BasicEventTest, testLambda);
	return pSuite;
}
