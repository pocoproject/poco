//
// Client.h
//
// Library: AIMCP
// Package: Core
// Module:  Client
//
// An MCP client that speaks the protocol over any std::istream/std::ostream
// pair (a pipe, a socket stream, an in-memory buffer). Transport-agnostic - the
// HTTP(S) client transport lives in the separate PocoAIMCPHTTP library.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Client_INCLUDED
#define AIMCP_Client_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <iosfwd>
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


class AIMCP_API Client
	/// A synchronous MCP client over a stream pair. Each call writes a request
	/// and blocks for the matching response.
{
public:
	Client(std::istream& in, std::ostream& out);
		/// Creates a client that reads responses from in and writes requests to out.

	~Client();
		/// Destroys the client.

	Poco::JSON::Object::Ptr initialize(const std::string& clientName, const std::string& clientVersion);
		/// Performs the MCP handshake: sends initialize, then the
		/// notifications/initialized notification. Returns the initialize result.

	void ping();
		/// Sends a ping and waits for the (empty) reply.

	Poco::JSON::Array::Ptr listTools();
		/// Returns the array of tool descriptors from tools/list.

	ToolResult callTool(const std::string& name, Poco::JSON::Object::Ptr arguments);
		/// Invokes a tool via tools/call and returns its result.

private:
	Poco::JSON::Object::Ptr call(const std::string& method, Poco::JSON::Object::Ptr params);
	void notify(const std::string& method, Poco::JSON::Object::Ptr params);

	Client(const Client&) = delete;
	Client& operator = (const Client&) = delete;

	std::istream& _in;
	std::ostream& _out;
	Poco::Int64 _nextId;
};


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Client_INCLUDED
