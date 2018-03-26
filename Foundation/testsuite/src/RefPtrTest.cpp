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
#include "Poco/RefCountedObject.h"
#include "Poco/NestedDiagnosticContext.h"
#include "Poco/Stopwatch.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"
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
using Poco::AtomicCounter;
using Poco::Stopwatch;
using Poco::Thread;
using Poco::Runnable;
using Poco::Event;
using Poco::NullPointerException;
using Poco::NDC;
using Poco::Mutex;


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

private:
	int _dummy = 1;
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
	//NDC::dumpLeakRef(std::cout);

}


void RefPtrTest::testRefPtr()
{
	poco_check_ptr(0);
	{
		RefPtr<TestObj> ptr = new TestObj;
		assert (ptr->rc() == 1);
		RefPtr<TestObj> ptr2 = ptr;
		assert (ptr->rc() == 2);
		ptr2 = new TestObj;
		assert (ptr->rc() == 1);
		RefPtr<TestObj> ptr3;
		ptr3 = ptr2;
		assert (ptr2->rc() == 2);
		ptr3 = new TestObj;
		assert (ptr2->rc() == 1);
		ptr3 = ptr2;
		assert (ptr2->rc() == 2);
		assert (TestObj::count() > 0);
	}
	assert (TestObj::count() == 0);
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
	assert (pTO1 < pTO2);
	ptr1 = pTO1;
	RefPtr<TestObj> ptr2 = pTO2;
	RefPtr<TestObj> ptr3 = ptr1;
	RefPtr<TestObj> ptr4;
	assert (ptr1.get() == pTO1);
	assert (ptr1 == pTO1);
	assert (ptr2.get() == pTO2);
	assert (ptr2 == pTO2);
	assert (ptr3.get() == pTO1);
	assert (ptr3 == pTO1);

	assert (ptr1 == pTO1);
	assert (ptr1 != pTO2);
	assert (ptr1 < pTO2);
	assert (ptr1 <= pTO2);
	assert (ptr2 > pTO1);
	assert (ptr2 >= pTO1);

	assert (ptr1 == ptr3);
	assert (ptr1 != ptr2);
	assert (ptr1 < ptr2);
	assert (ptr1 <= ptr2);
	assert (ptr2 > ptr1);
	assert (ptr2 >= ptr1);

	ptr1 = pTO1;
	ptr2 = pTO2;
	ptr1.swap(ptr2);
	assert (ptr2.get() == pTO1);
	assert (ptr1.get() == pTO2);

	try
	{
		assert (ptr4->rc() > 0);
		fail ("must throw NullPointerException");
	}
	catch (NullPointerException&)
	{
	}

	assert (!(ptr4 == ptr1));
	assert (!(ptr4 == ptr2));
	assert (ptr4 != ptr1);
	assert (ptr4 != ptr2);

	ptr4 = ptr2;
	assert (ptr4 == ptr2);
	assert (!(ptr4 != ptr2));

	assert (!(!ptr1));
	ptr1 = 0;
	assert (!ptr1);
}


void RefPtrTest::testMove()
{
	TestObj* pObj = new TestObj;
	RefPtr<TestObj> ptr1 = pObj;
	RefPtr<TestObj> ptr2;

	assert (!ptr1.isNull());
	assert (ptr1.get() == pObj);
	assert (ptr2.isNull());
	ptr2 = std::move(ptr1);

	assert (ptr1.isNull());
	assert (ptr2.get() == pObj);
	assert (!ptr2.isNull());
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


void RefPtrTest::testInheritance()
{
	RefPtr<TestChild> child = new TestChild;
	RefPtr<TestParent1> parent1;
	parent1 = child;

	assert (!parent1.isNull());
	assert (parent1->func() == -1);

	RefPtr<TestParent2> parent2;
	parent2 = child;

	assert (!parent2.isNull());
	assert (parent2->func() == -1);
}


void RefPtrTest::testMoveInherited()
{
	TestChild* pObj = new TestChild;
	assert (pObj->referenceCount() == 1);
	RefPtr<TestChild> ptr1 = pObj;
	ptr1 = ptr1;
	assert (ptr1->referenceCount() == 1);
	assert (!ptr1.isNull());
	assert (pObj->referenceCount() == 1);
	ptr1 = std::move(ptr1);
	assert (ptr1->referenceCount() == 1);
	assert (!ptr1.isNull());
	assert (pObj->referenceCount() == 1);
	RefPtr<TestChild> ptr2 = std::move(ptr1);
	assert (pObj->referenceCount() == 1);
	assert (ptr2->referenceCount() == 1);
	assert (ptr1.isNull());
	ptr2 = std::move(ptr2);
	assert (pObj->referenceCount() == 1);
	assert (ptr2->referenceCount() == 1);
	assert (ptr1.isNull());

	assert (!ptr2.isNull());
	assert (ptr2.get() == pObj);
	assert (ptr1.isNull());

	ptr1 = getObjPtr();
	assert (!ptr1.isNull());
	assert (ptr1.get() == ptrChild);
	assert (ptrChild->referenceCount() == 1);
	ptr2 = ptr1;
	assert (!ptr2.isNull());
	assert (ptr2.get() == ptrChild);
	assert (ptrChild->referenceCount() == 2);

	ptr1 = getObjRefPtr();
	assert (ptrChild->referenceCount() == 1);
	assert (ptr1->referenceCount() == 1);

	RefPtr<TestParent1> parent1 = new TestChild;
	assert (parent1->referenceCount() == 1);
	RefPtr<TestChild> child = parent1.cast<TestChild>();
	assert (parent1->referenceCount() == 2);
	assert (child->referenceCount() == 2);
	RefPtr<TestParent2> parent2 = std::move(child);
	assert (child.isNull());
	assert (parent1->referenceCount() == 2);
	parent1 = 0;
	assert (parent1.isNull());
	assert (parent2->referenceCount() == 1);
	child = parent2.unsafeCast<TestChild>();
	assert (parent2->referenceCount() == 2);
	assert (child->referenceCount() == 2);
	parent1 = child;
	assert (parent1->referenceCount() == 3);
	assert (child->referenceCount() == 3);
	parent1 = std::move(parent1);
	assert (parent1->referenceCount() == 3);
	assert (child->referenceCount() == 3);

	TestChild::Ptr child1 = child->getRefPtr();
	assert (child->referenceCount() == 4);

	TestChild::Ptr child2 = child1;
	assert (child->referenceCount() == 5);
	child1 = child2;
	assert (child->referenceCount() == 5);
	assert (child1->referenceCount() == 5);
	assert (child2->referenceCount() == 5);

	child = 0;
	assert (child1->referenceCount() == 4);
	assert (child2->referenceCount() == 4);

	child1 = 0;
	assert (child2->referenceCount() == 3);
	assert (parent1->referenceCount() == 3);
	assert (parent2->referenceCount() == 3);

	child2 = 0;
	assert (parent1->referenceCount() == 2);
	assert (parent2->referenceCount() == 2);

	parent1 = 0;
	assert (parent2->referenceCount() == 1);
}


void RefPtrTest::testWeakRefPtr()
{
	WeakRefPtr<TestWeakObj> toWPtr0;
	assert (toWPtr0.isNull());
	assert (!toWPtr0.lock());
	{
		RefPtr<TestWeakObj> toPtr;
		assert (toPtr.isNull());
		toWPtr0 = toPtr;
		assert (toWPtr0.isNull());
		{
			toPtr = new TestWeakObj;
			assert (toPtr->referenceCount() == 1);
			toWPtr0 = toPtr;
			assert (!toWPtr0.isNull());
			assert (toWPtr0.referenceCount() == 1);

			WeakRefPtr<TestWeakObj> toWPtr = toPtr;
			assert (toPtr->referenceCount() == 1);
			{
				if (auto l = toWPtr.lock())
					assert (toPtr->referenceCount() == 2);
				else
					fail ("1 WeakRefPtr failed to lock!");
			}
			assert (toPtr->referenceCount() == 1);
		}
		assert (!toPtr.isNull());
		assert (toPtr->referenceCount() == 1);
	}
	assert (toWPtr0.isNull());
	assert (!toWPtr0.lock());

	TestChild::Ptr child = new TestChild;
	assert (child->referenceCount() == 1);
	child->setParent1(child); // be one's self weak parent
	assert (child->referenceCount() == 1);
	WeakRefPtr<TestParent1> wp1 = child->getWeakParent1();
	assert (child->referenceCount() == 1);
	{
		if (auto l = wp1.lock())
		{
			assert (child->referenceCount() == 2);
			assert(-1 == wp1->func());
		}
		else
			fail ("2 WeakRefPtr failed to lock!");
	}
	assert (child->referenceCount() == 1);

	child->setParent2(child); // another weak self-parent
	assert (child->referenceCount() == 1);
	WeakRefPtr<TestParent2> wp2 = child->getWeakParent2();
	assert (child->referenceCount() == 1);
	{
		if (auto l = wp2.lock())
		{
			assert (child->referenceCount() == 2);
			assert(-1 == wp2->func());
		}
		else
			fail ("3 WeakRefPtr failed to lock!");
	}
	assert (child->referenceCount() == 1);

	{
		TestParent1::Ptr ap1 = child->getParent1();
		assert (ap1 == child.cast<TestParent1>());
	}

	TestChild::Ptr child2 = new TestChild;
	assert (child2->referenceCount() == 1);
	child->setParent1(child2);
	assert (child2->referenceCount() == 1);
	TestParent1::Ptr tp1 = child->getParent1();
	assert (tp1 == child->getParent1());

	WeakRefPtr<TestChild> wpChild = child->selfFromThis();
	assert (child->referenceCount() == 1);
	assert (!wpChild.isNull());
	assert (!wpChild.lock().isNull());

	WeakRefPtr<TestChild> wpChild1 = child->parent1FromThis().cast<TestChild>();
	assert (child->referenceCount() == 1);
	assert (!wpChild1.isNull());
	assert (!wpChild1.lock().isNull());

	WeakRefPtr<TestChild> wpChild2 = child->parent2FromThis().cast<TestChild>();
	assert (child->referenceCount() == 1);
	assert (!wpChild2.isNull());
	assert (!wpChild2.lock().isNull());
	/*
	WeakRefPtr<TestChild> wpChild = child->selfFromThis();
	assert (wpChild);
	assert (wpChild.lock());

	WeakRefPtr<TestChild> wpChild1 = child->parent1FromThis().cast<TestChild>();
	assert (wpChild1);
	assert (wpChild1.lock());

	WeakRefPtr<TestChild> wpChild2 = child->parent2FromThis().cast<TestChild>();
	assert (wpChild2);
	assert (wpChild2.lock());
	 */
}


void RefPtrTest::testWeakSemantics()
{
	/// The weak pointer integrity is preserved
	/// when the holding RefPtr is deleted.
	{
		WeakRefPtr<TestWeakObj> weak;
		assert (weak.isNull());
		assert (!weak.lock());
		{
			RefPtr<TestWeakObj> original = new TestWeakObj;
			assert (weak.isNull());
			assert (!weak.lock());

			weak = original;
			assert(!weak.isNull());
			RefPtr<TestWeakObj> locked = weak.lock();
			assert(!locked.isNull());
			assert(locked == original);
		}
		assert (weak.isNull());
		assert (!weak.lock());
	}

	// RefPtr does not affect the WeakRefPtr in any
	// other way, but by invalidating the pointed-to
	// reference when strong count expires

	// copy/assign
	{
		RefPtr<TestWeakObj> original = new TestWeakObj;
		assert (original->referenceCount() == 1);
		WeakRefPtr<TestWeakObj> weak = original;
		assert (original->referenceCount() == 1);
		RefPtr<TestWeakObj> locked = weak.lock();
		assert (original->referenceCount() == 2);
		assert (locked->referenceCount() == 2);
		assert(!locked.isNull());
		assert(locked == original);

		RefPtr<TestWeakObj> copied = original;
		assert (original->referenceCount() == 3);
		assert (locked->referenceCount() == 3);
		assert (copied->referenceCount() == 3);
		assert(!weak.isNull());
		locked = weak.lock();
		assert (original->referenceCount() == 3);
		assert (locked->referenceCount() == 3);
		assert(!locked.isNull());
		assert(locked == original);
		assert(locked.get() == original.get());
		assert(locked == copied);
		assert(locked.get() == copied.get());
		original = 0;
		assert (locked->referenceCount() == 2);
		assert (copied->referenceCount() == 2);
		assert(!weak.isNull());
		assert(!weak.lock().isNull());
		copied = 0;
		assert (locked->referenceCount() == 1);
		assert(!weak.isNull());
		assert(!weak.lock().isNull());
		locked = 0;
		assert(weak.isNull());
		assert(weak.lock().isNull());

	}

	// moving
	{
		RefPtr<TestWeakObj> original = new TestWeakObj;

		WeakRefPtr<TestWeakObj> weak = original;
		assert(!weak.isNull());
		RefPtr<TestWeakObj> locked = weak.lock();
		assert(!locked.isNull());
		assert(locked == original);

		RefPtr<TestWeakObj> moved = std::move(original);
		assert(!weak.isNull());
		locked = weak.lock();
		assert(!locked.isNull());
		assert(locked == moved);
	}
}


// threads


namespace {

	class RefPtrRunnable:public Runnable
	{
	public:
		class Dummy: public RCO {};
		class WeakDummy: public WRCO {};

		RefPtrRunnable(): _errors(0)
		{
		}

		void run()
		{
			work();
			workWeak();
		}

		int errors() const
		{
			return _errors.value();
		}

		static const int _size = 10;
		static Dummy* _rco[_size];
		static WeakDummy* _wrco[_size];

	private:
		void work()
		{
			RefPtr<Dummy> ptr;
			for (int i = 0; i < 10; ++i)
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

		void workWeak()
		{
			WeakRefPtr<WeakDummy> wptr;
			{
				if(wptr.lock()) ++_errors;
				RefPtr<WeakDummy> ptr;
				for(int i = 0; i < 10; ++i)
				{
					ptr = RefPtr<WeakDummy>(_wrco[i], true);
					if(!ptr) ++_errors;
					wptr = ptr;
					if(!wptr.lock()) ++_errors;
				}
			}
			// originals must be still alive
			if (!wptr.lock() || wptr.referenceCount() < 1) ++_errors;
		}

		AtomicCounter _errors;
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

	for (auto& r : runnables)
	{
		assert(r->errors() == 0);
		delete r;
	}

	for (int i = 0; i < num; ++i)
	{
		assert (RefPtrRunnable::_rco[i]->referenceCount() == 1);
		assert (RefPtrRunnable::_wrco[i]->referenceCount() == 1);
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

void RefPtrTest::testWeakCast()
{
	RefPtr<E> e = new E;
	WeakRefPtr<E> weakE = e;
	RefPtr<B> b = e.cast<B>();
	WeakRefPtr<B> weakB = b;
	RefPtr<C> c = e.cast<C>();
	WeakRefPtr<C> weakC = c;
	RefPtr<D> d = c.cast<D>();
	WeakRefPtr<D> weakD = d;

	// destroy all RefPtr's in random order
	b = 0;
	assert (!weakB.isNull());
	assert (!weakB.lock().isNull());
	e = 0;
	assert (!weakE.isNull());
	assert (!weakE.lock().isNull());
	d = 0;
	assert (!weakD.isNull());
	assert (!weakD.lock().isNull());
	c = 0;
	assert (weakB.isNull());
	assert (weakB.lock().isNull());
	assert (weakC.isNull());
	assert (weakC.lock().isNull());
	assert (weakD.isNull());
	assert (weakD.lock().isNull());
	assert (weakE.isNull());
	assert (weakE.lock().isNull());
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

	struct Dummy:public RefCountedObject
	{
		void dummy()
		{
			unoptimize();
		}

		Dummy &operator++()
		{
			unoptimize();
			return *this;
		}

		Dummy &operator++(int)
		{
			unoptimize();
			return *this;
		}

		Dummy &operator--()
		{
			unoptimize();
			return *this;
		}

		Dummy &operator--(int)
		{
			unoptimize();
			return *this;
		}
	};

	struct WeakDummy:public WeakRefCountedObject
	{
		void dummy()
		{
			unoptimize();
		}

		WeakDummy &operator++()
		{
			unoptimize();
			return *this;
		}

		WeakDummy &operator++(int)
		{
			unoptimize();
			return *this;
		}

		WeakDummy &operator--()
		{
			unoptimize();
			return *this;
		}

		WeakDummy &operator--(int)
		{
			unoptimize();
			return *this;
		}
	};

	struct WeakDummyNoMutex:public Poco::RefCountedObjectImpl<Poco::WeakRefCounter<Poco::NullMutex>>
	{
		void dummy()
		{
			unoptimize();
		}

		WeakDummyNoMutex &operator++()
		{
			unoptimize();
			return *this;
		}

		WeakDummyNoMutex &operator++(int)
		{
			unoptimize();
			return *this;
		}

		WeakDummyNoMutex &operator--()
		{
			unoptimize();
			return *this;
		}

		WeakDummyNoMutex &operator--(int)
		{
			unoptimize();
			return *this;
		}
	};


	template<typename P>
	void ops(P*p)
	{
		++(**p);
		--(**p);
		(**p)++;
		(**p)--;
	}

	template<typename P>
	void doWork(P*p)
	{
		(*p)->dummy();
		ops(p);
	}

	template<>
	void doWork(int*p)
	{
		unoptimize();
		ops(&p);
	}

	template<>
	void doWork(std::unique_ptr<int>*p)
	{
		unoptimize();
		ops(p);
	}

	template<>
	void doWork(std::shared_ptr<int>*p)
	{
		unoptimize();
		ops(p);
	}

	template<>
	void doWork(SharedPtr<int>*p)
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
		std::cout << '\t' << NDC::typeName<T>() << ", " << reps << " repetitions: " << sw.elapsed() / 1000 << " ms" << std::endl;
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
		std::cout << '\t' << NDC::typeName<Dummy*>() << ", " << reps << " repetitions: " << sw.elapsed() / 1000 << " ms" << std::endl;
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
		std::cout << '\t' << NDC::typeName<int*>() << ", " << reps << " repetitions: " << sw.elapsed() / 1000 << " ms" << std::endl;
	}
}

#endif // POCO_ARCH == POCO_ARCH_AMD64


void RefPtrTest::pointersBenchmark()
{
#ifdef REF_PTR_BENCHMARK

	int reps = 1000000;
	std::cout << std::endl;

	runBenchmark<int, int*>(reps);
	runBenchmark<int, std::unique_ptr<int>>(reps);
	runBenchmark<int, std::shared_ptr<int>>(reps);
	runBenchmark<int, SharedPtr<int>>(reps);
	std::cout << std::endl;

	runBenchmark<Dummy, Dummy*>(reps);
	runBenchmark<Dummy, RefPtr<Dummy>>(reps);
	runBenchmark<WeakDummy, RefPtr<WeakDummy>>(reps);
	runBenchmark<WeakDummyNoMutex, RefPtr<WeakDummyNoMutex>>(reps);
	runBenchmark<Dummy, SharedPtr<Dummy>>(reps);
	runBenchmark<Dummy, std::unique_ptr<Dummy>>(reps);
	runBenchmark<Dummy, std::shared_ptr<Dummy>>(reps);

#endif // POCO_ARCH == POCO_ARCH_AMD64
}


void RefPtrTest::setUp()
{
}


void RefPtrTest::tearDown()
{
	//Poco::NDC::dumpLeakRef(std::cout);
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
	CppUnit_addTest(pSuite, RefPtrTest, testWeakCast);
	CppUnit_addTest(pSuite, RefPtrTest, testRefPtrThread);
	//CppUnit_addTest(pSuite, RefPtrTest, pointersBenchmark);

	return pSuite;
}
