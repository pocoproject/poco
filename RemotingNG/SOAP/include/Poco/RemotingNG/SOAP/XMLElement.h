//
// XMLElement.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  XMLElement
//
// Definition of the XMLElement class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_XMLElement_INCLUDED
#define RemotingNG_SOAP_XMLElement_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/SAX/AttributesImpl.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace SOAP {


struct RemotingNGSOAP_API XMLElement
	/// This class provides access to XML elements in the SOAP envelope header
	/// to interceptors.
{
	XMLElement() = default;

	XMLElement(const XMLElement& element);
		/// Copy constructor.

	XMLElement(XMLElement&& element) noexcept;
		/// Move constructor.

	XMLElement(const std::string& name, const std::string& namespaceURI);
		/// Creates an element with the given local name and namespaceURI.

	XMLElement(const std::string& name, const std::string& namespaceURI, const std::string& value);
		/// Creates an element with the given local, and namespaceURI and text content.

	XMLElement& operator = (const XMLElement&) = default;
	XMLElement& operator = (XMLElement&&) = default;

	void addChild(const XMLElement& child);
		/// Adds a child element.

	void addChild(XMLElement&& child);
		/// Adds a child element.

	void setAttribute(const std::string& name, const std::string& namespaceURI, const std::string& value);
		/// Sets an attribute value.

	const XMLElement* findChild(const std::string& name, const std::string& namespaceURI) const;
		/// Finds a child element with the given local name and namespaceURI.
		/// Returns a pointer to the XMLElement if found, otherwise nullptr.
	
	const std::string& localName() const;
		/// Returns the local name of the element.

	const std::string& namespaceURI() const;
		/// Returns the namespace URI of the element.

	const std::string& value() const;
		/// Returns the text content of the element.

	const Poco::XML::AttributesImpl& attributes() const;
		/// Returns the attributes of the element.

	Poco::XML::AttributesImpl& attributes();
		/// Returns the attributes of the element.

	const std::vector<XMLElement>& children() const;
		/// Returns the children of the element.

	std::vector<XMLElement>& children();
		/// Returns the children of the element.

	std::string _name;
	std::string _uri;
	std::string _value;
	Poco::XML::AttributesImpl _attributes;
	std::vector<XMLElement> _children;
};


//
// inlines
//
inline const std::string& XMLElement::localName() const
{
	return _name;
}


inline const std::string& XMLElement::namespaceURI() const
{
	return _uri;
}


inline const std::string& XMLElement::value() const
{
	return _value;
}


inline const Poco::XML::AttributesImpl& XMLElement::attributes() const
{
	return _attributes;
}


inline Poco::XML::AttributesImpl& XMLElement::attributes()
{
	return _attributes;
}


inline const std::vector<XMLElement>& XMLElement::children() const
{
	return _children;
}


inline std::vector<XMLElement>& XMLElement::children()
{
	return _children;
}


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_XMLElement_INCLUDED
