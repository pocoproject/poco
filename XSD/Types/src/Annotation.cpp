//
// Annotation.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Annotation
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Annotation.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Annotation::Annotation(const std::string& id):
	_content(),
	_id(id)
{
}


Annotation::~Annotation()
{
}


Annotation::Annotation(const Annotation& ann):
	Visitable(),
	_content(ann._content),
	_id(ann._id)
{
}


Annotation& Annotation::operator=(const Annotation& ann)
{
	if (this != &ann)
	{
		Annotation tmp(ann);
		swap(tmp);
	}
	return *this;
}


void Annotation::swap(Annotation& ann)
{
	using std::swap;
	swap(_content, ann._content);
	swap(_id, ann._id);
}


void Annotation::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
