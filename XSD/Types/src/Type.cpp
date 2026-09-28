//
// Type.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Type
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/SequenceIterator.h"
#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco {
namespace XSD {
namespace Types {


const std::vector<const Type*> Type::NOPARENTS;


Type::Type():
	_name(),
	_pSchema(nullptr)
{
}


Type::Type(const std::string& id, const std::string& name):
	AnnotatedObject(id),
	_name(name),
	_pSchema(nullptr)
{
}


Type::~Type()
{
}


OrderIterator Type::iterator() const
{
	std::vector<OrderIterator> seq;
	iteratorRec(seq);
	return OrderIterator(new SequenceIterator(seq, 1));
}


void Type::iteratorRec(std::vector<OrderIterator>& seq) const
{
	const std::vector<const Type*> parent = parents();
	std::vector<const Type*>::const_iterator it = parent.begin();
	std::vector<const Type*>::const_iterator itEnd = parent.end();
	for (; it != itEnd; ++it)
		(*it)->iteratorRec(seq);

	createIterator(seq);
}


} } } // namespace Poco::XSD::Types
