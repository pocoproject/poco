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
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


TransportFactory::TransportFactory():
	_connectionManager(ConnectionManager::defaultManager())
{
}


TransportFactory::TransportFactory(ConnectionManager& connectionManager):
	_connectionManager(connectionManager)
{
}


TransportFactory::~TransportFactory()
{
}


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


} } } // namespace Poco::RemotingNG::TCP
