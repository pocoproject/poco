//
// AnyTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AnyTest.h"
#include "ValueTestTypes.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Any.h"
#include "Poco/SharedPtr.h"
#include "Poco/Bugcheck.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/UUID.h"
#include "Poco/UTFString.h"
#include <vector>
#include <memory>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>


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
	assertTrue (nullptr == AnyCast<int>(&value));
	assertTrue (value.type() == typeid(void));
}


void AnyTest::testAnyConvertingCtor()
{
	std::string text = "test message";
	Any value = text;

	assertTrue (!value.empty());
	assertTrue (value.type() == typeid(std::string));
	assertTrue (nullptr == AnyCast<int>(&value));
	assertTrue (nullptr != AnyCast<std::string>(&value));
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
	assertTrue (nullptr == AnyCast<int>(&value));
	assertTrue (nullptr != AnyCast<std::string>(&value));
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
	// NOTE: The volatile AnyCasts will trigger the
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
		(void) AnyCast<char &>(a);
		failmsg ("AnyCast to incorrect reference type");
	}
	catch (BadCastException&) { }

	try
	{
		(void) AnyCast<const char &>(b);
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
		(void) AnyCast<const char *>(value);
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

	Any localInt = 1;
	Any localText = text;
	localInt.swap(localText);
	assertTrue (AnyCast<std::string>(localInt) == text);
	assertTrue (AnyCast<int>(localText) == 1);
	localText.swap(localInt);
	assertTrue (AnyCast<int>(localInt) == 1);
	assertTrue (AnyCast<std::string>(localText) == text);

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
	assertTrue (nullptr != originalPtr);
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

		bool operator==(const BigObject& other) const
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
	assertTrue (nullptr != bigPtr);
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
	assertTrue (s == nullptr);

	[[maybe_unused]] int tmp = AnyCast<int>(a);
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
	assertTrue (s == nullptr);
	assertTrue (AnyCast<std::nullptr_t>(&a) == nullptr);

	[[maybe_unused]] int* tmp = AnyCast<int*>(a);
	const Any c = a;
	tmp = AnyCast<int*>(a);

	Any nullPtr(nullptr);
	assertFalse (AnyHoldsNullPtr<std::nullptr_t>(nullptr));
	assertFalse (AnyHoldsNullPtr<void*>(nullptr));
	assertTrue (AnyHoldsNullPtr<std::nullptr_t>(nullPtr));
	assertTrue (AnyHoldsNullPtr<std::nullptr_t>(&nullPtr));
	try
	{
		(void) AnyHoldsNullPtr<void*>(nullPtr);
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
		(void) AnyHoldsNullPtr<std::nullptr_t>(voidPtr);
		fail ("AnyCast must fail", __LINE__, __FILE__);
	}
	catch(const Poco::BadCastException&) {}

	using FP = void (AnyTest::*)();
	FP fp = nullptr;
	Any funcPtr(fp);
	assertTrue (AnyHoldsNullPtr<FP>(funcPtr));
	try
	{
		(void) AnyHoldsNullPtr<FP>(voidPtr);
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
		(void) AnyHoldsNullPtr<OP>(funcPtr);
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
		(void) AnyHoldsNullPtr<MP>(objPtr);
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


void AnyTest::testAnySelfMoveAssign()
{
	const std::string text(100, 'x');
	Any value = text;
	Any& alias = value;
	value = std::move(alias);
	assertTrue (value.type() == typeid(std::string));
	assertTrue (AnyCast<std::string>(value) == text);
}


void AnyTest::testAnyMoveDoesNotCopy()
{
	CopyCounter::resetCopies();
	Any source = CopyCounter(7);
	Any target(std::move(source));
	assertTrue (source.empty());
	assertTrue (RefAnyCast<CopyCounter>(target).value() == 7);

	Any assigned = 1;
	assigned = std::move(target);
	assertTrue (target.empty());
	assertTrue (RefAnyCast<CopyCounter>(assigned).value() == 7);
	assertTrue (CopyCounter::copies() == 0);
}


void AnyTest::testAnyFailedCopyLeavesTargetEmpty()
{
	const Any source = ThrowOnCopy(true);
	Any target;
	try
	{
		target = source;
		fail ("copy must throw");
	}
	catch (RuntimeException&)
	{
	}
	assertTrue (target.empty());
	assertTrue (target.type() == typeid(void));
	assertTrue (source.type() == typeid(ThrowOnCopy));

	const ThrowOnCopy value(true);
	try
	{
		target = value;
		fail ("copy must throw");
	}
	catch (RuntimeException&)
	{
	}
	assertTrue (target.empty());
	assertTrue (target.type() == typeid(void));
}


void AnyTest::testAnyFailedCopyKeepsTargetValue()
{
	const Any source = ThrowOnCopy(true);
	Any target = 42;
	try
	{
		target = source;
		fail ("copy must throw");
	}
	catch (RuntimeException&)
	{
	}
	assertTrue (AnyCast<int>(target) == 42);
}


void AnyTest::testAnyMoveUsesNothrowCopy()
{
	const ThrowOnMove value(true);
	Any source = value;
#ifndef POCO_NO_SOO
	assertTrue (source.local());
#endif
	Any target(std::move(source));
	assertTrue (source.empty());
	assertTrue (RefAnyCast<ThrowOnMove>(target).throwing());

	Any assigned = 1;
	assigned = std::move(target);
	assertTrue (target.empty());
	assertTrue (RefAnyCast<ThrowOnMove>(assigned).throwing());

	Any other = std::string("abc");
	assigned.swap(other);
	assertTrue (RefAnyCast<ThrowOnMove>(other).throwing());
	assertTrue (RefAnyCast<std::string>(assigned) == "abc");
}


void AnyTest::testAnyCopyAssignmentPaths()
{
	const std::vector<int> values{1, 2, 3};
	const Any vectorAny = values;
	const Any intAny = 7;
	const Any emptyAny;

	Any target = 1;
	target = vectorAny;
	assertTrue (RefAnyCast<std::vector<int>>(target) == values);

	target = 1;
	target = intAny;
	assertTrue (AnyCast<int>(target) == 7);

	target = emptyAny;
	assertTrue (target.empty());
	target = intAny;
	assertTrue (AnyCast<int>(target) == 7);
}


void AnyTest::testAnyVolatileValue()
{
	volatile int value = 5;
	Any any(value);
	assertTrue (any.type() == typeid(int));
	assertTrue (AnyCast<int>(any) == 5);

	value = 6;
	any = value;
	assertTrue (AnyCast<int>(any) == 6);
}


void AnyTest::testAnyOverAlignedValue()
{
	const std::vector<Any> values(4, Any(OverAligned{7}));
	for (const auto& value: values)
	{
		assertFalse (value.local());
		const OverAligned* pValue = AnyCast<OverAligned>(&value);
		assertTrue (pValue != nullptr);
		// volatile, so that the compiler cannot fold the check from the alignment of the type
		const volatile std::uintptr_t address = std::bit_cast<std::uintptr_t>(pValue);
		assertTrue (address % alignof(OverAligned) == 0);
		assertTrue (pValue->value == 7);
	}
}


void AnyTest::testAnyLocalStorage()
{
#ifdef POCO_NO_SOO
	constexpr bool smallIsLocal = false;
#else
	constexpr bool smallIsLocal = true;
#endif
	assertTrue (Any(1).local() == smallIsLocal);
	assertTrue (Any(std::string("abc")).local() == smallIsLocal);
	assertTrue (Any(CopyCounter(1)).local() == smallIsLocal);
	assertTrue (Any(Timestamp()).local() == smallIsLocal);
	assertTrue (Any(DateTime()).local() == smallIsLocal);
	assertTrue (Any(LocalDateTime()).local() == smallIsLocal);
	assertTrue (Any(UUID()).local() == smallIsLocal);
	assertFalse (Any(MayThrowOnMove()).local());
}


void AnyTest::testAnyCopyOnlyValue()
{
	const CopyOnly value(1);
	Any any = value;
	any = CopyOnly(2);
	assertTrue (RefAnyCast<CopyOnly>(any).value() == 2);

	Any moved(std::move(any));
	assertTrue (any.empty());
	assertTrue (RefAnyCast<CopyOnly>(moved).value() == 2);

	Any assigned = 1;
	assigned = std::move(moved);
	assertTrue (moved.empty());
	assertTrue (RefAnyCast<CopyOnly>(assigned).value() == 2);
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
	// the copy of a vector can throw, and a vector has no moveTo()
	assertFalse(vph.isLocal());

	// ...
#endif
}


void AnyTest::testStringAssignmentKeepsBuffer()
{
	const std::string text(100, 'x');
	Any target(std::string(200, 'a'));
	const char* pData = AnyCast<std::string>(&target)->data();
	const Any source(text);
	target = source;
	assertTrue (*AnyCast<std::string>(&target) == text);
	assertTrue (AnyCast<std::string>(&target)->data() == pData);
	assertTrue (*AnyCast<std::string>(&source) == text);

	target = Any(42);
	assertTrue (AnyCast<int>(target) == 42);

	const std::wstring wideText(100, L'x');
	Any wideTarget(std::wstring(200, L'a'));
	const wchar_t* pWideData = AnyCast<std::wstring>(&wideTarget)->data();
	const Any wideSource(wideText);
	wideTarget = wideSource;
	assertTrue (*AnyCast<std::wstring>(&wideTarget) == wideText);
	assertTrue (AnyCast<std::wstring>(&wideTarget)->data() == pWideData);

	const std::u16string utf16Text(100, u'x');
	Any utf16Target(std::u16string(200, u'a'));
	const char16_t* pUtf16Data = AnyCast<std::u16string>(&utf16Target)->data();
	const Any utf16Source(utf16Text);
	utf16Target = utf16Source;
	assertTrue (*AnyCast<std::u16string>(&utf16Target) == utf16Text);
	assertTrue (AnyCast<std::u16string>(&utf16Target)->data() == pUtf16Data);

	// Unless UTF16String is std::wstring, Any has no type id for it and compares the types by type().
	const UTF16String utf16StringText(100, 'x');
	Any utf16StringTarget(UTF16String(200, 'a'));
	const UTF16Char* pUtf16StringData = AnyCast<UTF16String>(&utf16StringTarget)->data();
	const Any utf16StringSource(utf16StringText);
	utf16StringTarget = utf16StringSource;
	assertTrue (*AnyCast<UTF16String>(&utf16StringTarget) == utf16StringText);
	assertTrue (AnyCast<UTF16String>(&utf16StringTarget)->data() == pUtf16StringData);
}


void AnyTest::testAnyStorageSize()
{
#if !defined(POCO_NO_SOO) && POCO_SMALL_OBJECT_SIZE == 64
	// With the default buffer size, the type id byte fits into the padding behind the tag byte.
	constexpr std::size_t alignment = alignof(std::max_align_t);
	constexpr std::size_t expectedSize = (POCO_SMALL_OBJECT_SIZE + 1 + alignment - 1) / alignment * alignment;
	assertEqual (expectedSize, sizeof(Any));
#endif
}


void AnyTest::testAnyCastByTypeId()
{
	Any intAny = 42;
	assertTrue (AnyCast<int>(&intAny) != nullptr);
	assertTrue (*AnyCast<int>(&intAny) == 42);
	assertTrue (AnyCast<unsigned>(&intAny) == nullptr);
	assertTrue (AnyCast<long>(&intAny) == nullptr);

	const Any charAny = 'a';
	assertTrue (*AnyCast<char>(&charAny) == 'a');
	assertTrue (AnyCast<signed char>(&charAny) == nullptr);
	assertTrue (AnyCast<unsigned char>(&charAny) == nullptr);

	const Any signedCharAny = static_cast<signed char>(1);
	assertTrue (*AnyCast<signed char>(&signedCharAny) == 1);
	assertTrue (AnyCast<char>(&signedCharAny) == nullptr);

	const Any textAny = std::string("abc");
	assertTrue (*AnyCast<std::string>(&textAny) == "abc");
	assertTrue (AnyCast<const char*>(&textAny) == nullptr);
	const Any pointerAny = "abc";
	assertTrue (AnyCast<std::string>(&pointerAny) == nullptr);
	assertTrue (AnyCast<const char*>(&pointerAny) != nullptr);

	assertTrue (RefAnyCast<bool>(Any(true)));
	assertTrue (RefAnyCast<double>(Any(1.5)) == 1.5);
	assertTrue (AnyCast<float>(Any(2.5f)) == 2.5f);
	assertTrue (AnyCast<long long>(Any(3LL)) == 3LL);
	assertTrue (AnyCast<unsigned long>(Any(4UL)) == 4UL);

	const Any dateTimeAny = DateTime(2026, 11, 26);
	assertTrue (AnyCast<DateTime>(&dateTimeAny)->year() == 2026);
	assertTrue (AnyCast<LocalDateTime>(&dateTimeAny) == nullptr);
	assertTrue (AnyCast<Timestamp>(&dateTimeAny) == nullptr);
	assertTrue (AnyCast<UUID>(Any(UUID("f81d4fae-7dec-11d0-a765-00a0c91e6bf6"))).toString() == "f81d4fae-7dec-11d0-a765-00a0c91e6bf6");

	const Any counterAny = CopyCounter(5);
	assertTrue (AnyCast<CopyCounter>(&counterAny)->value() == 5);
	assertTrue (AnyCast<int>(&counterAny) == nullptr);

	const Any emptyAny;
	assertTrue (AnyCast<int>(&emptyAny) == nullptr);
	assertTrue (AnyCast<int>(static_cast<Any*>(nullptr)) == nullptr);
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
	CppUnit_addTest(pSuite, AnyTest, testAnySelfMoveAssign);
	CppUnit_addTest(pSuite, AnyTest, testAnyMoveDoesNotCopy);
	CppUnit_addTest(pSuite, AnyTest, testAnyFailedCopyLeavesTargetEmpty);
	CppUnit_addTest(pSuite, AnyTest, testAnyFailedCopyKeepsTargetValue);
	CppUnit_addTest(pSuite, AnyTest, testAnyOverAlignedValue);
	CppUnit_addTest(pSuite, AnyTest, testAnyLocalStorage);
	CppUnit_addTest(pSuite, AnyTest, testAnyCopyOnlyValue);
	CppUnit_addTest(pSuite, AnyTest, testAnyMoveUsesNothrowCopy);
	CppUnit_addTest(pSuite, AnyTest, testAnyCopyAssignmentPaths);
	CppUnit_addTest(pSuite, AnyTest, testAnyVolatileValue);
	CppUnit_addTest(pSuite, AnyTest, testPlaceholder);
	CppUnit_addTest(pSuite, AnyTest, testStringAssignmentKeepsBuffer);
	CppUnit_addTest(pSuite, AnyTest, testAnyStorageSize);
	CppUnit_addTest(pSuite, AnyTest, testAnyCastByTypeId);

	return pSuite;
}
