//
// TransportFactory.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  TransportFactory
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/JSONRPC/TransportFactory.h"
#include "Poco/RemotingNG/JSONRPC/Transport.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Net/HTTPSessionInstantiator.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


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


} } } // namespace Poco::RemotingNG::JSONRPC
