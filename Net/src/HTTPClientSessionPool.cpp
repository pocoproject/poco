//
// HTTPClientSessionPool.cpp
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPClientSessionPool
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPClientSessionPool.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/ProxyConfig.h"
#include "Poco/Any.h"
#include "Poco/Bugcheck.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Timestamp.h"
#include <algorithm>
#include <utility>


using Poco::FastMutex;
using Poco::NumberFormatter;


namespace Poco::Net {


HTTPClientSessionPool::Handle::Handle(HTTPClientSessionPool& pool, std::string key, PooledSession session):
	_pPool(&pool),
	_key(std::move(key)),
	_session(std::move(session))
{
}


HTTPClientSessionPool::Handle::Handle(Handle&& other) noexcept:
	_pPool(other._pPool),
	_key(std::move(other._key)),
	_session(std::move(other._session))
{
}


HTTPClientSessionPool::Handle::~Handle()
{
	release();
}


HTTPClientSessionPool::Handle& HTTPClientSessionPool::Handle::operator = (Handle&& other) noexcept
{
	if (this != &other)
	{
		release();
		_pPool = other._pPool;
		_key = std::move(other._key);
		_session = std::move(other._session);
	}
	return *this;
}


HTTPClientSession& HTTPClientSessionPool::Handle::session() const
{
	poco_check_ptr (_session.pSession.get());

	return *_session.pSession;
}


void HTTPClientSessionPool::Handle::release() noexcept
{
	if (_session.pSession == nullptr) return;
	try
	{
		if (_session.pSession->stateAllowsReuse() && restore(_session)) _pPool->putBack(_key, std::move(_session));
	}
	catch (...)
	{
		poco_unexpected();
	}
	_session.pSession.reset();
}


HTTPClientSessionPool::HTTPClientSessionPool():
	HTTPClientSessionPool(HTTPSessionFactory::defaultFactory())
{
}


HTTPClientSessionPool::HTTPClientSessionPool(HTTPSessionFactory& factory, std::size_t maxIdleSessions):
	_factory(factory),
	_maxIdleSessions(maxIdleSessions)
{
	poco_assert (maxIdleSessions > 0);
}


HTTPClientSessionPool::~HTTPClientSessionPool() = default;


HTTPClientSessionPool::Handle HTTPClientSessionPool::acquire(const Poco::URI& uri)
{
	std::string key = endpointKey(uri);
	PooledSession session = takeIdle(key);
	while (session.pSession != nullptr && !session.pSession->canReuse())
	{
		session = takeIdle(key);
	}
	if (session.pSession == nullptr)
	{
		session.pSession.reset(_factory.createClientSession(uri));
		session.pSession->setKeepAlive(true);
		session.host = session.pSession->getHost();
		session.port = session.pSession->getPort();
		session.proxyConfig = session.pSession->getProxyConfig();
		session.connectTimeout = session.pSession->getConnectTimeout();
		session.sendTimeout = session.pSession->getSendTimeout();
		session.receiveTimeout = session.pSession->getReceiveTimeout();
		session.keepAliveTimeout = session.pSession->getKeepAliveTimeout();
	}
	return Handle(*this, std::move(key), std::move(session));
}


std::size_t HTTPClientSessionPool::idleSessions() const
{
	FastMutex::ScopedLock lock(_mutex);

	std::size_t count = 0;
	for (const auto& entry: _idleSessions)
	{
		count += entry.second.size();
	}
	return count;
}


void HTTPClientSessionPool::clear()
{
	std::map<std::string, SessionList> idleSessions;
	{
		FastMutex::ScopedLock lock(_mutex);
		idleSessions.swap(_idleSessions);
	}
}


std::string HTTPClientSessionPool::endpointKey(const Poco::URI& uri) const
{
	std::string key(uri.getScheme());
	key.append("://");
	key.append(uri.getHost());
	key.append(":");
	NumberFormatter::append(key, uri.getPort());
	const ProxyConfig& proxyConfig = _factory.getProxyConfig();
	if (!proxyConfig.host.empty())
	{
		key.append(" via ");
		key.append(proxyConfig.protocol);
		key.append("://");
		key.append(proxyConfig.username);
		key.append("@");
		key.append(proxyConfig.host);
		key.append(":");
		NumberFormatter::append(key, proxyConfig.port);
		if (proxyConfig.tunnel) key.append(" tunnel");
	}
	return key;
}


HTTPClientSessionPool::PooledSession HTTPClientSessionPool::takeIdle(const std::string& key)
{
	// Destroying a session closes its connection; this happens after the lock is released.
	SessionList expired;
	PooledSession session;
	FastMutex::ScopedLock lock(_mutex);

	const Poco::Timestamp now;
	removeExpired(now, expired);
	auto it = _idleSessions.find(key);
	if (it != _idleSessions.end())
	{
		removeExpired(it->second, now, expired);
		if (!it->second.empty())
		{
			session = std::move(it->second.back());
			it->second.pop_back();
		}
	}
	return session;
}


void HTTPClientSessionPool::putBack(const std::string& key, PooledSession session)
{
	SessionList expired;
	FastMutex::ScopedLock lock(_mutex);

	const Poco::Timestamp now;
	removeExpired(now, expired);
	SessionList& sessions = _idleSessions[key];
	removeExpired(sessions, now, expired);
	if (sessions.size() < _maxIdleSessions) sessions.push_back(std::move(session));
}


void HTTPClientSessionPool::removeExpired(const Poco::Timestamp& now, SessionList& expired)
{
	if (now < _nextSweep) return;
	_nextSweep = now + SWEEP_INTERVAL;
	for (auto it = _idleSessions.begin(); it != _idleSessions.end();)
	{
		removeExpired(it->second, now, expired);
		if (it->second.empty())
			it = _idleSessions.erase(it);
		else
			++it;
	}
}


void HTTPClientSessionPool::removeExpired(SessionList& sessions, const Poco::Timestamp& now, SessionList& expired)
{
	// Expiry follows the last request, not the release order, so every entry is checked.
	for (auto& session: sessions)
	{
		if (session.pSession->getKeepAliveTimeout() <= now - session.pSession->getLastRequest())
			expired.push_back(std::move(session));
	}
	sessions.erase(std::remove_if(sessions.begin(), sessions.end(), [](const PooledSession& session) { return session.pSession == nullptr; }), sessions.end());
}


bool HTTPClientSessionPool::restore(const PooledSession& session)
{
	HTTPClientSession& clientSession = *session.pSession;
	const ProxyConfig& current = clientSession.getProxyConfig();
	const ProxyConfig& created = session.proxyConfig;
	if (clientSession.getHost() != session.host || clientSession.getPort() != session.port)
		return false;
	if (current.host != created.host || current.port != created.port || current.protocol != created.protocol ||
		current.tunnel != created.tunnel || current.nonProxyHosts != created.nonProxyHosts || current.authMethod != created.authMethod ||
		current.username != created.username || current.password != created.password)
		return false;
	if (clientSession._connectionAuthenticated)
		return false;
	// A timeout setter of a connected session makes system calls.
	if (clientSession.getConnectTimeout() != session.connectTimeout || clientSession.getSendTimeout() != session.sendTimeout || clientSession.getReceiveTimeout() != session.receiveTimeout)
		clientSession.setTimeout(session.connectTimeout, session.sendTimeout, session.receiveTimeout);
	clientSession.setKeepAliveTimeout(session.keepAliveTimeout);
	clientSession.requestTrailer().clear();
	clientSession.attachSessionData(Poco::Any());
	return true;
}


} // namespace Poco::Net
