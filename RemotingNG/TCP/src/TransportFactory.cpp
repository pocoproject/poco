//
// TransportFactory.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  TransportFactory
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"


namespace Poco::RemotingNG::TCP {


TransportFactory::TransportFactory():
	_connectionManager(ConnectionManager::defaultManager())
{
}


TransportFactory::TransportFactory(ConnectionManager& connectionManager):
	_connectionManager(connectionManager)
{
}


TransportFactory::~TransportFactory() = default;


Poco::RemotingNG::Transport* TransportFactory::createTransport()
{
	return new Transport(_connectionManager);
}


void TransportFactory::registerFactory()
{
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL, new TransportFactory);
}


void TransportFactory::registerFactory(ConnectionManager& connectionManager)
{
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL, new TransportFactory(connectionManager));
}


void TransportFactory::unregisterFactory()
{
	Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(Transport::PROTOCOL);
}


} // namespace Poco::RemotingNG::TCP
