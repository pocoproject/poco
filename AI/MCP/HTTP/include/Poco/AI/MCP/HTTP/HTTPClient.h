//
// HTTPClient.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  HTTPClient
//
// Definition of the HTTPClient class.
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
#include "Poco/AI/MCP/Client.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include "Poco/URI.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace Poco::AI::MCP::HTTP {


class AIMCPHTTP_API HTTPClient: public Poco::AI::MCP::Client
	/// The Streamable HTTP transport of MCP protocol revision 2026-07-28: every
	/// request is a POST of one JSON-RPC message to the MCP endpoint.
	///
	/// Each request carries the headers MCP-Protocol-Version, Mcp-Method and,
	/// for tools/call, resources/read and prompts/get, Mcp-Name, and for a
	/// tool call the Mcp-Param headers the tool's input schema asks for with
	/// x-mcp-header. A value that is not plain printable ASCII, or that has
	/// leading or trailing spaces, is sent in the base64 form the transport
	/// defines. listTools() leaves out tools whose x-mcp-header annotations
	/// are invalid.
	///
	/// The reply is a JSON object or an event stream; on an event stream the
	/// client passes over the notifications before the reply. A JSON-RPC error
	/// in a 400 or 404 answer becomes an MCPException with the error's code;
	/// any other status that is not 200 becomes a Poco::Net::HTTPException
	/// with the status and the start of the body. Replies are limited in size.
	/// The whole request, from sending it to the end of its reply, must take
	/// no longer than the client's timeout: every read that waits is bounded
	/// by the time that is left, so a reply that trickles in times out too.
	///
	/// The Mcp-Param headers of a tool come from its definition, which
	/// listTools() reads. callTool() answered with HEADER_MISMATCH lists the
	/// tools again and calls once more, as the specification asks of a client:
	/// a tool called before it was listed, or whose definition changed, gets
	/// its headers that way.
	///
	/// The client keeps its session for later requests, and opens a new one
	/// when the server has closed the kept connection. A request that fails
	/// on a kept session is sent once more on a new one, as a new request
	/// with an id of its own, only when it cannot have reached the server, or
	/// when it only reads (server/discover and the lists): a tool call the
	/// server may already have run is not repeated.
{
public:
	using RequestHook = std::function<void (Poco::Net::HTTPRequest& request)>;
		/// Called for every request once the client has set its headers, e.g.
		/// to add an Authorization header.

	explicit HTTPClient(const Poco::URI& endpoint, SessionFactory sessionFactory = SessionFactory());
		/// Creates a client of the MCP endpoint. Sessions come from the given
		/// factory, or from createClientSession() when it is empty.

	~HTTPClient() override;
		/// Destroys the client.

	[[nodiscard]] const Poco::URI& endpoint() const;
		/// Returns the MCP endpoint.

	void setRequestHook(RequestHook hook);
		/// Sets the hook called for every request.

	void setMaxResponseSize(std::size_t maxSize);
		/// Sets the limit, in bytes, of a reply; 16 MB by default.

	[[nodiscard]] std::size_t getMaxResponseSize() const;
		/// Returns the limit of a reply.

protected:
	Poco::JSON::Object::Ptr exchange(const Poco::JSON::Object::Ptr& request) override;
	void toolsListed(std::vector<Tool>& tools) override;

private:
	struct ParamHeader
	{
		std::string name;
		std::vector<std::string> path;
	};

	Poco::JSON::Object::Ptr post(const Poco::JSON::Object::Ptr& request, const Poco::Timestamp& deadline, bool& retry);
	void addHeaders(Poco::Net::HTTPRequest& httpRequest, const Poco::JSON::Object::Ptr& request);
	Poco::Net::HTTPClientSession& session(bool& reused);
	std::string redactedEndpoint() const;

	HTTPClient(const HTTPClient&) = delete;
	HTTPClient& operator = (const HTTPClient&) = delete;

	const Poco::URI _uri;
	const SessionFactory _sessionFactory;
	mutable Poco::FastMutex _mutex;
	RequestHook _hook;
	std::size_t _maxResponseSize;
	std::map<std::string, std::vector<ParamHeader>> _paramHeaders;
	std::unique_ptr<Poco::Net::HTTPClientSession> _pSession;
	bool _sessionUsed;
};


//
// inlines
//
inline const Poco::URI& HTTPClient::endpoint() const
{
	return _uri;
}


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_HTTPClient_INCLUDED
