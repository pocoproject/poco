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


namespace Poco {


Checksum::Checksum():
	_type(TYPE_CRC32),
	_value(crc32(0L, nullptr, 0))
{
}


Checksum::Checksum(Type t):
	_type(t),
	_value(0)
{
	if (t == TYPE_CRC32)
		_value = crc32(0L, nullptr, 0);
	else
		_value = adler32(0L, nullptr, 0);
}


Checksum::~Checksum()
{
}


void Checksum::update(const char* data, unsigned length)
{
	if (_type == TYPE_ADLER32)
		_value = adler32(_value, reinterpret_cast<const Bytef*>(data), length);
	else
		_value = crc32(_value, reinterpret_cast<const Bytef*>(data), length);
}

uLong Checksum::crc32_combine(uLong crc1, uLong crc2, z_off_t length)
{
	return crc32_combine(crc1, crc2, length);
}


} // namespace Poco
