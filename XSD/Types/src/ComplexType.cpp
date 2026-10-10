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
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/Any.h"


namespace Poco::XSD::Types {


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
	_attrContent()
{
}


ComplexType::~ComplexType() = default;


void ComplexType::fixup()
{
	if (_pContent)
		_pContent->fixup();

	if (_pParent)
		_pParent->fixup();

	for (auto& pAttr: _attrContent)
	{
		pAttr->fixup();
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


} // namespace Poco::XSD::Types
