//
// MutationEvent.cpp
//
// $Id: //poco/1.4/XML/src/MutationEvent.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOMEvents
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/MutationEvent.h"


namespace Poco {
namespace XML {


const XMLString MutationEvent::DOMSubtreeModified          = toXMLString("DOMSubtreeModified");
const XMLString MutationEvent::DOMNodeInserted             = toXMLString("DOMNodeInserted");
const XMLString MutationEvent::DOMNodeRemoved              = toXMLString("DOMNodeRemoved");
const XMLString MutationEvent::DOMNodeRemovedFromDocument  = toXMLString("DOMNodeRemovedFromDocument");
const XMLString MutationEvent::DOMNodeInsertedIntoDocument = toXMLString("DOMNodeInsertedIntoDocument");
const XMLString MutationEvent::DOMAttrModified             = toXMLString("DOMAttrModified");
const XMLString MutationEvent::DOMCharacterDataModified    = toXMLString("DOMCharacterDataModified");


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& rType): 
	Event(pOwnerDocument, rType, 0, true, false),
	_change(MODIFICATION),
	_pRelatedNode(0)
{
}


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& rType, EventTarget* pTarget, bool canBubble, bool isCancelable, Node* pRelatedNode):
	Event(pOwnerDocument, rType, pTarget, canBubble, isCancelable),
	_change(MODIFICATION),
	_pRelatedNode(pRelatedNode)
{
}


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& rType, EventTarget* pTarget, bool canBubble, bool isCancelable, Node* pRelatedNode, 
	                         const XMLString& rPrevValue, const XMLString& rNewValue, const XMLString& rAttrName, AttrChangeType change):
	Event(pOwnerDocument, rType, pTarget, canBubble, isCancelable),
	_prevValue(rPrevValue),
	_newValue(rNewValue),
	_attrName(rAttrName),
	_change(change),
	_pRelatedNode(pRelatedNode)
{
}


MutationEvent::~MutationEvent()
{
}


void MutationEvent::initMutationEvent(const XMLString& rType, bool canBubble, bool isCancelable, Node* pRelatedNode, 
	                                  const XMLString& rPrevValue, const XMLString& rNewValue, const XMLString& rAttrName, AttrChangeType change)
{
	initEvent(rType, canBubble, isCancelable);
	_pRelatedNode = pRelatedNode;
	_prevValue    = rPrevValue;
	_newValue     = rNewValue;
	_attrName     = rAttrName;
	_change       = change;
}

	 
} } // namespace Poco::XML
