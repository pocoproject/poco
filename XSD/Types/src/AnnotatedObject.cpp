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
// SPDX-License-Identifier:	BSL-1.0
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
