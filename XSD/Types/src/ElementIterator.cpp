//
// ElementIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  ElementIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


ElementIterator::ElementIterator(Element& elem):
	_pElem(&elem),
	_next()
{
	_next.insert(elem.name());
}


ElementIterator::~ElementIterator() = default;

OrderContent::Ptr ElementIterator::next(const std::string& name)
{
	if (_pElem->name() == name)
	{
		if (_pElem->getMaxOccurs() > _cnt)
		{
			++_cnt;
			return OrderContent::Ptr(_pElem, true);
		}
		throw IllegalOrderException("MaxOccurs violated for element " + name);
	}

	throw IllegalOrderException("Illegal element " + name);
}


const std::set<std::string>& ElementIterator::validNexts() const
{
	if (_pElem->getMaxOccurs() <= _cnt)
		_next.clear();

	return _next;
}


bool ElementIterator::validNext(const std::string& name) const
{
	return (validNexts().find(name) != _next.end());
}


bool ElementIterator::end() const
{
	return validNexts().empty();
}


void ElementIterator::close()
{
	if (_cnt < _pElem->getMinOccurs())
		throw IllegalOrderException("MinOccurs violated for element " + _pElem->name());
	_cnt = _pElem->getMaxOccurs();
}


bool ElementIterator::canClose() const
{
	return (_cnt >= _pElem->getMinOccurs());
}


void ElementIterator::reset()
{
	_cnt = 0;
	_next.insert(_pElem->name());
}


} // namespace Poco::XSD::Types
