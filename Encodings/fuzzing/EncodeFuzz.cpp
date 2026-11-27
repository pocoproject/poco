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
#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>


using namespace Poco;


namespace
{


void convertFromUtf8(const TextEncoding& encoding, const std::string& text, std::string& out)
{
	static const UTF8Encoding utf8;
	TextConverter converter(utf8, encoding);
	converter.convert(text, out);
	if (!text.empty())
	{
		std::string fromBytes;
		converter.convert(text.data(), static_cast<int>(text.size()), fromBytes);
	}
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	FuzzedDataProvider provider(data, size);
	const std::size_t index = provider.ConsumeIntegralInRange<std::size_t>(0, ENCODING_COUNT - 1);
	const std::string text = provider.ConsumeRemainingBytesAsString();

	const TextEncoding& encoding = encodingByIndex(index);
	std::string encoded;
	convertFromUtf8(encoding, text, encoded);

	if (index < DOUBLE_BYTE_ENCODING_COUNT)
	{
		// Decode the produced bytes first: the shared check takes the
		// decoded form and re-encodes it.
		static const UTF8Encoding utf8;
		TextConverter toUtf8(encoding, utf8);
		std::string decoded;
		toUtf8.convert(encoded, decoded);
		checkDoubleByteRoundTrip(encoding, decoded);
	}

	return 0;
}
