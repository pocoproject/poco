//
// RedisNotifications.cpp
//
// Library: Redis
// Package: Redis
// Module:  RedisNotifications
//
// Implementation of Redis notification classes.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Redis/RedisNotifications.h"


namespace Poco {
namespace Redis {


RedisNotification::RedisNotification()
{
}


RedisNotification::~RedisNotification()
{
}


RedisConnectNotification::RedisConnectNotification()
{
}


RedisConnectNotification::~RedisConnectNotification()
{
}


RedisDisconnectNotification::RedisDisconnectNotification()
{
}


RedisDisconnectNotification::~RedisDisconnectNotification()
{
}


RedisErrorNotification::RedisErrorNotification(const Exception& error):
	_pError(error.clone()),
	_message(error.displayText())
{
}


RedisErrorNotification::RedisErrorNotification(const std::string& message):
	_pError(nullptr),
	_message(message)
{
}


RedisErrorNotification::~RedisErrorNotification()
{
	delete _pError;
}


} } // namespace Poco::Redis
