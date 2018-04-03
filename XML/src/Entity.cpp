//
// Entity.cpp
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


#include "Poco/DOM/Entity.h"


namespace Poco {
namespace XML {


const XMLString Entity::NODE_NAME = toXMLString("#entity");


Entity::Entity(Document::Ptr pOwnerDocument, const XMLString& name, const XMLString& publicId, const XMLString& systemId, const XMLString& notationName):
	AbstractContainerNode(pOwnerDocument),
	_name(name),
	_publicId(publicId),
	_systemId(systemId),
	_notationName(notationName)
{
}


Entity::Entity(Document::Ptr pOwnerDocument, const Entity& entity):
	AbstractContainerNode(pOwnerDocument),
	_name(entity._name),
	_publicId(entity._publicId),
	_systemId(entity._systemId),
	_notationName(entity._notationName)
{
}


Entity::~Entity()
{
}


const XMLString& Entity::nodeName() const
{
	return _name;
}


unsigned short Entity::nodeType() const
{
	return Node::ENTITY_NODE;
}


Node::Ptr Entity::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	return new Entity(pOwnerDocument, *this);
}


} } // namespace Poco::XML
