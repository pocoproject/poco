//
// Content.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Content
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Content.h"


namespace Poco {
namespace XSD {
namespace Types {


Content::Content()
{
}


Content::Content(const std::string& id):
	AnnotatedObject(id)
{
}


Content::~Content()
{
}


} } } // namespace Poco::XSD::Types
