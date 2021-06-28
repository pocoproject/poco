//
// HTTPSessionInstantiator.cpp
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPSessionInstantiator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPSessionInstantiator.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPClientSession.h"


using Poco::URI;


namespace Poco {
namespace Net {


HTTPSessionInstantiator::HTTPSessionInstantiator()
{
}


HTTPSessionInstantiator::~HTTPSessionInstantiator()
{
}


HTTPClientSession* HTTPSessionInstantiator::createClientSession(const Poco::URI& uri)
{
	poco_assert (uri.getScheme() == "http");
	HTTPClientSession* pSession = new HTTPClientSession(uri.getHost(), uri.getPort());
	if (!getProxyConfig().host.empty())
	{
		pSession->setProxyConfig(getProxyConfig());
	}
	return pSession;
}


void HTTPSessionInstantiator::registerInstantiator()
{
	HTTPSessionFactory::defaultFactory().registerProtocol("http", new HTTPSessionInstantiator);
}


void HTTPSessionInstantiator::unregisterInstantiator()
{
	HTTPSessionFactory::defaultFactory().unregisterProtocol("http");
}


void HTTPSessionInstantiator::setProxyConfig(const HTTPClientSession::ProxyConfig& proxyConfig)
{
	_proxyConfig = proxyConfig;
}



} } // namespace Poco::Net
