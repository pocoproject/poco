//
// AnyIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  AnyIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AnyIterator.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Any.h"


namespace Poco::XSD::Types {


AnyIterator::AnyIterator(Any& any):
	_min(any.getMinOccurs()),
	_max(any.getMaxOccurs()),
	_pAny(&any),
	_next()
{
	_next.insert("*");
}


AnyIterator::~AnyIterator() = default;


OrderContent::Ptr AnyIterator::next(const std::string& name)
{
	if (_cnt < _max)
	{
		++_cnt;
		return OrderContent::Ptr(_pAny, true);
	}

	throw IllegalOrderException("maxoccurs exceed in Any");
}


const std::set<std::string>& AnyIterator::validNexts() const
{
	if (_cnt >= _max)
		_next.clear();

	return _next;
}


bool AnyIterator::end() const
{
	return (_cnt >= _max);
}


bool AnyIterator::validNext(const std::string& name) const
{
	return (_cnt < _max);
}


void AnyIterator::close()
{
	_cnt = _max;
}


bool AnyIterator::canClose() const
{
	return (_cnt >= _min);
}


void AnyIterator::reset()
{
	_cnt = 0;
	_next.insert("*");
}


} // namespace Poco::XSD::Types
