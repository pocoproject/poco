//
// Bulk.cpp
//
// Library: Data
// Package: DataCore
// Module:  Bulk
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/Bulk.h"


namespace Poco {
namespace SQL {


Bulk::Bulk(const Limit& rLimit): _limit(rLimit.value(), false, false)
{
}


Bulk::Bulk(Poco::UInt32 value): _limit(value, false, false)
{
}


Bulk::~Bulk()
{
}


} } // namespace Poco::SQL
