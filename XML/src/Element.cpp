//
// Element.cpp
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


#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/ElementsByTagNameList.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/AttrMap.h"


namespace Poco {
namespace XML {


Element::Element(Document::Ptr& pOwnerDocument, const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname):
	AbstractContainerNode(pOwnerDocument.get()),
	_name(pOwnerDocument->namePool().insert(qname, namespaceURI, localName))
{
}


Element::Element(Document::Ptr& pOwnerDocument, const Element& element):
	AbstractContainerNode(pOwnerDocument.get()),
	_name(pOwnerDocument->namePool().insert(element._name))
{
	Attr::Ptr pAttr = element._pFirstAttr;
	while (pAttr)
	{
		Attr::Ptr pClonedAttr = pAttr->copyNode(false, pOwnerDocument).unsafeCast<Attr>();
		setAttributeNode(pClonedAttr);
		pAttr = pAttr->_pNext.unsafeCast<Attr>();
	}
}


Element::Element(RefPtr<Document>&& pOwnerDocument, const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname):
	AbstractContainerNode(pOwnerDocument.get()),
	_name(pOwnerDocument->namePool().insert(qname, namespaceURI, localName))
{

}


Element::Element(RefPtr<Document>&& pOwnerDocument, const Element& element):
	AbstractContainerNode(pOwnerDocument.get()),
	_name(pOwnerDocument->namePool().insert(element._name))
{
	Attr::Ptr pAttr = element._pFirstAttr;
	while (pAttr)
	{
		Attr::Ptr pClonedAttr = pAttr->copyNode(false, pOwnerDocument).unsafeCast<Attr>();
		setAttributeNode(pClonedAttr);
		pAttr = pAttr->_pNext.unsafeCast<Attr>();
	}
}


Element::~Element()
{
}


const XMLString& Element::getAttribute(const XMLString& name) const
{
	Attr::Ptr pAttr = getAttributeNode(name);
	if (pAttr)
		return pAttr->getValue();
	else
		return EMPTY_STRING;
}


void Element::setAttribute(const XMLString& name, const XMLString& value)
{
	Attr::Ptr pAttr = getAttributeNode(name);
	if (pAttr)
	{
		pAttr->setValue(value);
	}
	else
	{
		pAttr = ownerDocument()->createAttribute(name);
		pAttr->setValue(value);
		setAttributeNode(pAttr);
	}
}


void Element::removeAttribute(const XMLString& name)
{
	Attr::Ptr pAttr = getAttributeNode(name);
	if (pAttr) removeAttributeNode(pAttr);
}


Attr::Ptr Element::getAttributeNode(const XMLString& name) const
{
	Attr::Ptr pAttr = _pFirstAttr;
	while (pAttr && pAttr->_name.qname() != name)
		pAttr = pAttr->_pNext.unsafeCast<Attr>();
	return pAttr;
}


Attr::Ptr Element::setAttributeNode(Attr::Ptr newAttr)
{
	if (newAttr->ownerDocument() != ownerDocument())
		throw DOMException(DOMException::WRONG_DOCUMENT_ERR);
	if (newAttr->ownerElement())
		throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR);

	Attr::Ptr oldAttr = getAttributeNode(newAttr->name());
	if (oldAttr) removeAttributeNode(oldAttr);

	Attr::Ptr pCur = _pFirstAttr;
	if (pCur)
	{
		while (pCur->_pNext) pCur = pCur->_pNext.unsafeCast<Attr>();
		pCur->_pNext = newAttr;
	}
	else
		_pFirstAttr = newAttr;
	newAttr->_pParent = Ptr(this, true).cast<AbstractContainerNode>();
	if (_pOwner->events())
		dispatchAttrModified(newAttr, MutationEvent::ADDITION, EMPTY_STRING, newAttr->getValue());

	return oldAttr;
}


Attr::Ptr Element::removeAttributeNode(Attr::Ptr oldAttr)
{
	poco_check_ptr (oldAttr);

	if (_pOwner->events())
		dispatchAttrModified(oldAttr, MutationEvent::REMOVAL, oldAttr->getValue(), EMPTY_STRING);

	if(oldAttr != _pFirstAttr)
	{
		Attr::Ptr pCur = _pFirstAttr;
		while(pCur->_pNext.cast<Attr>() != oldAttr) pCur = pCur->_pNext.unsafeCast<Attr>();
		if(pCur)
		{
			pCur->_pNext = pCur->_pNext->_pNext.cast<AbstractNode>();
		}
		else throw DOMException(DOMException::NOT_FOUND_ERR);
	}
	else
		_pFirstAttr = _pFirstAttr->_pNext.unsafeCast<Attr>();
	oldAttr->_pNext = 0;
	oldAttr->_pParent = 0;
	oldAttr = 0;

	return oldAttr;
}


Attr::Ptr Element::addAttributeNodeNP(Attr::Ptr oldAttr, Attr::Ptr newAttr)
{
	newAttr->_pParent = Ptr(this, true).cast<AbstractContainerNode>();
	if (oldAttr)
	{
		oldAttr->_pNext = newAttr;
	}
	else if (_pFirstAttr)
	{
		newAttr->_pNext = _pFirstAttr;
		_pFirstAttr = newAttr;
	}
	else
	{
		_pFirstAttr = newAttr;
	}
	return newAttr;
}


NodeList::Ptr Element::getElementsByTagName(const XMLString& name) const
{
	return new ElementsByTagNameList(RefPtr<Element>(const_cast<Element*>(this), true), name);
}


NodeList::Ptr Element::getElementsByTagNameNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	return new ElementsByTagNameListNS(RefPtr<Element>(const_cast<Element*>(this), true), namespaceURI, localName);
}


void Element::normalize()
{
	Node::Ptr pCur = firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == Node::ELEMENT_NODE)
		{
			pCur->normalize();
		}
		else if (pCur->nodeType() == Node::TEXT_NODE)
		{
			Node::Ptr pNext = pCur->nextSibling();
			while (pNext && pNext->nodeType() == Node::TEXT_NODE)
			{
				pCur.unsafeCast<Text>()->appendData(pNext->nodeValue());
				removeChild(pNext);
				pNext = pCur->nextSibling();
			}
		}
		pCur = pCur->nextSibling();
	}
}


const XMLString& Element::nodeName() const
{
	return tagName();
}


NamedNodeMap::Ptr Element::attributes() const
{
	return new AttrMap(RefPtr<Element>(const_cast<Element*>(this), true));
}


unsigned short Element::nodeType() const
{
	return Node::ELEMENT_NODE;
}


const XMLString& Element::getAttributeNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	Attr::Ptr pAttr = getAttributeNodeNS(namespaceURI, localName);
	if (pAttr)
		return pAttr->getValue();
	else
		return EMPTY_STRING;
}


void Element::setAttributeNS(const XMLString& namespaceURI, const XMLString& qualifiedName, const XMLString& value)
{
	Attr::Ptr pAttr = getAttributeNodeNS(namespaceURI, qualifiedName);
	if (pAttr)
	{
		pAttr->setValue(value);
	}
	else
	{
		pAttr = _pOwner->createAttributeNS(namespaceURI, qualifiedName);
		pAttr->setValue(value);
		setAttributeNodeNS(pAttr);
	}
}


void Element::removeAttributeNS(const XMLString& namespaceURI, const XMLString& localName)
{
	Attr::Ptr pAttr = getAttributeNodeNS(namespaceURI, localName);
	if (pAttr) removeAttributeNode(pAttr);
}


Attr::Ptr Element::getAttributeNodeNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	Attr::Ptr pAttr = _pFirstAttr;
	while (!pAttr.isNull() && (pAttr->_name.namespaceURI() != namespaceURI || pAttr->_name.localName() != localName))
		pAttr = pAttr->_pNext.cast<Attr>();
	return pAttr;
}


Attr::Ptr Element::setAttributeNodeNS(Attr::Ptr newAttr)
{
	poco_check_ptr (newAttr);

	if (newAttr->ownerDocument() != ownerDocument())
		throw DOMException(DOMException::WRONG_DOCUMENT_ERR);
	if (newAttr->ownerElement())
		throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR);

	Attr::Ptr oldAttr = getAttributeNodeNS(newAttr->namespaceURI(), newAttr->localName());
	if (oldAttr) removeAttributeNode(oldAttr);

	Attr::Ptr pCur = _pFirstAttr;
	if (pCur)
	{
		while (pCur->_pNext) pCur = pCur->_pNext.cast<Attr>();
		pCur->_pNext = newAttr;
	}
	else
		_pFirstAttr = newAttr;
	newAttr->_pParent = Ptr(this, true).cast<AbstractContainerNode>();
	if (_pOwner->events())
		dispatchAttrModified(newAttr, MutationEvent::ADDITION, EMPTY_STRING, newAttr->getValue());

	return oldAttr;
}


bool Element::hasAttribute(const XMLString& name) const
{
	return !getAttributeNode(name).isNull();
}


bool Element::hasAttributeNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	return !getAttributeNodeNS(namespaceURI, localName).isNull();
}


const XMLString& Element::namespaceURI() const
{
	return _name.namespaceURI();
}


XMLString Element::prefix() const
{
	return _name.prefix();
}


const XMLString& Element::localName() const
{
	return _name.localName();
}


bool Element::hasAttributes() const
{
	return !_pFirstAttr.isNull();
}


XMLString Element::innerText() const
{
	XMLString result;
	Node::Ptr pChild = firstChild();
	while (pChild)
	{
		result.append(pChild->innerText());
		pChild = pChild->nextSibling();
	}
	return result;
}


Element::Ptr Element::getChildElement(const XMLString& name) const
{
	Node::Ptr pNode = firstChild().unsafeCast<Node>();
	while (!pNode.isNull() && !(pNode->nodeType() == Node::ELEMENT_NODE && pNode->nodeName() == name))
		pNode = pNode->nextSibling().unsafeCast<Node>();
	return pNode.unsafeCast<Element>();
}


Element::Ptr Element::getChildElementNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	Node::Ptr pNode = firstChild();
	while (pNode && !(pNode->nodeType() == Node::ELEMENT_NODE && pNode->namespaceURI() == namespaceURI && pNode->localName() == localName))
		pNode = pNode->nextSibling();
	return pNode.unsafeCast<Element>();
}


void Element::dispatchNodeRemovedFromDocument()
{
	AbstractContainerNode::dispatchNodeRemovedFromDocument();
	Attr::Ptr pAttr = _pFirstAttr;
	while (pAttr)
	{
		pAttr->dispatchNodeRemovedFromDocument();
		pAttr = pAttr->_pNext.unsafeCast<Attr>();
	}
}


void Element::dispatchNodeInsertedIntoDocument()
{
	AbstractContainerNode::dispatchNodeInsertedIntoDocument();
	Attr::Ptr pAttr = _pFirstAttr;
	while (pAttr)
	{
		pAttr->dispatchNodeInsertedIntoDocument();
		pAttr = pAttr->_pNext.unsafeCast<Attr>();
	}
}


Node::Ptr Element::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	Element::Ptr pClone = new Element(pOwnerDocument, *this);
	if (deep)
	{
		Node::Ptr pNode = firstChild();
		while (pNode)
		{
			pClone->appendChild(pNode.unsafeCast<AbstractNode>()->copyNode(true, pOwnerDocument));
			pNode = pNode->nextSibling();
		}
	}
	return pClone;
}


Element::Ptr Element::getElementById(const XMLString& elementId, const XMLString& idAttribute) const
{
	if (getAttribute(idAttribute) == elementId)
		return RefPtr<Element>(const_cast<Element*>(this), true);

	Node::Ptr pNode = firstChild();
	while (pNode)
	{
		if (pNode->nodeType() == Node::ELEMENT_NODE)
		{
			Element::Ptr pResult = pNode.unsafeCast<Element>()->getElementById(elementId, idAttribute);
			if (pResult) return pResult;
		}
		pNode = pNode->nextSibling();
	}
	return 0;
}


Element::Ptr Element::getElementByIdNS(const XMLString& elementId, const XMLString& idAttributeURI, const XMLString& idAttributeLocalName) const
{
	if (getAttributeNS(idAttributeURI, idAttributeLocalName) == elementId)
		return RefPtr<Element>(const_cast<Element*>(this), true);

	Node::Ptr pNode = firstChild();
	while (pNode)
	{
		if (pNode->nodeType() == Node::ELEMENT_NODE)
		{
			Element::Ptr pResult = pNode.unsafeCast<Element>()->getElementByIdNS(elementId, idAttributeURI, idAttributeLocalName);
			if (pResult) return pResult;
		}
		pNode = pNode->nextSibling();
	}
	return 0;
}


} } // namespace Poco::XML
