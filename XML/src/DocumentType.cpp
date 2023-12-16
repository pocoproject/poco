//
// DocumentType.cpp
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


#include <utility>

#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DTDMap.h"
#include "Poco/DOM/DOMException.h"


namespace Poco {
namespace XML {


DocumentType::DocumentType(Document* pOwner, XMLString  name, XMLString  publicId, XMLString  systemId):
	AbstractContainerNode(pOwner),
	_name(std::move(name)),
	_publicId(std::move(publicId)),
	_systemId(std::move(systemId))
{
}


DocumentType::DocumentType(Document* pOwner, const DocumentType& doctype):
	AbstractContainerNode(pOwner, doctype),
	_name(doctype._name),
	_publicId(doctype._publicId),
	_systemId(doctype._systemId)
{
}


DocumentType::~DocumentType()
= default;


NamedNodeMap* DocumentType::entities() const
{
	return new DTDMap(this, Node::ENTITY_NODE);
}


NamedNodeMap* DocumentType::notations() const
{
	return new DTDMap(this, Node::NOTATION_NODE);
}


const XMLString& DocumentType::nodeName() const
{
	return _name;
}


unsigned short DocumentType::nodeType() const
{
	return Node::DOCUMENT_TYPE_NODE;
}


const XMLString& DocumentType::internalSubset() const
{
	return EMPTY_STRING;
}


Node* DocumentType::copyNode(bool deep, Document* pOwnerDocument) const
{
	return new DocumentType(pOwnerDocument, *this);
}


} } // namespace Poco::XML
