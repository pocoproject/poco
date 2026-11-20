//
// Adler32.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// The portable Adler-32 implementation, the update of the checksum from the
// sums of the vector implementations and the selection of an implementation.
//
// Reference: RFC 1950, section 8.2: the Adler-32 algorithm.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Adler32.h"
#include <zlib.h>
#include <algorithm>
#include <limits>


namespace Poco {


namespace {


constexpr UInt32 MODULUS = 65521;


} // namespace


Adler32::Update Adler32::implementation()
{
	// one selection, made on the first use (function-local statics are initialized thread-safely)
	static const Update fastest = implementations().back().update;
	return fastest;
}


std::vector<Adler32::Implementation> Adler32::implementations()
{
	std::vector<Implementation> result;
	result.push_back({"zlib", &adler32Zlib});
	// the implementations with instruction set extensions come from Adler32_X86.cpp or Adler32_ARM.cpp
	addInstructionImplementations(result);
	return result;
}


UInt32 Adler32::updateInSteps(UInt32 adler, std::span<const unsigned char> data, std::size_t stepSize, SumSteps sumSteps)
{
	static_assert(8ull * 255 * MAX_STEPS * (MAX_STEPS - 1) / 2 <= std::numeric_limits<UInt32>::max(), "a prefix lane must not overflow");
	while (data.size() >= stepSize)
	{
		const std::size_t steps = std::min(data.size() / stepSize, MAX_STEPS);
		const std::size_t blockLength = steps * stepSize;
		const Sums sums = sumSteps(data.first(blockLength));
		// RFC 1950: A is one plus the sum of the bytes, B the sum of the values
		// of A after each byte; for a block of n bytes d[i]:
		// A' = A + sum(d[i]), B' = B + n * A + sum((n - i) * d[i]), both modulo 65521
		const UInt64 a = adler & 0xFFFF;
		const UInt64 b = adler >> 16;
		const UInt64 newA = (a + sums.bytes) % MODULUS;
		const UInt64 newB = (b + blockLength % MODULUS * a + sums.weighted) % MODULUS;
		adler = static_cast<UInt32>((newB << 16) | newA);
		data = data.subspan(blockLength);
	}
	return adler32Zlib(adler, data);
}


UInt32 Adler32::adler32Zlib(UInt32 adler, std::span<const unsigned char> data)
{
	// an empty update may come with a null buffer, for which zlib returns its
	// initial value, 1, instead of the given one
	if (data.empty()) return adler;
	return static_cast<UInt32>(::adler32_z(adler, data.data(), data.size()));
}


} // namespace Poco


#if defined(POCO_ADLER32_X86_64)
#include "Adler32_X86.cpp"
#elif defined(POCO_ADLER32_ARMV8)
#include "Adler32_ARM.cpp"
#else


namespace Poco {


void Adler32::addInstructionImplementations(std::vector<Implementation>&)
	// other architectures use the portable implementation only
{
}


} // namespace Poco


#endif
