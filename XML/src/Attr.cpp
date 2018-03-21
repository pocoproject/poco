//
// Attr.cpp
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


#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Document.h"
#include "Poco/XML/NamePool.h"


namespace Poco {
namespace XML {


Attr::Attr(Document::Ptr pOwnerDocument, const XMLString& namespaceURI,
		const XMLString& localName, const XMLString& qname, const XMLString& value, bool specified):
	AbstractNode(pOwnerDocument),
	_name(pOwnerDocument->namePool().insert(qname, namespaceURI, localName)),
	_value(value),
	_specified(specified)
{
}


Attr::Attr(Document::Ptr pOwnerDocument, const Attr& attr):
	AbstractNode(pOwnerDocument),
	_name(pOwnerDocument->namePool().insert(attr._name)),
	_value(attr._value),
	_specified(attr._specified)
{
}


Attr::~Attr()
{
}


void Attr::setValue(const XMLString& value)
{
	XMLString oldValue = _value;
	_value     = value;
	_specified = true;
	if (_pParent && !_pOwner->eventsSuspended())
		_pParent->dispatchAttrModified(Ptr(this, true), MutationEvent::MODIFICATION, oldValue, value);
}


Node::Ptr Attr::parentNode() const
{
	return 0;
}


Node::Ptr Attr::previousSibling() const
{
	if (_pParent)
	{
		Attr::Ptr pSibling = _pParent.cast<Element>()->_pFirstAttr;
		while (pSibling)
		{
			if (pSibling->_pNext == this) return pSibling;
			pSibling = pSibling->_pNext.cast<Attr>();
		}
		return pSibling;
	}
	return 0;
}


const XMLString& Attr::nodeName() const
{
	return _name.qname();
}


const XMLString& Attr::getNodeValue() const
{
	return _value;
}


void Attr::setNodeValue(const XMLString& value)
{
	setValue(value);
}


unsigned short Attr::nodeType() const
{
	return ATTRIBUTE_NODE;
}


const XMLString& Attr::namespaceURI() const
{
	return _name.namespaceURI();
}


XMLString Attr::prefix() const
{
	return _name.prefix();
}


const XMLString& Attr::localName() const
{
	return _name.localName();
}


XMLString Attr::innerText() const
{
	return nodeValue();
}


Node::Ptr Attr::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	return new Attr(pOwnerDocument, *this);
}


} } // namespace Poco::XML
