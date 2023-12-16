//
// RedisEventArgs.cpp
//
// Library: Redis
// Package: Redis
// Module:  RedisEventArgs
//
// Implementation of the RedisEventArgs class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Redis/RedisEventArgs.h"


namespace Poco {
namespace Redis {


RedisEventArgs::RedisEventArgs(RedisType::Ptr pMessage):
	_message(std::move(pMessage)),
	_exception(nullptr),
	_stop(false)
{
}


RedisEventArgs::RedisEventArgs(Exception* pException):
	_message(),
	_exception(pException ? pException->clone() : nullptr),
	_stop(false)
{
}


RedisEventArgs::~RedisEventArgs()
{
	delete _exception;
}


} } // namespace Poco::Redis
