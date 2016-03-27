//
// Binary.cpp
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Binary
//
// Implementation of the Binary class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Binary.h"


namespace Poco {
namespace MongoDB {


Binary::Binary() : _buffer(0)
{
}


Binary::Binary(Poco::Int32 size, unsigned char subtype) : _buffer(size), _subtype(subtype)
{
}


Binary::Binary(const UUID& uuid) : _buffer(128 / 8), _subtype(0x04)
{
    unsigned char szUUID[16];
    uuid.copyTo((char*) szUUID);
    _buffer.assign(szUUID, 16);
}

Binary::~Binary()
{
}


std::string Binary::toString(int indent) const
{
	std::ostringstream oss;
	Base64Encoder encoder(oss);
	MemoryInputStream mis((const char*) _buffer.begin(), _buffer.size());
	StreamCopier::copyStream(mis, encoder);
	return oss.str();
}

UUID Binary::uuid() const
{
	if ( _subtype == 0x04 && _buffer.size() == 16 )
	{
		UUID uuid;
		uuid.copyFrom((const char*) _buffer.begin());
		return uuid;
	}
	throw BadCastException("Invalid subtype");
}

} } // namespace Poco::MongoDB
