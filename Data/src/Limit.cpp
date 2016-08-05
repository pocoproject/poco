//
// Limit.cpp
//
// $Id: //poco/Main/Data/src/Limit.cpp#5 $
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


#include "Poco/Data/Limit.h"


namespace Poco {
namespace Data {


Limit::Limit(SizeT limitValue, bool hardLimit, bool lowerLimit) :
	_value(limitValue),
	_hardLimit(hardLimit),
	_isLowerLimit(lowerLimit)
{
}


Limit::~Limit()
{
}


} } // namespace Poco::Data
