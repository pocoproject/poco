//
// TransportFactory.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  TransportFactory
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/TransportFactory.h"
#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Net/HTTPSessionInstantiator.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


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


} } } // namespace Poco::RemotingNG::REST
