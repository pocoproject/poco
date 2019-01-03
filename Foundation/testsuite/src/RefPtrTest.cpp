//
// RefPtrTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//



#include "Poco/RefPtr.h"
#include "Poco/WeakRefPtr.h"
#include "Poco/NestedDiagnosticContext.h"
#include "Poco/Stopwatch.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"
#include "Poco/AtomicCounter.h"
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include <iostream>

#include "Poco/CppUnit/TestCase.h"
#include "RefPtrTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

using Poco::RefPtr;
using Poco::RefPtr;
using Poco::WeakRefPtr;
using Poco::SharedPtr;
using Poco::RefCountedObject;
using Poco::RCO;
using Poco::WeakRefCountedObject;
using Poco::WRCO;
using Poco::RCDC;
using Poco::AtomicCounter;
using Poco::Stopwatch;
using Poco::Thread;
using Poco::Runnable;
using Poco::Event;
using Poco::NullPointerException;
using Poco::NDC;
using Poco::Mutex;
using Poco::WeakRefCounter;
using Poco::Mutex;
using Poco::FastMutex;
using Poco::SpinlockMutex;
using Poco::NullMutex;

namespace
{
	class TestWeakObj: public WeakRefCountedObject
	{
	public:
		TestWeakObj()
		{
			++_count;
		}

	protected:
		virtual ~TestWeakObj()
		{
			--_count;
		}

	private:
		TestWeakObj(const TestWeakObj&);
		TestWeakObj& operator=(const TestWeakObj&);
		TestWeakObj(TestWeakObj&&);
		TestWeakObj& operator=(TestWeakObj&&);

		static AtomicCounter _count;
	};

	AtomicCounter TestWeakObj::_count(0);


	class TestRefObj: public RefCountedObject
	{
	public:
		TestRefObj()
		{
			++_count;
		}

	protected:
		virtual ~TestRefObj()
		{
			--_count;
		}

	private:
		TestRefObj(const TestRefObj&);
		TestRefObj& operator=(const TestRefObj&);
		TestRefObj(TestRefObj&&);
		TestRefObj& operator=(TestRefObj&&);

		static AtomicCounter _count;
	};

	AtomicCounter TestRefObj::_count(0);


	class TestObj
	{
	public:
		TestObj(int id = 0): _rc(1), _id(id)
		{
			++_count;
		}

		void duplicate()
		{
			++_rc;
		}

		void release()
		{
			if (--_rc == 0) delete this;
		}

		int rc() const
		{
			return _rc;
		}

		static int count()
		{
			return _count;
		}

		int referenceCount()
		{
			return _rc.value();
		}

		int id()
		{
			return _id.load();
		}

	protected:
		virtual ~TestObj()
		{
			--_count;
		}

	private:
		TestObj(const TestObj&);
		TestObj& operator=(const TestObj&);
		TestObj(TestObj&&);
		TestObj& operator=(TestObj&&);

		AtomicCounter _rc;
		std::atomic<int> _id;
		static AtomicCounter _count;
};

AtomicCounter TestObj::_count(0);


class TestParent1: virtual public WeakRefCountedObject
{
public:
	typedef RefPtr<TestParent1> Ptr;

	virtual int func() = 0;

protected:
	~TestParent1() { }
};

class TestParent2: virtual public WeakRefCountedObject
{
public:
	typedef RefPtr<TestParent2> Ptr;

	virtual int func() = 0;

protected:
	~TestParent2() { }
};

class TestIntermediate: public TestParent1, public TestParent2
{
public:
	int func() { return 0; }
	virtual int func1() { return 1; }
};

class TestChild: public TestIntermediate
{
public:
	typedef RefPtr<TestChild> Ptr;

	int func() { return -1; }

	Ptr getRefPtr()
	{
		return Ptr(this, true);
	}

	void setParent1(TestParent1::Ptr parent1)
	{
		_parent1 = parent1;
	}

	TestParent1::Ptr getParent1()
	{
		return _parent1.lock();
	}

	WeakRefPtr<TestParent1> getWeakParent1()
	{
		return _parent1;
	}

	void setParent2(TestParent2::Ptr parent2)
	{
		_parent2 = parent2;
	}

	TestParent2::Ptr getParent2()
	{
		return _parent2.lock();
	}

	WeakRefPtr<TestParent2> getWeakParent2()
	{
		return _parent2;
	}

	Ptr selfFromThis()
	{
		return Ptr(this, true);
	}

	TestParent1::Ptr parent1FromThis()
	{
		return Ptr(this, true).cast<TestParent1>();
	}

	TestParent2::Ptr parent2FromThis()
	{
		return Ptr(this, true).cast<TestParent2>();
	}

protected:
	~TestChild()
	{
		int i = 0;
	}

private:
	WeakRefPtr<TestParent1> _parent1;
	WeakRefPtr<TestParent2> _parent2;
};
}


RefPtrTest::RefPtrTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


RefPtrTest::~RefPtrTest()
{
}


void RefPtrTest::testRefPtr()
{
	{
		RefPtr<TestObj> ptr = new TestObj;
		assertTrue (ptr->rc() == 1);
		RefPtr<TestObj> ptr2 = ptr;
		assertTrue (ptr->rc() == 2);
		ptr2 = new TestObj;
		assertTrue (ptr->rc() == 1);
		RefPtr<TestObj> ptr3;
		ptr3 = ptr2;
		assertTrue (ptr2->rc() == 2);
		ptr3 = new TestObj;
		assertTrue (ptr2->rc() == 1);
		ptr3 = ptr2;
		assertTrue (ptr2->rc() == 2);
		assertTrue (TestObj::count() > 0);
	}
	assertTrue (TestObj::count() == 0);
}


void RefPtrTest::testOps()
{
	RefPtr<TestObj> ptr1;
	assertNull(ptr1.get());
	TestObj* pTO1 = new TestObj;
	TestObj* pTO2 = new TestObj;
	if (pTO2 < pTO1)
	{
		TestObj* pTmp = pTO1;
		pTO1 = pTO2;
		pTO2 = pTmp;
	}
	assertTrue (pTO1 < pTO2);
	ptr1 = pTO1;
	RefPtr<TestObj> ptr2 = pTO2;
	RefPtr<TestObj> ptr3 = ptr1;
	RefPtr<TestObj> ptr4;
	assertTrue (ptr1.get() == pTO1);
	assertTrue (ptr1 == pTO1);
	assertTrue (ptr2.get() == pTO2);
	assertTrue (ptr2 == pTO2);
	assertTrue (ptr3.get() == pTO1);
	assertTrue (ptr3 == pTO1);

	assertTrue (ptr1 == pTO1);
	assertTrue (ptr1 != pTO2);
	assertTrue (ptr1 < pTO2);
	assertTrue (ptr1 <= pTO2);
	assertTrue (ptr2 > pTO1);
	assertTrue (ptr2 >= pTO1);

	assertTrue (ptr1 == ptr3);
	assertTrue (ptr1 != ptr2);
	assertTrue (ptr1 < ptr2);
	assertTrue (ptr1 <= ptr2);
	assertTrue (ptr2 > ptr1);
	assertTrue (ptr2 >= ptr1);

	ptr1 = pTO1;
	ptr2 = pTO2;
	ptr1.swap(ptr2);
	assertTrue (ptr2.get() == pTO1);
	assertTrue (ptr1.get() == pTO2);

	try
	{
		assertTrue (ptr4->rc() > 0);
		fail ("must throw NullPointerException");
	}
	catch (NullPointerException&)
	{
	}

	assertTrue (!(ptr4 == ptr1));
	assertTrue (!(ptr4 == ptr2));
	assertTrue (ptr4 != ptr1);
	assertTrue (ptr4 != ptr2);

	ptr4 = ptr2;
	assertTrue (ptr4 == ptr2);
	assertTrue (!(ptr4 != ptr2));

	assertTrue (!(!ptr1));
	ptr1 = 0;
	assertTrue (!ptr1);
}


void RefPtrTest::testMove()
{
	TestObj* pObj = new TestObj;
	RefPtr<TestObj> ptr1 = pObj;
	RefPtr<TestObj> ptr2;

	assertTrue (ptr2.isNull());
	RefPtr<TestObj> ptr3 = std::move(ptr2);
	assertTrue (ptr3.isNull());
	assertTrue (ptr2.isNull());

	assertTrue (!ptr1.isNull());
	assertTrue (ptr1.get() == pObj);
	assertTrue (ptr2.isNull());
	ptr2 = std::move(ptr1);

	assertTrue (ptr1.isNull());
	assertTrue (ptr2.get() == pObj);
	assertTrue (!ptr2.isNull());
}


TestChild* ptrChild = 0;

RefPtr<TestChild> getObjPtr()
{
	return ptrChild = new TestChild;
}

RefPtr<TestChild> getObjRefPtr()
{
	RefPtr<TestChild> ptr = new TestChild;
	return ptr;
}


struct AbstractNode : public WeakRefCountedObject
{
	RefPtr<AbstractNode> next;
};


struct AbstractContainerNode: public AbstractNode
{
	RefPtr<AbstractNode> first;
};


struct Element: public AbstractContainerNode
{

};

void RefPtrTest::testInheritance()
{
	RefPtr<TestChild> child1 = new TestChild;
	RefPtr<TestChild> child0 = new TestChild;

	RefPtr<TestChild> first = child1;
	WeakRefPtr<TestChild> next = first;
	first = child0;
	first = 0;
	RefPtr<TestChild> child = new TestChild;
	RefPtr<TestParent1> parent1;
	parent1 = child;

	assertTrue (!parent1.isNull());
	assertTrue (parent1->func() == -1);

	RefPtr<TestParent2> parent2;
	parent2 = child;

	assertTrue (!parent2.isNull());
	assertTrue (parent2->func() == -1);

	WeakRefPtr<TestChild> weakChild = child;
	WeakRefPtr<TestParent1> weakParent1 = parent1;
	WeakRefPtr<TestParent2> weakParent2(parent2);

	assertTrue (!weakParent1.isNull());
	assertTrue (weakParent1->func() == -1);

	assertTrue (!weakParent2.isNull());
	assertTrue (weakParent2->func() == -1);

	parent1 = weakChild.lock();
	parent2 = weakChild.lock();

	assertTrue (!parent1.isNull());
	assertTrue (parent1->func() == -1);

	assertTrue (!parent2.isNull());
	assertTrue (parent2->func() == -1);
}


void RefPtrTest::testMoveInherited()
{
	TestChild* pObj = new TestChild;
	assertTrue (pObj->referenceCount() == 1);
	RefPtr<TestChild> ptr1 = pObj;
	ptr1 = ptr1;
	assertTrue (ptr1->referenceCount() == 1);
	assertTrue (!ptr1.isNull());
	assertTrue (pObj->referenceCount() == 1);
	ptr1 = std::move(ptr1);
	assertTrue (ptr1->referenceCount() == 1);
	assertTrue (!ptr1.isNull());
	assertTrue (pObj->referenceCount() == 1);
	RefPtr<TestChild> ptr2 = std::move(ptr1);
	assertTrue (pObj->referenceCount() == 1);
	assertTrue (ptr2->referenceCount() == 1);
	assertTrue (ptr1.isNull());
	ptr2 = std::move(ptr2);
	assertTrue (pObj->referenceCount() == 1);
	assertTrue (ptr2->referenceCount() == 1);
	assertTrue (ptr1.isNull());

	assertTrue (!ptr2.isNull());
	assertTrue (ptr2.get() == pObj);
	assertTrue (ptr1.isNull());

	ptr1 = getObjPtr();
	assertTrue (!ptr1.isNull());
	assertTrue (ptr1.get() == ptrChild);
	assertTrue (ptrChild->referenceCount() == 1);
	ptr2 = ptr1;
	assertTrue (!ptr2.isNull());
	assertTrue (ptr2.get() == ptrChild);
	assertTrue (ptrChild->referenceCount() == 2);

	ptr1 = getObjRefPtr();
	assertTrue (ptrChild->referenceCount() == 1);
	assertTrue (ptr1->referenceCount() == 1);

	RefPtr<TestParent1> parent1 = new TestChild;
	assertTrue (parent1->referenceCount() == 1);
	RefPtr<TestChild> child = parent1.cast<TestChild>();
	assertTrue (parent1->referenceCount() == 2);
	assertTrue (child->referenceCount() == 2);
	RefPtr<TestParent2> parent2 = std::move(child);
	assertTrue (child.isNull());
	assertTrue (parent1->referenceCount() == 2);
	parent1 = 0;
	assertTrue (parent1.isNull());
	assertTrue (parent2->referenceCount() == 1);
	child = parent2.unsafeCast<TestChild>();
	assertTrue (parent2->referenceCount() == 2);
	assertTrue (child->referenceCount() == 2);
	parent1 = child;
	assertTrue (parent1->referenceCount() == 3);
	assertTrue (child->referenceCount() == 3);
	parent1 = std::move(parent1);
	assertTrue (parent1->referenceCount() == 3);
	assertTrue (child->referenceCount() == 3);

	TestChild::Ptr child1 = child->getRefPtr();
	assertTrue (child->referenceCount() == 4);

	TestChild::Ptr child2 = child1;
	assertTrue (child->referenceCount() == 5);
	child1 = child2;
	assertTrue (child->referenceCount() == 5);
	assertTrue (child1->referenceCount() == 5);
	assertTrue (child2->referenceCount() == 5);

	child = 0;
	assertTrue (child1->referenceCount() == 4);
	assertTrue (child2->referenceCount() == 4);

	child1 = 0;
	assertTrue (child2->referenceCount() == 3);
	assertTrue (parent1->referenceCount() == 3);
	assertTrue (parent2->referenceCount() == 3);

	child2 = 0;
	assertTrue (parent1->referenceCount() == 2);
	assertTrue (parent2->referenceCount() == 2);

	parent1 = 0;
	assertTrue (parent2->referenceCount() == 1);
}


void RefPtrTest::testWeakRefPtr()
{
	WeakRefPtr<TestWeakObj> toWPtr0;
	assertTrue (toWPtr0.isNull());
	assertTrue (!toWPtr0.lock());
	{
		RefPtr<TestWeakObj> toPtr;
		assertTrue (toPtr.isNull());
		toWPtr0 = toPtr;
		assertTrue (toWPtr0.isNull());
		{
			toPtr = new TestWeakObj;
			assertTrue (toPtr->referenceCount() == 1);
			toWPtr0 = toPtr;
			assertTrue (!toWPtr0.isNull());
			assertTrue (toWPtr0.referenceCount() == 1);

			WeakRefPtr<TestWeakObj> toWPtr = toPtr;
			assertTrue (toPtr->referenceCount() == 1);
			{
				if (auto l = toWPtr.lock())
					assertTrue (toPtr->referenceCount() == 2);
				else
					fail ("1 WeakRefPtr failed to lock!");
			}
			assertTrue (toPtr->referenceCount() == 1);
		}
		assertTrue (!toPtr.isNull());
		assertTrue (toPtr->referenceCount() == 1);
	}
	assertTrue (toWPtr0.isNull());
	assertTrue (!toWPtr0.lock());

	TestChild::Ptr child = new TestChild;
	assertTrue (child->referenceCount() == 1);
	child->setParent1(child); // be one's self weak parent
	assertTrue (child->referenceCount() == 1);
	WeakRefPtr<TestParent1> wp1 = child->getWeakParent1();
	assertTrue (child->referenceCount() == 1);
	{
		if (auto l = wp1.lock())
		{
			assertTrue (child->referenceCount() == 2);
			assertTrue(-1 == wp1->func());
		}
		else
			fail ("2 WeakRefPtr failed to lock!");
	}
	assertTrue (child->referenceCount() == 1);

	child->setParent2(child); // another weak self-parent
	assertTrue (child->referenceCount() == 1);
	WeakRefPtr<TestParent2> wp2 = child->getWeakParent2();
	assertTrue (child->referenceCount() == 1);
	{
		if (auto l = wp2.lock())
		{
			assertTrue (child->referenceCount() == 2);
			assertTrue(-1 == wp2->func());
		}
		else
			fail ("3 WeakRefPtr failed to lock!");
	}
	assertTrue (child->referenceCount() == 1);

	{
		TestParent1::Ptr ap1 = child->getParent1();
		assertTrue (ap1 == child.cast<TestParent1>());
	}

	TestChild::Ptr child2 = new TestChild;
	assertTrue (child2->referenceCount() == 1);
	child->setParent1(child2);
	assertTrue (child2->referenceCount() == 1);
	TestParent1::Ptr tp1 = child->getParent1();
	assertTrue (tp1 == child->getParent1());

	WeakRefPtr<TestChild> wpChild = child->selfFromThis();
	assertTrue (child->referenceCount() == 1);
	assertTrue (!wpChild.isNull());
	assertTrue (!wpChild.lock().isNull());

	WeakRefPtr<TestChild> wpChild1 = child->parent1FromThis().cast<TestChild>();
	assertTrue (child->referenceCount() == 1);
	assertTrue (!wpChild1.isNull());
	assertTrue (!wpChild1.lock().isNull());

	WeakRefPtr<TestChild> wpChild2 = child->parent2FromThis().cast<TestChild>();
	assertTrue (child->referenceCount() == 1);
	assertTrue (!wpChild2.isNull());
	assertTrue (!wpChild2.lock().isNull());
}


void RefPtrTest::testWeakSemantics()
{
	/// The weak pointer integrity is preserved
	/// when the holding RefPtr is deleted.
	{
		WeakRefPtr<TestWeakObj> weak;
		assertTrue (weak.isNull());
		assertTrue (!weak.lock());
		{
			RefPtr<TestWeakObj> original = new TestWeakObj;
			assertTrue (weak.isNull());
			assertTrue (!weak.lock());

			weak = original;
			assertTrue(!weak.isNull());
			RefPtr<TestWeakObj> locked = weak.lock();
			assertTrue(!locked.isNull());
			assertTrue(locked == original);
		}
		assertTrue (weak.isNull());
		assertTrue (!weak.lock());
	}

	// RefPtr does not affect the WeakRefPtr in any
	// other way, but by invalidating the pointed-to
	// reference when strong count expires

	// copy/assign
	{
		RefPtr<TestWeakObj> original = new TestWeakObj;
		assertTrue (original->referenceCount() == 1);
		WeakRefPtr<TestWeakObj> weak = original;
		assertTrue (original->referenceCount() == 1);
		RefPtr<TestWeakObj> locked = weak.lock();
		assertTrue (original->referenceCount() == 2);
		assertTrue (locked->referenceCount() == 2);
		assertTrue(!locked.isNull());
		assertTrue(locked == original);

		RefPtr<TestWeakObj> copied = original;
		assertTrue (original->referenceCount() == 3);
		assertTrue (locked->referenceCount() == 3);
		assertTrue (copied->referenceCount() == 3);
		assertTrue(!weak.isNull());
		locked = weak.lock();
		assertTrue (original->referenceCount() == 3);
		assertTrue (locked->referenceCount() == 3);
		assertTrue(!locked.isNull());
		assertTrue(locked == original);
		assertTrue(locked.get() == original.get());
		assertTrue(locked == copied);
		assertTrue(locked.get() == copied.get());
		original = 0;
		assertTrue (locked->referenceCount() == 2);
		assertTrue (copied->referenceCount() == 2);
		assertTrue(!weak.isNull());
		assertTrue(!weak.lock().isNull());
		copied = 0;
		assertTrue (locked->referenceCount() == 1);
		assertTrue(!weak.isNull());
		assertTrue(!weak.lock().isNull());
		locked = 0;
		assertTrue(weak.isNull());
		assertTrue(weak.lock().isNull());

	}

	// moving
	{
		RefPtr<TestWeakObj> original = new TestWeakObj;

		WeakRefPtr<TestWeakObj> weak = original;
		assertTrue(!weak.isNull());
		RefPtr<TestWeakObj> locked = weak.lock();
		assertTrue(!locked.isNull());
		assertTrue(locked == original);

		RefPtr<TestWeakObj> moved = std::move(original);
		assertTrue(!weak.isNull());
		locked = weak.lock();
		assertTrue(!locked.isNull());
		assertTrue(locked == moved);
	}
}


// threads


namespace {

	class RefPtrRunnable:public Runnable
	{
	public:
		class Dummy: public RCO {};
		class WeakDummy: public WeakRefCountedObject {};

		RefPtrRunnable(): _errors(0), _msWeak(0) {}

		void run()
		{
			work();
			work<WeakDummy>(_wrco, _msWeak);
		}

		int64_t errors() const { return _errors.value(); }
		int64_t msWeak() const { return _msWeak.load(); }

		static const int _size = 10;
		static Dummy* _rco[_size];
		static WeakDummy* _wrco[_size];

	private:
		typedef std::atomic<unsigned long> TimerType;

		void work()
		{
			RefPtr<Dummy> ptr;
			for (int i = 0; i < _size; ++i)
			{
				ptr = RefPtr<Dummy>(_rco[i], true);
				if (!ptr) ++_errors;
				RefPtr<Dummy> ptr2 = ptr;
				if (!ptr2) ++_errors;
				RefPtr<Dummy> ptr3 = std::move(ptr2);
				if (!ptr2.isNull()) ++_errors;
				if (!ptr3) ++_errors;
			}
		}

		template <typename T>
		void work(T** rco, TimerType& timer)
		{
			Stopwatch sw; sw.start();
			WeakRefPtr<T> wptr;
			{
				if(wptr.lock()) ++_errors;
				RefPtr<T> ptr;
				for(int i = 0; i < _size; ++i)
				{
					ptr = RefPtr<T>(rco[i], true);
					if(!ptr) ++_errors;
					wptr = ptr;
					if(!wptr.lock()) ++_errors;
				}
			}
			// originals must be still alive
			if (!wptr.lock() || wptr.referenceCount() < 1) ++_errors;
			sw.stop(); timer += sw.elapsed();
		}

		AtomicCounter _errors;
		TimerType     _msWeak;
	};

	RefPtrRunnable::Dummy* RefPtrRunnable::_rco[RefPtrRunnable::_size] = {0};
	RefPtrRunnable::WeakDummy* RefPtrRunnable::_wrco[RefPtrRunnable::_size] = {0};
}


void RefPtrTest::testRefPtrThread()
{
	const int num = RefPtrRunnable::_size;

	for (int i = 0; i < num; ++i)
	{
		RefPtrRunnable::_rco[i] = new RefPtrRunnable::Dummy;
		RefPtrRunnable::_wrco[i] = new RefPtrRunnable::WeakDummy;
	}

	std::vector<Thread*> threads;
	std::vector<RefPtrRunnable*> runnables;
	for (int i = 0; i < num; ++i)
	{
		threads.push_back(new Thread());
		runnables.push_back(new RefPtrRunnable());
	}

	int i = 0;
	for (auto& t : threads)
	{
		t->start(*runnables[i++]);
	}

	for (auto& t : threads)
	{
		t->join();
		delete t;
	}

	std::cout << num << " threads, " << num << " loops/thread" << std::endl;
	double weak = 0;
	for (auto& r : runnables)
	{
		assertTrue(r->errors() == 0);
		weak += r->msWeak();
		delete r;
	}
	std::cout << "WeakRefPtr (avg. ms): "
			<< weak/num << " us" << std::endl;

	for (int i = 0; i < num; ++i)
	{
		assertTrue (RefPtrRunnable::_rco[i]->referenceCount() == 1);
		assertTrue (RefPtrRunnable::_wrco[i]->referenceCount() == 1);
		delete RefPtrRunnable::_rco[i];
		delete RefPtrRunnable::_wrco[i];
	}
}


// casting with multiple virtual inheritance

namespace {

struct A:public WeakRefCountedObject
{
	virtual ~A() { }
};

struct B:virtual public A { };
struct C:virtual public A { };
struct D:public B, public C { };
struct E:public D { };

}


void RefPtrTest::testWeakLeak()
{
	{
		E* e = new E;
		e->release();
		e = new E;
		delete e;
		WeakRefPtr<E> we(new E);
	}
	// there should be no leak reports for this,
	// valgrind, or refcount DC (if enabled)
}


void RefPtrTest::testWeakCast()
{
	RefPtr<E> e = new E;
	WeakRefPtr<E> weakE = e;
	RefPtr<A> a = e.cast<A>();
	WeakRefPtr<A> weakA = a;
	RefPtr<B> b = e.cast<B>();
	WeakRefPtr<B> weakB = b;
	RefPtr<C> c = e.cast<C>();
	WeakRefPtr<C> weakC = c;
	RefPtr<D> d = c.cast<D>();
	WeakRefPtr<D> weakD = d;

	// check construction/assignment/cast locked RefPtr
	{
		RefPtr<E> e1(weakE);
		RefPtr<E> e2 = weakE;
		e = weakE;
		e = weakA.lock().cast<E>();
		e = weakB.lock().cast<E>();
		e = weakC.lock().cast<E>();
		e = weakD.lock().cast<E>();
		a = weakA;
		b = weakB;
		c = weakC;
		d = weakD;
	}

	// destroy all RefPtr's in random order
	b = 0;
	assertTrue (!weakB.isNull());
	assertTrue (!weakB.lock().isNull());
	a = 0;
	assertTrue (!weakA.isNull());
	assertTrue (!weakA.lock().isNull());
	e = 0;
	assertTrue (!weakE.isNull());
	assertTrue (!weakE.lock().isNull());
	d = 0;
	assertTrue (!weakD.isNull());
	assertTrue (!weakD.lock().isNull());
	c = 0;
	assertTrue (weakB.isNull());
	assertTrue (weakB.lock().isNull());
	assertTrue (weakC.isNull());
	assertTrue (weakC.lock().isNull());
	assertTrue (weakD.isNull());
	assertTrue (weakD.lock().isNull());
	assertTrue (weakE.isNull());
	assertTrue (weakE.lock().isNull());
}


// benchmark

#if (defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_CLANG)) && (POCO_ARCH == POCO_ARCH_AMD64)

#define REF_PTR_BENCHMARK

#endif

#ifdef REF_PTR_BENCHMARK

namespace {

	template<typename T>
	void use(T &&t) {
		__asm__ __volatile__ (""::"g" (t));
	}

	void escape(void*p) {
		__asm__ __volatile__ ("" : "+r" (p)::"memory");
	}

	void clobber() {
		__asm__ __volatile__ ("" : : : "memory");
	}

}

namespace unopt {

	extern void unoptimizer();

}

namespace {

	void unoptimize()
	{
		int v[1];
		escape(v);
		use(v);
		v[0] = 42;
		unopt::unoptimizer();
		clobber();
	}

	struct DummyImpl
	{
		void dummy()
		{
			unoptimize();
		}

		DummyImpl& operator++()
		{
			unoptimize();
			return *this;
		}

		DummyImpl& operator++(int)
		{
			unoptimize();
			return *this;
		}

		DummyImpl& operator--()
		{
			unoptimize();
			return *this;
		}

		DummyImpl& operator--(int)
		{
			unoptimize();
			return *this;
		}
	};

	struct Dummy: public DummyImpl, public RefCountedObject
	{
	};

	struct WeakDummy: public DummyImpl, public WeakRefCountedObject
	{
	};

	template<typename P>
	void ops(P* p)
	{
		++(**p);
		--(**p);
		(**p)++;
		(**p)--;
	}

	template<typename P>
	void doWork(P* p)
	{
		(*p)->dummy();
		ops(p);
	}

	template<>
	void doWork(int* p)
	{
		unoptimize();
		ops(&p);
	}

	template<>
	void doWork(std::unique_ptr<int>* p)
	{
		unoptimize();
		ops(p);
	}

	template<>
	void doWork(std::shared_ptr<int>* p)
	{
		unoptimize();
		ops(p);
	}

	template<>
	void doWork(SharedPtr<int>* p)
	{
		unoptimize();
		ops(p);
	}


	template<typename F, typename T>
	void runBenchmark(int reps)
	{
		Stopwatch sw;
		sw.start();
		for(int r = 0; r < reps; ++r)
		{
			T p(new F);
			doWork(&p);
		}
		sw.stop();
		std::cout << '\t' << NDC::typeName<T>() << " : " << sw.elapsed() / 1000 << " ms" << std::endl;
	}


	template<>
	void runBenchmark<Dummy, Dummy*>(int reps)
	{
		Stopwatch sw;
		sw.start();
		for(int r = 0; r < reps; ++r)
		{
			Dummy*p = new Dummy;
			doWork(&p);
			delete p;
		}
		sw.stop();
		std::cout << '\t' << NDC::typeName<Dummy*>() << " : " << sw.elapsed() / 1000 << " ms" << std::endl;
	}


	template<>
	void runBenchmark<int, int*>(int reps)
	{
		Stopwatch sw;
		sw.start();
		for(int r = 0; r < reps; ++r)
		{
			int*p = new int;
			doWork(p);
			delete p;
		}
		sw.stop();
		std::cout << '\t' << NDC::typeName<int*>() << " : " << sw.elapsed() / 1000 << " ms" << std::endl;
	}
}

#endif // REF_PTR_BENCHMARK


void RefPtrTest::pointersBenchmark()
{
#ifdef REF_PTR_BENCHMARK

	int reps = 100;//0000;
	std::cout << reps << " repetitions" << std::endl;
	std::cout << std::endl;

	runBenchmark<int, int*>(reps);
	runBenchmark<int, std::unique_ptr<int>>(reps);
	runBenchmark<int, std::shared_ptr<int>>(reps);
	runBenchmark<int, SharedPtr<int>>(reps);
	std::cout << std::endl;

	runBenchmark<Dummy, Dummy*>(reps);
	runBenchmark<Dummy, RefPtr<Dummy>>(reps);
	runBenchmark<WeakDummy, RefPtr<WeakDummy>>(reps);
	runBenchmark<Dummy, SharedPtr<Dummy>>(reps);
	runBenchmark<Dummy, std::unique_ptr<Dummy>>(reps);
	runBenchmark<Dummy, std::shared_ptr<Dummy>>(reps);

#endif // REF_PTR_BENCHMARK
}


void RefPtrTest::setUp()
{
}


void RefPtrTest::tearDown()
{
}


CppUnit::Test* RefPtrTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RefPtrTest");

	CppUnit_addTest(pSuite, RefPtrTest, testRefPtr);
	CppUnit_addTest(pSuite, RefPtrTest, testInheritance);
	CppUnit_addTest(pSuite, RefPtrTest, testOps);
	CppUnit_addTest(pSuite, RefPtrTest, testMove);
	CppUnit_addTest(pSuite, RefPtrTest, testMoveInherited);
	CppUnit_addTest(pSuite, RefPtrTest, testWeakRefPtr);
	CppUnit_addTest(pSuite, RefPtrTest, testWeakSemantics);
	CppUnit_addTest(pSuite, RefPtrTest, testWeakLeak);
	CppUnit_addTest(pSuite, RefPtrTest, testWeakCast);
	CppUnit_addTest(pSuite, RefPtrTest, testRefPtrThread);
	//CppUnit_addTest(pSuite, RefPtrTest, pointersBenchmark);

	return pSuite;
}
