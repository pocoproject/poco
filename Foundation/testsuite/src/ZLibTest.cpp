//
// ZLibTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ZLibTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/MemoryStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include <sstream>

using Poco::InflatingInputStream;
using Poco::InflatingOutputStream;
using Poco::DeflatingOutputStream;
using Poco::DeflatingInputStream;
using Poco::InflatingStreamBuf;
using Poco::DeflatingStreamBuf;
using Poco::StreamCopier;


namespace
{
	std::string compress(const std::string& data, DeflatingStreamBuf::StreamType type)
	{
		std::stringstream buffer;
		DeflatingOutputStream deflater(buffer, type);
		deflater << data;
		deflater.close();
		return buffer.str();
	}

	bool readFails(std::istream& istr)
	{
		std::string data;
		try
		{
			istr >> data;
			return istr.bad();
		}
		catch (Poco::IOException&)
		{
			// libc++ lets the exception escape from the istream sentry instead of setting badbit
			return true;
		}
	}
}


ZLibTest::ZLibTest(const std::string& name): CppUnit::TestCase(name)
{
}


ZLibTest::~ZLibTest()
{
}


void ZLibTest::testDeflate1()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	InflatingInputStream inflater(buffer);
	std::string data;
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testDeflate2()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	std::stringstream buffer2;
	InflatingOutputStream inflater(buffer2);
	StreamCopier::copyStream(buffer, inflater);
	inflater.close();
	std::string data;
	buffer2 >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	buffer2 >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testDeflate3()
{
	std::stringstream buffer;
	buffer << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	buffer << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	DeflatingInputStream deflater(buffer);
	std::stringstream buffer2;
	StreamCopier::copyStream(deflater, buffer2);
	std::stringstream buffer3;
	InflatingOutputStream inflater(buffer3);
	StreamCopier::copyStream(buffer2, inflater);
	inflater.close();
	std::string data;
	buffer3 >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	buffer3 >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testDeflate4()
{
	Poco::Buffer<char> buffer(1024);
	Poco::MemoryOutputStream ostr(buffer.begin(), static_cast<std::streamsize>(buffer.size()));
	DeflatingOutputStream deflater(ostr, -10, DeflatingStreamBuf::BEST_SPEED);
	std::string data(36828, 'x');
	deflater << data;
	deflater.close();
	Poco::MemoryInputStream istr(buffer.begin(), ostr.charsWritten());
	InflatingInputStream inflater(istr, -10);
	std::string data2;
	inflater >> data2;
	assertTrue (data2 == data);
}


void ZLibTest::testGzip1()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer, DeflatingStreamBuf::STREAM_GZIP);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	InflatingInputStream inflater(buffer, InflatingStreamBuf::STREAM_GZIP);
	std::string data;
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testGzip2()
{
	// created with gzip ("Hello, world!"):
	const unsigned char gzdata[] =
	{
		0x1f, 0x8b, 0x08, 0x08, 0xb0, 0x73, 0xd0, 0x41, 0x00, 0x03, 0x68, 0x77, 0x00, 0xf3, 0x48, 0xcd,
		0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f, 0xca, 0x49, 0x51, 0xe4, 0x02, 0x00, 0x18, 0xa7, 0x55,
		0x7b, 0x0e, 0x00, 0x00, 0x00, 0x00
	};

	std::string gzstr((char*) gzdata, sizeof(gzdata));
	std::istringstream istr(gzstr);
	InflatingInputStream inflater(istr, InflatingStreamBuf::STREAM_GZIP);
	std::string data;
	inflater >> data;
	assertTrue (data == "Hello,");
	inflater >> data;
	assertTrue (data == "world!");
}


void ZLibTest::testGzip3()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater1(buffer, DeflatingStreamBuf::STREAM_GZIP);
	deflater1 << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater1 << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater1.close();
	DeflatingOutputStream deflater2(buffer, DeflatingStreamBuf::STREAM_GZIP);
	deflater2 << "bcdefabcdefabcdefabcdefabcdefabcdefa" << std::endl;
	deflater2 << "bcdefabcdefabcdefabcdefabcdefabcdefa" << std::endl;
	deflater2.close();
	InflatingInputStream inflater(buffer, InflatingStreamBuf::STREAM_GZIP);
	std::string data;
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	inflater >> data;
	assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	data.clear();
	inflater >> data;
	assertTrue (data.empty());
	assertTrue (inflater.eof());
	inflater.reset();
	inflater >> data;
	assertTrue (data == "bcdefabcdefabcdefabcdefabcdefabcdefa");
	inflater >> data;
	assertTrue (data == "bcdefabcdefabcdefabcdefabcdefabcdefa");
}


void ZLibTest::testInflateResetAfterError()
{
	// a header followed by an invalid deflate block
	struct
	{
		InflatingStreamBuf::StreamType inflateType;
		DeflatingStreamBuf::StreamType deflateType;
		std::string corrupt;
	} cases[] =
	{
		{InflatingStreamBuf::STREAM_ZLIB, DeflatingStreamBuf::STREAM_ZLIB, std::string("\x78\x9c\xff\xff\xff\xff\xff\xff\xff\xff", 10)},
		{InflatingStreamBuf::STREAM_GZIP, DeflatingStreamBuf::STREAM_GZIP, std::string("\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x03\xff\xff\xff\xff\xff\xff\xff\xff", 18)}
	};
	for (const auto& c: cases)
	{
		std::stringstream buffer;
		buffer << c.corrupt;
		InflatingInputStream inflater(buffer, c.inflateType);
		assertTrue (readFails(inflater));

		// the underlying stream is now positioned at the start of a valid stream
		buffer.clear();
		buffer << compress("abcdefabcdefabcdefabcdefabcdefabcdef\n", c.deflateType);

		// a retried read fails again without consuming the next stream
		inflater.clear();
		assertTrue (readFails(inflater));
		assertTrue (buffer.tellg() == static_cast<std::streamoff>(c.corrupt.size()));

		inflater.reset();
		std::string data;
		inflater >> data;
		assertTrue (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	}
}


void ZLibTest::testInflateResetAfterTrailerError()
{
	// two concatenated streams, the first one with a damaged checksum or length in its trailer
	struct
	{
		InflatingStreamBuf::StreamType inflateType;
		DeflatingStreamBuf::StreamType deflateType;
		std::size_t damaged; // offset of the damaged byte from the end of the first stream
	} cases[] =
	{
		{InflatingStreamBuf::STREAM_ZLIB, DeflatingStreamBuf::STREAM_ZLIB, 1}, // Adler-32
		{InflatingStreamBuf::STREAM_GZIP, DeflatingStreamBuf::STREAM_GZIP, 8}, // CRC-32
		{InflatingStreamBuf::STREAM_GZIP, DeflatingStreamBuf::STREAM_GZIP, 1}  // ISIZE
	};
	for (const auto& c: cases)
	{
		std::string first = compress("first\n", c.deflateType);
		first[first.size() - c.damaged] ^= 0x01;
		std::stringstream buffer(first + compress("second\n", c.deflateType));
		InflatingInputStream inflater(buffer, c.inflateType);
		assertTrue (readFails(inflater));

		inflater.reset();
		std::string data;
		inflater >> data;
		assertTrue (data == "second");
		data.clear();
		inflater >> data;
		assertTrue (data.empty());
		assertTrue (inflater.eof());
	}

	// a gzip stream with a damaged CRC-32 whose length field straddles the end of the
	// first 32768-byte read from the underlying stream, inflated with header auto-detection
	const std::size_t size = 32768 + 2; // the last 2 bytes of the length field are not in the first read
	std::string first = compress("first\n", DeflatingStreamBuf::STREAM_GZIP);
	const std::size_t extraLength = size - first.size() - 2; // XLEN takes 2 bytes
	first[3] |= 0x04; // FEXTRA
	first.insert(10, std::string(1, static_cast<char>(extraLength & 0xFF)) + static_cast<char>(extraLength >> 8) + std::string(extraLength, '\0'));
	assertTrue (first.size() == size);
	first[first.size() - 8] ^= 0x01;
	std::stringstream buffer(first + compress("second\n", DeflatingStreamBuf::STREAM_GZIP));
	InflatingInputStream inflater(buffer, 15 + 32);
	assertTrue (readFails(inflater));

	inflater.reset();
	std::string data;
	inflater >> data;
	assertTrue (data == "second");
}


void ZLibTest::setUp()
{
}


void ZLibTest::tearDown()
{
}


CppUnit::Test* ZLibTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ZLibTest");

	CppUnit_addTest(pSuite, ZLibTest, testDeflate1);
	CppUnit_addTest(pSuite, ZLibTest, testDeflate2);
	CppUnit_addTest(pSuite, ZLibTest, testDeflate3);
	CppUnit_addTest(pSuite, ZLibTest, testDeflate4);
	CppUnit_addTest(pSuite, ZLibTest, testGzip1);
	CppUnit_addTest(pSuite, ZLibTest, testGzip2);
	CppUnit_addTest(pSuite, ZLibTest, testGzip3);
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetAfterError);
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetAfterTrailerError);

	return pSuite;
}
