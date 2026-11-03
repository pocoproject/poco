//
// ChecksumTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ChecksumTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Checksum.h"
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
#include <sys/mman.h>
#endif
#include <string>


using Poco::Checksum;


ChecksumTest::ChecksumTest(const std::string& name): CppUnit::TestCase(name)
{
}


ChecksumTest::~ChecksumTest() = default;


void ChecksumTest::testCRC32()
{
	const std::string data("123456789");

	Checksum crc;
	crc.update(data.data(), data.size());
	assertTrue (crc.checksum() == 0xCBF43926);
	// zlib returns the initial value for a null buffer; an empty update must not reset the checksum
	crc.update(nullptr, 0);
	assertTrue (crc.checksum() == 0xCBF43926);

	Checksum crcString(Checksum::TYPE_CRC32);
	crcString.update(data);
	assertTrue (crcString.checksum() == 0xCBF43926);

	Checksum crcChars(Checksum::TYPE_CRC32);
	for (const char c: data)
		crcChars.update(c);
	assertTrue (crcChars.checksum() == 0xCBF43926);
}


void ChecksumTest::testAdler32()
{
	const std::string data("Wikipedia");

	Checksum adler(Checksum::TYPE_ADLER32);
	adler.update(data.data(), data.size());
	assertTrue (adler.checksum() == 0x11E60398);
	adler.update(nullptr, 0);
	assertTrue (adler.checksum() == 0x11E60398);

	Checksum adlerString(Checksum::TYPE_ADLER32);
	adlerString.update(data);
	assertTrue (adlerString.checksum() == 0x11E60398);

	Checksum adlerChars(Checksum::TYPE_ADLER32);
	for (const char c: data)
		adlerChars.update(c);
	assertTrue (adlerChars.checksum() == 0x11E60398);
}


void ChecksumTest::testLargeInput()
{
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
	// more than 4 GB of zero bytes in a single update() call; on Linux, reads
	// of a read-only anonymous mapping go to the shared zero page and use no memory
	constexpr std::size_t size = (static_cast<std::size_t>(1) << 32) + 100;
	void* p = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	assertTrue (p != MAP_FAILED);

	Checksum crc(Checksum::TYPE_CRC32);
	crc.update(static_cast<const char*>(p), size);
	Checksum adler(Checksum::TYPE_ADLER32);
	adler.update(static_cast<const char*>(p), size);
	::munmap(p, size);
	assertTrue (crc.checksum() == 0xA92A4CE5);
	assertTrue (adler.checksum() == 0x01450001);
#endif
}


void ChecksumTest::setUp()
{
}


void ChecksumTest::tearDown()
{
}


CppUnit::Test* ChecksumTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ChecksumTest");

	CppUnit_addTest(pSuite, ChecksumTest, testCRC32);
	CppUnit_addTest(pSuite, ChecksumTest, testAdler32);
	CppUnit_addLongTest(pSuite, ChecksumTest, testLargeInput);

	return pSuite;
}
