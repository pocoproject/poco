//
// StreamCopierTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StreamCopierTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::StreamCopier;


StreamCopierTest::StreamCopierTest(const std::string& name): CppUnit::TestCase(name)
{
}


StreamCopierTest::~StreamCopierTest()
{
}


void StreamCopierTest::testBufferedCopy()
{
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streamsize n = StreamCopier::copyStream(istr, ostr);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streamsize n = StreamCopier::copyStream(istr, ostr, 100);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streamsize n = StreamCopier::copyStream(istr, ostr, 128);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streamsize n = StreamCopier::copyStream(istr, ostr, 512);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
}


void StreamCopierTest::testUnbufferedCopy()
{
	std::string src;
	for (int i = 0; i < 255; ++i) src += char(i);
	std::istringstream istr(src);
	std::ostringstream ostr;
	std::streamsize n = StreamCopier::copyStreamUnbuffered(istr, ostr);
	assertTrue (ostr.str() == src);
	assertTrue (n == src.size());
}


void StreamCopierTest::testCopyToString()
{
	std::string src;
	for (int i = 0; i < 512; ++i) src += char(i % 256);
	std::istringstream istr(src);
	std::string dest;
	std::streamsize n = StreamCopier::copyToString(istr, dest, 100);
	assertTrue (src == dest);
	assertTrue (n == src.size());
}


void StreamCopierTest::testBufferedCopyRange()
{
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 20;
		std::string subSrc = src.substr(100, 20);
		std::streamsize n = StreamCopier::copyStreamRange(istr, ostr, rangeStart, rangeLength);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 20;
		std::string subSrc = src.substr(100, 20);
		std::streamsize n = StreamCopier::copyStreamRange(istr, ostr, rangeStart, rangeLength, 100);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 150;
		std::string subSrc = src.substr(100, 150);
		std::streamsize n = StreamCopier::copyStreamRange(istr, ostr, rangeStart, rangeLength, 100);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 128;
		std::string subSrc = src.substr(100, 128);
		std::streamsize n = StreamCopier::copyStreamRange(istr, ostr, rangeStart, rangeLength, 128);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
}


void StreamCopierTest::testUnbufferedCopyRange()
{
	std::string src;
	for (int i = 0; i < 255; ++i) src += char(i);
	std::istringstream istr(src);
	std::ostringstream ostr;
	std::streampos rangeStart = 100;
	std::streamsize rangeLength = 20;
	std::string subSrc = src.substr(100, 20);
	std::streamsize n = StreamCopier::copyStreamRangeUnbuffered(istr, ostr, rangeStart, rangeLength);
	assertTrue (ostr.str() == subSrc);
	assertTrue (n == subSrc.size());
}


#if defined(POCO_HAVE_INT64)
void StreamCopierTest::testBufferedCopy64()
{
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::istringstream istr(src);
		std::ostringstream ostr;
		Poco::UInt64 n = StreamCopier::copyStream64(istr, ostr);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		Poco::UInt64 n = StreamCopier::copyStream64(istr, ostr, 100);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		Poco::UInt64 n = StreamCopier::copyStream64(istr, ostr, 128);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		Poco::UInt64 n = StreamCopier::copyStream64(istr, ostr, 512);
		assertTrue (ostr.str() == src);
		assertTrue (n == src.size());
	}
}


void StreamCopierTest::testUnbufferedCopy64()
{
	std::string src;
	for (int i = 0; i < 255; ++i) src += char(i);
	std::istringstream istr(src);
	std::ostringstream ostr;
	Poco::UInt64 n = StreamCopier::copyStreamUnbuffered64(istr, ostr);
	assertTrue (ostr.str() == src);
	assertTrue (n == src.size());
}


void StreamCopierTest::testCopyToString64()
{
	std::string src;
	for (int i = 0; i < 512; ++i) src += char(i % 256);
	std::istringstream istr(src);
	std::string dest;
	Poco::UInt64 n = StreamCopier::copyToString64(istr, dest, 100);
	assertTrue (src == dest);
	assertTrue (n == src.size());
}


void StreamCopierTest::testBufferedCopyRange64()
{
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 20;
		std::string subSrc = src.substr(100, 20);
		Poco::UInt64 n = StreamCopier::copyStreamRange64(istr, ostr, rangeStart, rangeLength);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 20;
		std::string subSrc = src.substr(100, 20);
		Poco::UInt64 n = StreamCopier::copyStreamRange64(istr, ostr, rangeStart, rangeLength, 100);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 150;
		std::string subSrc = src.substr(100, 150);
		Poco::UInt64 n = StreamCopier::copyStreamRange64(istr, ostr, rangeStart, rangeLength, 100);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
	{
		std::string src;
		for (int i = 0; i < 512; ++i) src += char(i % 256);
		std::istringstream istr(src);
		std::ostringstream ostr;
		std::streampos rangeStart = 100;
		std::streamsize rangeLength = 128;
		std::string subSrc = src.substr(100, 128);
		Poco::UInt64 n = StreamCopier::copyStreamRange64(istr, ostr, rangeStart, rangeLength, 128);
		assertTrue (ostr.str() == subSrc);
		assertTrue (n == subSrc.size());
	}
}


void StreamCopierTest::testUnbufferedCopyRange64()
{
	std::string src;
	for (int i = 0; i < 255; ++i) src += char(i);
	std::istringstream istr(src);
	std::ostringstream ostr;
	std::streampos rangeStart = 100;
	std::streamsize rangeLength = 20;
	std::string subSrc = src.substr(100, 20);
	Poco::UInt64 n = StreamCopier::copyStreamRangeUnbuffered64(istr, ostr, rangeStart, rangeLength);
	assertTrue (ostr.str() == subSrc);
	assertTrue (n == subSrc.size());
}
#endif


void StreamCopierTest::setUp()
{
}


void StreamCopierTest::tearDown()
{
}


CppUnit::Test* StreamCopierTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("StreamCopierTest");

	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopy);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopy);
	CppUnit_addTest(pSuite, StreamCopierTest, testCopyToString);
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyRange);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopyRange);

#if defined(POCO_HAVE_INT64)
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopy64);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopy64);
	CppUnit_addTest(pSuite, StreamCopierTest, testCopyToString64);
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyRange64);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopyRange64);
#endif

	return pSuite;
}
