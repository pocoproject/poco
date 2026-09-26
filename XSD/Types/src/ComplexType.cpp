//
// ComplexType.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  ComplexType
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/Any.h"


namespace Poco {
namespace XSD {
namespace Types {


ComplexType::ComplexType(const std::string& id, 
		const std::string& name,
		bool isAbstract,
		bool blockExtension,
		bool blockRestriction,
		bool finalExtension,
		bool finalRestriction,
		bool mixed):
	Type(id, name),
	_abstract(isAbstract),
	_blockExtension(blockExtension),
	_blockRestriction(blockRestriction),
	_finalExtension(finalExtension),
	_finalRestriction(finalRestriction),
	_mixed(mixed),
	_pParent(),
	_pContent(),
	_attrContent(),
	_containsAny(false)
{
}


ComplexType::~ComplexType()
{
}


void ComplexType::fixup()
{
	if (_pContent)
		_pContent->fixup();

	if (_pParent)
		_pParent->fixup();

	std::vector<AttributeContent::Ptr>::iterator it = _attrContent.begin();
	for (; it != _attrContent.end(); ++it)
	{
		(*it)->fixup();
	}

	if (!_pContent && !_pParent && !_attrContent.empty())
		_pContent = new Any("", 0, 1);
}


const std::vector<const Type*>& ComplexType::parents() const
{
	if (_pParent)
		return _pParent->parents();
	if (_pContent)
		return _pContent->types();

	return Type::NOPARENTS;
}


void ComplexType::accept(Visitor& v) const
{
	v.visit(*this);
}


void ComplexType::createIterator(std::vector<OrderIterator>& seq) const
{
	// if we inherit by restriction we overwrite the parent's order, otherwise we add to the parent's order
	if (_pContent)
		seq.push_back(_pContent->iterator());
}


} } } // namespace Poco::XSD::Types
