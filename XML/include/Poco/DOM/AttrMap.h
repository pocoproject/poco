//
// AttrMap.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Definition of the AttrMap class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_AttrMap_INCLUDED
#define DOM_AttrMap_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/DOM/NamedNodeMap.h"


namespace Poco {
namespace XML {


class Element;


class XML_API AttrMap: public NamedNodeMap
	// This implementation of NamedNodeMap is
	// returned by Element::attributes()
{
public:
	typedef AutoPtr<AttrMap> Ptr;

	Node::Ptr getNamedItem(const XMLString& name) const;
	Node::Ptr setNamedItem(Node::Ptr arg);
	Node::Ptr removeNamedItem(const XMLString& name);
	Node::Ptr item(unsigned long index) const;
	unsigned long length() const;

	Node::Ptr getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const;
	Node::Ptr setNamedItemNS(Node::Ptr arg);
	Node::Ptr removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName);

protected:
	AttrMap(AutoPtr<Element> pElement);
	~AttrMap();

private:
	AttrMap();

	AutoPtr<Element> _pElement;

	friend class Element;
};


} } // namespace Poco::XML


#endif // DOM_AttrMap_INCLUDED
