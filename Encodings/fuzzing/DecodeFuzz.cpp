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

#include <cstddef>
#include <fuzzer/FuzzedDataProvider.h>
#include <string>


using namespace Poco;


namespace
{


void convertToUtf8(const TextEncoding& encoding, const std::string& text, std::string& out)
{
	static const UTF8Encoding utf8;
	TextConverter converter(encoding, utf8);
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
	std::string out;
	convertToUtf8(encoding, text, out);

	if (index < DOUBLE_BYTE_ENCODING_COUNT)
		checkDoubleByteRoundTrip(encoding, out);

	return 0;
}
