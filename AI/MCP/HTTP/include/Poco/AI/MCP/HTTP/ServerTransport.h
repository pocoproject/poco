//
// ServerTransport.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  ServerTransport
//
// Definition of the ServerTransport class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_ServerTransport_INCLUDED
#define AIMCPHTTP_ServerTransport_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/JSON/Object.h"
#include <string>
#include <vector>


namespace Poco::AI::MCP::HTTP {


class AIMCPHTTP_API ServerTransport
	/// The server side of the Streamable HTTP transport of MCP revision
	/// 2026-07-28, for one endpoint of a host's HTTP server. The revision is
	/// stateless: every POST carries one JSON-RPC message and gets one reply,
	/// a JSON object, or 202 Accepted without a body for a notification. The
	/// transport opens no stream of its own and keeps no session, so every
	/// other method, GET and DELETE among them, gets 405.
	///
	/// The host's HTTP server must pass the request headers on as they were
	/// sent: Poco::Net::HTTPServerParams::setAutoDecodeHeaders(false). By
	/// default it decodes RFC 2047 encoded words in every header, which would
	/// have the MCP headers checked in a rewritten form; the transport answers
	/// every request of such a server with 500 and INTERNAL_ERROR, and logs
	/// why at the error level.
	///
	/// The host signs the caller in and passes the request context with the
	/// identity. The transport checks what the protocol asks of a request, in
	/// this order:
	///
	///   - the method: POST, else 405 with an Allow header;
	///   - the Origin header, when there is one: an allowed origin, else 403,
	///     which keeps out the pages of other sites (DNS rebinding);
	///   - the Content-Type: application/json, else 415;
	///   - the Accept header, when there is one: a range that takes
	///     application/json, else 406;
	///   - the size of the body: at most maxMessageSize bytes, else 413;
	///   - the body: JSON, else 400 with PARSE_ERROR; one object, not a batch,
	///     else 400 with INVALID_REQUEST;
	///   - the MCP headers: each comes once at most, and an
	///     MCP-Protocol-Version or Mcp-Method header that is sent matches
	///     the message. A request whose _meta object names
	///     this revision carries both, the Mcp-Name header of tools/call,
	///     prompts/get and resources/read, with the name or the URI, and the
	///     Mcp-Param headers of the arguments with a value that the input
	///     schema of the tool marks with x-mcp-header. A value with other
	///     characters than printable ASCII and tabs is refused, also in the
	///     Mcp-Param header of an argument without a value; a value in the
	///     base64 form is decoded first; an integer argument is compared as a
	///     number.
	///     A mismatch gets 400 with HEADER_MISMATCH.
	///
	/// The refusals before the body carry no message, and a request refused
	/// before its body was read ends its connection.
	///
	/// The server then answers the message. Its reply goes out with 200, but
	/// for the errors that refuse a request before its method runs:
	/// METHOD_NOT_FOUND gets 404; INVALID_REQUEST, UNSUPPORTED_PROTOCOL_VERSION,
	/// MISSING_REQUIRED_CLIENT_CAPABILITY and any error of a request without a
	/// complete _meta object get 400. A request without a _meta object or of
	/// another revision, e.g. the initialize request of an earlier one, goes to
	/// the server without the header checks, so that its reply names what is
	/// missing, or the revision the server implements.
	///
	/// Browsers send a cross-origin request only after a preflight request,
	/// and read its reply only with CORS headers: a host that serves the pages
	/// of other origins answers the preflight and adds the headers itself.
	///
	/// The transport keeps no state between requests: one serves the requests
	/// of all threads.
{
public:
	static constexpr std::size_t DEFAULT_MAX_REQUEST_SIZE = 1024*1024;
		/// The default limit of a request body, in bytes. It is smaller than
		/// the limit of a message of the stdio transport, as a server answers
		/// many requests at once.

	struct Params
		/// The settings of a transport.
	{
		std::size_t maxMessageSize = DEFAULT_MAX_REQUEST_SIZE;
			/// The limit of a request body, in bytes. The transport holds the
			/// body of a request in memory, and its JSON, which takes several
			/// times as much, while it answers: each worker thread of the
			/// HTTPServer may need that much at once.
		std::vector<std::string> allowedOrigins;
			/// The origins a browser may send a request from, as it names
			/// them: scheme://host, and :port unless it is the default one.
			/// "*" allows every origin. A request without an Origin header,
			/// from a client that is no browser, needs none.
		std::string loggerName = "AI.MCP.HTTP.ServerTransport";
			/// The logger of the transport.
	};

	struct Reply
		/// What the transport answers a request.
	{
		Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_OK;
		Poco::JSON::Object::Ptr message;
			/// The JSON-RPC message of the body; null for no body.
		Poco::Net::NameValueCollection headers;
			/// Headers to send besides Content-Type and Content-Length.
	};

	ServerTransport();
		/// Creates a transport with the default settings.

	explicit ServerTransport(const Params& params);
		/// Creates a transport.

	~ServerTransport();
		/// Destroys the transport.

	[[nodiscard]] Reply answer(Server& server, Poco::Net::HTTPServerRequest& request, const RequestContext& context = RequestContext()) const;
		/// Reads the body of the request, checks the request and has the
		/// server answer its message, with the context the host gives. Never
		/// throws: what fails in it is a 500 with INTERNAL_ERROR.

	[[nodiscard]] Reply answer(Server& server, const Poco::Net::HTTPServerRequest& request, const std::string& body, const RequestContext& context = RequestContext()) const;
		/// Answers a request whose body the host has read itself. Never
		/// throws.

	void send(const Reply& reply, Poco::Net::HTTPServerResponse& response) const;
		/// Sends a reply.

	void handle(Server& server, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const RequestContext& context = RequestContext()) const;
		/// Answers the request and sends the reply. Never throws: a reply
		/// that cannot be sent, because the client has gone away, is dropped.

	[[nodiscard]] bool allowsOrigin(const std::string& origin) const;
		/// Returns true if a request may come from the origin.

	[[nodiscard]] static Reply unavailable();
		/// The reply of a host that has no server to answer the request now:
		/// 503 with INTERNAL_ERROR.

	[[nodiscard]] const Params& params() const;
		/// Returns the settings of the transport.

private:
	[[nodiscard]] bool refuse(const Poco::Net::HTTPServerRequest& request, Reply& reply) const;
	[[nodiscard]] Reply answerMessage(Server& server, const Poco::Net::HTTPServerRequest& request, const std::string& body, const RequestContext& context) const;
	[[nodiscard]] Reply tooLarge() const;
	[[nodiscard]] Reply decodedHeaders() const;
	[[nodiscard]] Reply failed(const std::string& what) const;

	Params _params;
};


//
// inlines
//
inline const ServerTransport::Params& ServerTransport::params() const
{
	return _params;
}


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_ServerTransport_INCLUDED
