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


ObjectId::ObjectId()
{
}


ObjectId::~ObjectId()
{
}


std::string ObjectId::toString() const
{
	std::string s;

	for(int i = 0; i < 12; ++i)
	{
		s += format("%x", (unsigned int) _id[i]);
	}
	return s;
}

} } // namespace Poco::MongoDB
