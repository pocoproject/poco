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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
