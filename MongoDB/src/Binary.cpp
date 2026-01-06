//
// Binary.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Binary
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Binary.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Exception.h"
#include "Poco/MemoryStream.h"
#include "Poco/StreamCopier.h"
#include <cstring>
#include <sstream>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


Binary::Binary():
	_buffer(0),
	_subtype(0)
{
}


Binary::Binary(Poco::Int32 size, unsigned char subtype):
	_buffer(size),
	_subtype(subtype)
{
}


Binary::Binary(const UUID& uuid):
	_buffer(128 / 8),
	_subtype(SUBTYPE_UUID)
{
    unsigned char szUUID[16];
    uuid.copyTo(reinterpret_cast<char*>(szUUID));
    _buffer.assign(szUUID, 16);
}


Binary::Binary(const char* data, unsigned char subtype):
	_buffer(reinterpret_cast<const unsigned char*>(data), std::strlen(data)),
	_subtype(subtype)
{
}


Binary::Binary(const std::string& data, unsigned char subtype):
	_buffer(reinterpret_cast<const unsigned char*>(data.data()), data.size()),
	_subtype(subtype)
{
}


Binary::Binary(const void* data, Poco::Int32 size, unsigned char subtype):
	_buffer(reinterpret_cast<const unsigned char*>(data), size),
	_subtype(subtype)
{
}


Binary::~Binary()
{
}


std::string Binary::toString(int indent) const
{
	// Special handling for UUID subtype - format like MongoDB tools: UUID("...")
	if (_subtype == SUBTYPE_UUID && _buffer.size() == 16)
	{
		try
		{
			return "UUID(\""s + uuid().toString() + "\")"s;
		}
		catch (...)
		{
			// Fall through to Base64 encoding if UUID extraction fails
		}
	}

	// Default: Base64 encode the binary data
	std::ostringstream oss;
	Base64Encoder encoder(oss);
	MemoryInputStream mis(reinterpret_cast<const char*>(_buffer.begin()), _buffer.size());
	StreamCopier::copyStream(mis, encoder);
	encoder.close();
	return oss.str();
}


UUID Binary::uuid() const
{
	if (_subtype == SUBTYPE_UUID && _buffer.size() == 16)
	{
		UUID uuid;
		uuid.copyFrom(reinterpret_cast<const char*>(_buffer.begin()));
		return uuid;
	}
	throw BadCastException("Invalid subtype");
}


} } // namespace Poco::MongoDB
