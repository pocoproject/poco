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
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Expire.h"
#include "Poco/Delegate.h"
#include "Poco/FunctionDelegate.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"


using namespace Poco;


#define LARGEINC 100


namespace
{
	// Subscriber whose handler removes another delegate from the event during
	// notify. Used by testRemoveDelegateFromNotify to exercise the lock-order
	// pattern in AbstractEvent::operator -=.
	//
	// Note: a SELF-removing handler does not trigger the cycle reliably,
	// because the back-edge (Delegate::disable inside operator -=) re-acquires
	// the same Delegate mutex that Delegate::notify is already holding. tsan
	// treats that as a recursive same-mutex re-acquire and does not add a
	// lock-order graph edge. Cross-removal sidesteps the recursion: the
	// disable inside the target operator -= touches a *different* delegate's
	// mutex, so the back-edge `M_event -> M_delegate` only materialises on
	// the same mutex pair as the forward edge when the surviving delegate
	// is later removed from outside the notify.
	class CrossRemover
	{
	public:
		CrossRemover(Poco::BasicEvent<int>& ev): _ev(ev), _pTarget(nullptr), _fired(0) {}

		void setTarget(CrossRemover* pTarget) { _pTarget = pTarget; }

		void onNotify(const void*, int&)
		{
			++_fired;
			if (_pTarget)
				_ev -= delegate(_pTarget, &CrossRemover::onNotify);
		}

		int fired() const { return _fired; }

	private:
		Poco::BasicEvent<int>& _ev;
		CrossRemover* _pTarget;
		int _fired;
	};
}


BasicEventTest::BasicEventTest(const std::string& name): CppUnit::TestCase(name)
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
	Poco::BasicEvent<int> simple;
	simple += delegate(this, &BasicEventTest::onAsync);
	assertTrue (_count == 0);
	int tmp = 0;
	Poco::ActiveResult<int>retArg = simple.notifyAsync(this, tmp);
	assertTrue (_count == 0);
	retArg.wait();
	assertTrue (retArg.data() == tmp);
	assertTrue (_count == LARGEINC);
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


void BasicEventTest::testRemoveDelegateFromNotify()
{
	// Removing a delegate from an event during notify() is documented as
	// supported by AbstractEvent::notify's doc comment:
	//
	//   "If a delegate is removed during a notify(), the delegate will no
	//    longer be invoked (unless it has already been invoked prior to
	//    removal)."
	//
	// The implementation closes a lock-order cycle between Delegate::_mutex
	// (M_delegate) and AbstractEvent::_mutex (M_event):
	//
	//   Forward edge M_delegate -> M_event:
	//     Delegate::notify holds M_delegate around the target callback;
	//     the callback does ev -= other_delegate, which acquires M_event
	//     inside AbstractEvent::operator -=.
	//
	//   Back edge M_event -> M_delegate:
	//     A separate operator -= holds M_event and calls
	//     DefaultStrategy::remove -> Delegate::disable, which acquires
	//     that delegate's M_delegate.
	//
	// Both orders observed on the same (M_delegate, M_event) pair forms a
	// cycle in the lock-order graph. ThreadSanitizer reports it as a
	// potential deadlock. Poco::Delegate uses recursive Poco::Mutex, so
	// single-threaded sequential code does not actually deadlock today,
	// but the multi-thread analogue is a real deadlock (one thread in
	// notify, another in operator -= on a delegate registered on the
	// same event).
	//
	// To exhibit the cycle we set up two cross-removing delegates:
	//   - A's target does ev -= B, which records M_A -> M_event during
	//     A's notify (B's disable acquires M_B, not M_A, so no recursion
	//     masks the forward edge on M_A).
	//   - Then external ev -= A records M_event -> M_A: M_event acquired
	//     by operator -=, then A's disable acquires M_A fresh.
	//
	// Both edges sit on the same (M_A, M_event) pair; tsan reports the
	// cycle. After the proposed fix (disable outside the event mutex),
	// the back edge disappears and the test passes cleanly.

	Poco::BasicEvent<int> ev;
	CrossRemover a(ev), b(ev);
	a.setTarget(&b);   // a's target removes b during a's notify

	ev += delegate(&a, &CrossRemover::onNotify);
	ev += delegate(&b, &CrossRemover::onNotify);

	int v = 0;
	ev.notify(this, v);
	// Forward edge M_A -> M_event recorded during a's notify.
	assertEqual(1, a.fired());

	// External remove of a: back edge M_event -> M_A recorded.
	// Cycle on (M_A, M_event) now visible in the lock-order graph.
	ev -= delegate(&a, &CrossRemover::onNotify);
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
	CppUnit_addTest(pSuite, BasicEventTest, testRemoveDelegateFromNotify);
	return pSuite;
}
