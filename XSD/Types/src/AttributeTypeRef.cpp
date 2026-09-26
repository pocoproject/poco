//
// AttributeTypeRef.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeTypeRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AttributeTypeRef.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/SimpleType.h"


namespace Poco {
namespace XSD {
namespace Types {


AttributeTypeRef::AttributeTypeRef(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const QName& typeRef, 
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use):
	AbstractAttribute(id, name),
	_nameSpace(nameSpace),
	_typeRef(typeRef),
	_fixedValue(fixedValue),
	_defaultValue(defaultValue),
	_qualifiedForm(qualifiedForm),
	_use(use),
	_pType(0)
{
}


AttributeTypeRef::AttributeTypeRef(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const QName& typeRef, 
		const SimpleType* pType,
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use):
	AbstractAttribute(id, name),
	_nameSpace(nameSpace),
	_typeRef(typeRef),
	_fixedValue(fixedValue),
	_defaultValue(defaultValue),
	_qualifiedForm(qualifiedForm),
	_use(use),
	_pType(pType)
{
	poco_check_ptr (_pType);
}


AttributeTypeRef::~AttributeTypeRef()
{
}


const std::string& AttributeTypeRef::nameSpace() const
{
	return _nameSpace;
}


const std::string& AttributeTypeRef::defaultValue() const
{
	return _defaultValue;
}


const std::string& AttributeTypeRef::fixedValue() const
{
	return _fixedValue;
}


bool AttributeTypeRef::qualifiedForm() const
{
	return _qualifiedForm;
}


const SimpleType* AttributeTypeRef::type() const
{
	return _pType;
}


void AttributeTypeRef::fixup()
{
	if (!_pType)
	{
		const Type* pType = TypesManager::instance().getType(_typeRef);
		if (pType == 0)
			throw NullTypeException("Referenced type not found:" + _typeRef.name());

		_pType = dynamic_cast<const SimpleType*>(pType);
		if (_pType == 0)
			throw InvalidTypeException("AttributeTypeRef type reference to complex type. Simple type required:" + _typeRef.name());
	}
}


AbstractAttribute::Usage AttributeTypeRef::usage() const
{
	return _use;
}


void AttributeTypeRef::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
