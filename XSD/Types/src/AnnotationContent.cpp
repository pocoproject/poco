//
// AnnotationContent.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AnnotationContent
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AnnotationContent.h"


namespace Poco {
namespace XSD {
namespace Types {


AnnotationContent::AnnotationContent(const std::string& sourceUri):
	_source(sourceUri),
	_data()
{
}


AnnotationContent::~AnnotationContent()
{
}


} } } // namespace Poco::XSD::Types
