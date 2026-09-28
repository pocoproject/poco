//
// GroupRef.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  GroupRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/GroupRef.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/TypesManager.h"


namespace Poco::XSD::Types {


GroupRef::GroupRef(const std::string& id, const QName& ref, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	AbstractGroup(id, minOcc, maxOcc),
	_ref(ref)
{
}


GroupRef::~GroupRef() = default;


void GroupRef::fixup()
{
	if (!_pGroup)
	{
		_pGroup = TypesManager::instance().getGroup(_ref);
		if (!_pGroup)
			throw InvalidTypeException("GroupRef references invalid attribute group:" + _ref.name());
	}
}


void GroupRef::accept(Visitor& v) const
{
	v.visit(*this);
}


Order::Ptr GroupRef::getChild() const
{
	poco_assert_dbg (_pGroup);
	return _pGroup->getChild();
}


const std::string& GroupRef::name() const
{
	poco_assert_dbg (_pGroup);
	return _pGroup->name();
}


} // namespace Poco::XSD::Types
