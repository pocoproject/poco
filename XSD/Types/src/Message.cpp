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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Message.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Message::Message()
{
}


Message::Message(const std::string& name):
	_name(name)
{
}


Message::~Message()
{
}

	
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


} } } // namespace Poco::XSD::Types
