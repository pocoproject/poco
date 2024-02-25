//
// CoreTest.cpp
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
#include "Poco/Debugger.h"
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
using Poco::BasicFIFOBuffer;
using Poco::FIFOBuffer;
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


CoreTest::CoreTest(const std::string& name): CppUnit::TestCase(name)
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

	Environment::set("FOO", "BAR");
	assertTrue (Environment::has("FOO"));
	assertTrue (Environment::get("FOO") == "BAR");

	try
	{
		std::string v = Environment::get("THISONEDOESNOTEXIST123");
		failmsg("Environment variable does not exist - must throw exception");
	}
	catch (Exception&)
	{
	}

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
}


void CoreTest::testFIFOBufferEOFAndError()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);

	assertTrue (f.isEmpty());
	assertTrue (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &CoreTest::onReadable);
	f.writable += delegate(this, &CoreTest::onWritable);

	for (T c = '0'; c < '0' +  10; ++c)
		v.push_back(c);

	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	assertTrue (0 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (10 == f.write(b));
	assertTrue (1 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (10 == f.used());
	assertTrue (!f.isEmpty());
	f.setEOF();
	assertTrue (0 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (f.hasEOF());
	assertTrue (!f.isEOF());
	assertTrue (1 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.write(b));
	assertTrue (!f.isEmpty());
	assertTrue (5 == f.read(b, 5));
	assertTrue (1 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (f.hasEOF());
	assertTrue (!f.isEOF());
	assertTrue (5 == f.read(b, 5));
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (f.hasEOF());
	assertTrue (f.isEOF());
	assertTrue (0 == _notToWritable);
	assertTrue (1 == _writableToNot);

	f.setEOF(false);
	assertTrue (!f.hasEOF());
	assertTrue (!f.isEOF());
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _readableToNot);

	assertTrue (5 == f.write(b));
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (2 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	f.setError();
	assertTrue (0 == f.write(b));

	try
	{
		f.copy(b.begin(), 5);
		fail ("must throw InvalidAccessException");
	}
	catch (InvalidAccessException&) { }

	try
	{
		f.advance(5);
		fail ("must throw InvalidAccessException");
	}
	catch (InvalidAccessException&) { }

	assertTrue (1 == _notToWritable);
	assertTrue (2 == _writableToNot);
	assertTrue (2 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (0 == f.used());
	f.setError(false);
	assertTrue (2 == _notToWritable);
	assertTrue (2 == _writableToNot);
	assertTrue (2 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (0 == f.used());
	assertTrue (5 == f.write(b));
	assertTrue (2 == _notToWritable);
	assertTrue (2 == _writableToNot);
	assertTrue (3 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
}


void CoreTest::testFIFOBufferChar()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);

	assertTrue (f.isEmpty());
	assertTrue (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &CoreTest::onReadable);
	f.writable += delegate(this, &CoreTest::onWritable);

	for (T c = '0'; c < '0' +  10; ++c)
		v.push_back(c);

	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	assertTrue (0 == _notToReadable);
	assertTrue (0 == _readableToNot);
	f.write(b);
	assertTrue (1 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (20 == f.size());
	assertTrue (10 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('0' == f[0]);
	assertTrue ('1' == f[1]);
	assertTrue ('2' == f[2]);
	assertTrue ('3' == f[3]);
	assertTrue ('4' == f[4]);
	assertTrue ('5' == f[5]);
	assertTrue ('6' == f[6]);
	assertTrue ('7' == f[7]);
	assertTrue ('8' == f[8]);
	assertTrue ('9' == f[9]);

	b.resize(5);
	f.read(b, b.size());
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('5' == f[0]);
	assertTrue ('6' == f[1]);
	assertTrue ('7' == f[2]);
	assertTrue ('8' == f[3]);
	assertTrue ('9' == f[4]);
	try { T POCO_UNUSED i = f[10]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	v.clear();
	for (T c = 'a'; c < 'a' + 10; ++c)
		v.push_back(c);

	b.resize(10);
	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	f.write(b);
	assertTrue (20 == f.size());
	assertTrue (15 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('5' == f[0]);
	assertTrue ('6' == f[1]);
	assertTrue ('7' == f[2]);
	assertTrue ('8' == f[3]);
	assertTrue ('9' == f[4]);
	assertTrue ('a' == f[5]);
	assertTrue ('b' == f[6]);
	assertTrue ('c' == f[7]);
	assertTrue ('d' == f[8]);
	assertTrue ('e' == f[9]);
	assertTrue ('f' == f[10]);
	assertTrue ('g' == f[11]);
	assertTrue ('h' == f[12]);
	assertTrue ('i' == f[13]);
	assertTrue ('j' == f[14]);
	try { T POCO_UNUSED i = f[15]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 10);
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('f' == f[0]);
	assertTrue ('g' == f[1]);
	assertTrue ('h' == f[2]);
	assertTrue ('i' == f[3]);
	assertTrue ('j' == f[4]);
	try { T POCO_UNUSED i = f[5]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	assertTrue (1 == _notToReadable);
	assertTrue (0 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);
	f.read(b, 6);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);

	assertTrue (5 == b.size());
	assertTrue (20 == f.size());
	assertTrue (0 == f.used());
	try { T POCO_UNUSED i = f[0]; fail ("must fail"); }
	catch (InvalidAccessException&) { }
	assertTrue (f.isEmpty());

	assertTrue (1 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);
	assertTrue (5 == f.write(b));
	assertTrue (2 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);

	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('f' == f[0]);
	assertTrue ('g' == f[1]);
	assertTrue ('h' == f[2]);
	assertTrue ('i' == f[3]);
	assertTrue ('j' == f[4]);

	f.resize(10);
	assertTrue (10 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue ('f' == f[0]);
	assertTrue ('g' == f[1]);
	assertTrue ('h' == f[2]);
	assertTrue ('i' == f[3]);
	assertTrue ('j' == f[4]);

	assertTrue (2 == _notToReadable);
	assertTrue (1 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);
	f.resize(3, false);
	assertTrue (2 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);
	assertTrue (3 == f.size());
	assertTrue (0 == f.used());
	assertTrue (f.isEmpty());

	b.resize(3);
	b[0] = 'x';
	b[1] = 'y';
	b[2] = 'z';
	f.resize(3);

	assertTrue (2 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (0 == _writableToNot);
	f.write(b);
	assertTrue (3 == _notToReadable);
	assertTrue (2 == _readableToNot);
	assertTrue (0 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (f.isFull());

	f.read(b);
	assertTrue (3 == _notToReadable);
	assertTrue (3 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (f.isEmpty());

	f.resize(10);
	assertTrue (10 == f.size());
	assertTrue (0 == f.used());
	assertTrue (10 == f.available());
	assertTrue (f.isEmpty());

	assertTrue (3 == _notToReadable);
	assertTrue (3 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);
	f.write(b);
	assertTrue (4 == _notToReadable);
	assertTrue (3 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (10 == f.size());
	assertTrue (3 == f.used());
	assertTrue (7 == f.available());
	assertTrue (!f.isEmpty());

	f.drain(1);
	assertTrue (4 == _notToReadable);
	assertTrue (3 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (10 == f.size());
	assertTrue (2 == f.used());
	assertTrue (8 == f.available());
	assertTrue (!f.isEmpty());

	f.drain(2);
	assertTrue (4 == _notToReadable);
	assertTrue (4 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (10 == f.size());
	assertTrue (0 == f.used());
	assertTrue (10 == f.available());
	assertTrue (f.isEmpty());

	f.write(b);
	assertTrue (5 == _notToReadable);
	assertTrue (4 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (10 == f.size());
	assertTrue (3 == f.used());
	assertTrue (7 == f.available());
	assertTrue (!f.isEmpty());

	f.drain();
	assertTrue (5 == _notToReadable);
	assertTrue (5 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);
	assertTrue (10 == f.size());
	assertTrue (0 == f.used());
	assertTrue (10 == f.available());
	assertTrue (f.isEmpty());

	f.write(b, 2);
	assertTrue (10 == f.size());
	assertTrue (2 == f.used());
	assertTrue (8 == f.available());
	assertTrue (!f.isEmpty());

	assertTrue (6 == _notToReadable);
	assertTrue (5 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	f.drain();
	assertTrue (6 == _notToReadable);
	assertTrue (6 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (3 == f.write(b, 10));
	assertTrue (10 == f.size());
	assertTrue (3 == f.used());
	assertTrue (7 == f.available());
	assertTrue (!f.isEmpty());

	assertTrue (7 == _notToReadable);
	assertTrue (6 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	const char arr[3] = {'4', '5', '6' };
	try
	{
		f.copy(&arr[0], 8);
		fail("must fail");
	} catch (InvalidAccessException&) { }

	f.copy(&arr[0], 3);
	assertTrue (7 == _notToReadable);
	assertTrue (6 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (1 == _writableToNot);

	assertTrue (10 == f.size());
	assertTrue (6 == f.used());
	assertTrue (4 == f.available());

	f.copy(&arr[0], 4);
	assertTrue (7 == _notToReadable);
	assertTrue (6 == _readableToNot);
	assertTrue (1 == _notToWritable);
	assertTrue (2 == _writableToNot);
	assertTrue (f.isFull());

	assertTrue (10 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.available());

	try
	{
		f.copy(&arr[0], 1);
		fail("must fail");
	} catch (InvalidAccessException&) { }

	f.drain(1);
	assertTrue (7 == _notToReadable);
	assertTrue (6 == _readableToNot);
	assertTrue (2 == _notToWritable);
	assertTrue (2 == _writableToNot);

	f.drain(9);
	assertTrue (10 == f.size());
	assertTrue (0 == f.used());
	assertTrue (10 == f.available());

	const char e[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	f.copy(&e[0], 10);
	assertTrue (10 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.available());
	f.drain(1);
	f.write(e, 1);
	assertTrue (10 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.available());

	assertTrue (f[0] == '2');
	assertTrue (f[1] == '3');
	assertTrue (f[2] == '4');
	assertTrue (f[3] == '5');
	assertTrue (f[4] == '6');
	assertTrue (f[5] == '7');
	assertTrue (f[6] == '8');
	assertTrue (f[7] == '9');
	assertTrue (f[8] == '0');
	assertTrue (f[9] == '1');

	f.readable -= delegate(this, &CoreTest::onReadable);
	f.writable -= delegate(this, &CoreTest::onReadable);
}


void CoreTest::testFIFOBufferInt()
{
	typedef int T;

	BasicFIFOBuffer<T> f(20);
	Buffer<T> b(10);
	std::vector<T> v;

	for (T c = 0; c < 10; ++c)
		v.push_back(c);

	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	f.write(b);
	assertTrue (20 == f.size());
	assertTrue (10 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (0 == f[0]);
	assertTrue (1 == f[1]);
	assertTrue (2 == f[2]);
	assertTrue (3 == f[3]);
	assertTrue (4 == f[4]);
	assertTrue (5 == f[5]);
	assertTrue (6 == f[6]);
	assertTrue (7 == f[7]);
	assertTrue (8 == f[8]);
	assertTrue (9 == f[9]);

	b.resize(5);
	f.read(b, b.size());
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (5 == f[0]);
	assertTrue (6 == f[1]);
	assertTrue (7 == f[2]);
	assertTrue (8 == f[3]);
	assertTrue (9 == f[4]);
	try { T POCO_UNUSED i = f[10]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	v.clear();
	for (T c = 10; c < 20; ++c)
		v.push_back(c);

	b.resize(10);
	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	f.write(b);
	assertTrue (20 == f.size());
	assertTrue (15 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (5 == f[0]);
	assertTrue (6 == f[1]);
	assertTrue (7 == f[2]);
	assertTrue (8 == f[3]);
	assertTrue (9 == f[4]);
	assertTrue (10 == f[5]);
	assertTrue (11 == f[6]);
	assertTrue (12 == f[7]);
	assertTrue (13 == f[8]);
	assertTrue (14 == f[9]);
	assertTrue (15 == f[10]);
	assertTrue (16 == f[11]);
	assertTrue (17 == f[12]);
	assertTrue (18 == f[13]);
	assertTrue (19 == f[14]);
	try { T POCO_UNUSED i = f[15]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 10);
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (15 == f[0]);
	assertTrue (16 == f[1]);
	assertTrue (17 == f[2]);
	assertTrue (18 == f[3]);
	assertTrue (19 == f[4]);
	try { T POCO_UNUSED i = f[5]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 6);
	assertTrue (5 == b.size());
	assertTrue (20 == f.size());
	assertTrue (0 == f.used());
	try { T POCO_UNUSED i = f[0]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	assertTrue (f.isEmpty());

	assertTrue (5 == f.write(b));
	assertTrue (20 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (15 == f[0]);
	assertTrue (16 == f[1]);
	assertTrue (17 == f[2]);
	assertTrue (18 == f[3]);
	assertTrue (19 == f[4]);

	f.resize(10);
	assertTrue (10 == f.size());
	assertTrue (5 == f.used());
	assertTrue (!f.isEmpty());
	assertTrue (15 == f[0]);
	assertTrue (16 == f[1]);
	assertTrue (17 == f[2]);
	assertTrue (18 == f[3]);
	assertTrue (19 == f[4]);

	f.drain(9);
	assertTrue (10 == f.size());
	assertTrue (0 == f.used());
	assertTrue (10 == f.available());

	const int e[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	f.copy(&e[0], 10);
	assertTrue (10 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.available());
	f.drain(1);
	f.write(e, 1);
	assertTrue (10 == f.size());
	assertTrue (10 == f.used());
	assertTrue (0 == f.available());

	assertTrue (f[0] == 2);
	assertTrue (f[1] == 3);
	assertTrue (f[2] == 4);
	assertTrue (f[3] == 5);
	assertTrue (f[4] == 6);
	assertTrue (f[5] == 7);
	assertTrue (f[6] == 8);
	assertTrue (f[7] == 9);
	assertTrue (f[8] == 0);
	assertTrue (f[9] == 1);

	f.resize(3, false);
	assertTrue (3 == f.size());
	assertTrue (0 == f.used());
	assertTrue (f.isEmpty());
}


void CoreTest::testAtomicCounter()
{
	AtomicCounter ac;

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


void CoreTest::testNullable()
{
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
		int POCO_UNUSED tmp = n1.value();
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

	NullType nd{};
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


void CoreTest::testSrcLoc()
{
	// must be all on a single line to succeed
	assertTrue(poco_src_loc == Poco::format("CoreTest.cpp::testSrcLoc():%d", __LINE__));
}


void CoreTest::onReadable(bool& b)
{
	if (b) ++_notToReadable;
	else ++_readableToNot;
};


void CoreTest::onWritable(bool& b)
{
	if (b) ++_notToWritable;
	else ++_writableToNot;
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
	CppUnit_addTest(pSuite, CoreTest, testFIFOBufferChar);
	CppUnit_addTest(pSuite, CoreTest, testFIFOBufferInt);
	CppUnit_addTest(pSuite, CoreTest, testFIFOBufferEOFAndError);
	CppUnit_addTest(pSuite, CoreTest, testAtomicCounter);
	CppUnit_addTest(pSuite, CoreTest, testNullable);
	CppUnit_addTest(pSuite, CoreTest, testAscii);
	CppUnit_addTest(pSuite, CoreTest, testSrcLoc);

	return pSuite;
}
