//
// FIFOBufferStreamTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FIFOBufferStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FIFOBuffer.h"
#include "Poco/FIFOBufferStream.h"
#include "Poco/Delegate.h"


using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;
using Poco::delegate;


FIFOBufferStreamTest::FIFOBufferStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


FIFOBufferStreamTest::~FIFOBufferStreamTest()
{
}


void FIFOBufferStreamTest::testInput()
{
	const char* data = "This is a test";
	FIFOBuffer fb1(data, 14);
	FIFOBufferStream str1(fb1);
	assertTrue (str1.rdbuf()->fifoBuffer().isFull());

	int c = str1.get();
	assertTrue (c == 'T');
	c = str1.get();
	assertTrue (c == 'h');

	std::string str;
	str1 >> str;
	assertTrue (str == "is");

	char buffer[32];
	str1.read(buffer, sizeof(buffer));
	assertTrue (str1.gcount() == 10);
	buffer[str1.gcount()] = 0;
	assertTrue (std::string(" is a test") == buffer);

	const char* data2 = "123";
	FIFOBufferStream str2(data2, 3);

	c = str2.get();
	assertTrue (c == '1');
	assertTrue (str2.good());
	c = str2.get();
	assertTrue (c == '2');
	str2.unget();
	c = str2.get();
	assertTrue (c == '2');
	assertTrue (str2.good());
	c = str2.get();
	assertTrue (c == '3');
	assertTrue (str2.good());
	c = str2.get();
	assertTrue (c == -1);
	assertTrue (str2.eof());
}


void FIFOBufferStreamTest::testOutput()
{
	char output[64];
	FIFOBufferStream iostr1(output, 64);
	iostr1 << "This is a test " << 42 << std::ends << std::flush;
	assertTrue (std::string("This is a test 42") == output);
}


void FIFOBufferStreamTest::testNotify()
{
	FIFOBuffer fb(18);
	FIFOBufferStream iostr(fb);
	assertTrue (iostr.rdbuf()->fifoBuffer().isEmpty());

	assertTrue (0 == _readableToNot);
	assertTrue (0 == _notToReadable);
	assertTrue (0 == _writableToNot);
	assertTrue (0 == _notToWritable);

	iostr.readable += delegate(this, &FIFOBufferStreamTest::onReadable);
	iostr.writable += delegate(this, &FIFOBufferStreamTest::onWritable);

	iostr << "This is a test " << 42 << std::ends << std::flush;
	assertTrue (iostr.rdbuf()->fifoBuffer().isFull());

	assertTrue (0 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (0 == _notToWritable);

	char input[64];
	iostr >> input;
	assertTrue (std::string("This") == input);
	assertTrue (iostr.rdbuf()->fifoBuffer().isEmpty());

	assertTrue (1 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr >> input;
	assertTrue (std::string("is") == input);

	assertTrue (1 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr >> input;
	assertTrue (std::string("a") == input);

	assertTrue (1 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr >> input;
	assertTrue (std::string("test") == input);

	assertTrue (1 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr >> input;
	assertTrue (std::string("42") == input);

	assertTrue (1 == _readableToNot);
	assertTrue (1 == _notToReadable);
	assertTrue (1 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr.clear();
	assertTrue (iostr.good());
	iostr << "This is a test " << 42 << std::ends << std::flush;
	assertTrue (iostr.rdbuf()->fifoBuffer().isFull());

	assertTrue (1 == _readableToNot);
	assertTrue (2 == _notToReadable);
	assertTrue (2 == _writableToNot);
	assertTrue (1 == _notToWritable);

	iostr.readable -= delegate(this, &FIFOBufferStreamTest::onReadable);
	iostr.writable -= delegate(this, &FIFOBufferStreamTest::onWritable);
}


void FIFOBufferStreamTest::onReadable(bool& b)
{
	if (b) ++_notToReadable;
	else ++_readableToNot;
};


void FIFOBufferStreamTest::onWritable(bool& b)
{
	if (b) ++_notToWritable;
	else ++_writableToNot;
}


void FIFOBufferStreamTest::setUp()
{
	_readableToNot = 0;
	_notToReadable = 0;
	_writableToNot = 0;
	_notToWritable = 0;
}


void FIFOBufferStreamTest::tearDown()
{
}


CppUnit::Test* FIFOBufferStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FIFOBufferStreamTest");

	CppUnit_addTest(pSuite, FIFOBufferStreamTest, testInput);
	CppUnit_addTest(pSuite, FIFOBufferStreamTest, testOutput);
	CppUnit_addTest(pSuite, FIFOBufferStreamTest, testNotify);

	return pSuite;
}
