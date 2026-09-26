//
// InheritanceInfo.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  InheritanceInfo
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/InheritanceInfo.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


InheritanceInfo::InheritanceInfo():
	_ref(),
	_restriction(false),
	_simpleContent(false),
	_parent(),
	_pSimple()
{
}


InheritanceInfo::~InheritanceInfo()
{
}


void InheritanceInfo::fixup()
{
	if (_parent.empty())
	{
		// find the element, throw exception if not found
		const Type* pParent = TypesManager::instance().getType(_ref);
		if (!pParent)
			throw NullTypeException("Failed to resolve type reference " + _ref.name());
		_parent.push_back(pParent);
		if (_pSimple)
			_pSimple->fixup();
	}
}


const Type* InheritanceInfo::type() const
{
	if (!_parent.empty())
		return _parent[0];
	return 0;
}


void InheritanceInfo::setType(const QName& parent)
{
	_ref = parent;
	_parent.clear();
	poco_assert (!_pSimple);
}


void InheritanceInfo::setRestrictionType(SimpleType::Ptr pInline)
{
	poco_assert (getRestriction());
	poco_assert (getSimpleContent());
	poco_check_ptr (pInline);
	_ref = QName::INVALID;
	_pSimple = pInline;
	const Type* pParent = _pSimple.get();
	_parent.clear();
	_parent.push_back(pParent);
}


void InheritanceInfo::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
