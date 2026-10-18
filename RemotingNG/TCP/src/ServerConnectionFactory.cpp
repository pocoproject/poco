//
// ServerConnectionFactory.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ServerConnectionFactory
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/ServerConnectionFactory.h"
#include "Poco/RemotingNG/TCP/ServerConnection.h"


namespace Poco::RemotingNG::TCP {

	
ServerConnectionFactory::ServerConnectionFactory(Listener::Ptr pListener):
	_pListener(pListener)
{
}


ServerConnectionFactory::~ServerConnectionFactory() = default;


Poco::Net::TCPServerConnection* ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket& socket)
{
	return new ServerConnection(_pListener, socket);
}


} // namespace Poco::RemotingNG::TCP
