//
// BinaryReader.cpp
//
// $Id: //poco/1.4/Foundation/src/BinaryReader.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  BinaryReaderWriter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/BinaryReader.h"
#include "Poco/TextEncoding.h"
#include "Poco/TextConverter.h"
#include <algorithm>


namespace Poco {


BinaryReader::BinaryReader(std::istream& istr, StreamByteOrder order):
	_istr(istr),
	_pTextConverter(0)
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (order == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (order == BIG_ENDIAN_BYTE_ORDER);
#endif
}


BinaryReader::BinaryReader(std::istream& istr, TextEncoding& encoding, StreamByteOrder order):
	_istr(istr),
	_pTextConverter(new TextConverter(encoding, Poco::TextEncoding::global()))
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (order == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (order == BIG_ENDIAN_BYTE_ORDER);
#endif
}


BinaryReader::~BinaryReader()
{
	delete _pTextConverter;
}


BinaryReader& BinaryReader::operator >> (bool& value)
{
	return read(value, false);
}


BinaryReader& BinaryReader::operator >> (char& value)
{
	return read(value, false);
}


BinaryReader& BinaryReader::operator >> (unsigned char& value)
{
	return read(value, false);
}


BinaryReader& BinaryReader::operator >> (signed char& value)
{
	return read(value, false);
}


BinaryReader& BinaryReader::operator >> (short& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (unsigned short& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (int& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (unsigned int& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (long& value)
{
#if defined(POCO_LONG_IS_64_BIT)
	return read((Int64&) value, _flipBytes);
#else
	return read((Int32&) value, _flipBytes);
#endif
}


BinaryReader& BinaryReader::operator >> (unsigned long& value)
{
#if defined(POCO_LONG_IS_64_BIT)
	return read((UInt64&) value, _flipBytes);
#else
	return read((UInt32&) value, _flipBytes);
#endif
}


BinaryReader& BinaryReader::operator >> (float& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (double& value)
{
	return read(value, _flipBytes);
}


#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)


BinaryReader& BinaryReader::operator >> (Int64& value)
{
	return read(value, _flipBytes);
}


BinaryReader& BinaryReader::operator >> (UInt64& value)
{
	return read(value, _flipBytes);
}


#endif


BinaryReader& BinaryReader::operator >> (std::string& value)
{
	if (!_istr.good()) return *this;

	UInt32 size = 0;
	read7BitEncoded(size);
	readRaw(size, value);
	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, converted);
		std::swap(value, converted);
	}
	return *this;
}


void BinaryReader::read7BitEncoded(UInt32& value)
{
	read7BitEncoded<UInt32>(value);
}


#if defined(POCO_HAVE_INT64)


void BinaryReader::read7BitEncoded(UInt64& value)
{
	read7BitEncoded<UInt64>(value);
}


#endif


void BinaryReader::readRaw(std::streamsize length, std::string& value)
{
	value.clear();
	value.reserve(static_cast<std::string::size_type>(length));
	while (length--)
	{
		char c;
		if (!_istr.read(&c, 1).good()) break;
		value += c;
	}
}


void BinaryReader::readRaw(char* buffer, std::streamsize length)
{
	_istr.read(buffer, length);
}


void BinaryReader::readBOM()
{
	UInt16 bom;
	_istr.read((char*) &bom, sizeof(bom));
	_flipBytes = bom != 0xFEFF;
}


} // namespace Poco
