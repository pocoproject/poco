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
#include "Poco/Types.h"
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

	std::string incompressible(std::size_t size)
	{
		std::string data;
		Poco::UInt32 state = 1;
		while (data.size() < size)
		{
			state = state*1103515245 + 12345;
			data += static_cast<char>((state >> 16) & 0xFF);
		}
		return data;
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
	data.clear();
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


void ZLibTest::testInflateResetAfterPartialRead()
{
	const std::string random = incompressible(50000); // larger than the input buffer
	struct
	{
		InflatingStreamBuf::StreamType inflateType;
		DeflatingStreamBuf::StreamType deflateType;
	} types[] =
	{
		{InflatingStreamBuf::STREAM_ZLIB, DeflatingStreamBuf::STREAM_ZLIB},
		{InflatingStreamBuf::STREAM_GZIP, DeflatingStreamBuf::STREAM_GZIP}
	};
	struct
	{
		std::string first;
		std::size_t read;
	} cases[] =
	{
		{std::string(40000, 'x'), 1},
		{std::string(40000, 'x'), 40000 - 500},
		{std::string(40000, 'x'), 40000 - 10}, // zlib has reached the end of the stream
		{"first-a first-b\n", 7},              // zlib has reached the end of the stream
		{random, 1}
	};
	for (const auto& t: types)
	{
		for (const auto& c: cases)
		{
			std::stringstream buffer(compress(c.first, t.deflateType) + compress(random, t.deflateType));
			InflatingInputStream inflater(buffer, t.inflateType);
			std::string data(c.read, '\0');
			inflater.read(&data[0], static_cast<std::streamsize>(data.size()));
			assertTrue (data == c.first.substr(0, c.read));

			inflater.reset();
			data.clear();
			StreamCopier::copyToString(inflater, data);
			assertTrue (data == random);
		}
	}
}


void ZLibTest::testInflateResetUnreadData()
{
	std::stringstream buffer(
		compress("first-a first-b\n", DeflatingStreamBuf::STREAM_ZLIB) +
		compress("second\n", DeflatingStreamBuf::STREAM_ZLIB));
	InflatingInputStream inflater(buffer);
	std::string data;
	inflater >> data;
	assertTrue (data == "first-a");

	inflater.reset();
	data.clear();
	inflater >> data;
	assertTrue (data == "second");
	data.clear();
	inflater >> data;
	assertTrue (data.empty());
	assertTrue (inflater.eof());
}


void ZLibTest::testInflateResetBeforeRead()
{
	// a stream of which nothing has been read is not skipped
	std::stringstream buffer(
		compress("first\n", DeflatingStreamBuf::STREAM_ZLIB) +
		compress("second\n", DeflatingStreamBuf::STREAM_ZLIB));
	InflatingInputStream inflater(buffer);
	inflater.reset();
	std::string data;
	inflater >> data;
	assertTrue (data == "first");

	inflater.reset();
	inflater.reset();
	data.clear();
	inflater >> data;
	assertTrue (data == "second");
}


void ZLibTest::testInflateResetDamagedRest()
{
	const std::string valid = compress("second\n", DeflatingStreamBuf::STREAM_ZLIB);

	// the checksum in the trailer is damaged: the next stream follows
	std::string first = compress(std::string(40000, 'x'), DeflatingStreamBuf::STREAM_ZLIB);
	first[first.size() - 1] ^= 0x01;
	std::stringstream buffer(first + valid);
	InflatingInputStream inflater(buffer);
	char c = 0;
	inflater.get(c);
	assertTrue (c == 'x');
	inflater.reset();
	std::string data;
	inflater >> data;
	assertTrue (data == "second");

	// the stream is truncated: the next stream is read from the underlying stream
	std::stringstream truncated;
	truncated << first.substr(0, first.size() - 10);
	InflatingInputStream inflater2(truncated);
	inflater2.get(c);
	assertTrue (c == 'x');
	inflater2.reset();
	truncated.clear();
	truncated << valid;
	data.clear();
	inflater2 >> data;
	assertTrue (data == "second");

	// a read has reached the end of the truncated stream before reset()
	std::stringstream truncated3;
	truncated3 << first.substr(0, first.size() - 10);
	InflatingInputStream inflater3(truncated3);
	assertTrue (readFails(inflater3) || inflater3.eof());
	truncated3.clear();
	truncated3 << valid;
	inflater3.reset();
	data.clear();
	inflater3 >> data;
	assertTrue (data == "second");
}


void ZLibTest::testInflateOutputReset()
{
	const std::string random = incompressible(5000);
	const std::string second = "abcdefabcdefabcdefabcdefabcdefabcdef\n";
	for (const auto& first: {std::string("short first text\n"), random})
	{
		for (bool flush: {false, true})
		{
			std::stringstream buffer;
			InflatingOutputStream inflater(buffer);
			inflater << compress(first, DeflatingStreamBuf::STREAM_ZLIB);
			if (flush) inflater.flush();
			inflater.rdbuf()->reset();
			assertTrue (buffer.str() == first);
			inflater << compress(second, DeflatingStreamBuf::STREAM_ZLIB);
			inflater.flush();
			assertTrue (inflater.good());
			assertTrue (buffer.str() == first + second);
		}
	}

	std::stringstream buffer;
	InflatingOutputStream inflater(buffer);
	inflater << compress("short first text\n", DeflatingStreamBuf::STREAM_ZLIB);
	inflater.rdbuf()->reset();
	inflater.close();
	assertTrue (buffer.str() == "short first text\n");
}


void ZLibTest::testInflateOutputResetAfterError()
{
	const std::string second = "abcdefabcdefabcdefabcdefabcdefabcdef\n";
	std::stringstream buffer;
	InflatingOutputStream inflater(buffer);
	inflater << "not a zlib stream";
	inflater.flush();
	assertTrue (inflater.bad());

	inflater.clear();
	inflater.rdbuf()->reset();
	inflater << compress(second, DeflatingStreamBuf::STREAM_ZLIB);
	inflater.flush();
	assertTrue (inflater.good());
	assertTrue (buffer.str() == second);

	// the error occurs when reset() inflates the pending data
	inflater.rdbuf()->reset();
	inflater << "not a zlib stream";
	try
	{
		inflater.rdbuf()->reset();
		fail("must throw");
	}
	catch (Poco::IOException&)
	{
	}
	inflater.rdbuf()->reset();
	inflater << compress(second, DeflatingStreamBuf::STREAM_ZLIB);
	inflater.flush();
	assertTrue (inflater.good());
	assertTrue (buffer.str() == second + second);
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
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetAfterPartialRead);
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetUnreadData);
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetBeforeRead);
	CppUnit_addTest(pSuite, ZLibTest, testInflateResetDamagedRest);
	CppUnit_addTest(pSuite, ZLibTest, testInflateOutputReset);
	CppUnit_addTest(pSuite, ZLibTest, testInflateOutputResetAfterError);

	return pSuite;
}
