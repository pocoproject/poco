//
// Choice.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Choice
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/ChoiceIterator.h"
#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco {
namespace XSD {
namespace Types {


Choice::Choice(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	Order(id, minOcc, maxOcc)
{
}


Choice::~Choice()
{
}


void Choice::add(OrderContent::Ptr pChild)
{
	if (!pChild)
		throw NullTypeException("Cannot add null content to choice");
	_content.push_back(pChild);
}


void Choice::fixup()
{
	Choice::Content::iterator it = _content.begin();
	Choice::Content::iterator itEnd = _content.end();
	for (; it != itEnd; ++it)
		(*it)->fixup();
}


void Choice::accept(Visitor& v) const
{
	v.visit(*this);
}


OrderIterator Choice::iterator() const
{
	return OrderIterator(new ChoiceIterator(*this));
}


} } } // namespace Poco::XSD::Types
