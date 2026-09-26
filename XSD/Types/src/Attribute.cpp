//
// Attribute.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  Attribute
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Attribute.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/SimpleType.h"


namespace Poco {
namespace XSD {
namespace Types {


Attribute::Attribute(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use):
	AbstractAttribute(id, name),
	_nameSpace(nameSpace),
	_fixedValue(fixedValue),
	_defaultValue(defaultValue),
	_qualifiedForm(qualifiedForm),
	_use(use),
	_pType()
{
}


Attribute::~Attribute()
{
}


const std::string& Attribute::defaultValue() const
{
	return _defaultValue;
}


const std::string& Attribute::fixedValue() const
{
	return _fixedValue;
}


bool Attribute::qualifiedForm() const
{
	return _qualifiedForm;
}


const SimpleType* Attribute::type() const
{
	return _pType.get();
}


const std::string& Attribute::nameSpace() const
{
	return _nameSpace;
}


void Attribute::fixup()
{
	if (_pType)
	{
		_pType->fixup();
	}
}


AbstractAttribute::Usage Attribute::usage() const
{
	return _use;
}


void Attribute::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
