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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco {
namespace XSD {
namespace Types {


Element::Element()
{
}


Element::Element(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	OrderContent(id, minOcc, maxOcc)
{
}


Element::~Element()
{
}


OrderIterator Element::iterator() const
{
	return OrderIterator(new ElementIterator(*const_cast<Element*>(this)));
}


} } } // namespace Poco::XSD::Types
