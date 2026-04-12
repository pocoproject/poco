//
// AsyncReader.cpp
//
// Library: Redis
// Package: Redis
// Module:  AsyncReader
//
// Implementation of the AsyncReader class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Redis/AsyncReader.h"


namespace Poco {
namespace Redis {


AsyncReader::AsyncReader(Client& client):
	_client(client),
	_activity(this, &AsyncReader::runActivity)
{
}


AsyncReader::~AsyncReader()
{
	stop();
}


void AsyncReader::runActivity()
{
	while (!_activity.isStopped())
	{
		try
		{
			// readReply() blocks on the socket read, so no sleep is needed
			// between iterations -- the blocking read yields the CPU naturally.
			RedisType::Ptr reply = _client.readReply();

			RedisEventArgs args(reply);
			redisResponse.notify(this, args);

			if (args.isStopped()) stop();
		}
		catch (Exception& e)
		{
			RedisEventArgs args(&e);
			redisException.notify(this, args);
			stop();
		}
	}
}


} } // namespace Poco::Redis
