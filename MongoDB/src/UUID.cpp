//
// UUID.cpp
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  UUID
//
// Implementation of the UUID class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/UUID.h"


namespace Poco {
namespace MongoDB {

const Poco::Int32    UUID::uuidSize;
const unsigned char  UUID::uuidSubtypeOld;
const unsigned char  UUID::uuidSubtype;

UUID::UUID()
{
}

UUID::UUID(const Poco::UUID& uuid) : _uuid(uuid)
{
}

UUID::~UUID()
{
}

std::string UUID::toString(int indent) const
{
	return _uuid.toString();
}

} } // namespace Poco::MongoDB
