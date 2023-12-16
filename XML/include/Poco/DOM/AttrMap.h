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
	Node* getNamedItem(const XMLString& name) const override;
	Node* setNamedItem(Node* arg) override;
	Node* removeNamedItem(const XMLString& name) override;
	Node* item(unsigned long index) const override;
	unsigned long length() const override;

	Node* getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const override;
	Node* setNamedItemNS(Node* arg) override;
	Node* removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) override;

	void autoRelease() override;

protected:
	AttrMap(Element* pElement);
	~AttrMap() override;

private:
	AttrMap();

	Element* _pElement;

	friend class Element;
};


} } // namespace Poco::XML


#endif // DOM_AttrMap_INCLUDED
