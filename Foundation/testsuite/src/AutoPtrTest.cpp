//
// AutoPtrTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AutoPtrTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Exception.h"


using Poco::AutoPtr;
using Poco::RefCountedObject;
using Poco::NullPointerException;


namespace
{
	class TestObj
	{
	public:
		TestObj(): _rc(1)
		{
			++_count;
		}

		void duplicate()
		{
			++_rc;
		}

		void release()
		{
			if (--_rc == 0)
				delete this;
		}

		int rc() const
		{
			return _rc;
		}

		static int count()
		{
			return _count;
		}

	protected:
		virtual ~TestObj()
		{
			--_count;
		}

	private:
		int _rc;
		static int _count;
	};

	int TestObj::_count = 0;

	class TestParent1: virtual public RefCountedObject
	{
	public:
		typedef AutoPtr<TestParent1> Ptr;

		virtual int func() = 0;

	protected:
		~TestParent1() {}
	};

	class TestParent2: virtual public RefCountedObject
	{
	public:
		typedef AutoPtr<TestParent2> Ptr;

		virtual int func() = 0;

	protected:
		~TestParent2() {}
	};

	class TestChild: public TestParent1, public TestParent2
	{
	public:
		typedef AutoPtr<TestChild> Ptr;

		int func() { return -1; }

		Ptr getAutoPtr()
		{
			return Ptr(this, true);
		}

	protected:
		~TestChild() {}
	};
}


AutoPtrTest::AutoPtrTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


AutoPtrTest::~AutoPtrTest()
{
}


void AutoPtrTest::testAutoPtr()
{
	{
		AutoPtr<TestObj> ptr = new TestObj;
		assert (ptr->rc() == 1);
		AutoPtr<TestObj> ptr2 = ptr;
		assert (ptr->rc() == 2);
		ptr2 = new TestObj;
		assert (ptr->rc() == 1);
		AutoPtr<TestObj> ptr3;
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


void AutoPtrTest::testOps()
{
	AutoPtr<TestObj> ptr1;
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
	AutoPtr<TestObj> ptr2 = pTO2;
	AutoPtr<TestObj> ptr3 = ptr1;
	AutoPtr<TestObj> ptr4;
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


void AutoPtrTest::testMove()
{
	TestObj* pObj = new TestObj;
	AutoPtr<TestObj> ptr1 = pObj;
	AutoPtr<TestObj> ptr2;

	assert (!ptr1.isNull());
	assert (ptr1.get() == pObj);
	assert (ptr2.isNull());
	ptr2 = std::move(ptr1);

	assert (ptr1.isNull());
	assert (ptr2.get() == pObj);
	assert (!ptr2.isNull());
}


TestChild* ptrChild = 0;

AutoPtr<TestChild> getObjPtr()
{
	return ptrChild = new TestChild;
}

AutoPtr<TestChild> getObjAutoPtr()
{
	AutoPtr<TestChild> ptr = new TestChild;
	return ptr;
}


void AutoPtrTest::testInheritance()
{
	AutoPtr<TestChild> child = new TestChild;
	AutoPtr<TestParent1> parent1;
	parent1 = child;

	assert (!parent1.isNull());
	assert (parent1->func() == -1);

	AutoPtr<TestParent2> parent2;
	parent2 = child;

	assert (!parent2.isNull());
	assert (parent2->func() == -1);
}


void AutoPtrTest::testMoveInherited()
{
	TestChild* pObj = new TestChild;
	assert (pObj->referenceCount() == 1);
	AutoPtr<TestChild> ptr1 = pObj;
	ptr1 = ptr1;
	assert (ptr1->referenceCount() == 1);
	assert (!ptr1.isNull());
	assert (pObj->referenceCount() == 1);
	ptr1 = std::move(ptr1);
	assert (ptr1->referenceCount() == 1);
	assert (!ptr1.isNull());
	assert (pObj->referenceCount() == 1);
	AutoPtr<TestChild> ptr2 = std::move(ptr1);
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

	ptr1 = getObjAutoPtr();
	assert (ptrChild->referenceCount() == 1);
	assert (ptr1->referenceCount() == 1);

	AutoPtr<TestParent1> parent1 = new TestChild;
	assert (parent1->referenceCount() == 1);
	AutoPtr<TestChild> child = parent1.unsafeCast<TestChild>();
	assert (parent1->referenceCount() == 2);
	assert (child->referenceCount() == 2);
	AutoPtr<TestParent2> parent2 = std::move(child);
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

	TestChild::Ptr child1 = child->getAutoPtr();
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


void AutoPtrTest::setUp()
{
}


void AutoPtrTest::tearDown()
{
	Poco::NDC::dumpLeakRef(std::cout);
}


CppUnit::Test* AutoPtrTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AutoPtrTest");

	CppUnit_addTest(pSuite, AutoPtrTest, testAutoPtr);
	CppUnit_addTest(pSuite, AutoPtrTest, testInheritance);
	CppUnit_addTest(pSuite, AutoPtrTest, testOps);
	CppUnit_addTest(pSuite, AutoPtrTest, testMove);
	CppUnit_addTest(pSuite, AutoPtrTest, testMoveInherited);

	return pSuite;
}
