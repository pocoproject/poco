//
// FIFOBufferTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FIFOBufferTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/Delegate.h"
#include "Poco/FIFOBuffer.h"
#include <iostream>
#include <memory>
#include <cstring>


// NOTE: using for certain namespace
using Poco::BasicFIFOBuffer;
using Poco::FIFOBuffer;
using Poco::Buffer;
using Poco::InvalidAccessException;
using Poco::delegate;
using std::memcpy;


FIFOBufferTest::FIFOBufferTest(const std::string& rName):
	CppUnit::TestCase(rName),
	_notToReadable(0),
	_notToWritable(0),
	_readableToNot(0),
	_writableToNot(0)
{
}


FIFOBufferTest::~FIFOBufferTest()
{
}

void FIFOBufferTest::onReadable(bool& b)
{
	if (b) ++_notToReadable;
	else ++_readableToNot;
};


void FIFOBufferTest::onWritable(bool& b)
{
	if (b) ++_notToWritable;
	else ++_writableToNot;
}

void FIFOBufferTest::testNextWrite()
{
	// String length is 88 characters.
	const int BUFFER_SIZE = 128;
	std::string text("The Quick Brown Dog Jumps Over The Lazy Fox.");
	BasicFIFOBuffer<char> buffer(BUFFER_SIZE);
	buffer.write(text.data(), text.size());
	char cbuffer[BUFFER_SIZE];
	std::memset(cbuffer, 0, buffer.size());
	
	buffer.read(cbuffer, 4);

	assertTrue (std::string(cbuffer, 4) == std::string("The "));
	
	buffer.peek(cbuffer, buffer.used());
	assertTrue (std::string(cbuffer, buffer.used()) ==
			std::string("Quick Brown Dog Jumps Over The Lazy Fox."));

	memcpy(buffer.next(), "The ", 4);
	buffer.advance(4);

	buffer.peek(cbuffer, buffer.used());

	assertTrue (std::string("Quick Brown Dog Jumps Over The Lazy Fox.The ") == std::string(cbuffer, buffer.used()));
}


void FIFOBufferTest::testEOFAndError()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);
	
	assertTrue (f.isEmpty());
	assertTrue (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &FIFOBufferTest::onReadable);
	f.writable += delegate(this, &FIFOBufferTest::onWritable);

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


void FIFOBufferTest::testChar()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);
	
	assertTrue (f.isEmpty());
	assertTrue (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &FIFOBufferTest::onReadable);
	f.writable += delegate(this, &FIFOBufferTest::onWritable);

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
	try { T i = f[10]; fail ("must fail"); }
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
	try { T i = f[15]; fail ("must fail"); }
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
	try { T i = f[5]; fail ("must fail"); }
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
	try { T i = f[0]; fail ("must fail"); }
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

	f.readable -= delegate(this, &FIFOBufferTest::onReadable);
	f.writable -= delegate(this, &FIFOBufferTest::onReadable);
}


void FIFOBufferTest::testInt()
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
	try { T i = f[10]; fail ("must fail"); }
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
	try { T i = f[15]; fail ("must fail"); }
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
	try { T i = f[5]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 6);
	assertTrue (5 == b.size());
	assertTrue (20 == f.size());
	assertTrue (0 == f.used());
	try { T i = f[0]; fail ("must fail"); }
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

void FIFOBufferTest::setUp()
{
	_notToReadable = 0;
	_notToWritable = 0;
	_readableToNot = 0;
	_writableToNot = 0;
}


void FIFOBufferTest::tearDown()
{
}


CppUnit::Test* FIFOBufferTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FIFOBufferTest");

	CppUnit_addTest(pSuite, FIFOBufferTest, testNextWrite);
	CppUnit_addTest(pSuite, FIFOBufferTest, testChar);
	CppUnit_addTest(pSuite, FIFOBufferTest, testInt);
	CppUnit_addTest(pSuite, FIFOBufferTest, testEOFAndError);

	return pSuite;
}
