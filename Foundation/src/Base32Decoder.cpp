//
// Base32Decoder.cpp
//
// Library: Foundation
// Package: Streams
// Module:  Base32
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Base32Decoder.h"
#include "Poco/Base32Encoder.h"
#include "Poco/Exception.h"
#include <cstring>


namespace Poco {


Base32DecoderBuf::Base32DecoderBuf(std::istream& istr, int options):
	_groupLength(0),
	_groupIndex(0),
	_buf(*istr.rdbuf())
{
	const unsigned char* encoding = Base32EncoderBuf::encoding(options);
	std::memset(_encoding, 0xFF, sizeof(_encoding));
	for (unsigned i = 0; i < 32; i++)
	{
		_encoding[encoding[i]] = static_cast<UInt8>(i);
	}
	_encoding[static_cast<unsigned char>('=')] = '\0';
}


Base32DecoderBuf::~Base32DecoderBuf()
{
}


int Base32DecoderBuf::readFromDevice()
{
	if (_groupIndex < _groupLength)
	{
		return _group[_groupIndex++];
	}
	else
	{
		unsigned char buffer[8];
		std::memset(buffer, '=', sizeof(buffer));
		int c;

		// per RFC-4648, Section 6, permissible block lengths are:
		// 2, 4, 5, 7, and 8 bytes. Any other length is malformed.
		//
		do 
		{
			if ((c = readOne()) == -1) return -1;
			buffer[0] = (unsigned char) c;
			if (_encoding[buffer[0]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) throw DataFormatException();
			buffer[1] = (unsigned char) c;
			if (_encoding[buffer[1]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) break;
			buffer[2] = (unsigned char) c;
			if (_encoding[buffer[2]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) throw DataFormatException();
			buffer[3] = (unsigned char) c;
			if (_encoding[buffer[3]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) break;
			buffer[4] = (unsigned char) c;
			if (_encoding[buffer[4]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) break;
			buffer[5] = (unsigned char) c;
			if (_encoding[buffer[5]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) throw DataFormatException();
			buffer[6] = (unsigned char) c;
			if (_encoding[buffer[6]] == 0xFF) throw DataFormatException();
			if ((c = readOne()) == -1) break;
			buffer[7] = (unsigned char) c;
			if (_encoding[buffer[7]] == 0xFF) throw DataFormatException();
		} 
		while (false);

		_group[0] = (_encoding[buffer[0]] << 3) | (_encoding[buffer[1]] >> 2);
		_group[1] = ((_encoding[buffer[1]] & 0x03) << 6) | (_encoding[buffer[2]] << 1) | (_encoding[buffer[3]] >> 4);
		_group[2] = ((_encoding[buffer[3]] & 0x0F) << 4) | (_encoding[buffer[4]] >> 1);
		_group[3] = ((_encoding[buffer[4]] & 0x01) << 7) | (_encoding[buffer[5]] << 2) | (_encoding[buffer[6]] >> 3);
		_group[4] = ((_encoding[buffer[6]] & 0x07) << 5) | _encoding[buffer[7]];

		if (buffer[2] == '=')
			_groupLength = 1;
		else if (buffer[4] == '=')
			_groupLength = 2;
		else if (buffer[5] == '=')
			_groupLength = 3;
		else if (buffer[7] == '=')
			_groupLength = 4;
		else
			_groupLength = 5;
		_groupIndex = 1;
		return _group[0];
	}
}


int Base32DecoderBuf::readOne()
{
	int ch = _buf.sbumpc();
	return ch;
}


Base32DecoderIOS::Base32DecoderIOS(std::istream& istr, int options): _buf(istr, options)
{
	poco_ios_init(&_buf);
}


Base32DecoderIOS::~Base32DecoderIOS()
{
}


Base32DecoderBuf* Base32DecoderIOS::rdbuf()
{
	return &_buf;
}


Base32Decoder::Base32Decoder(std::istream& istr, int options): Base32DecoderIOS(istr, options), std::istream(&_buf)
{
}


Base32Decoder::~Base32Decoder()
{
}


} // namespace Poco
