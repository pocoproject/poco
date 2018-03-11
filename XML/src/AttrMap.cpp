//
// AttrMap.cpp
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/AttrMap.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMException.h"


namespace Poco {
namespace XML {


AttrMap::AttrMap(Element::Ptr pElement):
	_pElement(pElement)
{
	poco_check_ptr (pElement);
}


AttrMap::~AttrMap()
{
}


Node::Ptr AttrMap::getNamedItem(const XMLString& name) const
{
	return _pElement->getAttributeNode(name);
}


Node::Ptr AttrMap::setNamedItem(Node::Ptr arg)
{
	poco_check_ptr (arg);

	if (arg->nodeType() != Node::ATTRIBUTE_NODE)
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
		
	return _pElement->setAttributeNode(arg.cast<Attr>());
}


Node::Ptr AttrMap::removeNamedItem(const XMLString& name)
{
	Attr::Ptr pAttr = _pElement->getAttributeNode(name);
	if (pAttr)
		return _pElement->removeAttributeNode(pAttr);
	else
		return 0;
}


Node::Ptr AttrMap::item(unsigned long index) const
{
	AbstractNode::Ptr pAttr = _pElement->_pFirstAttr;
	while (index-- > 0 && pAttr) pAttr = pAttr->nextSibling().cast<AbstractNode>();
	return pAttr;
}


unsigned long AttrMap::length() const
{
	unsigned long result = 0;
	AbstractNode::Ptr pAttr = _pElement->_pFirstAttr;
	while (pAttr)
	{
		pAttr = pAttr->nextSibling().cast<AbstractNode>();
		++result;
	}
	return result;
}


Node::Ptr AttrMap::getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	return _pElement->getAttributeNodeNS(namespaceURI, localName);
}


Node::Ptr AttrMap::setNamedItemNS(Node::Ptr arg)
{
	poco_check_ptr (arg);

	if (arg->nodeType() != Node::ATTRIBUTE_NODE)
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);

	return _pElement->setAttributeNodeNS(arg.cast<Attr>());
}


Node::Ptr AttrMap::removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName)
{
	Attr::Ptr pAttr = _pElement->getAttributeNodeNS(namespaceURI, localName);
	if (pAttr)
		return _pElement->removeAttributeNode(pAttr);
	else
		return 0;
}


} } // namespace Poco::XML

