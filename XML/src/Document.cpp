//
// Document.cpp
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


#include "Poco/DOM/Document.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/Comment.h"
#include "Poco/DOM/CDATASection.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/EntityReference.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/ElementsByTagNameList.h"
#include "Poco/DOM/Entity.h"
#include "Poco/DOM/Notation.h"
#include "Poco/XML/Name.h"
#include "Poco/XML/NamePool.h"


namespace Poco {
namespace XML {


const XMLString Document::NODE_NAME = toXMLString("#document");


Document::Document(NamePool::Ptr pNamePool):
	AbstractContainerNode(0),
	_pDocumentType(0),
	_eventSuspendLevel(0)
{
	if (pNamePool)
	{
		_pNamePool = pNamePool;
	}
	else
	{
		_pNamePool = new NamePool;
	}
}


Document::Document(unsigned long namePoolSize):
	AbstractContainerNode(0),
	_pDocumentType(0),
	_pNamePool(new NamePool(namePoolSize)),
	_eventSuspendLevel(0)
{
}


Document::Document(DocumentType::Ptr pDocumentType, NamePool::Ptr pNamePool):
	AbstractContainerNode(0),
	_pDocumentType(pDocumentType),
	_eventSuspendLevel(0)
{
	if (pNamePool)
	{
		_pNamePool = pNamePool;
	}
	else
	{
		_pNamePool = new NamePool;
	}
	if (_pDocumentType)
	{
		_pDocumentType->setOwnerDocument(Ptr(this, true));
	}
}


Document::Document(DocumentType::Ptr pDocumentType, unsigned long namePoolSize):
	AbstractContainerNode(0),
	_pDocumentType(pDocumentType),
	_pNamePool(new NamePool(namePoolSize)),
	_eventSuspendLevel(0)
{
	if (_pDocumentType)
	{
		_pDocumentType->setOwnerDocument(Ptr(this, true));
	}
}


Document::~Document()
{
}


bool Document::dispatchEvent(Event::Ptr evt)
{
	return _eventSuspendLevel > 0 || AbstractContainerNode::dispatchEvent(evt);
}


void Document::suspendEvents()
{
	++_eventSuspendLevel;
}


void Document::resumeEvents()
{
	poco_assert_dbg (_eventSuspendLevel > 0);

	--_eventSuspendLevel;
}


const DOMImplementation& Document::implementation() const
{
	return DOMImplementation::instance();
}


Element::Ptr Document::documentElement() const
{
	// Skip non-element nodes before the document element
	Node::Ptr pCur = firstChild();
	while (pCur)
	{
		if (pCur.cast<Element>())
			return pCur.cast<Element>();
		pCur = pCur->nextSibling();
	}
	return 0;
}


Element::Ptr Document::createElement(const XMLString& tagName) const
{
	return new Element(Ptr(const_cast<Document*>(this), true), EMPTY_STRING, EMPTY_STRING, tagName);
}


DocumentFragment::Ptr Document::createDocumentFragment() const
{
	return new DocumentFragment(Ptr(const_cast<Document*>(this), true));
}


Text::Ptr Document::createTextNode(const XMLString& data) const
{
	return new Text(Ptr(const_cast<Document*>(this), true), data);
}


Comment::Ptr Document::createComment(const XMLString& data) const
{
	return new Comment(Ptr(const_cast<Document*>(this), true), data);
}


CDATASection::Ptr Document::createCDATASection(const XMLString& data) const
{
	return new CDATASection(Ptr(const_cast<Document*>(this), true), data);
}


ProcessingInstruction::Ptr Document::createProcessingInstruction(const XMLString& target, const XMLString& data) const
{
	return new ProcessingInstruction(Ptr(const_cast<Document*>(this), true), target, data);
}


Attr::Ptr Document::createAttribute(const XMLString& name) const
{
	return new Attr(Ptr(const_cast<Document*>(this), true), EMPTY_STRING, EMPTY_STRING, name, EMPTY_STRING);
}


EntityReference::Ptr Document::createEntityReference(const XMLString& name) const
{
	return new EntityReference(Ptr(const_cast<Document*>(this), true), name);
}


NodeList::Ptr Document::getElementsByTagName(const XMLString& name) const
{
	return new ElementsByTagNameList(Ptr(const_cast<Document*>(this), true), name);
}


const XMLString& Document::nodeName() const
{
	return NODE_NAME;
}


unsigned short Document::nodeType() const
{
	return Node::DOCUMENT_NODE;
}


Node::Ptr Document::importNode(Node::Ptr importedNode, bool deep)
{
	return importedNode.cast<AbstractNode>()->copyNode(deep, Ptr(this, true));
}


Element::Ptr Document::createElementNS(const XMLString& namespaceURI, const XMLString& qualifiedName) const
{
	return new Element(Ptr(const_cast<Document*>(this), true), namespaceURI, Name::localName(qualifiedName), qualifiedName);
}


Attr::Ptr Document::createAttributeNS(const XMLString& namespaceURI, const XMLString& qualifiedName) const
{
	return new Attr(Ptr(const_cast<Document*>(this), true), namespaceURI, Name::localName(qualifiedName), qualifiedName, EMPTY_STRING);
}


NodeList::Ptr Document::getElementsByTagNameNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	return new ElementsByTagNameListNS(Ptr(const_cast<Document*>(this), true), namespaceURI, localName);
}


Element::Ptr Document::getElementById(const XMLString& elementId) const
{
	return 0;
}


Event::Ptr Document::createEvent(const XMLString& eventType) const
{
	if (eventType == MutationEvent::DOMSubtreeModified          ||
	    eventType == MutationEvent::DOMNodeInserted             ||
		eventType == MutationEvent::DOMNodeRemoved              ||
		eventType == MutationEvent::DOMNodeRemovedFromDocument  ||
		eventType == MutationEvent::DOMNodeInsertedIntoDocument ||
		eventType == MutationEvent::DOMAttrModified             ||
		eventType == MutationEvent::DOMCharacterDataModified)
	{
		return new MutationEvent(Document::Ptr(const_cast<Document*>(this), true), eventType);
	}
	throw DOMException(DOMException::NOT_SUPPORTED_ERR);
}


Node::Ptr Document::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	throw DOMException(DOMException::NOT_SUPPORTED_ERR);
}


void Document::setDoctype(DocumentType::Ptr pDoctype)
{
	_pDocumentType = pDoctype;
	if (_pDocumentType)
	{
		_pDocumentType->setOwnerDocument(Ptr(this, true));
	}
}


DocumentType::Ptr Document::doctype() const
{
	return _pDocumentType;
}


DocumentType::Ptr Document::getDoctype()
{
	return _pDocumentType;
}


bool Document::eventsSuspended() const
{
	return _eventSuspendLevel > 0;
}


bool Document::events() const
{
	return _eventSuspendLevel == 0;
}


Entity::Ptr Document::createEntity(const XMLString& name, const XMLString& publicId, const XMLString& systemId, const XMLString& notationName) const
{
	return new Entity(Document::Ptr(const_cast<Document*>(this), true), name, publicId, systemId, notationName);
}


Notation::Ptr Document::createNotation(const XMLString& name, const XMLString& publicId, const XMLString& systemId) const
{
	return new Notation(Document::Ptr(const_cast<Document*>(this), true), name, publicId, systemId);
}


Element::Ptr Document::getElementById(const XMLString& elementId, const XMLString& idAttribute) const
{
	Element::Ptr pElem = documentElement();
	if (pElem) pElem = pElem->getElementById(elementId, idAttribute);
	return pElem;
}


Element::Ptr Document::getElementByIdNS(const XMLString& elementId, const XMLString& idAttributeURI, const XMLString& idAttributeLocalName) const
{
	Element::Ptr pElem = documentElement();
	if (pElem) pElem = pElem->getElementByIdNS(elementId, idAttributeURI, idAttributeLocalName);
	return pElem;
}


} } // namespace Poco::XML
