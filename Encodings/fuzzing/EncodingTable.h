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


namespace {


constexpr std::size_t kDoubleByteEncodingCount = 30;
constexpr std::size_t kEncodingCount = 34;


Poco::TextEncoding& encodingByIndex(std::size_t index)
	/// The encoding at index in the fixed table of 30 Encodings
	/// DoubleByteEncoding classes followed by UTF-16 and UTF-32 with
	/// explicit byte order. index must be less than kEncodingCount.
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

	static ISO8859_10Encoding iso8859_10;
	static ISO8859_11Encoding iso8859_11;
	static ISO8859_13Encoding iso8859_13;
	static ISO8859_14Encoding iso8859_14;
	static ISO8859_16Encoding iso8859_16;
	static ISO8859_3Encoding iso8859_3;
	static ISO8859_4Encoding iso8859_4;
	static ISO8859_5Encoding iso8859_5;
	static ISO8859_6Encoding iso8859_6;
	static ISO8859_7Encoding iso8859_7;
	static ISO8859_8Encoding iso8859_8;
	static ISO8859_9Encoding iso8859_9;
	static Windows1253Encoding windows1253;
	static Windows1254Encoding windows1254;
	static Windows1255Encoding windows1255;
	static Windows1256Encoding windows1256;
	static Windows1257Encoding windows1257;
	static Windows1258Encoding windows1258;
	static Windows874Encoding windows874;
	static Windows932Encoding windows932;
	static Windows936Encoding windows936;
	static Windows949Encoding windows949;
	static Windows950Encoding windows950;
	static MacRomanEncoding macRoman;
	static MacCentralEurRomanEncoding macCentralEurRoman;
	static MacCyrillicEncoding macCyrillic;
	static MacChineseTradEncoding macChineseTrad;
	static MacChineseSimpEncoding macChineseSimp;
	static MacJapaneseEncoding macJapanese;
	static MacKoreanEncoding macKorean;
	static UTF16Encoding utf16be(UTF16Encoding::BIG_ENDIAN_BYTE_ORDER);
	static UTF16Encoding utf16le(UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	static UTF32Encoding utf32be(UTF32Encoding::BIG_ENDIAN_BYTE_ORDER);
	static UTF32Encoding utf32le(UTF32Encoding::LITTLE_ENDIAN_BYTE_ORDER);

	static TextEncoding* const table[kEncodingCount] =
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


} // namespace


#endif // Encodings_fuzzing_EncodingTable_INCLUDED
