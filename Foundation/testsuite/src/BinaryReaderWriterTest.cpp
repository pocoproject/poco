//
// BinaryReaderWriterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BinaryReaderWriterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/BinaryWriter.h"
#include "Poco/BinaryReader.h"
#include "Poco/Buffer.h"
#include <sstream>


using Poco::BinaryWriter;
using Poco::MemoryBinaryWriter;
using Poco::BinaryReader;
using Poco::MemoryBinaryReader;
using Poco::Buffer;
using Poco::Int32;
using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;


BinaryReaderWriterTest::BinaryReaderWriterTest(const std::string& name): CppUnit::TestCase(name)
{
}


BinaryReaderWriterTest::~BinaryReaderWriterTest()
{
}


void BinaryReaderWriterTest::testNative()
{
	std::stringstream sstream;
	BinaryWriter writer(sstream);
	BinaryReader reader(sstream);
	write(writer);
	read(reader);
}


void BinaryReaderWriterTest::testBigEndian()
{
	std::stringstream sstream;
	BinaryWriter writer(sstream, BinaryWriter::BIG_ENDIAN_BYTE_ORDER);
	BinaryReader reader(sstream, BinaryReader::UNSPECIFIED_BYTE_ORDER);
	assertTrue (writer.byteOrder() == BinaryWriter::BIG_ENDIAN_BYTE_ORDER);
	writer.writeBOM();
	write(writer);
	reader.readBOM();
	assertTrue (reader.byteOrder() == BinaryReader::BIG_ENDIAN_BYTE_ORDER);
	read(reader);
}


void BinaryReaderWriterTest::testLittleEndian()
{
	std::stringstream sstream;
	BinaryWriter writer(sstream, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	BinaryReader reader(sstream, BinaryReader::UNSPECIFIED_BYTE_ORDER);
	assertTrue (writer.byteOrder() == BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeBOM();
	write(writer);
	reader.readBOM();
	assertTrue (reader.byteOrder() == BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	read(reader);
}


void BinaryReaderWriterTest::write(BinaryWriter& writer)
{
	writer << true;
	writer << false;
	writer << 'a';
	writer << (short) -100;
	writer << (unsigned short) 50000;
	writer << -123456;
	writer << (unsigned) 123456;
	writer << (long) -1234567890;
	writer << (unsigned long) 1234567890;

#if defined(POCO_HAVE_INT64)
	writer << (Int64) -1234567890;
	writer << (UInt64) 1234567890;
#endif

	writer << (float) 1.5;
	writer << (double) -1.5;

	writer << "foo";
	writer << "";

	writer << std::string("bar");
	writer << std::string();

	writer.write7BitEncoded((UInt32) 100);
	writer.write7BitEncoded((UInt32) 1000);
	writer.write7BitEncoded((UInt32) 10000);
	writer.write7BitEncoded((UInt32) 100000);
	writer.write7BitEncoded((UInt32) 1000000);

#if defined(POCO_HAVE_INT64)
	writer.write7BitEncoded((UInt64) 100);
	writer.write7BitEncoded((UInt64) 1000);
	writer.write7BitEncoded((UInt64) 10000);
	writer.write7BitEncoded((UInt64) 100000);
	writer.write7BitEncoded((UInt64) 1000000);
#endif

	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	writer << vec;

	writer.writeRaw("RAW");
}


void BinaryReaderWriterTest::read(BinaryReader& reader)
{
	bool b;
	reader >> b;
	assertTrue (b);
	reader >> b;
	assertTrue (!b);

	char c;
	reader >> c;
	assertTrue (c == 'a');

	short shortv;
	reader >> shortv;
	assertTrue (shortv == -100);

	unsigned short ushortv;
	reader >> ushortv;
	assertTrue (ushortv == 50000);

	int intv;
	reader >> intv;
	assertTrue (intv == -123456);

	unsigned uintv;
	reader >> uintv;
	assertTrue (uintv == 123456);

	long longv;
	reader >> longv;
	assertTrue (longv == -1234567890);

	unsigned long ulongv;
	reader >> ulongv;
	assertTrue (ulongv == 1234567890);

#if defined(POCO_HAVE_INT64)
	Int64 int64v;
	reader >> int64v;
	assertTrue (int64v == -1234567890);

	UInt64 uint64v;
	reader >> uint64v;
	assertTrue (uint64v == 1234567890);
#endif

	float floatv;
	reader >> floatv;
	assertTrue (floatv == 1.5);

	double doublev;
	reader >> doublev;
	assertTrue (doublev == -1.5);

	std::string str;
	reader >> str;
	assertTrue (str == "foo");
	reader >> str;
	assertTrue (str == "");
	reader >> str;
	assertTrue (str == "bar");
	reader >> str;
	assertTrue (str == "");

	UInt32 uint32v;
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 100);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 1000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 10000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 100000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 1000000);

#if defined(POCO_HAVE_INT64)
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 100);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 1000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 10000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 100000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 1000000);
#endif

	std::vector<int> vec;
	reader >> vec;
	assertTrue (vec.size() == 3);
	assertTrue (vec[0] == 1);
	assertTrue (vec[1] == 2);
	assertTrue (vec[2] == 3);

	reader.readRaw(3, str);
	assertTrue (str == "RAW");
}


void BinaryReaderWriterTest::testWrappers()
{
	bool b = false; char c = '0'; int i = 0;
	Buffer<char> buf(2 * sizeof(bool) + sizeof(char) + 2 * sizeof(int));

	MemoryBinaryWriter writer(buf);
	writer << true;
	writer << false;
	writer << 'a';
	writer << 1;
	writer << -1;

	MemoryBinaryReader reader(writer.data());
	reader >> b; assertTrue (b);
	reader >> b; assertTrue (!b);
	reader >> c; assertTrue ('a' == c);
	assertTrue (reader.available() == sizeof(i) * 2);
	reader >> i; assertTrue (1 == i);
	assertTrue (reader.available() == sizeof(i));
	reader >> i; assertTrue (-1 == i);
	assertTrue (reader.available() == 0);

	reader.setExceptions(std::istream::eofbit);
	try
	{
		reader >> i;
		fail ("must throw on EOF");
	} catch(std::exception&) { }
}


void BinaryReaderWriterTest::setUp()
{
}


void BinaryReaderWriterTest::tearDown()
{
}


CppUnit::Test* BinaryReaderWriterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BinaryReaderWriterTest");

	CppUnit_addTest(pSuite, BinaryReaderWriterTest, testNative);
	CppUnit_addTest(pSuite, BinaryReaderWriterTest, testBigEndian);
	CppUnit_addTest(pSuite, BinaryReaderWriterTest, testLittleEndian);
	CppUnit_addTest(pSuite, BinaryReaderWriterTest, testWrappers);

	return pSuite;
}
