//
// Union.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Union
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Union.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


Union::Union(const std::string& id, const std::vector<QName>& memberTypes):
	SimpleTypeInheritance(id),
	_memberTypes(memberTypes)
{
}


Union::~Union()
{
}


void Union::fixup()
{
	_allTypes.clear();
	std::vector<QName>::const_iterator itQN = _memberTypes.begin();
	std::vector<QName>::const_iterator itQNEnd = _memberTypes.end();
	TypesManager& tm = TypesManager::instance();
	for (; itQN != itQNEnd; ++itQN)
	{
		const Type* pType = tm.getType(*itQN);
		if (!pType)
			throw InvalidTypeException("Union references invalid simple type:" + itQN->name());
		_allTypes.push_back(pType);
	}

	std::vector<SimpleType::Ptr>::iterator it = _inlineTypes.begin();
	std::vector<SimpleType::Ptr>::iterator itEnd = _inlineTypes.end();
	for (; it != itEnd; ++it)
	{
		if (!(*it))
		{
			throw InvalidTypeException("Union uses null type");
		}
		(*it)->fixup();
		_allTypes.push_back(*it);
	}
}


void Union::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
