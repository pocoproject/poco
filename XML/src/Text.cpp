//
// Text.cpp
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


#include "Poco/DOM/Text.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMException.h"


namespace Poco {
namespace XML {


const XMLString Text::NODE_NAME = toXMLString("#text");


Text::Text(Document::Ptr& pOwnerDocument, const XMLString& data):
	CharacterData(std::move(pOwnerDocument), data)
{
}


Text::Text(Document::Ptr& pOwnerDocument, const Text& text):
	CharacterData(std::move(pOwnerDocument), text)
{
}


Text::Text(Document::Ptr&& pOwnerDocument, XMLString&& data):
		CharacterData(std::move(pOwnerDocument), std::move(data))
{
}


Text::Text(Document::Ptr&& pOwnerDocument, Text&& text):
		CharacterData(std::move(pOwnerDocument), std::move(text))
{
}


Text::~Text()
{
}


Text::Ptr Text::splitText(unsigned long offset)
{
	Node::Ptr pParent = parentNode();
	if (!pParent) throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
	int n = length() - offset;
	Text::Ptr pNew = ownerDocument()->createTextNode(substringData(offset, n));
	deleteData(offset, n);
	pParent->insertBefore(pNew, nextSibling());
	return pNew;
}


const XMLString& Text::nodeName() const
{
	return NODE_NAME;
}


unsigned short Text::nodeType() const
{
	return Node::TEXT_NODE;
}


XMLString Text::innerText() const
{
	return nodeValue();
}


Node::Ptr Text::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	return new Text(pOwnerDocument, *this);
}


} } // namespace Poco::XML
