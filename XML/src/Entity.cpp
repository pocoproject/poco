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


#include <utility>

#include "Poco/DOM/Entity.h"


namespace Poco {
namespace XML {


const XMLString Entity::NODE_NAME = toXMLString("#entity");


Entity::Entity(Document* pOwnerDocument, XMLString  name, XMLString  publicId, XMLString  systemId, XMLString  notationName):
	AbstractContainerNode(pOwnerDocument),
	_name(std::move(name)),
	_publicId(std::move(publicId)),
	_systemId(std::move(systemId)),
	_notationName(std::move(notationName))
{
}


Entity::Entity(Document* pOwnerDocument, const Entity& entity):
	AbstractContainerNode(pOwnerDocument, entity),
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


Node* Entity::copyNode(bool deep, Document* pOwnerDocument) const
{
	return new Entity(pOwnerDocument, *this);
}


} } // namespace Poco::XML
