//
// AttributeContent.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeContent
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AttributeContent.h"


namespace Poco {
namespace XSD {
namespace Types {


AttributeContent::AttributeContent(const std::string& id, const std::string& name):
	AnnotatedObject(id),
	_name(name)
{
}


AttributeContent::~AttributeContent()
{
}


} } } // namespace Poco::XSD::Types
