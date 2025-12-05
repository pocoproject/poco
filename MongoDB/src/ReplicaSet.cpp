//
// ReplicaSet.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSet
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/OpMsgMessage.h"


namespace Poco {
namespace MongoDB {


ReplicaSet::ReplicaSet(const std::vector<Net::SocketAddress> &addresses):
	_addresses(addresses)
{
}


ReplicaSet::~ReplicaSet()
{
}


Connection::Ptr ReplicaSet::findMaster()
{
	Connection::Ptr master;

	for (std::vector<Net::SocketAddress>::iterator it = _addresses.begin(); it != _addresses.end(); ++it)
	{
		master = isMaster(*it);
		if (!master.isNull())
		{
			break;
		}
	}

	return master;
}


Connection::Ptr ReplicaSet::isMaster(const Net::SocketAddress& address)
{
	Connection::Ptr conn = new Connection();

	try
	{
		conn->connect(address);

		OpMsgMessage request("admin", "");
		request.setCommandName(OpMsgMessage::CMD_HELLO);

		OpMsgMessage response;
		conn->sendRequest(request, response);

		if (response.responseOk())
		{
			const Document& doc = response.body();
			if (doc.get<bool>("isWritablePrimary", false) || doc.get<bool>("ismaster", false))
			{
				return conn;
			}
			else if (doc.exists("primary"))
			{
				return isMaster(Net::SocketAddress(doc.get<std::string>("primary")));
			}
		}
	}
	catch (...)
	{
		conn = nullptr;
	}

	return nullptr;
}


} } // namespace Poco::MongoDB
