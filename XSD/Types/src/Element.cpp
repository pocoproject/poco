//
// Element.cpp
//
// Library: XSD/Types
// Package: XSDElements
// Module:  Element
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco::XSD::Types {


Element::Element() = default;


Element::Element(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	OrderContent(id, minOcc, maxOcc)
{
}


Element::~Element() = default;


OrderIterator Element::iterator() const
{
	return OrderIterator(new ElementIterator(*const_cast<Element*>(this)));
}


} // namespace Poco::XSD::Types
