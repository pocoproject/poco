//
// AnyTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AnyTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Any.h"
#include "Poco/SharedPtr.h"
#include "Poco/Bugcheck.h"
#include <vector>
#include <memory>


#if defined(_MSC_VER) && _MSC_VER < 1400
	#pragma warning(disable:4800)//forcing value to bool 'true' or 'false'
#endif


using namespace Poco;


class SomeClass
{
public:
	int i;
	std::string str;
	SomeClass(int h, std::string s): i (h), str(s)
	{
	};
	bool operator==(const SomeClass& other) const
	{
		return i == other.i && str == other.str;
	}
};


AnyTest::AnyTest(const std::string& name): CppUnit::TestCase(name)
{
}


AnyTest::~AnyTest()
{
}


void AnyTest::testDefaultCtor()
{
	const Any value;

	assertTrue (value.empty());
	assertTrue (0 == AnyCast<int>(&value));
	assertTrue (value.type() == typeid(void));
}


void AnyTest::testConvertingCtor()
{
	std::string text = "test message";
	Any value = text;

	assertTrue (!value.empty());
	assertTrue (value.type() == typeid(std::string));
	assertTrue (0 == AnyCast<int>(&value));
	assertTrue (0 != AnyCast<std::string>(&value));
	assertTrue (AnyCast<std::string>(value) == text);
	assertTrue (AnyCast<std::string>(&value) != &text);
}


void AnyTest::testCopyCtor()
{
	std::string text = "test message";
	Any original = text, copy = original;

	assertTrue (!copy.empty());
	assertTrue (original.type() == copy.type());
	assertTrue (AnyCast<std::string>(original) == AnyCast<std::string>(copy));
	assertTrue (text == AnyCast<std::string>(copy));
	assertTrue (AnyCast<std::string>(&original) != AnyCast<std::string>(&copy));
}


void AnyTest::testCopyAssign()
{
	std::string text = "test message";
	Any original = text, copy;
	Any* assignResult = &(copy = original);

	assertTrue (!copy.empty());
	assertTrue (original.type() == copy.type());
	assertTrue (AnyCast<std::string>(original) == AnyCast<std::string>(copy));
	assertTrue (text == AnyCast<std::string>(copy));
	assertTrue (AnyCast<std::string>(&original) != AnyCast<std::string>(&copy));
	assertTrue (assignResult == &copy);

	// test self assignment
	Any& ref = original;
	original = ref;
	assertTrue (AnyCast<std::string>(original) == AnyCast<std::string>(copy));
	original = original;
	assertTrue (AnyCast<std::string>(original) == AnyCast<std::string>(copy));
}


void AnyTest::testConvertingAssign()
{
	std::string text = "test message";
	Any value;
	Any* assignResult = &(value = text);

	assertTrue (!value.empty());
	assertTrue (value.type() == typeid(std::string));
	assertTrue (0 == AnyCast<int>(&value));
	assertTrue (0 != AnyCast<std::string>(&value));
	assertTrue (AnyCast<std::string>(value) == text);
	assertTrue (AnyCast<std::string>(&value) != &text);
	assertTrue (assignResult == &value);
}


void AnyTest::testCastToReference()
{
	Any a(137);
	const Any b(a);

	int&                ra    = AnyCast<int &>(a);
	int const&          ra_c  = AnyCast<int const &>(a);
	// NOTE: The following two AnyCasts will trigger the
	// undefined behavior sanitizer.
	int volatile&       ra_v  = AnyCast<int volatile &>(a);
	int const volatile& ra_cv = AnyCast<int const volatile&>(a);

	// cv references to same obj
	assertTrue (&ra == &ra_c && &ra == &ra_v && &ra == &ra_cv);

	int const &          rb_c  = AnyCast<int const &>(b);
	int const volatile & rb_cv = AnyCast<int const volatile &>(b);

	assertTrue (&rb_c == &rb_cv); // cv references to copied const obj
	assertTrue (&ra != &rb_c); // copies hold different objects

	++ra;
	int incremented = AnyCast<int>(a);
	assertTrue (incremented == 138); // increment by reference changes value

	try
	{
		AnyCast<char &>(a);
		failmsg ("AnyCast to incorrect reference type");
	}
	catch (BadCastException&) { }

	try
	{
		AnyCast<const char &>(b),
		failmsg ("AnyCast to incorrect const reference type");
	}
	catch (BadCastException&) { }
}


void AnyTest::testBadCast()
{
	std::string text = "test message";
	Any value = text;

	try
	{
		AnyCast<const char *>(value);
		fail ("must throw");
	}
	catch (BadCastException&) { }
}


void AnyTest::testSwap()
{
	std::string text = "test message";
	Any original = text, swapped;
	std::string* originalPtr = AnyCast<std::string>(&original);
	Any* swapResult = &original.swap(swapped);

	assertTrue (original.empty());
	assertTrue (!swapped.empty());
	assertTrue (swapped.type() == typeid(std::string));
	assertTrue (text == AnyCast<std::string>(swapped));
	assertTrue (0 != originalPtr);
#ifdef POCO_NO_SOO // pointers only match when heap-allocated
	assertTrue (originalPtr == AnyCast<std::string>(&swapped));
#endif
	assertTrue (swapResult == &original);
}


void AnyTest::testEmptyCopy()
{
	const Any null;
	Any copied = null, assigned;
	assigned = null;

	assertTrue (null.empty());
	assertTrue (copied.empty());
	assertTrue (assigned.empty());
}


void AnyTest::testInt()
{
	Any e;
	assertTrue (e.empty());

	Any a = 13;
	assertTrue (a.type() == typeid(int));
	int* i = AnyCast<int>(&a);
	assertTrue (*i == 13);
	Any b = a;
	assertTrue (b.type() == typeid(int));
	int *cpyI = AnyCast<int>(&b);
	assertTrue (*cpyI == *i);
	*cpyI = 20;
	assertTrue (*cpyI != *i);
	std::string* s = AnyCast<std::string>(&a);
	assertTrue (s == NULL);

	int POCO_UNUSED tmp = AnyCast<int>(a);
	const Any c = a;
	tmp = AnyCast<int>(a);
}


void AnyTest::testComplexType()
{
	SomeClass str(13,std::string("hello"));
	Any a = str;
	Any b = a;
	assertTrue (a.type() == typeid(SomeClass));
	assertTrue (b.type() == typeid(SomeClass));
	SomeClass str2 = AnyCast<SomeClass>(a);
	assertTrue (str == str2);
	const SomeClass& strCRef = RefAnyCast<SomeClass>(a);
	assertTrue (str == strCRef);
	SomeClass& strRef = RefAnyCast<SomeClass>(a);
	assertTrue (str == strRef);
}


void AnyTest::testVector()
{
	std::vector<int> tmp;
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(3);
	Any a = tmp;
	assertTrue (a.type() == typeid(std::vector<int>));
	std::vector<int> tmp2 = AnyCast<std::vector<int> >(a);
	assertTrue (tmp2.size() == 3);
	const std::vector<int>& vecCRef = RefAnyCast<std::vector<int> >(a);
	std::vector<int>& vecRef = RefAnyCast<std::vector<int> >(a);

	assertTrue (vecRef[0] == 1);
	assertTrue (vecRef[1] == 2);
	assertTrue (vecRef[2] == 3);
	vecRef[0] = 0;
	assertTrue (vecRef[0] == vecCRef[0]);
}


void AnyTest::testPlaceholder()
{
#ifndef POCO_NO_SOO
	Placeholder<char> ph;
	assertTrue(ph.isEmpty());
	assertFalse(ph.isLocal());

	char c = *ph.assignStack<char, char>(1);
	assertTrue(1 == c);
	assertFalse(ph.isEmpty());
	assertTrue(ph.isLocal());

	c = *ph.assignHeap<char, char>(2);
	assertTrue(2 == c);
	assertFalse(ph.isEmpty());
	assertFalse(ph.isLocal());

	ph.erase();
	assertTrue(ph.isEmpty());
	assertFalse(ph.isLocal());

	if (sizeof(std::shared_ptr<int>) <= POCO_SMALL_OBJECT_SIZE)
	{
		Placeholder<std::shared_ptr<int>> sph;
		assertTrue(sph.isEmpty());
		assertFalse(sph.isLocal());

		int i = **sph.assignStack<std::shared_ptr<int>, int*>(new int(1));
		assertTrue(1 == i);
		assertFalse(sph.isEmpty());
		assertTrue(sph.isLocal());

		i = **sph.assignHeap<std::shared_ptr<int>, int*>(new int(1));
		assertTrue(1 == i);
		assertFalse(sph.isEmpty());
		assertFalse(sph.isLocal());
	}

	if (sizeof(Poco::SharedPtr<int>) <= POCO_SMALL_OBJECT_SIZE)
	{
		Placeholder<Poco::SharedPtr<int>> psph;
		assertTrue(psph.isEmpty());
		assertFalse(psph.isLocal());

		int i = **psph.assignStack<Poco::SharedPtr<int>, int*>(new int(1));
		assertTrue(1 == i);
		assertFalse(psph.isEmpty());
		assertTrue(psph.isLocal());

		i = **psph.assignHeap<Poco::SharedPtr<int>, int*>(new int(1));
		assertTrue(1 == i);
		assertFalse(psph.isEmpty());
		assertFalse(psph.isLocal());
	}

	if (sizeof(std::vector<int>) <= POCO_SMALL_OBJECT_SIZE)
	{
		Placeholder<std::vector<int>> vph;
		assertTrue(vph.isEmpty());
		assertFalse(vph.isLocal());

		std::vector<int> inv{1,2,3};
		std::vector<int> outv = *vph.assignStack<std::vector<int>, std::vector<int>>(inv);
		assertTrue(inv == outv);
		assertFalse(vph.isEmpty());
		assertTrue(vph.isLocal());

		outv.clear();
		outv = *vph.assignHeap<std::vector<int>, std::vector<int>>(inv);
		assertTrue(inv == outv);
		assertFalse(vph.isEmpty());
		assertFalse(vph.isLocal());
	}

	// ...
#endif
}


void AnyTest::setUp()
{
}


void AnyTest::tearDown()
{
}


CppUnit::Test* AnyTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AnyTest");

	CppUnit_addTest(pSuite, AnyTest, testConvertingCtor);
	CppUnit_addTest(pSuite, AnyTest, testDefaultCtor);
	CppUnit_addTest(pSuite, AnyTest, testCopyCtor);
	CppUnit_addTest(pSuite, AnyTest, testCopyAssign);
	CppUnit_addTest(pSuite, AnyTest, testConvertingAssign);
	CppUnit_addTest(pSuite, AnyTest, testBadCast);
	CppUnit_addTest(pSuite, AnyTest, testSwap);
	CppUnit_addTest(pSuite, AnyTest, testEmptyCopy);
	CppUnit_addTest(pSuite, AnyTest, testCastToReference);
	CppUnit_addTest(pSuite, AnyTest, testInt);
	CppUnit_addTest(pSuite, AnyTest, testComplexType);
	CppUnit_addTest(pSuite, AnyTest, testVector);
	CppUnit_addTest(pSuite, AnyTest, testPlaceholder);

	return pSuite;
}
