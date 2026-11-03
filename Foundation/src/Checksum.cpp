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
#include <zlib.h>
#include <algorithm>
#include <limits>


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


void Checksum::update(const char* data, std::size_t length)
{
	// zlib takes the length as uInt, so a longer input is passed in parts
	constexpr std::size_t maxPart = std::numeric_limits<uInt>::max();
	while (length > 0)
	{
		const uInt part = static_cast<uInt>(std::min(length, maxPart));
		if (_type == TYPE_ADLER32)
			_value = adler32(_value, reinterpret_cast<const Bytef*>(data), part);
		else
			_value = crc32(_value, reinterpret_cast<const Bytef*>(data), part);
		data += part;
		length -= part;
	}
}


} // namespace Poco
