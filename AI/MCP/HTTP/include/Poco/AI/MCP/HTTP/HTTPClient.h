//
// HTTPClient.h
//
// Library: AIMCPHTTP
// Package: HTTP
// Module:  HTTPClient
//
// An MCP client over the Streamable HTTP(S) transport. POSTs JSON-RPC messages
// to a single endpoint and carries the Mcp-Session-Id across calls.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_HTTPClient_INCLUDED
#define AIMCPHTTP_HTTPClient_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/AI/MCP/HTTP/SessionFactory.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/URI.h"
#include <string>


namespace Poco {
namespace AI {
namespace MCP {
namespace HTTP {


class AIMCPHTTP_API HTTPClient
	/// A synchronous MCP client speaking Streamable HTTP(S) to one endpoint.
	///
	/// Every call opens its own session through the session factory, so the
	/// client needs only Poco::Net: an http endpoint works as is, an https
	/// endpoint works once the application has registered
	/// Poco::Net::HTTPSSessionInstantiator (see createClientSession()), and
	/// a caller-supplied factory can carry its own TLS context or proxy.
	///
	/// Not safe for concurrent use: one call at a time per client.
{
public:
	explicit HTTPClient(const Poco::URI& endpoint, SessionFactory sessionFactory = SessionFactory());
		/// Creates a client targeting the given MCP endpoint URI. Sessions come
		/// from sessionFactory, or from createClientSession() when none is given.

	~HTTPClient();
		/// Destroys the client.

	Poco::JSON::Object::Ptr initialize(const std::string& clientName, const std::string& clientVersion);
		/// Performs the MCP handshake (initialize + notifications/initialized) and
		/// captures the Mcp-Session-Id. Returns the initialize result.

	void ping();
		/// Sends a ping and waits for the reply.

	Poco::JSON::Array::Ptr listTools();
		/// Returns the array of tool descriptors from tools/list.

	ToolResult callTool(const std::string& name, Poco::JSON::Object::Ptr arguments);
		/// Invokes a tool via tools/call and returns its result.

	const std::string& sessionId() const;
		/// Returns the current Mcp-Session-Id (empty before initialize).

private:
	Poco::JSON::Object::Ptr call(const std::string& method, Poco::JSON::Object::Ptr params);
	void notify(const std::string& method, Poco::JSON::Object::Ptr params);
	Poco::JSON::Object::Ptr post(const Poco::JSON::Object::Ptr& message, bool expectReply);

	HTTPClient(const HTTPClient&) = delete;
	HTTPClient& operator = (const HTTPClient&) = delete;

	Poco::URI _uri;
	SessionFactory _sessionFactory;
	std::string _sessionId;
	Poco::Int64 _nextId;
};


//
// inlines
//
inline const std::string& HTTPClient::sessionId() const
{
	return _sessionId;
}


} } } } // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_HTTPClient_INCLUDED
