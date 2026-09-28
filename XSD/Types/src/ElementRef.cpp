//
// ElementRef.cpp
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ElementRef.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


ElementRef::ElementRef(const QName& ref):
	_ref(ref),
	_pElement(nullptr)
{
}


ElementRef::ElementRef(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc, const QName& ref):
	Element(id, minOcc, maxOcc),
	_ref(ref),
	_pElement(nullptr)
{
}


ElementRef::~ElementRef()
{
}


const std::string& ElementRef::nameSpace() const
{
	return _ref.getNamespace();
}


void ElementRef::fixup()
{
	if (_pElement == nullptr)
	{
		// find the element, throw exception if not found
		_pElement = TypesManager::instance().getElement(_ref);
		if (!_pElement)
			throw NullElementException("Failed to resolve element reference " + _ref.name());
	}
}


void ElementRef::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
