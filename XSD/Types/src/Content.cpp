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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
