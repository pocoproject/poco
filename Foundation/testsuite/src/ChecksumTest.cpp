//
// ChecksumTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ChecksumTest.h"
#include "CppUnit/CppUnitException.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Checksum.h"
#include "Poco/Exception.h"
#include "Poco/Random.h"
#include "Adler32.h"
#include "CRC.h"
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
#include <sys/mman.h>
#endif
#include <algorithm>
#include <span>
#include <string>
#include <type_traits>
#include <vector>


using Poco::Adler32;
using Poco::Checksum;
using Poco::CRC;
using Poco::UInt32;
using Poco::UInt64;


namespace
{
	constexpr Checksum::Type CRC_TYPES[] = {Checksum::TYPE_CRC32, Checksum::TYPE_CRC64_XZ, Checksum::TYPE_CRC64_ECMA182};

	[[nodiscard]] std::string randomData(std::size_t size, Poco::UInt32 seed)
	{
		Poco::Random random;
		random.seed(seed);
		std::string data(size, '\0');
		std::ranges::generate(data, [&random] { return random.nextChar(); });
		return data;
	}

	[[nodiscard]] std::span<const unsigned char> bytesOf(const std::string& data)
	{
		return {reinterpret_cast<const unsigned char*>(data.data()), data.size()};
	}

	[[nodiscard]] UInt64 checksumValue(const Checksum& checksum)
	{
		return checksum.type() == Checksum::TYPE_CRC32 || checksum.type() == Checksum::TYPE_ADLER32 ?
			checksum.checksum() : checksum.checksum64();
	}

	void require(bool condition, const std::string& message)
	{
		if (!condition)
			throw CppUnit::CppUnitException("fail: " + message);
	}

	void checkKnownAnswer(Checksum::Type type, const std::string& input, UInt64 expected)
		// Requires the checksum of input to be expected for every form of update().
	{
		Checksum checksum(type);
		if (type == Checksum::TYPE_CRC64_XZ || type == Checksum::TYPE_CRC64_ECMA182)
			require(checksum.checksum64() == 0, "a new CRC-64 checksum reads 0");
		checksum.update(input.data(), input.size());
		require(checksumValue(checksum) == expected, "update with a pointer and a length");
		checksum.update(nullptr, 0);
		require(checksumValue(checksum) == expected, "an empty update with a null pointer keeps the checksum");

		Checksum fromString(type);
		fromString.update(input);
		require(checksumValue(fromString) == expected, "update with a string");

		Checksum byBytes(type);
		for (const char c: input)
			byBytes.update(c);
		require(checksumValue(byBytes) == expected, "update byte by byte");
	}

	template <typename Exception, typename Function>
	void requireThrows(Function function, const std::string& message)
		// Requires function to throw an Exception.
	{
		static_assert(std::is_invocable_v<Function>, "function must be callable without arguments");
		try
		{
			function();
		}
		catch (const Exception&)
		{
			return;
		}
		throw CppUnit::CppUnitException("fail: " + message);
	}
}


ChecksumTest::ChecksumTest(const std::string& name): CppUnit::TestCase(name)
{
}


ChecksumTest::~ChecksumTest() = default;


void ChecksumTest::testCRC32()
{
	checkKnownAnswer(Checksum::TYPE_CRC32, "123456789", 0xCBF43926);

	Checksum byDefault;
	assertTrue (byDefault.type() == Checksum::TYPE_CRC32);
	byDefault.update("123456789");
	assertTrue (byDefault.checksum() == 0xCBF43926);
}


void ChecksumTest::testAdler32()
{
	checkKnownAnswer(Checksum::TYPE_ADLER32, "Wikipedia", 0x11E60398);
}


void ChecksumTest::testCRC64XZ()
{
	checkKnownAnswer(Checksum::TYPE_CRC64_XZ, "123456789", 0x995DC9BBDF1939FAull);
}


void ChecksumTest::testCRC64ECMA182()
{
	checkKnownAnswer(Checksum::TYPE_CRC64_ECMA182, "123456789", 0x6C40DF5F0B497347ull);
}


void ChecksumTest::testChecksumWidth()
{
	using Poco::InvalidAccessException;
	requireThrows<InvalidAccessException>([] { return Checksum(Checksum::TYPE_CRC64_XZ).checksum(); }, "CRC-64/XZ has no 32-bit checksum");
	requireThrows<InvalidAccessException>([] { return Checksum(Checksum::TYPE_CRC64_ECMA182).checksum(); }, "CRC-64/ECMA-182 has no 32-bit checksum");
	requireThrows<InvalidAccessException>([] { return Checksum(Checksum::TYPE_CRC32).checksum64(); }, "CRC-32 has no 64-bit checksum");
	requireThrows<InvalidAccessException>([] { return Checksum(Checksum::TYPE_ADLER32).checksum64(); }, "Adler-32 has no 64-bit checksum");
}


void ChecksumTest::testKnownAnswers()
{
	// long enough for the folding implementations, with a tail that is not a multiple of 16 bytes
	std::string data(1000, '\0');
	for (std::size_t i = 0; i < data.size(); ++i)
		data[i] = static_cast<char>((i * 31 + 7) & 0xFF);

	Checksum crc32(Checksum::TYPE_CRC32);
	crc32.update(data);
	assertTrue (crc32.checksum() == 0x8902161E);

	Checksum xz(Checksum::TYPE_CRC64_XZ);
	xz.update(data);
	assertTrue (xz.checksum64() == 0x5E9723037B38C574ull);

	Checksum ecma(Checksum::TYPE_CRC64_ECMA182);
	ecma.update(data);
	assertTrue (ecma.checksum64() == 0x014E7C83D104BC0Bull);
}


void ChecksumTest::testIncrementalUpdate()
{
	const std::string data = randomData(100000, 1);
	constexpr Checksum::Type types[] = {Checksum::TYPE_ADLER32, Checksum::TYPE_CRC32, Checksum::TYPE_CRC64_XZ, Checksum::TYPE_CRC64_ECMA182};
	for (const auto type: types)
	{
		Checksum whole(type);
		whole.update(data);

		Checksum pieces(type);
		std::size_t offset = 0;
		std::size_t piece = 1;
		while (offset < data.size())
		{
			const std::size_t length = std::min(piece, data.size() - offset);
			pieces.update(data.data() + offset, length);
			offset += length;
			piece = piece * 3 + 1;
			if (piece > 5000)
				piece = 7;
		}
		assertTrue (checksumValue(pieces) == checksumValue(whole));
	}
}


void ChecksumTest::testImplementationsAgree()
{
	constexpr std::size_t MAX_LENGTH = 1024;
	constexpr std::size_t MAX_OFFSET = 16;
	constexpr std::size_t LARGE_LENGTH = 3 * 1024 * 1024 + 13;
	// the three-stream CRC-32 of ARMv8 takes three streams of 1024 bytes per pass;
	// lengths around one and two passes, at an odd address
	constexpr std::size_t ONE_PASS = 3 * 1024;
	constexpr std::size_t TWO_PASSES = 2 * ONE_PASS;
	constexpr std::size_t PASS_MARGIN = 40;
	const std::string data = randomData(MAX_LENGTH + MAX_OFFSET, 2);
	const std::string large = randomData(LARGE_LENGTH, 3);
	const auto bytes = bytesOf(data);
	const auto largeBytes = bytesOf(large);

	using Poco::InvalidArgumentException;
	requireThrows<InvalidArgumentException>([] { return CRC::implementation(Checksum::TYPE_ADLER32); }, "Adler-32 has no CRC implementation");
	requireThrows<InvalidArgumentException>([] { return CRC::implementations(Checksum::TYPE_ADLER32); }, "Adler-32 has no CRC implementations");

	for (const auto type: CRC_TYPES)
	{
		const std::vector<CRC::Implementation> implementations = CRC::implementations(type);
		assertTrue (!implementations.empty());
		assertTrue (CRC::implementation(type) == implementations.back().update);

		const CRC::Update portable = implementations.front().update;
		const UInt64 mask = type == Checksum::TYPE_CRC32 ? 0xFFFFFFFFull : ~0ull;
		for (const auto& impl: implementations)
		{
			// zlib returns the initial value for a null buffer; an empty update must keep the register
			const UInt64 registerValue = 0x0123456789ABCDEFull & mask;
			if (impl.update(registerValue, {}) != registerValue)
				fail(std::string(impl.name) + " changes the register on an empty update");
			for (std::size_t offset = 0; offset < MAX_OFFSET; ++offset)
			{
				for (std::size_t length = 0; length <= MAX_LENGTH; ++length)
				{
					const UInt64 initial = (0x0123456789ABCDEFull * (length + 1)) & mask;
					const UInt64 expected = portable(initial, bytes.subspan(offset, length));
					if (impl.update(initial, bytes.subspan(offset, length)) != expected)
						fail(std::string(impl.name) + " differs at offset " + std::to_string(offset) + ", length " + std::to_string(length));
				}
			}
			if (impl.update(mask, largeBytes) != portable(mask, largeBytes))
				fail(std::string(impl.name) + " differs on large input");
			for (const std::size_t passes: {ONE_PASS, TWO_PASSES})
			{
				for (std::size_t length = passes - PASS_MARGIN; length <= passes + PASS_MARGIN; ++length)
				{
					const UInt64 initial = (0xFEDCBA9876543210ull * (length + 1)) & mask;
					const auto input = largeBytes.subspan(1, length);
					if (impl.update(initial, input) != portable(initial, input))
						fail(std::string(impl.name) + " differs at an odd address, length " + std::to_string(length));
				}
			}
		}
	}
}


void ChecksumTest::testAdler32ImplementationsAgree()
{
	constexpr std::size_t MAX_LENGTH = 512;
	constexpr std::size_t MAX_OFFSET = 32;
	constexpr UInt32 INITIAL = 1;
	// the largest register value: both sums at 65520
	constexpr UInt32 LARGEST = 0xFFF0FFF0;
	// both sums above the modulus
	constexpr UInt32 UNREDUCED = 0xFFFFFFFF;
	constexpr std::size_t MIB = static_cast<std::size_t>(1024) * 1024;
	const std::string data = randomData(MAX_LENGTH + MAX_OFFSET, 4);
	const auto bytes = bytesOf(data);
	// all bytes 0xFF give the largest sums; 5 MiB spans many reductions
	const std::string large[] = {
		randomData(MIB, 5),
		randomData(5 * MIB, 6),
		std::string(MIB, '\xFF'),
		std::string(5 * MIB, '\xFF')
	};
	const std::string check("123456789");

	const std::vector<Adler32::Implementation> implementations = Adler32::implementations();
	assertTrue (!implementations.empty());
	assertTrue (Adler32::implementation() == implementations.back().update);

	const Adler32::Update portable = implementations.front().update;
	for (const auto& impl: implementations)
	{
		const std::string name(impl.name);
		if (impl.update(INITIAL, bytesOf(check)) != 0x091E01DE)
			fail(name + " gives the wrong check value");
		if (impl.update(LARGEST, {}) != LARGEST)
			fail(name + " changes the checksum on an empty update");
		for (const UInt32 initial: {INITIAL, UNREDUCED})
		{
			for (std::size_t offset = 0; offset < MAX_OFFSET; ++offset)
			{
				for (std::size_t length = 0; length <= MAX_LENGTH; ++length)
				{
					if (impl.update(initial, bytes.subspan(offset, length)) != portable(initial, bytes.subspan(offset, length)))
						fail(name + " differs at offset " + std::to_string(offset) + ", length " + std::to_string(length) + " from " + std::to_string(initial));
				}
			}
		}
		for (const auto& input: large)
		{
			const auto largeBytes = bytesOf(input);
			const UInt32 expected = portable(INITIAL, largeBytes);
			if (impl.update(INITIAL, largeBytes) != expected)
				fail(name + " differs on " + std::to_string(input.size()) + " bytes");
			if (impl.update(LARGEST, largeBytes) != portable(LARGEST, largeBytes))
				fail(name + " differs on " + std::to_string(input.size()) + " bytes from the largest initial value");
			if (impl.update(UNREDUCED, largeBytes) != portable(UNREDUCED, largeBytes))
				fail(name + " differs on " + std::to_string(input.size()) + " bytes from an unreduced initial value");

			// uneven pieces, so that the vector steps start at varying alignments
			UInt32 pieces = INITIAL;
			std::size_t offset = 0;
			std::size_t piece = 1;
			while (offset < input.size())
			{
				const std::size_t length = std::min(piece, input.size() - offset);
				pieces = impl.update(pieces, largeBytes.subspan(offset, length));
				offset += length;
				piece = piece * 5 + 3;
				if (piece > 200000)
					piece = 11;
			}
			if (pieces != expected)
				fail(name + " differs on " + std::to_string(input.size()) + " bytes in pieces");
		}
	}
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
	Checksum xz(Checksum::TYPE_CRC64_XZ);
	xz.update(static_cast<const char*>(p), size);
	// the CRC-64/ECMA-182 of zero bytes is 0; a leading byte 0x01 makes the zero bytes count
	Checksum ecma(Checksum::TYPE_CRC64_ECMA182);
	ecma.update('\x01');
	ecma.update(static_cast<const char*>(p), size);
	::munmap(p, size);
	assertTrue (crc.checksum() == 0xA92A4CE5);
	assertTrue (adler.checksum() == 0x01450001);
	assertTrue (xz.checksum64() == 0xD62553E2246A0E31ull);
	assertTrue (ecma.checksum64() == 0xDCC10D85D28DA610ull);
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
	CppUnit_addTest(pSuite, ChecksumTest, testCRC64XZ);
	CppUnit_addTest(pSuite, ChecksumTest, testCRC64ECMA182);
	CppUnit_addTest(pSuite, ChecksumTest, testChecksumWidth);
	CppUnit_addTest(pSuite, ChecksumTest, testKnownAnswers);
	CppUnit_addTest(pSuite, ChecksumTest, testIncrementalUpdate);
	CppUnit_addTest(pSuite, ChecksumTest, testImplementationsAgree);
	CppUnit_addTest(pSuite, ChecksumTest, testAdler32ImplementationsAgree);
	CppUnit_addLongTest(pSuite, ChecksumTest, testLargeInput);

	return pSuite;
}
