//
// HTTPClientSessionPool.h
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPClientSessionPool
//
// Definition of the HTTPClientSessionPool class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPClientSessionPool_INCLUDED
#define Net_HTTPClientSessionPool_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/ProxyConfig.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/URI.h"
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace Poco::Net {

class HTTPSessionFactory;


class Net_API HTTPClientSessionPool
	/// Keeps HTTP client sessions whose connection can carry another
	/// request, for later requests to the same endpoint.
	///
	/// A HTTPSessionFactory creates the sessions, with persistent
	/// connections enabled. Idle sessions are kept per scheme, host and
	/// port of the URI and per proxy server of the factory. acquire()
	/// returns a Handle that gives exclusive access to one session. When
	/// the Handle is destroyed, the session is kept if the endpoint has
	/// fewer idle sessions than the limit and HTTPClientSession::canReuse()
	/// would return true without its check of the socket for data or a close
	/// by the server; otherwise it is destroyed. acquire() makes that check
	/// before it returns an idle session.
	///
	/// Before a session is kept, the pool clears its request trailer and
	/// attached session data, and restores the timeouts and the keep-alive
	/// timeout it had when the pool created it. A session whose host, port,
	/// proxy server, tunnel, non-proxy hosts, proxy credentials or proxy
	/// authentication method has been changed is destroyed. A session that
	/// has sent NTLM, Negotiate or Kerberos credentials to the server or to
	/// the proxy is destroyed, because these schemes authenticate the
	/// connection, not the request.
	///
	/// Idle sessions whose keep-alive timeout
	/// (HTTPClientSession::setKeepAliveTimeout()) has expired are destroyed
	/// by later calls of acquire() and Handle releases; those of other
	/// endpoints are checked at most once per second. idleSessions() can
	/// count expired sessions that have not been destroyed yet.
	///
	/// The pool is thread safe. It must outlive every Handle it returns.
	/// The proxy configuration of the factory must not change while
	/// another thread uses the pool, and clear() must be called after
	/// a change of the proxy configuration of the factory or of the
	/// global proxy configuration (HTTPClientSession::setGlobalProxyConfig()).
{
public:
	class Handle;

	static constexpr std::size_t DEFAULT_MAX_IDLE_SESSIONS = 8;

	HTTPClientSessionPool();
		/// Creates a pool that uses HTTPSessionFactory::defaultFactory()
		/// and keeps up to DEFAULT_MAX_IDLE_SESSIONS idle sessions per endpoint.

	explicit HTTPClientSessionPool(HTTPSessionFactory& factory, std::size_t maxIdleSessions = DEFAULT_MAX_IDLE_SESSIONS);
		/// Creates a pool that uses the given factory, which must outlive
		/// the pool, and keeps up to maxIdleSessions idle sessions per endpoint.
		/// maxIdleSessions must be greater than zero.

	HTTPClientSessionPool(const HTTPClientSessionPool&) = delete;
	HTTPClientSessionPool& operator = (const HTTPClientSessionPool&) = delete;

	~HTTPClientSessionPool();
		/// Destroys the pool and its idle sessions.

	[[nodiscard]] Handle acquire(const Poco::URI& uri);
		/// Returns a Handle to an idle session for the endpoint of uri
		/// that can carry another request, or to a new session from
		/// the factory if there is none.
		///
		/// Throws the exceptions of HTTPClientSession::canReuse() and
		/// HTTPSessionFactory::createClientSession().

	[[nodiscard]] std::size_t idleSessions() const;
		/// Returns the number of idle sessions of all endpoints.

	void clear();
		/// Destroys all idle sessions.

private:
	struct PooledSession
		/// A session and the settings it had when the pool created it.
	{
		std::unique_ptr<HTTPClientSession> pSession;
		std::string host;
		Poco::UInt16 port = 0;
		ProxyConfig proxyConfig;
		Poco::Timespan connectTimeout;
		Poco::Timespan sendTimeout;
		Poco::Timespan receiveTimeout;
		Poco::Timespan keepAliveTimeout;
	};

	using SessionList = std::vector<PooledSession>;

	[[nodiscard]] std::string endpointKey(const Poco::URI& uri) const;
		/// Returns the key of the idle sessions for uri.

	[[nodiscard]] PooledSession takeIdle(const std::string& key);
		/// Removes the most recently used idle session for key from the pool
		/// and returns it, or returns an entry without a session if there is none.

	void putBack(const std::string& key, PooledSession session);
		/// Keeps session as an idle session for key if the limit allows.

	void removeExpired(const Poco::Timestamp& now, SessionList& expired);
		/// Moves the expired idle sessions of all endpoints to expired and
		/// removes the endpoints without idle sessions, at most once per
		/// SWEEP_INTERVAL. The caller holds _mutex.

	static void removeExpired(SessionList& sessions, const Poco::Timestamp& now, SessionList& expired);
		/// Moves the sessions whose keep-alive timeout has expired to expired
		/// and keeps the order of the others.

	[[nodiscard]] static bool restore(const PooledSession& session);
		/// Clears the request trailer and the session data of the session and
		/// restores the settings it had when the pool created it. Returns false
		/// if its endpoint, proxy settings or proxy credentials differ or it has
		/// sent NTLM, Negotiate or Kerberos credentials to the server or to the
		/// proxy.

	static constexpr Poco::Timespan::TimeDiff SWEEP_INTERVAL = Poco::Timespan::SECONDS;

	HTTPSessionFactory& _factory;
	const std::size_t _maxIdleSessions;
	std::map<std::string, SessionList> _idleSessions;
	Poco::Timestamp _nextSweep;
	mutable Poco::FastMutex _mutex;
};


class Net_API HTTPClientSessionPool::Handle
	/// Gives exclusive access to a session of the pool and
	/// returns the session to the pool when destroyed.
{
public:
	Handle() = delete;
	Handle(const Handle&) = delete;
	Handle(Handle&& other) noexcept;
	~Handle();

	Handle& operator = (const Handle&) = delete;
	Handle& operator = (Handle&& other) noexcept;

	[[nodiscard]] HTTPClientSession& session() const;
		/// Returns the session.

	[[nodiscard]] HTTPClientSession* operator -> () const;
		/// Returns a pointer to the session.

private:
	Handle(HTTPClientSessionPool& pool, std::string key, PooledSession session);

	void release() noexcept;
		/// Returns the session to the pool if it can be reused,
		/// and destroys it otherwise.

	HTTPClientSessionPool* _pPool;
	std::string _key;
	PooledSession _session;

	friend class HTTPClientSessionPool;
};


//
// inlines
//
inline HTTPClientSession* HTTPClientSessionPool::Handle::operator -> () const
{
	return &session();
}


} // namespace Poco::Net


#endif // Net_HTTPClientSessionPool_INCLUDED
