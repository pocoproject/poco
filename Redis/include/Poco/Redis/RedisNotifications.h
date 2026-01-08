//
// RedisNotifications.h
//
// Library: Redis
// Package: Redis
// Module:  RedisNotifications
//
// Definition of Redis notification classes.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_RedisNotifications_INCLUDED
#define Redis_RedisNotifications_INCLUDED


#include "Poco/Redis/Redis.h"
#include "Poco/Notification.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Redis {


class Redis_API RedisNotification: public Notification
	/// Base class for all Redis notifications.
{
public:
	RedisNotification();
		/// Creates the RedisNotification.

	virtual ~RedisNotification();
		/// Destroys the RedisNotification.
};


class Redis_API RedisConnectNotification: public RedisNotification
	/// Notification sent when a Redis client successfully connects.
{
public:
	RedisConnectNotification();
		/// Creates the RedisConnectNotification.

	virtual ~RedisConnectNotification();
		/// Destroys the RedisConnectNotification.
};


class Redis_API RedisDisconnectNotification: public RedisNotification
	/// Notification sent when a Redis client disconnects.
{
public:
	RedisDisconnectNotification();
		/// Creates the RedisDisconnectNotification.

	virtual ~RedisDisconnectNotification();
		/// Destroys the RedisDisconnectNotification.
};


class Redis_API RedisErrorNotification: public RedisNotification
	/// Notification sent when a Redis error occurs.
{
public:
	RedisErrorNotification(const Exception& error);
		/// Creates the RedisErrorNotification with the given error.

	RedisErrorNotification(const std::string& message);
		/// Creates the RedisErrorNotification with the given error message.

	virtual ~RedisErrorNotification();
		/// Destroys the RedisErrorNotification.

	[[nodiscard]]
	const Exception& error() const;
		/// Returns the error that caused this notification.

	[[nodiscard]]
	const std::string& message() const;
		/// Returns the error message.

private:
	Exception* _pError;
	std::string _message;
};


//
// inlines
//


inline const Exception& RedisErrorNotification::error() const
{
	if (_pError)
		return *_pError;
	else
		throw InvalidAccessException("No exception available");
}


inline const std::string& RedisErrorNotification::message() const
{
	return _message;
}


} } // namespace Poco::Redis


#endif // Redis_RedisNotifications_INCLUDED
