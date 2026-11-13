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
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::StreamCopier;


namespace
{
	class NonSeekableStringBuf: public std::stringbuf
	{
	public:
		explicit NonSeekableStringBuf(const std::string& str): std::stringbuf(str)
		{
		}

	protected:
		pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override
		{
			return pos_type(off_type(-1));
		}

		pos_type seekpos(pos_type, std::ios_base::openmode) override
		{
			return pos_type(off_type(-1));
		}
	};
}


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


void StreamCopierTest::testBufferedCopyN()
{
	const std::string src("0123456789abcdefghijklmnopqrstuvwxyz");
	NonSeekableStringBuf streamBuf(src);
	std::istream istr(&streamBuf);
	char prefix[10];
	istr.read(prefix, sizeof(prefix));
	std::ostringstream ostr;
	std::streamsize n = StreamCopier::copyStreamN(istr, ostr, 20, 7);
	assertTrue (ostr.str() == src.substr(10, 20));
	assertTrue (n == 20);

	std::ostringstream truncated;
	n = StreamCopier::copyStreamN(istr, truncated, 20, 7);
	assertTrue (truncated.str() == src.substr(30));
	assertTrue (n == 6);

	std::istringstream zeroInput(src);
	std::ostringstream zeroOutput;
	n = StreamCopier::copyStreamN(zeroInput, zeroOutput, 0);
	assertTrue (zeroOutput.str().empty());
	assertTrue (n == 0);
	assertTrue (zeroInput.peek() == src[0]);

	try
	{
		n = StreamCopier::copyStreamN(zeroInput, zeroOutput, -1);
		fail("negative count must fail");
	}
	catch (const Poco::AssertionViolationException&)
	{
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


void StreamCopierTest::testBufferedCopyN64()
{
	const std::string src("0123456789abcdefghijklmnopqrstuvwxyz");
	NonSeekableStringBuf streamBuf(src);
	std::istream istr(&streamBuf);
	std::ostringstream ostr;
	Poco::UInt64 n = StreamCopier::copyStreamN64(istr, ostr, 13, 5);
	assertTrue (ostr.str() == src.substr(0, 13));
	assertTrue (n == 13);
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
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyN);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopy);
	CppUnit_addTest(pSuite, StreamCopierTest, testCopyToString);
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyRange);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopyRange);

#if defined(POCO_HAVE_INT64)
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopy64);
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyN64);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopy64);
	CppUnit_addTest(pSuite, StreamCopierTest, testCopyToString64);
	CppUnit_addTest(pSuite, StreamCopierTest, testBufferedCopyRange64);
	CppUnit_addTest(pSuite, StreamCopierTest, testUnbufferedCopyRange64);
#endif

	return pSuite;
}
