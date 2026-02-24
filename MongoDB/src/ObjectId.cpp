//
// ObjectId.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ObjectId
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ObjectId.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <cctype>
#include <cstring>


namespace Poco {
namespace MongoDB {


ObjectId::ObjectId()
{
	std::memset(_id, 0, sizeof(_id));
}


ObjectId::ObjectId(const std::string& id)
{
	if (id.size() != 24)
		throw Poco::InvalidArgumentException("ObjectId string must be exactly 24 hexadecimal characters");

    const char* p = id.c_str();
    for (std::size_t i = 0; i < 12; ++i)
    {
		// Validate that both characters are valid hex digits
		if (!std::isxdigit(static_cast<unsigned char>(p[0])) ||
		    !std::isxdigit(static_cast<unsigned char>(p[1])))
		{
			throw Poco::InvalidArgumentException("ObjectId string contains invalid hexadecimal characters");
		}
		_id[i] = fromHex(p);
		p += 2;
	}
}


ObjectId::ObjectId(const ObjectId& copy) noexcept
{
	std::memcpy(_id, copy._id, sizeof(_id));
}


ObjectId::ObjectId(ObjectId&& other) noexcept
{
	std::memcpy(_id, other._id, sizeof(_id));
}


ObjectId& ObjectId::operator=(const ObjectId& copy) noexcept
{
	if (this != &copy)
	{
		std::memcpy(_id, copy._id, sizeof(_id));
	}
	return *this;
}


ObjectId& ObjectId::operator=(ObjectId&& other) noexcept
{
	if (this != &other)
	{
		std::memcpy(_id, other._id, sizeof(_id));
	}
	return *this;
}


ObjectId::~ObjectId()
{
}


std::string ObjectId::toString(const std::string& fmt) const
{
	std::string s;
	s.reserve(24);  // Pre-allocate for 12 bytes * 2 hex chars

	for (std::size_t i = 0; i < 12; ++i)
	{
		s += format(fmt, static_cast<unsigned int>(_id[i]));
	}
	return s;
}


} } // namespace Poco::MongoDB
