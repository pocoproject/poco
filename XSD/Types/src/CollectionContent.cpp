//
// CollectionContent.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  CollectionContent
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/CollectionContent.h"


namespace Poco::XSD::Types {


CollectionContent::CollectionContent() = default;


CollectionContent::CollectionContent(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	OrderContent(id, minOcc, maxOcc)
{
}


CollectionContent::~CollectionContent() = default;


} // namespace Poco::XSD::Types
