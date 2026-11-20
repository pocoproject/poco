//
// DecodeFuzz.cpp
//
// Converts text in a selected legacy encoding to UTF-8.
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


void convertToUtf8(TextEncoding& encoding, const std::string& text, std::string& out)
{
	static UTF8Encoding utf8;
	TextConverter converter(encoding, utf8);
	converter.convert(text, out);
	if (!text.empty())
	{
		std::string fromBytes;
		converter.convert(text.data(), static_cast<int>(text.size()), fromBytes);
	}
}


void checkDoubleByteRoundTrip(TextEncoding& encoding, const std::string& text)
	/// decode(encode(decode(x))) must equal decode(x) for DoubleByteEncoding.
{
	static UTF8Encoding utf8;

	TextConverter toUtf8(encoding, utf8);
	std::string decoded;
	toUtf8.convert(text, decoded);

	TextConverter fromUtf8(utf8, encoding);
	std::string encoded;
	fromUtf8.convert(decoded, encoded);

	std::string redecoded;
	toUtf8.convert(encoded, redecoded);

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
	std::string out;
	convertToUtf8(encoding, text, out);

	if (index < kDoubleByteEncodingCount)
		checkDoubleByteRoundTrip(encoding, text);

	return 0;
}
