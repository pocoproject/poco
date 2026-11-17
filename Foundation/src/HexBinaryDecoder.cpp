//
// HexBinaryDecoder.cpp
//
// Library: Foundation
// Package: Streams
// Module:  HexBinary
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/HexBinaryDecoder.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "DecoderError.h"


namespace Poco {


namespace
{
	[[noreturn]] void throwInvalidDigit(int n, std::streamoff pos)
	{
		throw DataFormatException("invalid hex digit " + describeDecoderChar(n) + " at position " + NumberFormatter::format(static_cast<Int64>(pos)));
	}

	inline int decodeDigit(int n, std::streamoff pos)
		/// Returns the value of the hex digit n, or throws if n is not a hex digit.
	{
		if (n >= '0' && n <= '9')
			return n - '0';
		else if (n >= 'A' && n <= 'F')
			return n - 'A' + 10;
		else if (n >= 'a' && n <= 'f')
			return n - 'a' + 10;
		throwInvalidDigit(n, pos);
	}
}


HexBinaryDecoderBuf::HexBinaryDecoderBuf(std::istream& istr):
	_pos(0),
	_buf(*istr.rdbuf())
{
}


HexBinaryDecoderBuf::~HexBinaryDecoderBuf()
{
}


int HexBinaryDecoderBuf::readFromDevice()
{
	int n;
	if ((n = readOne()) == -1) return -1;
	int c = decodeDigit(n, _pos - 1) << 4;
	if ((n = readOne()) == -1) throw DataFormatException("odd number of hex digits");
	c |= decodeDigit(n, _pos - 1);
	return c;
}


int HexBinaryDecoderBuf::readOne()
{
	int ch = _buf.sbumpc();
	++_pos;
	while (ch == ' ' || ch == '\r' || ch == '\t' || ch == '\n')
	{
		ch = _buf.sbumpc();
		++_pos;
	}
	return ch;
}


HexBinaryDecoderIOS::HexBinaryDecoderIOS(std::istream& istr): _buf(istr)
{
	poco_ios_init(&_buf);
}


HexBinaryDecoderIOS::~HexBinaryDecoderIOS()
{
}


HexBinaryDecoderBuf* HexBinaryDecoderIOS::rdbuf()
{
	return &_buf;
}


HexBinaryDecoder::HexBinaryDecoder(std::istream& istr): HexBinaryDecoderIOS(istr), std::istream(&_buf)
{
}


HexBinaryDecoder::~HexBinaryDecoder()
{
}


} // namespace Poco
