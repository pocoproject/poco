//
// ObjectId.cpp
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  ObjectId
//
// Implementation of the ObjectId class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ObjectId.h"
#include "Poco/Format.h"


namespace Poco {
namespace MongoDB {


ObjectId::ObjectId(const std::string& id)
{
	if (id.size() == 12)
	{
		std::string::const_iterator it = id.begin();
		std::string::const_iterator end = id.end();
		for (int i = 0; i < 12; ++it, ++i) _id[i] = *it;
	}
	else if (id.size())
	{
		throw Poco::InvalidArgumentException("ID must be 12 characters long.");
	}
}


ObjectId::~ObjectId()
{
}


std::string ObjectId::toString(const std::string& fmt) const
{
	std::string s;

	for(int i = 0; i < 12; ++i)
	{
		s += format(fmt, (unsigned int) _id[i]);
	}
	return s;
}


} } // namespace Poco::MongoDB
