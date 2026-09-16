//
// Proxy.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  Proxy
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Proxy.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"


namespace Poco {
namespace RemotingNG {


Proxy::Proxy(const Identifiable::ObjectId& oid): 
	Identifiable(oid)
{
}


Proxy::~Proxy()
{
}


void Proxy::remoting__connect(const std::string& protocol, const std::string& uri)
{
	_pTransport = TransportFactoryManager::instance().createTransport(protocol, uri);
	poco_check_ptr (_pTransport);
	remoting__setURI(Poco::URI(uri));
}


void Proxy::remoting__disconnect()
{
	_pTransport = 0;
}


void Proxy::remoting__setEventURI(const Poco::URI& eventURI)
{
	_eventURI = eventURI;
}


} } // namespace Poco::RemotingNG
