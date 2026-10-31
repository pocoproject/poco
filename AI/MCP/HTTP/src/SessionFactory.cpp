//
// SessionFactory.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  SessionFactory
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/SessionFactory.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/String.h"


namespace Poco::AI::MCP::HTTP {


std::unique_ptr<Poco::Net::HTTPClientSession> createClientSession(const Poco::URI& uri)
{
	if (Poco::icompare(uri.getScheme(), std::string("http")) == 0)
	{
		auto pSession = std::make_unique<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
		// The constructor applied the global proxy; the factory's replaces it only when set.
		if (!Poco::Net::HTTPSessionFactory::defaultFactory().getProxyConfig().host.empty())
		{
			pSession->setProxyConfig(Poco::Net::HTTPSessionFactory::defaultFactory().getProxyConfig());
		}
		return pSession;
	}
	return std::unique_ptr<Poco::Net::HTTPClientSession>(Poco::Net::HTTPSessionFactory::defaultFactory().createClientSession(uri));
}


} // namespace Poco::AI::MCP::HTTP
