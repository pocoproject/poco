//
// AsyncClient.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  AsyncClient
//
// Implementation of the AsyncClient class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/AsyncClient.h"

namespace Poco {
namespace Redis {


AsyncClient::AsyncClient() : Client(),
	_activity(this, &AsyncClient::runActivity)
{
}


AsyncClient::AsyncClient(const std::string& hostAndPort) : Client(hostAndPort),
	_activity(this, &AsyncClient::runActivity)
{
}


AsyncClient::AsyncClient(const std::string& host, int port) : Client(host, port),
	_activity(this, &AsyncClient::runActivity)
{
}


AsyncClient::AsyncClient(const Net::SocketAddress& addrs) : Client(addrs),
	_activity(this, &AsyncClient::runActivity)
{
}


AsyncClient::~AsyncClient()
{
	stop();
}


void AsyncClient::runActivity()
{
	while(!_activity.isStopped())
	{
		try
		{
			RedisType::Ptr reply = readReply();
			redisResponse.notify(this, reply);
		}
		catch(TimeoutException&)
		{
			continue;
		}
		catch(Exception &)
		{
			stop();
		}
	}
}


} } // Poco::Redis
