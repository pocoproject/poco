//
// TransportFactory.cpp
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  TransportFactory
//
// Copyright (c) 2006-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/HTTP/TransportFactory.h"
#include "Poco/RemotingNG/HTTP/Transport.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Net/HTTPSessionInstantiator.h"


namespace Poco {
namespace RemotingNG {
namespace HTTP {


TransportFactory::TransportFactory()
{
}


TransportFactory::~TransportFactory()
{
}


Poco::RemotingNG::Transport* TransportFactory::createTransport()
{
	return new Transport;
}


void TransportFactory::registerFactory()
{
	Transport::httpSessionFactory().registerProtocol("http", new Poco::Net::HTTPSessionInstantiator);
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL, new TransportFactory);
}


void TransportFactory::unregisterFactory()
{
	Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(Transport::PROTOCOL);
	Transport::httpSessionFactory().unregisterProtocol("http");
}


} } } // namespace Poco::RemotingNG::HTTP
