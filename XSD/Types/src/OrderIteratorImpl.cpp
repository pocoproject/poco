//
// OrderIteratorImpl.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  OrderIteratorImpl
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/OrderIteratorImpl.h"


namespace Poco {
namespace XSD {
namespace Types {


OrderIteratorImpl::OrderIteratorImpl():
	_closed(false)
{
}


OrderIteratorImpl::~OrderIteratorImpl()
{
}


} } } // namespace Poco::XSD::Types
