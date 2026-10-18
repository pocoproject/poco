//
// SessionFactory.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  SessionFactory
//
// The session factory hook of the HTTP client transport: a callable that
// creates the Poco::Net::HTTPClientSession for a request to a URI, and the
// default used when a caller supplies none.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_SessionFactory_INCLUDED
#define AIMCPHTTP_SessionFactory_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/URI.h"
#include <functional>
#include <memory>


namespace Poco::AI::MCP::HTTP {


using SessionFactory = std::function<std::unique_ptr<Poco::Net::HTTPClientSession>(const Poco::URI& uri)>;
	/// Creates a client session for requests to uri. The host, port, proxy
	/// and transport security of the session are the factory's choice. The
	/// caller keeps a session with a host for later requests and lets it
	/// reconnect; a session without a host, e.g. one attached to a tunnel,
	/// serves one request. An application supplies its own factory to use a
	/// particular TLS context, a proxy that needs special handling, or a
	/// tunnel. A factory that cannot serve uri throws; it must not return null.


[[nodiscard]] AIMCPHTTP_API std::unique_ptr<Poco::Net::HTTPClientSession> createClientSession(const Poco::URI& uri);
	/// The default session factory.
	///
	/// For an http URI it creates a plain Poco::Net::HTTPClientSession, with
	/// the proxy configuration of Poco::Net::HTTPSessionFactory::defaultFactory()
	/// when that is set, and the global one otherwise.
	/// For every other scheme it asks that factory, which needs a registered
	/// instantiator: for https, the application calls
	/// Poco::Net::HTTPSSessionInstantiator::registerInstantiator() once
	/// (from NetSSL_OpenSSL or NetSSL_Win, its choice), so this library
	/// never links a TLS implementation itself.
	///
	/// Throws Poco::UnknownURISchemeException when no instantiator is
	/// registered for the scheme.


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_SessionFactory_INCLUDED
