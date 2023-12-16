//
// AbstractBinding.cpp
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


#include <utility>

#include "Poco/Data/AbstractBinding.h"


namespace Poco {
namespace Data {


AbstractBinding::AbstractBinding(std::string  name,
	Direction direction,
	Poco::UInt32 bulkSize):
	_pBinder(nullptr),
	_name(std::move(name)),
	_direction(direction),
	_bulkSize(bulkSize)
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
