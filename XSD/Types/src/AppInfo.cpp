//
// AppInfo.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AppInfo
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AppInfo.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


AppInfo::AppInfo(const std::string& sourceUri):
	AnnotationContent(sourceUri)
{
}


AppInfo::~AppInfo()
{
}


void AppInfo::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
