//
// CoreTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CoreTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Bugcheck.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Buffer.h"
#include "Poco/FIFOBuffer.h"
#include "Poco/AtomicCounter.h"
#include "Poco/AtomicFlag.h"
#include "Poco/Nullable.h"
#include "Poco/Ascii.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/Checksum.h"
#include "Poco/MakeUnique.h"
#include "Poco/Exception.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>


using Poco::Bugcheck;
using Poco::Exception;
using Poco::Environment;
using Poco::Thread;
using Poco::Runnable;
using Poco::Buffer;
using Poco::AtomicCounter;
using Poco::AtomicFlag;
using Poco::Nullable;
using Poco::Ascii;
using Poco::BasicEvent;
using Poco::delegate;
using Poco::NullType;
using Poco::InvalidAccessException;
using Poco::Checksum;
using Poco::makeUnique;


namespace
{
	class ACTRunnable: public Poco::Runnable
	{
	public:
		ACTRunnable(AtomicCounter& counter):
			_counter(counter)
		{
		}
		
		void run()
		{
			for (int i = 0; i < 100000; ++i)
			{
				_counter++;
				_counter--;
				++_counter;
				--_counter;
			}
		}
		
	private:
		AtomicCounter& _counter;
	};

	class NonDefaultConstructible
	{
	public:
		NonDefaultConstructible(int val) : _val(val)
		{
		}

		NonDefaultConstructible& operator=(int val)
		{
			_val = val;
			return *this;
		}

		bool operator == (const NonDefaultConstructible& other) const
		{
			return (_val == other._val);
		}

		bool operator < (const NonDefaultConstructible& other) const
		{
			return (_val < other._val);
		}

		int value() const
		{
			return _val;
		}

	private:
		NonDefaultConstructible();
		int _val;
	};
}


class Small
{
};


struct Parent
{
	Parent() { i = -1; }
	virtual ~Parent() { i= -2; }

	static int i;
};


int Parent::i = 0;


struct Medium : public Parent
{
};


struct Large
{
	Large() : i(1), j(2), k(3), l(4) { }
	long i,j,k;
	const long l;
};


//
// The bugcheck test is normally disabled, as it
// causes a break into the debugger.
//
#define ENABLE_BUGCHECK_TEST 0


CoreTest::CoreTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


CoreTest::~CoreTest()
{
}


void CoreTest::testPlatform()
{
	std::cout << "POCO_OS:   " << POCO_OS << std::endl;
	std::cout << "POCO_ARCH: " << POCO_ARCH << std::endl;
}


void CoreTest::testFixedLength()
{
	assertTrue (sizeof(Poco::Int8) == 1);
	assertTrue (sizeof(Poco::UInt8) == 1);
	assertTrue (sizeof(Poco::Int16) == 2);
	assertTrue (sizeof(Poco::UInt16) == 2);
	assertTrue (sizeof(Poco::Int32) == 4);
	assertTrue (sizeof(Poco::UInt32) == 4);
#if defined(POCO_HAVE_INT64)
	assertTrue (sizeof(Poco::Int64) == 8);
	assertTrue (sizeof(Poco::UInt64) == 8);
#endif
	assertTrue (sizeof(Poco::IntPtr) == sizeof(void*));
	assertTrue (sizeof(Poco::UIntPtr) == sizeof(void*));	
}


void CoreTest::testBugcheck()
{
#if ENABLE_BUGCHECK_TEST
	try
	{
		Bugcheck::assertion("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Bugcheck::nullPointer("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}

	try
	{
		Bugcheck::bugcheck("test", __FILE__, __LINE__);	
		failmsg("must throw exception");
	}
	catch (Exception&)
	{
	}
#endif
}


void CoreTest::testEnvironment()
{
#if !defined(_WIN32_WCE)
	Environment::set("FOO", "BAR");
	assertTrue (Environment::has("FOO"));
	assertTrue (Environment::get("FOO") == "BAR");
#endif
	try
	{
		std::string v = Environment::get("THISONEDOESNOTEXIST123");
		failmsg("Environment variable does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}
	std::cout << std::endl;
	std::cout << "OS Name:         " << Environment::osName() << std::endl;
	std::cout << "OS Display Name: " << Environment::osDisplayName() << std::endl;
	std::cout << "OS Version:      " << Environment::osVersion() << std::endl;
	std::cout << "OS Architecture: " << Environment::osArchitecture() << std::endl;
	std::cout << "Node Name:       " << Environment::nodeName() << std::endl;
	std::cout << "Node ID:         " << Environment::nodeId() << std::endl;
	std::cout << "Number of CPUs:  " << Environment::processorCount() << std::endl;
}


void CoreTest::testBuffer()
{
	std::size_t s = 10;
	Buffer<int> b(s);
	assertTrue (b.size() == s);
	assertTrue (b.sizeBytes() == s * sizeof(int));
	assertTrue (b.capacity() == s);
	assertTrue (b.capacityBytes() == s * sizeof(int));
	std::vector<int> v;
	for (int i = 0; i < s; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

	assertTrue (s == b.size());
	for (int i = 0; i < s; ++i)
		assertTrue (b[i] == i);

	b.resize(s/2);
	for (int i = 0; i < s/2; ++i)
		assertTrue (b[i] == i);

	assertTrue (b.size() == s/2);
	assertTrue (b.capacity() == s);

	b.resize(s*2);
	v.clear();
	for (int i = 0; i < s*2; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

	for (int i = 0; i < s*2; ++i)
		assertTrue (b[i] == i);

	assertTrue (b.size() == s*2);
	assertTrue (b.capacity() == s*2);

	b.setCapacity(s * 4);
	assertTrue (b.size() == s*2);
	assertTrue (b.capacity() == s*4);

	b.setCapacity(s);
	assertTrue (b.size() == s);
	assertTrue (b.capacity() == s);

#if ENABLE_BUGCHECK_TEST
	try { int i = b[s]; fail ("must fail"); }
	catch (Exception&) { }
#endif

	Buffer<int> c(s);
	Buffer<int> d(c);
	assertTrue (c == d);

	c[1] = -1;
	assertTrue (c[1] == -1);
	c.clear();
	assertTrue (c[1] == 0);

	Buffer<int> e(0);
	assertTrue (e.empty());

	assertTrue (c != e);

	Buffer<int> f = e;
	assertTrue (f == e);

	Buffer<char> g(0);
	g.append("hello", 5);
	assertTrue (g.size() == 5);

	g.append("hello", 5);
	assertTrue (g.size() == 10);
	assertTrue ( !std::memcmp(g.begin(), "hellohello", 10) );

	char h[10];
	Buffer<char> buf(h, 10);
	try
	{
		buf.append("hello", 5);
		fail ("must fail");
	}
	catch (InvalidAccessException&) { }

	buf.assign("hello", 5);
	assertTrue ( !std::memcmp(&h[0], "hello", 5) );

	const char j[10] = "hello";
	Buffer<char> k(j, 5);
	k.append("hello", 5);
	assertTrue ( !std::memcmp(&j[0], "hello", 5) );
	assertTrue ( !std::memcmp(k.begin(), "hellohello", 10) );
	k.append('w');
	assertTrue (k.size() == 11);
	assertTrue ( !std::memcmp(k.begin(), "hellohellow", k.size()) );
	k.append('o');
	assertTrue (k.size() == 12);
	assertTrue ( !std::memcmp(k.begin(), "hellohellowo", k.size()) );
	k.append('r');
	assertTrue (k.size() == 13);
	assertTrue ( !std::memcmp(k.begin(), "hellohellowor", k.size()) );
	k.append('l');
	assertTrue (k.size() == 14);
	assertTrue ( !std::memcmp(k.begin(), "hellohelloworl", k.size()) );
	k.append('d');
	assertTrue (k.size() == 15);
	assertTrue ( !std::memcmp(k.begin(), "hellohelloworld", k.size()) );

	char my[16];
	Poco::Buffer<char> buffer(16);
	Poco::Buffer<char> wrapper(my, sizeof(my));
	buffer.swap(wrapper);
}


void CoreTest::testAtomicCounter()
{
	AtomicCounter ac(0);

	assertTrue (ac.value() == 0);
	assertTrue (ac++ == 0);
	assertTrue (ac-- == 1);
	assertTrue (++ac == 1);
	assertTrue (--ac == 0);
	
	ac = 2;
	assertTrue (ac.value() == 2);
	
	ac = 0;
	assertTrue (ac.value() == 0);
	
	AtomicCounter ac2(2);
	assertTrue (ac2.value() == 2);
	
	ACTRunnable act(ac);
	Thread t1;
	Thread t2;
	Thread t3;
	Thread t4;
	Thread t5;
	
	t1.start(act);
	t2.start(act);
	t3.start(act);
	t4.start(act);
	t5.start(act);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	
	assertTrue (ac.value() == 0);
}


void CoreTest::testAtomicFlag()
{
	AtomicFlag f;
	assertTrue(!f);
	assertFalse(!f);
	assertTrue(f);
	f.reset();
	assertFalse(f);
	assertTrue(f);
	assertFalse(!f);
}


void CoreTest::testNullable()
{
	Nullable<NonDefaultConstructible> ndc1;
	Nullable<NonDefaultConstructible> ndc2;
	assertTrue (ndc1.isNull());
	assertTrue (ndc2.isNull());
	assertTrue (ndc1 == ndc2);
	assertTrue (!(ndc1 != ndc2));
	assertTrue (!(ndc1 > ndc2));
	bool ge = (ndc1 >= ndc2);
	assertTrue (ndc1 >= ndc2);
	assertTrue (!(ndc1 < ndc2));
	assertTrue (ndc1 <= ndc2);
	ndc1 = 42;
	assertTrue (!ndc1.isNull());
	assertTrue (ndc1.value() == 42);

	Nullable<int> i;
	Nullable<double> f;
	Nullable<std::string> s;

	assertTrue (i.isNull());
	assertTrue (f.isNull());
	assertTrue (s.isNull());

	i = 1;
	f = 1.5;
	s = "abc";

	assertTrue (!i.isNull());
	assertTrue (!f.isNull());
	assertTrue (!s.isNull());

	assertTrue (i == 1);
	assertTrue (f == 1.5);
	assertTrue (s == "abc");

	i.clear();
	f.clear();
	s.clear();

	assertTrue (i.isNull());
	assertTrue (f.isNull());
	assertTrue (s.isNull());

	Nullable<int> n1;
	assertTrue (n1.isNull());
	
	assertTrue (n1.value(42) == 42);
	assertTrue (n1.isNull());
	assertTrue (!(0 == n1));
	assertTrue (0 != n1);
	assertTrue (!(n1 == 0));
	assertTrue (n1 != 0);
	
	try
	{
		int tmp = n1.value();
		fail("null value, must throw");
	}
	catch (Poco::NullValueException&)
	{
	}
	
	n1 = 1;
	assertTrue (!n1.isNull());
	assertTrue (n1.value() == 1);
	
	Nullable<int> n2(42);
	assertTrue (!n2.isNull());
	assertTrue (n2.value() == 42);
	assertTrue (n2.value(99) == 42);
	
	assertTrue (!(0 == n2));
	assertTrue (0 != n2);
	assertTrue (!(n2 == 0));
	assertTrue (n2 != 0);
	
	n1 = n2;
	assertTrue (!n1.isNull());
	assertTrue (n1.value() == 42);
	
	std::ostringstream str;
	str << n1;
	assertTrue (str.str() == "42");

	n1.clear();
	assertTrue (n1.isNull());

	str.str(""); str << n1;
	assertTrue (str.str().empty());

	n2.clear();
	assertTrue (n1 == n2);
	n1 = 1; n2 = 1;
	assertTrue (n1 == n2);
	n1.clear();
	assertTrue (n1 < n2);
	assertTrue (n2 > n1);
	n2 = -1; n1 = 0;
	assertTrue (n2 < n1);
	assertTrue (n2 != n1);
	assertTrue (n1 > n2);

	NullType nd;
	assertTrue (n1 != nd);
	assertTrue (nd != n1);
	n1.clear();
	assertTrue (n1 == nd);
	assertTrue (nd == n1);
}


void CoreTest::testAscii()
{
	assertTrue (Ascii::isAscii('A'));
	assertTrue (!Ascii::isAscii(-1));
	assertTrue (!Ascii::isAscii(128));
	assertTrue (!Ascii::isAscii(222));
	
	assertTrue (Ascii::isSpace(' '));
	assertTrue (Ascii::isSpace('\t'));
	assertTrue (Ascii::isSpace('\r'));
	assertTrue (Ascii::isSpace('\n'));
	assertTrue (!Ascii::isSpace('A'));
	assertTrue (!Ascii::isSpace(-1));
	assertTrue (!Ascii::isSpace(222));
	
	assertTrue (Ascii::isDigit('0'));
	assertTrue (Ascii::isDigit('1'));
	assertTrue (Ascii::isDigit('2'));
	assertTrue (Ascii::isDigit('3'));
	assertTrue (Ascii::isDigit('4'));
	assertTrue (Ascii::isDigit('5'));
	assertTrue (Ascii::isDigit('6'));
	assertTrue (Ascii::isDigit('7'));
	assertTrue (Ascii::isDigit('8'));
	assertTrue (Ascii::isDigit('9'));
	assertTrue (!Ascii::isDigit('a'));
	
	assertTrue (Ascii::isHexDigit('0'));
	assertTrue (Ascii::isHexDigit('1'));
	assertTrue (Ascii::isHexDigit('2'));
	assertTrue (Ascii::isHexDigit('3'));
	assertTrue (Ascii::isHexDigit('4'));
	assertTrue (Ascii::isHexDigit('5'));
	assertTrue (Ascii::isHexDigit('6'));
	assertTrue (Ascii::isHexDigit('7'));
	assertTrue (Ascii::isHexDigit('8'));
	assertTrue (Ascii::isHexDigit('9'));
	assertTrue (Ascii::isHexDigit('a'));
	assertTrue (Ascii::isHexDigit('b'));
	assertTrue (Ascii::isHexDigit('c'));
	assertTrue (Ascii::isHexDigit('d'));
	assertTrue (Ascii::isHexDigit('e'));
	assertTrue (Ascii::isHexDigit('f'));
	assertTrue (Ascii::isHexDigit('A'));
	assertTrue (Ascii::isHexDigit('B'));
	assertTrue (Ascii::isHexDigit('C'));
	assertTrue (Ascii::isHexDigit('D'));
	assertTrue (Ascii::isHexDigit('E'));
	assertTrue (Ascii::isHexDigit('F'));
	assertTrue (!Ascii::isHexDigit('G'));

	assertTrue (Ascii::isPunct('.'));
	assertTrue (Ascii::isPunct(','));
	assertTrue (!Ascii::isPunct('A'));
	
	assertTrue (Ascii::isAlpha('a'));
	assertTrue (Ascii::isAlpha('Z'));
	assertTrue (!Ascii::isAlpha('0'));
	
	assertTrue (Ascii::isLower('a'));
	assertTrue (!Ascii::isLower('A'));
	
	assertTrue (Ascii::isUpper('A'));
	assertTrue (!Ascii::isUpper('a'));
	
	assertTrue (Ascii::toLower('A') == 'a');
	assertTrue (Ascii::toLower('z') == 'z');
	assertTrue (Ascii::toLower('0') == '0');
	
	assertTrue (Ascii::toUpper('a') == 'A');
	assertTrue (Ascii::toUpper('0') == '0');
	assertTrue (Ascii::toUpper('Z') == 'Z');
}


void CoreTest::testChecksum64()
{
	Poco::Checksum checksum64_0(Checksum::TYPE_CRC64);
	Poco::Checksum checksum64_1(Checksum::TYPE_CRC64);
	Poco::UInt64 crc64_0 = 0;
	Poco::UInt64 crc64_1 = 0;
	Poco::UInt64 crc64_2 = 0;
	Poco::UInt64 crc64_3 = 0;
	Poco::UInt64 crc64_4 = 0;
	Poco::UInt64 crc64_5 = 0;
	Poco::UInt64 crc64_6 = 0;
	Poco::UInt64 crc64_7 = 0;
	std::string str = "Hello world!!!";
	const char c_str[] = "Hello People!!!";
	const char c_str1[] = "Hello world!!!";
	const char c_str2[] = "b";
	const char c_str3[] = "c";
	char ch = 'c';

	checksum64_0.update(str);
	crc64_0 = checksum64_0.checksum();               // crc64 of "Hello world!!!"
	checksum64_0.update(c_str, (int)strlen(c_str));
	crc64_1 = checksum64_0.checksum();               // crc64 of "Hello People!!!"
	assertTrue (crc64_0 != crc64_1);

	checksum64_0.update(ch);
	crc64_2 = checksum64_0.checksum();               // crc64 of 'c'
	assertTrue (crc64_0 != crc64_2);

	assertTrue (crc64_1 != crc64_2);

	checksum64_0.update(c_str1);
	crc64_3 = checksum64_0.checksum();               // crc64 of "Hello world!!!"
	assertTrue (crc64_0 == crc64_3);

	str = "c";
	checksum64_0.update(str);
	crc64_4 = checksum64_0.checksum();              // crc64 of "c", fetching from checksum64_0 object
	checksum64_1.update(ch);
	crc64_5 = checksum64_1.checksum();              // crc64 of 'c', fetching from checksum64_1 object
	assertTrue (crc64_4 == crc64_5);

	checksum64_0.update(c_str2, (int)strlen(c_str2));
	crc64_6 = checksum64_0.checksum();              // crc64 of "b", fetching from checksum64_0 object
	assertTrue (crc64_5 != crc64_6);

	checksum64_0.update(c_str3, (int)strlen(c_str3));
	crc64_7 = checksum64_0.checksum();              // crc64 of "c", fetching from checksum64_0 object
	assertTrue (crc64_5 == crc64_7);
}


void CoreTest::testMakeUnique()
{
	assertTrue (*makeUnique<int>() == 0);
	assertTrue (*makeUnique<int>(1729) == 1729);
	assertTrue (*makeUnique<std::string>() == "");
	assertTrue (*makeUnique<std::string>("meow") == "meow");
	assertTrue (*makeUnique<std::string>(6, 'z') == "zzzzzz");

	auto up = makeUnique<int[]>(5);

	for (int i = 0; i < 5; ++i) up[i] = i;
	for (int i = 0; i < 5; ++i) assertTrue (up[i] == i);
}


void CoreTest::setUp()
{
	_readableToNot = 0;
	_notToReadable = 0;
	_writableToNot = 0;
	_notToWritable = 0;
}


void CoreTest::tearDown()
{
}


CppUnit::Test* CoreTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CoreTest");

	CppUnit_addTest(pSuite, CoreTest, testPlatform);
	CppUnit_addTest(pSuite, CoreTest, testFixedLength);
	CppUnit_addTest(pSuite, CoreTest, testBugcheck);
	CppUnit_addTest(pSuite, CoreTest, testEnvironment);
	CppUnit_addTest(pSuite, CoreTest, testBuffer);
	CppUnit_addTest(pSuite, CoreTest, testAtomicCounter);
	CppUnit_addTest(pSuite, CoreTest, testAtomicFlag);
	CppUnit_addTest(pSuite, CoreTest, testNullable);
	CppUnit_addTest(pSuite, CoreTest, testAscii);
	CppUnit_addTest(pSuite, CoreTest, testChecksum64);
	CppUnit_addTest(pSuite, CoreTest, testMakeUnique);

	return pSuite;
}
