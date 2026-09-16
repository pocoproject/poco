//
// TransportFactory.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  TransportFactory
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/Net/HTTPSessionInstantiator.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


TransportFactory::TransportFactory(Serializer::SOAPVersion soapVersion): 
	_soapVersion(soapVersion)
{
}


TransportFactory::~TransportFactory()
{
}


Poco::RemotingNG::Transport* TransportFactory::createTransport()
{
	return new Transport(_soapVersion);
}


void TransportFactory::registerFactory()
{
	Transport::httpSessionFactory().registerProtocol("http", new Poco::Net::HTTPSessionInstantiator);
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL, new TransportFactory(Serializer::SOAP_1_1));
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL_SOAP11, new TransportFactory(Serializer::SOAP_1_1));
	Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(Transport::PROTOCOL_SOAP12, new TransportFactory(Serializer::SOAP_1_2));
}


void TransportFactory::unregisterFactory()
{
	Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(Transport::PROTOCOL);
	Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(Transport::PROTOCOL_SOAP11);
	Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(Transport::PROTOCOL_SOAP12);
	Transport::httpSessionFactory().unregisterProtocol("http");
}


} } } // namespace Poco::RemotingNG::SOAP
