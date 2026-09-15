//
// XMLElement.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  XMLElement
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/XMLElement.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


XMLElement::XMLElement(const XMLElement& element):
	_name(element._name),
	_uri(element._uri),
	_value(element._value),
	_attributes(element._attributes),
	_children(element._children)
{
}


XMLElement::XMLElement(XMLElement&& element) noexcept:
	_name(std::move(element._name)),
	_uri(std::move(element._uri)),
	_value(std::move(element._value)),
	_attributes(std::move(element._attributes)),
	_children(std::move(element._children))
{
}


XMLElement::XMLElement(const std::string& name, const std::string& namespaceURI):
	_name(name),
	_uri(namespaceURI)
{
}


XMLElement::XMLElement(const std::string& name, const std::string& namespaceURI, const std::string& value):
	_name(name),
	_uri(namespaceURI),
	_value(value)
{
}


void XMLElement::addChild(const XMLElement& child)
{
	_children.push_back(child);
}


void XMLElement::addChild(XMLElement&& child)
{
	_children.push_back(std::move(child));
}


void XMLElement::setAttribute(const std::string& name, const std::string& namespaceURI, const std::string& value)
{
	_attributes.addAttribute(namespaceURI, name, ""s, ""s, value);
}


const XMLElement* XMLElement::findChild(const std::string& name, const std::string& namespaceURI) const
{
	for (const auto& c: _children)
	{
		if (c._name == name && c._uri == namespaceURI) return &c;
	}
	return nullptr;
}


} } } // namespace Poco::RemotingNG::SOAP
