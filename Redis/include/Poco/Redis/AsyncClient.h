//
// AsyncClient.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  AsyncClient
//
// Definition of the AsyncClient class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Redis_AsyncClient_INCLUDED
#define Redis_AsyncClient_INCLUDED


#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Client.h"
#include "Poco/Activity.h"

namespace Poco {
namespace Redis {


class Redis_API AsyncClient : public Client
{
public:


	BasicEvent<RedisType::Ptr> redisResponse;


	AsyncClient();
		/// Default constructor. Use this when you want to
		/// connect later on.

	AsyncClient(const std::string& hostAndPort);
		/// Constructor which connects to the given Redis host/port.
		/// The host and port must be separated with a colon.

	AsyncClient(const std::string& host, int port);
		/// Constructor which connects to the given Redis host/port.

	AsyncClient(const Net::SocketAddress& addrs);
		/// Constructor which connects to the given Redis host/port.

	virtual ~AsyncClient();
		/// Destructor

	bool isStopped();
		/// Returns true if the activity is not running, false when it is.

	void start();
		/// Starts the activity to read replies from the Redis server.

	void stop();
		/// Stops the read activity.

protected:

	void runActivity();

private:

	AsyncClient(const AsyncClient&);
	AsyncClient& operator = (const AsyncClient&);


	Activity<AsyncClient> _activity;

};


inline bool AsyncClient::isStopped()
{
	return _activity.isStopped();
}

inline void AsyncClient::start()
{
	_activity.start();
}

inline void AsyncClient::stop()
{
	_activity.stop();
}


} } // namespace Poco::Redis

#endif //Redis_Client_INCLUDED
