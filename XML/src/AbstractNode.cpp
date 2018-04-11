//
// AbstractNode.cpp
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


#include "Poco/DOM/AbstractNode.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/ChildNodesList.h"
#include "Poco/DOM/EventDispatcher.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/EventException.h"
#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/NamedNodeMap.h"


namespace Poco {
namespace XML {


const XMLString AbstractNode::NODE_NAME = toXMLString("#node");
const XMLString AbstractNode::EMPTY_STRING;


AbstractNode::AbstractNode(Document* pOwnerDocument):
	_pOwner(pOwnerDocument)
{
}


AbstractNode::~AbstractNode()
{
}


const XMLString& AbstractNode::nodeName() const
{
	return NODE_NAME;
}


const XMLString& AbstractNode::getNodeValue() const
{
	return EMPTY_STRING;
}


void AbstractNode::setNodeValue(const XMLString& value)
{
	throw DOMException(DOMException::NO_DATA_ALLOWED_ERR);
}


Node::Ptr AbstractNode::parentNode() const
{
	return _pParent;
}


NodeList::Ptr AbstractNode::childNodes() const
{
	return new ChildNodesList(RefPtr<AbstractNode>(const_cast<AbstractNode*>(this), true));
}


Node::Ptr AbstractNode::firstChild() const
{
	return 0;
}


Node::Ptr AbstractNode::lastChild() const
{
	return 0;
}


Node::Ptr AbstractNode::previousSibling() const
{
	if (_pParent)
	{
		AbstractContainerNode::Ptr pParent = _pParent;
		AbstractNode::Ptr pSibling = pParent->_pFirstChild;
		while (!pSibling.isNull())
		{
			if (pSibling->_pNext == this) return pSibling.unsafeCast<Node>();
			pSibling = pSibling->_pNext;
		}
	}
	return 0;
}


Node::Ptr AbstractNode::nextSibling() const
{
	return _pNext;
}


NamedNodeMap::Ptr AbstractNode::attributes() const
{
	return nullptr;
}


Document::Ptr AbstractNode::ownerDocument() const
{
	return Document::Ptr(_pOwner, true);
}


Node::Ptr AbstractNode::insertBefore(Node::Ptr newChild, Node::Ptr refChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


Node::Ptr AbstractNode::replaceChild(Node::Ptr newChild, Node::Ptr oldChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


Node::Ptr AbstractNode::removeChild(Node::Ptr oldChild)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


Node::Ptr AbstractNode::appendChild(Node::Ptr newChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


bool AbstractNode::hasChildNodes() const
{
	return false;
}


Node::Ptr AbstractNode::cloneNode(bool deep) const
{
	return copyNode(deep, Document::Ptr(_pOwner, true));
}


void AbstractNode::normalize()
{
}


bool AbstractNode::isSupported(const XMLString& feature, const XMLString& version) const
{
	return DOMImplementation::instance().hasFeature(feature, version);
}


const XMLString& AbstractNode::namespaceURI() const
{
	return EMPTY_STRING;
}


XMLString AbstractNode::prefix() const
{
	return EMPTY_STRING;
}


const XMLString& AbstractNode::localName() const
{
	return EMPTY_STRING;
}


bool AbstractNode::hasAttributes() const
{
	return false;
}


XMLString AbstractNode::innerText() const
{
	return EMPTY_STRING;
}


Node::Ptr AbstractNode::getNodeByPath(const XMLString& path) const
{
	return 0;
}


Node::Ptr AbstractNode::getNodeByPathNS(const XMLString& path, const NSMap& nsMap) const
{
	return 0;
}


void AbstractNode::addEventListener(const XMLString& type, EventListener::Ptr listener, bool useCapture)
{
	if (_pEventDispatcher)
		_pEventDispatcher->removeEventListener(type, listener, useCapture);
	else
		_pEventDispatcher = new EventDispatcher;

	_pEventDispatcher->addEventListener(type, listener, useCapture);
}


void AbstractNode::removeEventListener(const XMLString& type, EventListener::Ptr listener, bool useCapture)
{
	if (_pEventDispatcher)
		_pEventDispatcher->removeEventListener(type, listener, useCapture);
}


bool AbstractNode::dispatchEvent(Event::Ptr evt)
{
	if (eventsSuspended()) return true;

	if (evt.isNull() || evt->type().empty())
		throw EventException(EventException::UNSPECIFIED_EVENT_TYPE_ERR);

	evt->setTarget(EventTarget::Ptr(this, true));
	evt->setCurrentPhase(Event::CAPTURING_PHASE);

	if (_pParent) _pParent->captureEvent(evt);

	if (_pEventDispatcher && !evt->isStopped())
	{
		evt->setCurrentPhase(Event::AT_TARGET);
		evt->setCurrentTarget(EventTarget::Ptr(this, true));
		_pEventDispatcher->dispatchEvent(evt);
	}
	if (!evt->isStopped() && evt->bubbles() && _pParent)
	{
		evt->setCurrentPhase(Event::BUBBLING_PHASE);
		_pParent->bubbleEvent(evt);
	}

	return evt->isCanceled();
}


void AbstractNode::captureEvent(Event::Ptr evt)
{
	if (_pParent)
		_pParent->captureEvent(evt);
	
	if (_pEventDispatcher && !evt->isStopped())
	{
		evt->setCurrentTarget(EventTarget::Ptr(this, true));
		_pEventDispatcher->captureEvent(evt);
	}
}


void AbstractNode::bubbleEvent(Event::Ptr evt)
{
	evt->setCurrentTarget(EventTarget::Ptr(this, true));
	if (_pEventDispatcher)
	{
		_pEventDispatcher->bubbleEvent(evt);
	}
	if (_pParent && !evt->isStopped())
		_pParent->bubbleEvent(evt);
}


void AbstractNode::dispatchSubtreeModified()
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMSubtreeModified, makeTarget(this), true, false, 0));
}


void AbstractNode::dispatchNodeInserted()
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMNodeInserted, makeTarget(this), true, false, parentNode()));
}


void AbstractNode::dispatchNodeRemoved()
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMNodeRemoved, makeTarget(this), true, false, parentNode()));
}


void AbstractNode::dispatchNodeRemovedFromDocument()
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMNodeRemovedFromDocument, makeTarget(this), false, false, 0));
}


void AbstractNode::dispatchNodeInsertedIntoDocument()
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMNodeInsertedIntoDocument, makeTarget(this), false, false, 0));
}


void AbstractNode::dispatchAttrModified(Attr::Ptr pAttr, MutationEvent::AttrChangeType changeType, const XMLString& prevValue, const XMLString& newValue)
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMAttrModified, makeTarget(this), true, false, pAttr, prevValue, newValue, pAttr->name(), changeType));
}


void AbstractNode::dispatchCharacterDataModified(const XMLString& prevValue, const XMLString& newValue)
{
	dispatchEvent(new MutationEvent(makeTarget(_pOwner), MutationEvent::DOMCharacterDataModified, makeTarget(this), true, false, 0, prevValue, newValue, EMPTY_STRING, MutationEvent::MODIFICATION));
}


bool AbstractNode::events() const
{
	if (_pOwner) return _pOwner->events();
	return false;
}


bool AbstractNode::eventsSuspended() const
{
	if (_pOwner) return _pOwner->eventsSuspended();
	return false;
}


void AbstractNode::setOwnerDocument(Document::Ptr pOwnerDocument)
{
	if (_pOwner) _pOwner->release();
	_pOwner = pOwnerDocument.get();
	if (_pOwner) _pOwner->duplicate();
}


} } // namespace Poco::XML
