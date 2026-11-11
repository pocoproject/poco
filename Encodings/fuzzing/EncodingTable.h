#include "Poco/ASCIIEncoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/Latin2Encoding.h"
#include "Poco/Latin9Encoding.h"
#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Windows1250Encoding.h"
#include "Poco/Windows1251Encoding.h"
#include "Poco/Windows1252Encoding.h"
//
// EncodingTable.h
//
// Function-local static table of encoding objects for the Encodings fuzzers.
// Index order is fixed so seed corpora can append a trailing index byte.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Encodings_fuzzing_EncodingTable_INCLUDED
#define Encodings_fuzzing_EncodingTable_INCLUDED


#include "Poco/ISO8859_10Encoding.h"
#include "Poco/ISO8859_11Encoding.h"
#include "Poco/ISO8859_13Encoding.h"
#include "Poco/ISO8859_14Encoding.h"
#include "Poco/ISO8859_16Encoding.h"
#include "Poco/ISO8859_3Encoding.h"
#include "Poco/ISO8859_4Encoding.h"
#include "Poco/ISO8859_5Encoding.h"
#include "Poco/ISO8859_6Encoding.h"
#include "Poco/ISO8859_7Encoding.h"
#include "Poco/ISO8859_8Encoding.h"
#include "Poco/ISO8859_9Encoding.h"
#include "Poco/Windows1253Encoding.h"
#include "Poco/Windows1254Encoding.h"
#include "Poco/Windows1255Encoding.h"
#include "Poco/Windows1256Encoding.h"
#include "Poco/Windows1257Encoding.h"
#include "Poco/Windows1258Encoding.h"
#include "Poco/Windows874Encoding.h"
#include "Poco/Windows932Encoding.h"
#include "Poco/Windows936Encoding.h"
#include "Poco/Windows949Encoding.h"
#include "Poco/Windows950Encoding.h"
#include "Poco/MacRomanEncoding.h"
#include "Poco/MacCentralEurRomanEncoding.h"
#include "Poco/MacCyrillicEncoding.h"
#include "Poco/MacChineseTradEncoding.h"
#include "Poco/MacChineseSimpEncoding.h"
#include "Poco/MacJapaneseEncoding.h"
#include "Poco/MacKoreanEncoding.h"
#include "Poco/UTF16Encoding.h"
#include "Poco/UTF32Encoding.h"
#include "Poco/TextEncoding.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>




// The 30 Encodings-library DoubleByteEncoding tables, followed by
// UTF-16 and UTF-32 with explicit byte order and the Foundation
// encodings.
inline constexpr std::size_t DOUBLE_BYTE_ENCODING_COUNT = 30;
inline constexpr std::size_t ENCODING_COUNT = 42;
inline constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;


inline const Poco::TextEncoding& encodingByIndex(std::size_t index)
	/// The encoding at index in the fixed table of 30 Encodings
	/// DoubleByteEncoding classes followed by UTF-16 and UTF-32 with
	/// explicit byte order. index must be less than ENCODING_COUNT.
{
	using Poco::ISO8859_10Encoding;
	using Poco::ISO8859_11Encoding;
	using Poco::ISO8859_13Encoding;
	using Poco::ISO8859_14Encoding;
	using Poco::ISO8859_16Encoding;
	using Poco::ISO8859_3Encoding;
	using Poco::ISO8859_4Encoding;
	using Poco::ISO8859_5Encoding;
	using Poco::ISO8859_6Encoding;
	using Poco::ISO8859_7Encoding;
	using Poco::ISO8859_8Encoding;
	using Poco::ISO8859_9Encoding;
	using Poco::Windows1253Encoding;
	using Poco::Windows1254Encoding;
	using Poco::Windows1255Encoding;
	using Poco::Windows1256Encoding;
	using Poco::Windows1257Encoding;
	using Poco::Windows1258Encoding;
	using Poco::Windows874Encoding;
	using Poco::Windows932Encoding;
	using Poco::Windows936Encoding;
	using Poco::Windows949Encoding;
	using Poco::Windows950Encoding;
	using Poco::MacRomanEncoding;
	using Poco::MacCentralEurRomanEncoding;
	using Poco::MacCyrillicEncoding;
	using Poco::MacChineseTradEncoding;
	using Poco::MacChineseSimpEncoding;
	using Poco::MacJapaneseEncoding;
	using Poco::MacKoreanEncoding;
	using Poco::UTF16Encoding;
	using Poco::UTF32Encoding;
	using Poco::TextEncoding;

	static const ISO8859_10Encoding iso8859_10;
	static const ISO8859_11Encoding iso8859_11;
	static const ISO8859_13Encoding iso8859_13;
	static const ISO8859_14Encoding iso8859_14;
	static const ISO8859_16Encoding iso8859_16;
	static const ISO8859_3Encoding iso8859_3;
	static const ISO8859_4Encoding iso8859_4;
	static const ISO8859_5Encoding iso8859_5;
	static const ISO8859_6Encoding iso8859_6;
	static const ISO8859_7Encoding iso8859_7;
	static const ISO8859_8Encoding iso8859_8;
	static const ISO8859_9Encoding iso8859_9;
	static const Windows1253Encoding windows1253;
	static const Windows1254Encoding windows1254;
	static const Windows1255Encoding windows1255;
	static const Windows1256Encoding windows1256;
	static const Windows1257Encoding windows1257;
	static const Windows1258Encoding windows1258;
	static const Windows874Encoding windows874;
	static const Windows932Encoding windows932;
	static const Windows936Encoding windows936;
	static const Windows949Encoding windows949;
	static const Windows950Encoding windows950;
	static const MacRomanEncoding macRoman;
	static const MacCentralEurRomanEncoding macCentralEurRoman;
	static const MacCyrillicEncoding macCyrillic;
	static const MacChineseTradEncoding macChineseTrad;
	static const MacChineseSimpEncoding macChineseSimp;
	static const MacJapaneseEncoding macJapanese;
	static const MacKoreanEncoding macKorean;
	static const UTF16Encoding utf16be(UTF16Encoding::BIG_ENDIAN_BYTE_ORDER);
	static const UTF16Encoding utf16le(UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	static const UTF32Encoding utf32be(UTF32Encoding::BIG_ENDIAN_BYTE_ORDER);
	static const UTF32Encoding utf32le(UTF32Encoding::LITTLE_ENDIAN_BYTE_ORDER);

	static const TextEncoding* const table[ENCODING_COUNT] =
	{
		&iso8859_10,
		&iso8859_11,
		&iso8859_13,
		&iso8859_14,
		&iso8859_16,
		&iso8859_3,
		&iso8859_4,
		&iso8859_5,
		&iso8859_6,
		&iso8859_7,
		&iso8859_8,
		&iso8859_9,
		&windows1253,
		&windows1254,
		&windows1255,
		&windows1256,
		&windows1257,
		&windows1258,
		&windows874,
		&windows932,
		&windows936,
		&windows949,
		&windows950,
		&macRoman,
		&macCentralEurRoman,
		&macCyrillic,
		&macChineseTrad,
		&macChineseSimp,
		&macJapanese,
		&macKorean,
		&utf16be,
		&utf16le,
		&utf32be,
		&utf32le
	};

	return *table[index];
}





inline void checkDoubleByteRoundTrip(const Poco::TextEncoding& encoding, const std::string& decoded)
	/// encode(decoded) must decode back to |decoded| without a change;
	/// DoubleByteEncoding tables only, where every decoded byte has an
	/// encoding.
{
	static const Poco::UTF8Encoding utf8;

	Poco::TextConverter fromUtf8(utf8, encoding);
	std::string encoded;
	fromUtf8.convert(decoded, encoded);

	Poco::TextConverter toUtf8(encoding, utf8);
	std::string redecoded;
	toUtf8.convert(encoded, redecoded);

	if (redecoded != decoded)
	{
		std::cerr << "round trip mismatch for encoding " << encoding.canonicalName() << std::endl;
		std::abort();
	}
}
#endif // Encodings_fuzzing_EncodingTable_INCLUDED
