//
// Exception.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Exception
//
// Definition of the Exception class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_Exception_INCLUDED
#define Redis_Exception_INCLUDED

#include "Poco/Redis/Redis.h"
#include <typeinfo>
#include "Poco/Exception.h"

namespace Poco {
namespace Redis {

POCO_DECLARE_EXCEPTION(Redis_API, RedisException, Exception)

}}

#endif // Redis_Exception_INCLUDED
