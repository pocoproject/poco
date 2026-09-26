//
// Documentation.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Documentation
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Documentation.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Documentation::Documentation(const std::string& sourceUri, const std::string& lang):
	AnnotationContent(sourceUri),
	_language(lang)
{
}


Documentation::~Documentation()
{
}


void Documentation::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
