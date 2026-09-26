//
// AbstractElementImpl.cpp
//
// Library: XSD/Types
// Package: XSDElements
// Module:  AbstractElementImpl
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AbstractElementImpl.h"


namespace Poco {
namespace XSD {
namespace Types {


AbstractElementImpl::AbstractElementImpl():
		_abstract(false),
		_blockRestriction(false),
		_blockExtension(false),
		_blockSubstitution(false),
		_default(),
		_finalRestriction(false),
		_finalExtension(false),
		_fixed(),
		_qualified(false),
		_name(),
		_nameSpace(),
		_nillable(false),
		_substitutionGroup(QName::INVALID)
{
}


AbstractElementImpl::AbstractElementImpl(
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
	Element(id, minOcc, maxOcc),
	_abstract(isAbstract),
	_blockRestriction(blockRestriction),
	_blockExtension(blockExtension),
	_blockSubstitution(blockSubstitution),
	_default(defaultValue),
	_finalRestriction(finalRestriction),
	_finalExtension(finalExtension),
	_fixed(fixedValue),
	_qualified(qualified),
	_name(name),
	_nameSpace(nameSpace),
	_nillable(nillable),
	_substitutionGroup(substitutionGroup)
{
}


AbstractElementImpl::~AbstractElementImpl()
{
}


const std::string& AbstractElementImpl::nameSpace() const
{
	return _nameSpace;
}


} } } // namespace Poco::XSD::Types
