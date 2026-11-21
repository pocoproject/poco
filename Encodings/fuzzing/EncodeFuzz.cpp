//
// EncodeFuzz.cpp
//
// Converts UTF-8 text to a selected encoding.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EncodingTable.h"

#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"

#include <cstdint>
#include <cstdlib>
#include <fuzzer/FuzzedDataProvider.h>
#include <string>
#include <vector>


using namespace Poco;


namespace
{


void convertFromUtf8(TextEncoding& encoding, const std::string& text, std::string& out)
{
	static UTF8Encoding utf8;
	TextConverter converter(utf8, encoding);
	converter.convert(text, out);
	if (!text.empty())
	{
		std::string fromBytes;
		converter.convert(text.data(), static_cast<int>(text.size()), fromBytes);
	}
}


void checkDoubleByteRoundTrip(TextEncoding& encoding, const std::string& encoded)
	/// decode(encode(decode(x))) must equal decode(x) for DoubleByteEncoding.
	/// Here x is the encoded form produced from the UTF-8 input.
{
	static UTF8Encoding utf8;

	TextConverter toUtf8(encoding, utf8);
	std::string decoded;
	toUtf8.convert(encoded, decoded);

	TextConverter fromUtf8(utf8, encoding);
	std::string reencoded;
	fromUtf8.convert(decoded, reencoded);

	std::string redecoded;
	toUtf8.convert(reencoded, redecoded);

	if (decoded != redecoded)
		std::abort();
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > 64 * 1024) return 0;

	FuzzedDataProvider provider(data, size);
	const std::size_t index = provider.ConsumeIntegralInRange<std::size_t>(0, kEncodingCount - 1);
	const std::vector<uint8_t> bytes = provider.ConsumeRemainingBytes<uint8_t>();
	const std::string text(bytes.begin(), bytes.end());

	TextEncoding& encoding = encodingByIndex(index);
	std::string encoded;
	convertFromUtf8(encoding, text, encoded);

	if (index < kDoubleByteEncodingCount)
		checkDoubleByteRoundTrip(encoding, encoded);

	return 0;
}
