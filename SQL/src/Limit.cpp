//
// Limit.cpp
//
// Library: Data
// Package: DataCore
// Module:  Limit
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/Limit.h"


namespace Poco {
namespace SQL {


Limit::Limit(SizeT limitValue, bool hardLimit, bool lowerLimit) :
	_value(limitValue),
	_hardLimit(hardLimit),
	_isLowerLimit(lowerLimit)
{
}


Limit::~Limit()
{
}


} } // namespace Poco::SQL
