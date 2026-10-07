//
// SessionFactory.cpp
//
// Library: AIMCPHTTP
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


namespace Poco {
namespace AI {
namespace MCP {
namespace HTTP {


std::unique_ptr<Poco::Net::HTTPClientSession> createClientSession(const Poco::URI& uri)
{
	if (Poco::icompare(uri.getScheme(), std::string("http")) == 0)
	{
		auto pSession = std::make_unique<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
		pSession->setProxyConfig(Poco::Net::HTTPSessionFactory::defaultFactory().getProxyConfig());
		return pSession;
	}
	return std::unique_ptr<Poco::Net::HTTPClientSession>(Poco::Net::HTTPSessionFactory::defaultFactory().createClientSession(uri));
}


} } } } // namespace Poco::AI::MCP::HTTP
