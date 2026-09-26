//
// ListTypeRef.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  ListTypeRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/ListTypeRef.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


ListTypeRef::ListTypeRef(const std::string& id, const QName& itemType):
	AbstractList(id),
	_itemType(itemType),
	_baseType()
{
}


ListTypeRef::~ListTypeRef()
{
}


void ListTypeRef::fixup()
{
	if (_baseType.empty())
	{
		// find the element, throw exception if not found
		const Type* pBaseType = TypesManager::instance().getType(_itemType);
		if (!pBaseType)
			throw NullTypeException("Failed to resolve type reference " + _itemType.name());
		_baseType.push_back(pBaseType);
	}
}


void ListTypeRef::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
