//
// ElementImpl.cpp
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementImpl
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ElementImpl.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


ElementImpl::ElementImpl():
	AbstractElementImpl()
{
}


ElementImpl::ElementImpl(
		const std::string& id, 
		Poco::UInt32 minOcc, 
		Poco::UInt32 maxOcc,
		bool isAbstract,
		bool blockRestriction,
		bool blockExtension,
		bool blockSubstitution,
		const std::string& defaultValue,
		bool finalRestriction,
		bool finalExtension,
		const std::string& fixedValue,
		bool qualified,
		const std::string& name,
		const std::string& nameSpace,
		bool nillable,
		const QName& substitutionGroup): 
	AbstractElementImpl(
		id, 
		minOcc, 
		maxOcc,
		isAbstract,
		blockRestriction,
		blockExtension,
		blockSubstitution,
		defaultValue,
		finalRestriction,
		finalExtension,
		fixedValue,
		qualified,
		name,
		nameSpace,
		nillable,
		substitutionGroup)
{
}


ElementImpl::~ElementImpl()
{
	if (_pType)
		_pType->release();
}


void ElementImpl::setType(Type::Ptr pType)
{
	poco_assert (!_pType);

	_pType = pType.get();
	_pType->duplicate();
}


void ElementImpl::accept(Visitor& v) const
{
	v.visit(*this);
}


void ElementImpl::fixup()
{
	if (_pType)
	{
		const_cast<Type*>(_pType)->fixup();
	}
	else
	{
		_pType = TypesManager::instance().getType(QName(TypesManager::XSD_TYPE_ANYTYPE, TypesManager::XSD_NAMESPACE));
		_pType->duplicate();
	}
}


} // namespace Poco::XSD::Types
