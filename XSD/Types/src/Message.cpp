//
// Message.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  Message
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Message.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


Message::Message() = default;


Message::Message(const std::string& name):
	_name(name)
{
}


Message::~Message() = default;

	
void Message::addElementPart(const std::string& name, const Poco::XML::Name& elementName)
{
	Part part;
	part.name = name;
	part.elementName = elementName;
	_parts.push_back(part);
}


void Message::addTypePart(const std::string& name, const Poco::XML::Name& typeName)
{
	Part part;
	part.name = name;
	part.typeName = typeName;
	_parts.push_back(part);
}


void Message::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
