//
// AsyncReader.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  AsyncReader
//
// Implementation of the AsyncReader class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/AsyncReader.h"

namespace Poco {
namespace Redis {


AsyncReader::AsyncReader(Client& client) : _client(client),
	_activity(this, &AsyncReader::runActivity)
{
}


AsyncReader::~AsyncReader()
{
	stop();
}


void AsyncReader::runActivity()
{
	while(!_activity.isStopped())
	{
		try
		{
			RedisType::Ptr reply = _client.readReply();
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
