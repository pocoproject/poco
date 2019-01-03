//
// Checksum32.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum32
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Checksum32.h"
#if defined(POCO_UNBUNDLED)
#include <zlib.h>
#else
#include "Poco/zlib.h"
#endif


namespace Poco {


Checksum32::Checksum32():
	_type(TYPE_CRC32_IMPL),
	_value(crc32(0L, Z_NULL, 0))
{
}


Checksum32::Checksum32(ChecksumImpl::Type t):
	_type(t),
	_value(0)
{
	if (t == TYPE_CRC32_IMPL)
		_value = crc32(0L, Z_NULL, 0);
	else
		_value = adler32(0L, Z_NULL, 0);
}


Checksum32::~Checksum32()
{
}


void Checksum32::update(const char* data, unsigned length)
{
	if (_type == TYPE_ADLER32_IMPL)
		_value = adler32(_value, reinterpret_cast<const Bytef*>(data), length);
	else
		_value = crc32(_value, reinterpret_cast<const Bytef*>(data), length);
}


} // namespace Poco
