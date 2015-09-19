//
// BinaryWriter.cpp
//
// $Id: //poco/1.4/Foundation/src/BinaryWriter.cpp#1 $
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


#include "Poco/BinaryWriter.h"
#include "Poco/TextEncoding.h"
#include "Poco/TextConverter.h"
#include <cstring>


namespace Poco {


BinaryWriter::BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder):
	_ostr(ostr),
	_pTextConverter(0)
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#endif
}


BinaryWriter::BinaryWriter(std::ostream& ostr, TextEncoding& encoding, StreamByteOrder byteOrder):
	_ostr(ostr),
	_pTextConverter(new TextConverter(Poco::TextEncoding::global(), encoding))
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#endif
}


BinaryWriter::~BinaryWriter()
{
	delete _pTextConverter;
}


BinaryWriter& BinaryWriter::operator << (bool value)
{
	return write(value, false);
}


BinaryWriter& BinaryWriter::operator << (char value)
{
	return write(value, false);
}


BinaryWriter& BinaryWriter::operator << (unsigned char value)
{
	return write(value, false);
}


BinaryWriter& BinaryWriter::operator << (signed char value)
{
	return write(value, false);
}


BinaryWriter& BinaryWriter::operator << (short value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (unsigned short value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (int value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (unsigned int value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (long value)
{
#if defined(POCO_LONG_IS_64_BIT)
	return write((Int64) value, _flipBytes);
#else
	return write((Int32) value, _flipBytes);
#endif
}


BinaryWriter& BinaryWriter::operator << (unsigned long value)
{
#if defined(POCO_LONG_IS_64_BIT)
	return write((UInt64) value, _flipBytes);
#else
	return write((UInt32) value, _flipBytes);
#endif
}


BinaryWriter& BinaryWriter::operator << (float value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (double value)
{
	return write(value, _flipBytes);
}


#if defined(POCO_HAVE_INT64) && !defined(POCO_LONG_IS_64_BIT)


BinaryWriter& BinaryWriter::operator << (Int64 value)
{
	return write(value, _flipBytes);
}


BinaryWriter& BinaryWriter::operator << (UInt64 value)
{
	return write(value, _flipBytes);
}


#endif


BinaryWriter& BinaryWriter::operator << (const std::string& value)
{
	return *this << value.c_str();
}


BinaryWriter& BinaryWriter::operator << (const char* value)
{
	poco_check_ptr (value);
	
	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, static_cast<int>(std::strlen(value)), converted);
		UInt32 length = (UInt32) converted.size();
		write7BitEncoded(length);
		_ostr.write(converted.data(), length);
	}
	else
	{
		UInt32 length = static_cast<UInt32>(std::strlen(value));
		write7BitEncoded(length);
		_ostr.write(value, length);
	}
	return *this;
}


void BinaryWriter::write7BitEncoded(UInt32 value)
{
	write7BitEncoded<UInt32>(value);
}


#if defined(POCO_HAVE_INT64)


void BinaryWriter::write7BitEncoded(UInt64 value)
{
	write7BitEncoded<UInt64>(value);
}


#endif


void BinaryWriter::writeRaw(const std::string& rawData)
{
	_ostr.write(rawData.data(), (std::streamsize) rawData.length());
}


void BinaryWriter::writeRaw(const char* buffer, std::streamsize length)
{
	_ostr.write(buffer, length);
}


void BinaryWriter::writeBOM()
{
	UInt16 value = 0xFEFF;
	if (_flipBytes) value = ByteOrder::flipBytes(value);
	_ostr.write((const char*) &value, sizeof(value));
}


void BinaryWriter::flush()
{
	_ostr.flush();
}


} // namespace Poco
