//
// AutoPtrTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AutoPtrTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <atomic>


using Poco::AutoPtr;
using Poco::makeAuto;
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

		explicit TestObj(int value): _rc(1)
		{
		}

		TestObj(int value1, const std::string& value2): _rc(1)
		{
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
		~TestObj()
		{
			--_count;
		}

	private:
		std::atomic<int> _rc;
		static int _count;
	};

	int TestObj::_count = 0;
}


AutoPtrTest::AutoPtrTest(const std::string& name): CppUnit::TestCase(name)
{
}


AutoPtrTest::~AutoPtrTest()
{
}


void AutoPtrTest::testAutoPtr()
{
	{
		AutoPtr<TestObj> ptr = new TestObj;
		assertTrue (ptr->rc() == 1);
		AutoPtr<TestObj> ptr2 = ptr;
		assertTrue (ptr->rc() == 2);
		ptr2 = new TestObj;
		assertTrue (ptr->rc() == 1);
		AutoPtr<TestObj> ptr3;
		ptr3 = ptr2;
		assertTrue (ptr2->rc() == 2);
		ptr3 = new TestObj;
		assertTrue (ptr2->rc() == 1);
		ptr3 = ptr2;
		assertTrue (ptr2->rc() == 2);
		assertTrue (TestObj::count() > 0);

		AutoPtr<TestObj> ptr4 = std::move(ptr);
		assertTrue (ptr4->rc() == 1);
		assertTrue (ptr.isNull());

		ptr3 = std::move(ptr4);
		assertTrue (ptr4.isNull());
		assertTrue (ptr3->rc() == 1);
	}
	assertTrue (TestObj::count() == 0);
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
	assertTrue (pTO1 < pTO2);
	ptr1 = pTO1;
	AutoPtr<TestObj> ptr2 = pTO2;
	AutoPtr<TestObj> ptr3 = ptr1;
	AutoPtr<TestObj> ptr4;
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

	assertTrue (ptr1 == nullptr);
	assertTrue (ptr2 != nullptr);
}


void AutoPtrTest::testMakeAuto()
{
	AutoPtr<TestObj> ptr = makeAuto<TestObj>();
	assertTrue (ptr->rc() == 1);

	ptr = makeAuto<TestObj>(42);
	assertTrue (ptr->rc() == 1);

	ptr = makeAuto<TestObj>(42, "fortytwo");
	assertTrue (ptr->rc() == 1);
}


void AutoPtrTest::setUp()
{
}


void AutoPtrTest::tearDown()
{
}


CppUnit::Test* AutoPtrTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AutoPtrTest");

	CppUnit_addTest(pSuite, AutoPtrTest, testAutoPtr);
	CppUnit_addTest(pSuite, AutoPtrTest, testOps);
	CppUnit_addTest(pSuite, AutoPtrTest, testMakeAuto);

	return pSuite;
}
