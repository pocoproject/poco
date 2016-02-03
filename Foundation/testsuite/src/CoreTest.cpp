//
// CoreTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/CoreTest.cpp#2 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CoreTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Bugcheck.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Buffer.h"
#include "Poco/FIFOBuffer.h"
#include "Poco/AtomicCounter.h"
#include "Poco/Nullable.h"
#include "Poco/Ascii.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
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
using Poco::Nullable;
using Poco::Ascii;
using Poco::BasicEvent;
using Poco::delegate;
using Poco::NullType;
using Poco::InvalidAccessException;


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
	assert (sizeof(Poco::Int8) == 1);
	assert (sizeof(Poco::UInt8) == 1);
	assert (sizeof(Poco::Int16) == 2);
	assert (sizeof(Poco::UInt16) == 2);
	assert (sizeof(Poco::Int32) == 4);
	assert (sizeof(Poco::UInt32) == 4);
#if defined(POCO_HAVE_INT64)
	assert (sizeof(Poco::Int64) == 8);
	assert (sizeof(Poco::UInt64) == 8);
#endif
	assert (sizeof(Poco::IntPtr) == sizeof(void*));
	assert (sizeof(Poco::UIntPtr) == sizeof(void*));	
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
	assert (Environment::has("FOO"));
	assert (Environment::get("FOO") == "BAR");
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
	assert (b.size() == s);
	assert (b.sizeBytes() == s * sizeof(int));
	assert (b.capacity() == s);
	assert (b.capacityBytes() == s * sizeof(int));
	std::vector<int> v;
	for (int i = 0; i < s; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

	assert (s == b.size());
	for (int i = 0; i < s; ++i)
		assert (b[i] == i);

	b.resize(s/2);
	for (int i = 0; i < s/2; ++i)
		assert (b[i] == i);

	assert (b.size() == s/2);
	assert (b.capacity() == s);

	b.resize(s*2);
	v.clear();
	for (int i = 0; i < s*2; ++i)
		v.push_back(i);

	std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

	for (int i = 0; i < s*2; ++i)
		assert (b[i] == i);

	assert (b.size() == s*2);
	assert (b.capacity() == s*2);

	b.setCapacity(s * 4);
	assert (b.size() == s*2);
	assert (b.capacity() == s*4);

	b.setCapacity(s);
	assert (b.size() == s);
	assert (b.capacity() == s);

#if ENABLE_BUGCHECK_TEST
	try { int i = b[s]; fail ("must fail"); }
	catch (Exception&) { }
#endif

	Buffer<int> c(s);
	Buffer<int> d(c);
	assert (c == d);

	c[1] = -1;
	assert (c[1] == -1);
	c.clear();
	assert (c[1] == 0);

	Buffer<int> e(0);
	assert (e.empty());

	assert (c != e);

	Buffer<int> f = e;
	assert (f == e);

	Buffer<char> g(0);
	g.append("hello", 5);
	assert (g.size() == 5);

	g.append("hello", 5);
	assert (g.size() == 10);
	assert ( !std::memcmp(g.begin(), "hellohello", 10) );

	char h[10];
	Buffer<char> buf(h, 10);
	try
	{
		buf.append("hello", 5);
		fail ("must fail");
	}
	catch (InvalidAccessException&) { }

	buf.assign("hello", 5);
	assert ( !std::memcmp(&h[0], "hello", 5) );

	const char j[10] = "hello";
	Buffer<char> k(j, 5);
	k.append("hello", 5);
	assert ( !std::memcmp(&j[0], "hello", 5) );
	assert ( !std::memcmp(k.begin(), "hellohello", 10) );
	k.append('w');
	assert (k.size() == 11);
	assert ( !std::memcmp(k.begin(), "hellohellow", k.size()) );
	k.append('o');
	assert (k.size() == 12);
	assert ( !std::memcmp(k.begin(), "hellohellowo", k.size()) );
	k.append('r');
	assert (k.size() == 13);
	assert ( !std::memcmp(k.begin(), "hellohellowor", k.size()) );
	k.append('l');
	assert (k.size() == 14);
	assert ( !std::memcmp(k.begin(), "hellohelloworl", k.size()) );
	k.append('d');
	assert (k.size() == 15);
	assert ( !std::memcmp(k.begin(), "hellohelloworld", k.size()) );
}




void CoreTest::testAtomicCounter()
{
	AtomicCounter ac;
	
	assert (ac.value() == 0);
	assert (ac++ == 0);
	assert (ac-- == 1);
	assert (++ac == 1);
	assert (--ac == 0);
	
	ac = 2;
	assert (ac.value() == 2);
	
	ac = 0;
	assert (ac.value() == 0);
	
	AtomicCounter ac2(2);
	assert (ac2.value() == 2);
	
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
	
	assert (ac.value() == 0);
}


void CoreTest::testNullable()
{
	Nullable<int> i;
	Nullable<double> f;
	Nullable<std::string> s;

	assert (i.isNull());
	assert (f.isNull());
	assert (s.isNull());

	i = 1;
	f = 1.5;
	s = "abc";

	assert (!i.isNull());
	assert (!f.isNull());
	assert (!s.isNull());

	assert (i == 1);
	assert (f == 1.5);
	assert (s == "abc");

	i.clear();
	f.clear();
	s.clear();

	assert (i.isNull());
	assert (f.isNull());
	assert (s.isNull());

	Nullable<int> n1;
	assert (n1.isNull());
	
	assert (n1.value(42) == 42);
	assert (n1.isNull());
	assert (!(0 == n1));
	assert (0 != n1);
	assert (!(n1 == 0));
	assert (n1 != 0);
	
	try
	{
		int tmp = n1.value();
		fail("null value, must throw");
	}
	catch (Poco::NullValueException&)
	{
	}
	
	n1 = 1;
	assert (!n1.isNull());
	assert (n1.value() == 1);
	
	Nullable<int> n2(42);
	assert (!n2.isNull());
	assert (n2.value() == 42);
	assert (n2.value(99) == 42);
	
	assert (!(0 == n2));
	assert (0 != n2);
	assert (!(n2 == 0));
	assert (n2 != 0);
	
	n1 = n2;
	assert (!n1.isNull());
	assert (n1.value() == 42);
	
	std::ostringstream str;
	str << n1;
	assert (str.str() == "42");

	n1.clear();
	assert (n1.isNull());

	str.str(""); str << n1;
	assert (str.str().empty());

	n2.clear();
	assert (n1 == n2);
	n1 = 1; n2 = 1;
	assert (n1 == n2);
	n1.clear();
	assert (n1 < n2);
	assert (n2 > n1);
	n2 = -1; n1 = 0;
	assert (n2 < n1);
	assert (n2 != n1);
	assert (n1 > n2);

	NullType nd;
	assert (n1 != nd);
	assert (nd != n1);
	n1.clear();
	assert (n1 == nd);
	assert (nd == n1);
}


void CoreTest::testAscii()
{
	assert (Ascii::isAscii('A'));
	assert (!Ascii::isAscii(-1));
	assert (!Ascii::isAscii(128));
	assert (!Ascii::isAscii(222));
	
	assert (Ascii::isSpace(' '));
	assert (Ascii::isSpace('\t'));
	assert (Ascii::isSpace('\r'));
	assert (Ascii::isSpace('\n'));
	assert (!Ascii::isSpace('A'));
	assert (!Ascii::isSpace(-1));
	assert (!Ascii::isSpace(222));
	
	assert (Ascii::isDigit('0'));
	assert (Ascii::isDigit('1'));
	assert (Ascii::isDigit('2'));
	assert (Ascii::isDigit('3'));
	assert (Ascii::isDigit('4'));
	assert (Ascii::isDigit('5'));
	assert (Ascii::isDigit('6'));
	assert (Ascii::isDigit('7'));
	assert (Ascii::isDigit('8'));
	assert (Ascii::isDigit('9'));
	assert (!Ascii::isDigit('a'));
	
	assert (Ascii::isHexDigit('0'));
	assert (Ascii::isHexDigit('1'));
	assert (Ascii::isHexDigit('2'));
	assert (Ascii::isHexDigit('3'));
	assert (Ascii::isHexDigit('4'));
	assert (Ascii::isHexDigit('5'));
	assert (Ascii::isHexDigit('6'));
	assert (Ascii::isHexDigit('7'));
	assert (Ascii::isHexDigit('8'));
	assert (Ascii::isHexDigit('9'));
	assert (Ascii::isHexDigit('a'));
	assert (Ascii::isHexDigit('b'));
	assert (Ascii::isHexDigit('c'));
	assert (Ascii::isHexDigit('d'));
	assert (Ascii::isHexDigit('e'));
	assert (Ascii::isHexDigit('f'));
	assert (Ascii::isHexDigit('A'));
	assert (Ascii::isHexDigit('B'));
	assert (Ascii::isHexDigit('C'));
	assert (Ascii::isHexDigit('D'));
	assert (Ascii::isHexDigit('E'));
	assert (Ascii::isHexDigit('F'));
	assert (!Ascii::isHexDigit('G'));

	assert (Ascii::isPunct('.'));
	assert (Ascii::isPunct(','));
	assert (!Ascii::isPunct('A'));
	
	assert (Ascii::isAlpha('a'));
	assert (Ascii::isAlpha('Z'));
	assert (!Ascii::isAlpha('0'));
	
	assert (Ascii::isLower('a'));
	assert (!Ascii::isLower('A'));
	
	assert (Ascii::isUpper('A'));
	assert (!Ascii::isUpper('a'));
	
	assert (Ascii::toLower('A') == 'a');
	assert (Ascii::toLower('z') == 'z');
	assert (Ascii::toLower('0') == '0');
	
	assert (Ascii::toUpper('a') == 'A');
	assert (Ascii::toUpper('0') == '0');
	assert (Ascii::toUpper('Z') == 'Z');
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
	CppUnit_addTest(pSuite, CoreTest, testNullable);
	CppUnit_addTest(pSuite, CoreTest, testAscii);

	return pSuite;
}
