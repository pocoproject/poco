//
// Base64Decoder.cpp
//
// Library: Foundation
// Package: Streams
// Module:  Base64
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include <array>


namespace Poco {


namespace
{
	constexpr unsigned char INVALID = 0xFF;

	constexpr std::array<unsigned char, 256> makeDecodingTable(const char* alphabet)
	{
		std::array<unsigned char, 256> table{};
		for (auto& value: table) value = INVALID;
		for (unsigned i = 0; i < 64; ++i)
			table[static_cast<unsigned char>(alphabet[i])] = static_cast<unsigned char>(i);
		return table;
	}

	constexpr std::array<unsigned char, 256> DECODING = makeDecodingTable(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	constexpr std::array<unsigned char, 256> DECODING_URL = makeDecodingTable(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_");

	std::string describe(int c)
	{
		if (c > 0x20 && c < 0x7F)
			return std::string(1, '\'') + static_cast<char>(c) + '\'';
		else
			return "0x" + NumberFormatter::formatHex(static_cast<unsigned>(c), 2);
	}

	[[noreturn]] void throwIncompleteGroup()
	{
		throw DataFormatException("incomplete Base64 group at end of input");
	}

	[[noreturn]] void throwDataAfterPadding(int c, std::streamoff pos)
	{
		throw DataFormatException("Base64 data after padding: " + describe(c) + " at position " + NumberFormatter::format(static_cast<Int64>(pos)));
	}

	unsigned char decodeChar(const unsigned char* pTable, int c, std::streamoff pos)
		/// Returns the 6-bit value of c, or throws if c is padding or not in the alphabet.
	{
		if (c == '=')
			throw DataFormatException("unexpected Base64 padding at position " + NumberFormatter::format(static_cast<Int64>(pos)));
		const unsigned char value = pTable[static_cast<unsigned char>(c)];
		if (value == INVALID)
			throw DataFormatException("invalid Base64 character " + describe(c) + " at position " + NumberFormatter::format(static_cast<Int64>(pos)));
		return value;
	}
}


Base64DecoderBuf::Base64DecoderBuf(std::istream& istr, int options):
	_options(options),
	_groupLength(0),
	_groupIndex(0),
	_pos(0),
	_buf(*istr.rdbuf()),
	_pInEncoding((options & BASE64_URL_ENCODING) ? DECODING_URL.data() : DECODING.data())
{
}


Base64DecoderBuf::~Base64DecoderBuf()
{
}


int Base64DecoderBuf::readFromDevice()
{
	if (_groupIndex < _groupLength)
	{
		return _group[_groupIndex++];
	}

	const bool noPadding = (_options & BASE64_NO_PADDING) != 0;
	int c;
	if (_groupLength > 0 && _groupLength < 3)
	{
		// The previous group was the final one; only whitespace may follow.
		if ((c = readOne()) != -1) throwDataAfterPadding(c, _pos - 1);
		return -1;
	}

	std::array<unsigned char, 4> value{};
	if ((c = readOne()) == -1) return -1;
	value[0] = decodeChar(_pInEncoding, c, _pos - 1);
	if ((c = readOne()) == -1) throwIncompleteGroup();
	value[1] = decodeChar(_pInEncoding, c, _pos - 1);

	int length = 3;
	if ((c = readOne()) == -1)
	{
		if (!noPadding) throwIncompleteGroup();
		length = 1;
	}
	else if (c == '=')
	{
		if ((c = readOne()) == -1)
		{
			if (!noPadding) throwIncompleteGroup();
		}
		else if (c != '=')
		{
			throwDataAfterPadding(c, _pos - 1);
		}
		length = 1;
	}
	else
	{
		value[2] = decodeChar(_pInEncoding, c, _pos - 1);
		if ((c = readOne()) == -1)
		{
			if (!noPadding) throwIncompleteGroup();
			length = 2;
		}
		else if (c == '=')
		{
			length = 2;
		}
		else
		{
			value[3] = decodeChar(_pInEncoding, c, _pos - 1);
		}
	}

	_group[0] = static_cast<unsigned char>((value[0] << 2) | (value[1] >> 4));
	_group[1] = static_cast<unsigned char>(((value[1] & 0x0F) << 4) | (value[2] >> 2));
	_group[2] = static_cast<unsigned char>((value[2] << 6) | value[3]);
	_groupLength = length;
	_groupIndex = 1;
	return _group[0];
}


int Base64DecoderBuf::readOne()
{
	int ch = _buf.sbumpc();
	if (ch != -1) ++_pos;
	if (!(_options & BASE64_URL_ENCODING))
	{
		while (ch == ' ' || ch == '\r' || ch == '\t' || ch == '\n')
		{
			ch = _buf.sbumpc();
			if (ch != -1) ++_pos;
		}
	}
	return ch;
}


Base64DecoderIOS::Base64DecoderIOS(std::istream& istr, int options): _buf(istr, options)
{
	poco_ios_init(&_buf);
}


Base64DecoderIOS::~Base64DecoderIOS()
{
}


Base64DecoderBuf* Base64DecoderIOS::rdbuf()
{
	return &_buf;
}


Base64Decoder::Base64Decoder(std::istream& istr, int options): Base64DecoderIOS(istr, options), std::istream(&_buf)
{
}


Base64Decoder::~Base64Decoder()
{
}


} // namespace Poco
