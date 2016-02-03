//
// FIFOBufferTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/FIFOBufferTest.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FIFOBufferTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
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

	assert(std::string(cbuffer, 4) == std::string("The "));
	
	buffer.peek(cbuffer, buffer.used());
	assert(std::string(cbuffer, buffer.used()) ==
			std::string("Quick Brown Dog Jumps Over The Lazy Fox."));

	memcpy(buffer.next(), "The ", 4);
	buffer.advance(4);

	buffer.peek(cbuffer, buffer.used());

	assert(std::string("Quick Brown Dog Jumps Over The Lazy Fox.The ") == std::string(cbuffer, buffer.used()));
}


void FIFOBufferTest::testEOFAndError()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);
	
	assert (f.isEmpty());
	assert (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &FIFOBufferTest::onReadable);
	f.writable += delegate(this, &FIFOBufferTest::onWritable);

	for (T c = '0'; c < '0' +  10; ++c)
		v.push_back(c);

	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	assert(0 == _notToReadable);
	assert(0 == _readableToNot);
	assert (10 == f.write(b));
	assert(1 == _notToReadable);
	assert(0 == _readableToNot);
	assert (20 == f.size());
	assert (10 == f.used());
	assert (!f.isEmpty());
	f.setEOF();
	assert(0 == _notToWritable);
	assert(1 == _writableToNot);
	assert (f.hasEOF());
	assert (!f.isEOF());
	assert(1 == _notToReadable);
	assert(0 == _readableToNot);
	assert (20 == f.size());
	assert (10 == f.used());
	assert (0 == f.write(b));
	assert (!f.isEmpty());
	assert (5 == f.read(b, 5));
	assert(1 == _notToReadable);
	assert(0 == _readableToNot);
	assert (f.hasEOF());
	assert (!f.isEOF());
	assert (5 == f.read(b, 5));
	assert(1 == _notToReadable);
	assert(1 == _readableToNot);
	assert (f.hasEOF());
	assert (f.isEOF());
	assert(0 == _notToWritable);
	assert(1 == _writableToNot);

	f.setEOF(false);
	assert (!f.hasEOF());
	assert (!f.isEOF());
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);
	assert(1 == _notToReadable);
	assert(1 == _readableToNot);

	assert (5 == f.write(b));
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);
	assert(2 == _notToReadable);
	assert(1 == _readableToNot);
	assert (20 == f.size());
	assert (5 == f.used());
	f.setError();
	assert (0 == f.write(b));
	
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
	
	assert(1 == _notToWritable);
	assert(2 == _writableToNot);
	assert(2 == _notToReadable);
	assert(2 == _readableToNot);
	assert (20 == f.size());
	assert (0 == f.used());
	f.setError(false);
	assert(2 == _notToWritable);
	assert(2 == _writableToNot);
	assert(2 == _notToReadable);
	assert(2 == _readableToNot);
	assert (20 == f.size());
	assert (0 == f.used());
	assert (5 == f.write(b));
	assert(2 == _notToWritable);
	assert(2 == _writableToNot);
	assert(3 == _notToReadable);
	assert(2 == _readableToNot);
	assert (20 == f.size());
	assert (5 == f.used());
}


void FIFOBufferTest::testChar()
{
	typedef FIFOBuffer::Type T;

	FIFOBuffer f(20, true);
	
	assert (f.isEmpty());
	assert (!f.isFull());

	Buffer<T> b(10);
	std::vector<T> v;

	f.readable += delegate(this, &FIFOBufferTest::onReadable);
	f.writable += delegate(this, &FIFOBufferTest::onWritable);

	for (T c = '0'; c < '0' +  10; ++c)
		v.push_back(c);

	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	assert(0 == _notToReadable);
	assert(0 == _readableToNot);
	f.write(b);
	assert(1 == _notToReadable);
	assert(0 == _readableToNot);
	assert (20 == f.size());
	assert (10 == f.used());
	assert (!f.isEmpty());
	assert ('0' == f[0]);
	assert ('1' == f[1]);
	assert ('2' == f[2]);
	assert ('3' == f[3]);
	assert ('4' == f[4]);
	assert ('5' == f[5]);
	assert ('6' == f[6]);
	assert ('7' == f[7]);
	assert ('8' == f[8]);
	assert ('9' == f[9]);

	b.resize(5);
	f.read(b, b.size());
	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert ('5' == f[0]);
	assert ('6' == f[1]);
	assert ('7' == f[2]);
	assert ('8' == f[3]);
	assert ('9' == f[4]);
	try { T i = f[10]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	v.clear();
	for (T c = 'a'; c < 'a' + 10; ++c)
		v.push_back(c);

	b.resize(10);
	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	f.write(b);
	assert (20 == f.size());
	assert (15 == f.used());
	assert (!f.isEmpty());
	assert ('5' == f[0]);
	assert ('6' == f[1]);
	assert ('7' == f[2]);
	assert ('8' == f[3]);
	assert ('9' == f[4]);
	assert ('a' == f[5]);
	assert ('b' == f[6]);
	assert ('c' == f[7]);
	assert ('d' == f[8]);
	assert ('e' == f[9]);
	assert ('f' == f[10]);
	assert ('g' == f[11]);
	assert ('h' == f[12]);
	assert ('i' == f[13]);
	assert ('j' == f[14]);
	try { T i = f[15]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 10);
	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert ('f' == f[0]);
	assert ('g' == f[1]);
	assert ('h' == f[2]);
	assert ('i' == f[3]);
	assert ('j' == f[4]);
	try { T i = f[5]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	assert(1 == _notToReadable);
	assert(0 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);
	f.read(b, 6);
	assert(1 == _notToReadable);
	assert(1 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);

	assert (5 == b.size());
	assert (20 == f.size());
	assert (0 == f.used());
	try { T i = f[0]; fail ("must fail"); }
	catch (InvalidAccessException&) { }
	assert (f.isEmpty());

	assert(1 == _notToReadable);
	assert(1 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);
	assert (5 == f.write(b));
	assert(2 == _notToReadable);
	assert(1 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);

	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert ('f' == f[0]);
	assert ('g' == f[1]);
	assert ('h' == f[2]);
	assert ('i' == f[3]);
	assert ('j' == f[4]);

	f.resize(10);
	assert (10 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert ('f' == f[0]);
	assert ('g' == f[1]);
	assert ('h' == f[2]);
	assert ('i' == f[3]);
	assert ('j' == f[4]);

	assert(2 == _notToReadable);
	assert(1 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);
	f.resize(3, false);
	assert(2 == _notToReadable);
	assert(2 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);
	assert (3 == f.size());
	assert (0 == f.used());
	assert (f.isEmpty());

	b.resize(3);
	b[0] = 'x';
	b[1] = 'y';
	b[2] = 'z';
	f.resize(3);

	assert(2 == _notToReadable);
	assert(2 == _readableToNot);
	assert(0 == _notToWritable);
	assert(0 == _writableToNot);
	f.write(b);
	assert(3 == _notToReadable);
	assert(2 == _readableToNot);
	assert(0 == _notToWritable);
	assert(1 == _writableToNot);
	assert (f.isFull());

	f.read(b);
	assert(3 == _notToReadable);
	assert(3 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);
	assert (f.isEmpty());

	f.resize(10);
	assert (10 == f.size());
	assert (0 == f.used());
	assert (10 == f.available());
	assert (f.isEmpty());

	assert(3 == _notToReadable);
	assert(3 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);
	f.write(b);
	assert(4 == _notToReadable);
	assert(3 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (10 == f.size());
	assert (3 == f.used());
	assert (7 == f.available());
	assert (!f.isEmpty());

	f.drain(1);
	assert(4 == _notToReadable);
	assert(3 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (10 == f.size());
	assert (2 == f.used());
	assert (8 == f.available());
	assert (!f.isEmpty());

	f.drain(2);
	assert(4 == _notToReadable);
	assert(4 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (10 == f.size());
	assert (0 == f.used());
	assert (10 == f.available());
	assert (f.isEmpty());

	f.write(b);
	assert(5 == _notToReadable);
	assert(4 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (10 == f.size());
	assert (3 == f.used());
	assert (7 == f.available());
	assert (!f.isEmpty());

	f.drain();
	assert(5 == _notToReadable);
	assert(5 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);
	assert (10 == f.size());
	assert (0 == f.used());
	assert (10 == f.available());
	assert (f.isEmpty());

	f.write(b, 2);
	assert (10 == f.size());
	assert (2 == f.used());
	assert (8 == f.available());
	assert (!f.isEmpty());

	assert(6 == _notToReadable);
	assert(5 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	f.drain();
	assert(6 == _notToReadable);
	assert(6 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (3 == f.write(b, 10));
	assert (10 == f.size());
	assert (3 == f.used());
	assert (7 == f.available());
	assert (!f.isEmpty());

	assert(7 == _notToReadable);
	assert(6 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	const char arr[3] = {'4', '5', '6' };
	try
	{
		f.copy(&arr[0], 8);
		fail("must fail");
	} catch (InvalidAccessException&) { }

	f.copy(&arr[0], 3);
	assert(7 == _notToReadable);
	assert(6 == _readableToNot);
	assert(1 == _notToWritable);
	assert(1 == _writableToNot);

	assert (10 == f.size());
	assert (6 == f.used());
	assert (4 == f.available());

	f.copy(&arr[0], 4);
	assert(7 == _notToReadable);
	assert(6 == _readableToNot);
	assert(1 == _notToWritable);
	assert(2 == _writableToNot);
	assert (f.isFull());

	assert (10 == f.size());
	assert (10 == f.used());
	assert (0 == f.available());

	try
	{
		f.copy(&arr[0], 1);
		fail("must fail");
	} catch (InvalidAccessException&) { }

	f.drain(1);
	assert(7 == _notToReadable);
	assert(6 == _readableToNot);
	assert(2 == _notToWritable);
	assert(2 == _writableToNot);

	f.drain(9);
	assert (10 == f.size());
	assert (0 == f.used());
	assert (10 == f.available());

	const char e[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	f.copy(&e[0], 10);
	assert (10 == f.size());
	assert (10 == f.used());
	assert (0 == f.available());
	f.drain(1);
	f.write(e, 1);
	assert (10 == f.size());
	assert (10 == f.used());
	assert (0 == f.available());
	
	assert(f[0] == '2');
	assert(f[1] == '3');
	assert(f[2] == '4');
	assert(f[3] == '5');
	assert(f[4] == '6');
	assert(f[5] == '7');
	assert(f[6] == '8');
	assert(f[7] == '9');
	assert(f[8] == '0');
	assert(f[9] == '1');

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
	assert (20 == f.size());
	assert (10 == f.used());
	assert (!f.isEmpty());
	assert (0 == f[0]);
	assert (1 == f[1]);
	assert (2 == f[2]);
	assert (3 == f[3]);
	assert (4 == f[4]);
	assert (5 == f[5]);
	assert (6 == f[6]);
	assert (7 == f[7]);
	assert (8 == f[8]);
	assert (9 == f[9]);

	b.resize(5);
	f.read(b, b.size());
	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert (5 == f[0]);
	assert (6 == f[1]);
	assert (7 == f[2]);
	assert (8 == f[3]);
	assert (9 == f[4]);
	try { T i = f[10]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	v.clear();
	for (T c = 10; c < 20; ++c)
		v.push_back(c);

	b.resize(10);
	std::memcpy(b.begin(), &v[0], sizeof(T) * v.size());
	f.write(b);
	assert (20 == f.size());
	assert (15 == f.used());
	assert (!f.isEmpty());
	assert (5 == f[0]);
	assert (6 == f[1]);
	assert (7 == f[2]);
	assert (8 == f[3]);
	assert (9 == f[4]);
	assert (10 == f[5]);
	assert (11 == f[6]);
	assert (12 == f[7]);
	assert (13 == f[8]);
	assert (14 == f[9]);
	assert (15 == f[10]);
	assert (16 == f[11]);
	assert (17 == f[12]);
	assert (18 == f[13]);
	assert (19 == f[14]);
	try { T i = f[15]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 10);
	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert (15 == f[0]);
	assert (16 == f[1]);
	assert (17 == f[2]);
	assert (18 == f[3]);
	assert (19 == f[4]);
	try { T i = f[5]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	f.read(b, 6);
	assert (5 == b.size());
	assert (20 == f.size());
	assert (0 == f.used());
	try { T i = f[0]; fail ("must fail"); }
	catch (InvalidAccessException&) { }

	assert (f.isEmpty());

	assert (5 == f.write(b));
	assert (20 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert (15 == f[0]);
	assert (16 == f[1]);
	assert (17 == f[2]);
	assert (18 == f[3]);
	assert (19 == f[4]);

	f.resize(10);
	assert (10 == f.size());
	assert (5 == f.used());
	assert (!f.isEmpty());
	assert (15 == f[0]);
	assert (16 == f[1]);
	assert (17 == f[2]);
	assert (18 == f[3]);
	assert(19 == f[4]);

	f.drain(9);
	assert(10 == f.size());
	assert(0 == f.used());
	assert(10 == f.available());

	const int e[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	f.copy(&e[0], 10);
	assert(10 == f.size());
	assert(10 == f.used());
	assert(0 == f.available());
	f.drain(1);
	f.write(e, 1);
	assert(10 == f.size());
	assert(10 == f.used());
	assert(0 == f.available());

	assert(f[0] == 2);
	assert(f[1] == 3);
	assert(f[2] == 4);
	assert(f[3] == 5);
	assert(f[4] == 6);
	assert(f[5] == 7);
	assert(f[6] == 8);
	assert(f[7] == 9);
	assert(f[8] == 0);
	assert(f[9] == 1);

	f.resize(3, false);
	assert (3 == f.size());
	assert (0 == f.used());
	assert (f.isEmpty());
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
