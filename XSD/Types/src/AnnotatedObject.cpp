//
// AnnotatedObject.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AnnotatedObject
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AnnotatedObject.h"


namespace Poco {
namespace XSD {
namespace Types {


AnnotatedObject::AnnotatedObject():
	_annotation()
{
}


AnnotatedObject::AnnotatedObject(const std::string& id):
	Visitable(id),
	_annotation()
{
}


AnnotatedObject::AnnotatedObject(const std::string& id, const Annotation& ann):
	Visitable(id),
	_annotation()
{
	addAnnotation(ann);
}


AnnotatedObject::~AnnotatedObject()
{
}


} } } // namespace Poco::XSD::Types
