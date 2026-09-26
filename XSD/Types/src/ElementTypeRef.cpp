//
// ElementTypeRef.cpp
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementTypeRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ElementTypeRef.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


ElementTypeRef::ElementTypeRef():
	AbstractElementImpl(),
	_pType(0),
	_typeRef()
{
}


ElementTypeRef::ElementTypeRef(
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
		const QName& substitutionGroup,
		const QName& typeRef):
	AbstractElementImpl(id, 
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
		substitutionGroup),
	_pType(0),
	_typeRef(typeRef)
{
}


ElementTypeRef::~ElementTypeRef()
{
}


void ElementTypeRef::fixup()
{
	if (_pType == 0)
	{
		// find the element, throw exception if not found
		_pType = TypesManager::instance().getType(_typeRef);
		if (!_pType)
			throw NullTypeException("Failed to resolve type reference " + _typeRef.name());
	}
}


void ElementTypeRef::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
