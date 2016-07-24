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


BinaryWriter::BinaryWriter(std::ostream& ostr, StreamByteOrder order):
	_ostr(ostr),
	_pTextConverter(0)
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (order == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (order == BIG_ENDIAN_BYTE_ORDER);
#endif
}


BinaryWriter::BinaryWriter(std::ostream& ostr, TextEncoding& encoding, StreamByteOrder order):
	_ostr(ostr),
	_pTextConverter(new TextConverter(Poco::TextEncoding::global(), encoding))
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	_flipBytes = (order == LITTLE_ENDIAN_BYTE_ORDER);
#else
	_flipBytes = (order == BIG_ENDIAN_BYTE_ORDER);
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
	return write(value.c_str(), value.length());
}


BinaryWriter& BinaryWriter::operator << (const char* value)
{
	return write(value, std::strlen(value));
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


BinaryWriter& BinaryWriter::write(const char* value, std::size_t length)
{
	poco_check_ptr (value);
	
	if (_pTextConverter)
	{
		std::string converted;
		_pTextConverter->convert(value, static_cast<int>(length), converted);
		UInt32 convertedLength = (UInt32) converted.length();
		write7BitEncoded(convertedLength);
		_ostr.write(converted.data(), convertedLength);
	}
	else
	{
		UInt32 lengthUInt32 = static_cast<UInt32>(length);
		write7BitEncoded(lengthUInt32);
		_ostr.write(value, lengthUInt32);
	}
	return *this;	
}


} // namespace Poco
