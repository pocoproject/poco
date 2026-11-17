//
// Checksum.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Checksum.h"
#include "Poco/Exception.h"
#include "Adler32.h"
#include "CRC.h"
#include <zlib.h>
#include <span>


namespace Poco {


namespace {


// _value holds the checksum, the register after the final XOR: for CRC-32
// and CRC-64/XZ, _value 0 is the initial register of all ones
constexpr UInt64 CRC32_INVERSION = 0xFFFFFFFF;
constexpr UInt64 CRC64_XZ_INVERSION = ~static_cast<UInt64>(0);


} // namespace


Checksum::Checksum():
	_type(TYPE_CRC32),
	_value(crc32(0L, nullptr, 0))
{
}


Checksum::Checksum(Type t):
	_type(t),
	_value(0)
{
	if (t == TYPE_ADLER32)
		_value = ::adler32(0L, nullptr, 0);
}


Checksum::~Checksum()
{
}


void Checksum::update(const char* data, std::size_t length)
{
	const std::span<const unsigned char> bytes(reinterpret_cast<const unsigned char*>(data), length);
	switch (_type)
	{
	case TYPE_ADLER32:
		_value = Adler32::implementation()(static_cast<UInt32>(_value), bytes);
		break;
	case TYPE_CRC32:
		_value = CRC::implementation(_type)(_value ^ CRC32_INVERSION, bytes) ^ CRC32_INVERSION;
		break;
	case TYPE_CRC64_XZ:
		_value = CRC::implementation(_type)(_value ^ CRC64_XZ_INVERSION, bytes) ^ CRC64_XZ_INVERSION;
		break;
	case TYPE_CRC64_ECMA182:
		_value = CRC::implementation(_type)(_value, bytes);
		break;
	default:
		poco_bugcheck();
	}
}


Poco::UInt32 Checksum::checksum() const
{
	if (_type != TYPE_CRC32 && _type != TYPE_ADLER32)
		throw InvalidAccessException("checksum() returns 32-bit checksums only; use checksum64()");
	return static_cast<Poco::UInt32>(_value);
}


Poco::UInt64 Checksum::checksum64() const
{
	if (_type != TYPE_CRC64_XZ && _type != TYPE_CRC64_ECMA182)
		throw InvalidAccessException("checksum64() returns CRC-64 checksums only; use checksum()");
	return _value;
}


} // namespace Poco
