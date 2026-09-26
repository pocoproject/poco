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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AnyIterator.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Any.h"


namespace Poco {
namespace XSD {
namespace Types {


AnyIterator::AnyIterator(Any& any):
	_min(any.getMinOccurs()),
	_max(any.getMaxOccurs()),
	_cnt(0),
	_pAny(&any),
	_next()
{
	_next.insert("*");
}


AnyIterator::~AnyIterator()
{
}


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


} } } // namespace Poco::XSD::Types
