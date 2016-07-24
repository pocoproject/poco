//
// AbstractBinding.cpp
//
// $Id: //poco/Main/Data/src/AbstractBinding.cpp#5 $
//
// Library: Data
// Package: DataCore
// Module:  AbstractBinding
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/AbstractBinding.h"


namespace Poco {
namespace Data {


AbstractBinding::AbstractBinding(const std::string& rName, 
	Direction direction, 
	Poco::UInt32 bindingBulkSize): 
	_pBinder(0),
	_name(rName),
	_direction(direction),
	_bulkSize(bindingBulkSize)
{
}


AbstractBinding::~AbstractBinding()
{
}


void AbstractBinding::setBinder(BinderPtr pBinder)
{
	poco_check_ptr (pBinder);
	_pBinder = pBinder;
}


} } // namespace Poco::Data
