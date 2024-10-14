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


void AnyTest::testAnyDefaultCtor()
{
	const Any value;

	assertTrue (value.empty());
	assertTrue (0 == AnyCast<int>(&value));
	assertTrue (value.type() == typeid(void));
}


void AnyTest::testAnyConvertingCtor()
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


void AnyTest::testAnyCopyCtor()
{
	std::string text = "test message";
	Any original = text, copy = original;

	assertTrue (!copy.empty());
	assertTrue (original.type() == copy.type());
	assertTrue (AnyCast<std::string>(original) == AnyCast<std::string>(copy));
	assertTrue (text == AnyCast<std::string>(copy));
	assertTrue (AnyCast<std::string>(&original) != AnyCast<std::string>(&copy));
}


void AnyTest::testAnyCopyAssign()
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


void AnyTest::testAnyConvertingAssign()
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


void AnyTest::testAnyCastToReference()
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


void AnyTest::testAnyBadCast()
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


void AnyTest::testAnySwap()
{
	Any empty1, empty2;
	assertTrue (empty1.empty());
	assertTrue (empty2.empty());
	empty1.swap(empty2);
	assertTrue (empty1.empty());
	assertTrue (empty2.empty());

	std::string text = "test message";
	empty1 = text;
	assertTrue (!empty1.empty());
	assertTrue (empty2.empty());
	assertTrue (text == AnyCast<std::string>(empty1));

	empty1.swap(empty2);
	assertTrue (empty1.empty());
	assertTrue (!empty2.empty());
	assertTrue (text == AnyCast<std::string>(empty2));

	Any original = text, swapped;
#ifdef POCO_NO_SOO
	assertFalse (original.local());
#else
	assertTrue (original.local());
#endif
	assertFalse (original.empty());
	assertFalse (swapped.local());
	assertTrue (swapped.empty());
	std::string* originalPtr = AnyCast<std::string>(&original);
	Any* swapResult = &original.swap(swapped);

	assertTrue (original.empty());
	assertTrue (!swapped.empty());
	assertTrue (swapped.type() == typeid(std::string));
	assertTrue (text == AnyCast<std::string>(swapped));
	assertTrue (0 != originalPtr);
	assertTrue (swapResult == &original);

	struct BigObject
	{
		Poco::UInt64 one = 1;
		Poco::UInt64 two = 2;
		Poco::UInt64 three = 3;
		Poco::UInt64 four = 4;
		Poco::UInt64 five = 5;
		Poco::UInt64 six = 6;
		Poco::UInt64 seven = 7;
		Poco::UInt64 eight = 8;
		Poco::UInt64 nine = 9;

		bool operator==(const BigObject& other)
		{
			return one == other.one &&
				two == other.two &&
				three == other.three &&
				four == other.four &&
				five == other.five &&
				six == other.six &&
				seven == other.seven &&
				eight == other.eight &&
				nine == other.nine;
		}
	};

	poco_assert (sizeof(BigObject) > POCO_SMALL_OBJECT_SIZE);

	BigObject bigObject;
	Any bigOriginal = bigObject, swappedBig;
	assertFalse (bigOriginal.local());
	assertFalse (bigOriginal.empty());
	assertFalse (swappedBig.local());
	assertTrue (swappedBig.empty());
	BigObject* bigPtr = AnyCast<BigObject>(&bigOriginal);
	Any* swapBigResult = &bigOriginal.swap(swappedBig);

	assertTrue (bigOriginal.empty());
	assertTrue (!swappedBig.empty());
	assertTrue (swappedBig.type() == typeid(BigObject));
	assertTrue (bigObject == AnyCast<BigObject>(swappedBig));
	assertTrue (0 != bigPtr);
	assertTrue (swapBigResult == &bigOriginal);

	// assure proper assignment behavior after swapping
	original = text;
	bigOriginal = bigObject;
#ifdef POCO_NO_SOO
	assertFalse (original.local());
#else
	assertTrue (original.local());
#endif
	assertFalse (bigOriginal.local());

	Any temp = original;
#ifdef POCO_NO_SOO
	assertFalse (temp.local());
#else
	assertTrue (temp.local());
#endif

	original = bigOriginal;
	assertTrue (bigObject == AnyCast<BigObject>(original));
	assertFalse (original.local());

	bigOriginal = temp;
	assertTrue (text == AnyCast<std::string>(bigOriginal));
#ifdef POCO_NO_SOO
	assertFalse (bigOriginal.local());
#else
	assertTrue (bigOriginal.local());
#endif
}


void AnyTest::testAnyEmptyCopy()
{
	const Any null;
	Any copied = null, assigned;
	assigned = null;

	assertTrue (null.empty());
	assertTrue (copied.empty());
	assertTrue (assigned.empty());
}


void AnyTest::testAnyInt()
{
	Any e;
	assertTrue (e.empty());
	e = 0;
	assertFalse (e.empty());

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


class A
{
public:
    void f() {}
	int m;
};


void AnyTest::testAnyPointer()
{
	int i = 13;

	Any a = &i;
	assertTrue (a.type() == typeid(int*));
	assertFalse (AnyHoldsNullPtr<int*>(a));
	assertFalse (AnyHoldsNullPtr<int*>(&a));
	int* p = AnyCast<int*>(&i);
	assertTrue (*p == 13);
	Any b = a;
	assertTrue (b.type() == typeid(int*));
	int* cpyI = AnyCast<int*>(b);
	assertTrue (*cpyI == *p);
	*cpyI = 20;
	assertTrue (*cpyI == *p);
	std::string* s = AnyCast<std::string>(&a);
	assertTrue (s == NULL);
	assertTrue (AnyCast<std::nullptr_t>(&a) == nullptr);

	int* POCO_UNUSED tmp = AnyCast<int*>(a);
	const Any c = a;
	tmp = AnyCast<int*>(a);

	Any nullPtr(nullptr);
	assertFalse (AnyHoldsNullPtr<std::nullptr_t>(nullptr));
	assertFalse (AnyHoldsNullPtr<void*>(0));
	assertTrue (AnyHoldsNullPtr<std::nullptr_t>(nullPtr));
	assertTrue (AnyHoldsNullPtr<std::nullptr_t>(&nullPtr));
	try
	{
		AnyHoldsNullPtr<void*>(nullPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}
	nullPtr = &i;
	try
	{
		assertFalse (AnyHoldsNullPtr<std::nullptr_t>(nullPtr));
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}
	assertFalse (AnyHoldsNullPtr<int*>(nullPtr));

	void* voidPtr = nullptr;
	Any nullVoidPtr(voidPtr);
	assertTrue (AnyHoldsNullPtr<void*>(nullVoidPtr));
	try
	{
		AnyHoldsNullPtr<std::nullptr_t>(voidPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}

	using FP = void (AnyTest::*)();
	FP fp = nullptr;
	Any funcPtr(fp);
	assertTrue (AnyHoldsNullPtr<FP>(funcPtr));
	try
	{
		AnyHoldsNullPtr<FP>(voidPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}
	funcPtr = &AnyTest::testAnyPointer;
	assertFalse (AnyHoldsNullPtr<FP>(funcPtr));

	using OP = decltype(&AnyTest::_dummyObject);
	OP op = nullptr;
	Any objPtr(op);
	assertTrue (AnyHoldsNullPtr<OP>(objPtr));
	objPtr = &AnyTest::_dummyObject;
	try
	{
		AnyHoldsNullPtr<OP>(funcPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}

	assertFalse (AnyHoldsNullPtr<OP>(objPtr));

	using MP = decltype(&AnyTest::_dummy);
	MP mp = nullptr;
	Any memPtr(mp);
	assertTrue (AnyHoldsNullPtr<MP>(memPtr));
	memPtr = &AnyTest::_dummy;
	try
	{
		AnyHoldsNullPtr<MP>(objPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}

	assertFalse (AnyHoldsNullPtr<MP>(memPtr));
}


void AnyTest::testAnyComplexType()
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


void AnyTest::testAnyVector()
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

	char c = *ph.assign<char, char>(1);
	assertTrue(1 == c);
	assertFalse(ph.isEmpty());
	assertTrue(ph.isLocal());

	ph.erase();
	assertTrue(ph.isEmpty());
	assertFalse(ph.isLocal());

	Placeholder<int> phi;
	assertTrue(phi.isEmpty());
	assertFalse(phi.isLocal());

	int i = *phi.assign<int, int>(0);
	assertTrue(0 == i);
	assertFalse(phi.isEmpty());
	assertTrue(phi.isLocal());

	phi.erase();
	assertTrue(phi.isEmpty());
	assertFalse(phi.isLocal());

	Placeholder<std::shared_ptr<int>> sph;
	assertTrue(sph.isEmpty());
	assertFalse(sph.isLocal());

	i = **sph.assign<std::shared_ptr<int>, int*>(new int(1));
	assertTrue(1 == i);
	assertFalse(sph.isEmpty());
	assertTrue(sph.isLocal());

	Placeholder<Poco::SharedPtr<int>> psph;
	assertTrue(psph.isEmpty());
	assertFalse(psph.isLocal());

	i = **psph.assign<Poco::SharedPtr<int>, int*>(new int(2));
	assertTrue(2 == i);
	assertFalse(psph.isEmpty());
	assertTrue(psph.isLocal());

	Placeholder<std::vector<int>> vph;
	assertTrue(vph.isEmpty());
	assertFalse(vph.isLocal());

	std::vector<int> inv{1,2,3};
	std::vector<int> outv = *vph.assign<std::vector<int>, std::vector<int>>(inv);
	assertTrue(inv == outv);
	assertFalse(vph.isEmpty());
	assertTrue(vph.isLocal());

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

	CppUnit_addTest(pSuite, AnyTest, testAnyConvertingCtor);
	CppUnit_addTest(pSuite, AnyTest, testAnyDefaultCtor);
	CppUnit_addTest(pSuite, AnyTest, testAnyCopyCtor);
	CppUnit_addTest(pSuite, AnyTest, testAnyCopyAssign);
	CppUnit_addTest(pSuite, AnyTest, testAnyConvertingAssign);
	CppUnit_addTest(pSuite, AnyTest, testAnyBadCast);
	CppUnit_addTest(pSuite, AnyTest, testAnySwap);
	CppUnit_addTest(pSuite, AnyTest, testAnyEmptyCopy);
	CppUnit_addTest(pSuite, AnyTest, testAnyCastToReference);
	CppUnit_addTest(pSuite, AnyTest, testAnyPointer);
	CppUnit_addTest(pSuite, AnyTest, testAnyInt);
	CppUnit_addTest(pSuite, AnyTest, testAnyComplexType);
	CppUnit_addTest(pSuite, AnyTest, testAnyVector);
	CppUnit_addTest(pSuite, AnyTest, testPlaceholder);

	return pSuite;
}
