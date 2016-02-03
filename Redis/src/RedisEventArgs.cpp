//
// RedisEventArgs.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  RedisEventArgs
//
// Implementation of the RedisEventArgs class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/RedisEventArgs.h"

namespace Poco {
namespace Redis {

RedisEventArgs::RedisEventArgs(RedisType::Ptr message) :
	_message(message),
	_exception(0),
	_stop(false)
{
}

RedisEventArgs::RedisEventArgs(Exception* exception) :
	_message(),
	_exception(exception ? exception->clone() : 0),
	_stop(false)
{
}

RedisEventArgs::~RedisEventArgs()
{
	delete _exception;
}


}} // namespace Poco::Redis