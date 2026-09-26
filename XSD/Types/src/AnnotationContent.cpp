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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
