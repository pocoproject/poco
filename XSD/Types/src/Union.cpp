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


namespace Poco::XSD::Types {


Union::Union(const std::string& id, const std::vector<QName>& memberTypes):
	SimpleTypeInheritance(id),
	_memberTypes(memberTypes)
{
}


Union::~Union() = default;


void Union::fixup()
{
	_allTypes.clear();
	TypesManager& tm = TypesManager::instance();
	for (const auto& memberType: _memberTypes)
	{
		const Type* pType = tm.getType(memberType);
		if (!pType)
			throw InvalidTypeException("Union references invalid simple type:" + memberType.name());
		_allTypes.push_back(pType);
	}

	for (auto& pInlineType: _inlineTypes)
	{
		if (pInlineType == nullptr)
		{
			throw InvalidTypeException("Union uses null type");
		}
		pInlineType->fixup();
		_allTypes.push_back(pInlineType);
	}
}


void Union::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
