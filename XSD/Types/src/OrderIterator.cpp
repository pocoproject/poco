//
// OrderIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  OrderIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco {
namespace XSD {
namespace Types {


OrderIterator::OrderIterator():
	_pImpl()
{
}


OrderIterator::OrderIterator(OrderIteratorImpl::Ptr pImpl):
	_pImpl(pImpl)
{
}


OrderIterator::~OrderIterator()
{
}


OrderContent::Ptr OrderIterator::next(const std::string& name)
{
	return _pImpl->next(name);
}


const std::set<std::string>& OrderIterator::validNexts() const
{
	return _pImpl->validNexts();
}


bool OrderIterator::validNext(const std::string& name) const
{
	return _pImpl->validNext(name);
}


bool OrderIterator::end() const
{
	if (_pImpl)
		return _pImpl->end();

	return true;
}


void OrderIterator::close()
{
	_pImpl->closeImpl();
}


bool OrderIterator::canClose() const
{
	return _pImpl->canClose();
}


bool OrderIterator::closed() const
{
	return _pImpl->closed();
}


void OrderIterator::reset()
{
	_pImpl->resetImpl();
}


} } } // namespace Poco::XSD::Types
