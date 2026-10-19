//
// Client.h
//
// Library: AI/MCP
// Package: Core
// Module:  Client
//
// Definition of the Client class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Client_INCLUDED
#define AIMCP_Client_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/AI/MCP/Implementation.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include <string>
#include <vector>


namespace Poco::AI::MCP {


struct AIMCP_API ServerDescription
	/// What server/discover tells about a server.
{
	std::vector<std::string> supportedVersions;
		/// The protocol revisions the server implements.
	Poco::JSON::Object::Ptr capabilities;
		/// The server's capabilities, e.g. {"tools": {}}; never null.
	std::string instructions;
		/// How to use the server, if it says.
	Implementation serverInfo;
		/// The server's name and version, if it reports them.
};


class AIMCP_API Client
	/// The client side of MCP protocol revision 2026-07-28, written once over
	/// the virtual exchange() of a transport: StreamClient for a pair of
	/// streams, ProcessClient for a server process (the stdio transport), and
	/// Poco::AI::MCP::HTTP::HTTPClient for the Streamable HTTP transport.
	///
	/// Every request carries the protocol revision, the client's capabilities
	/// (none) and the client info in its _meta object. A reply must match the
	/// request's id and carry a result object or an error object; an error
	/// reply becomes an MCPException with the error's code. Lists are fetched
	/// page by page up to a page limit.
	///
	/// The specification asks a client to send server/discover first: call
	/// discover() before any other request. A server of an earlier revision
	/// answers it with an error (METHOD_NOT_FOUND, for instance), or with a
	/// result that does not list 2026-07-28, which discover() reports as an
	/// MCPException with code UNSUPPORTED_PROTOCOL_VERSION. Requests sent
	/// without it reach such a server and fail in its own terms.
	///
	/// A tools/call, resources/read or prompts/get the server answers with an
	/// input_required result that asks for no input (under load, for instance)
	/// is sent again, with a new id and the server's requestState, up to a
	/// limit of rounds. The client declares no capabilities, so a server must
	/// not ask it for input; one that does gets a Poco::ProtocolException.
	///
	/// Calls are serialized: a client runs one request at a time, whichever
	/// thread calls it.
	///
	/// What a server returns is untrusted input, tool descriptions and
	/// annotations included.
{
public:
	static constexpr int DEFAULT_MAX_PAGES = 100;
		/// The default limit of pages a list call fetches.

	static constexpr int DEFAULT_MAX_INPUT_ROUNDS = 8;
		/// The default limit of times a request is sent while the server
		/// answers it with input_required.

	virtual ~Client();
		/// Destroys the client.

	void setClientInfo(const Implementation& info);
		/// Sets the name and version the client sends with every request,
		/// Implementation::library() by default. Without a name, requests carry
		/// no client info; the specification asks a client to send it unless it
		/// is told not to.

	[[nodiscard]] Implementation getClientInfo() const;
		/// Returns the client info.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets how long a request may take, 60 seconds by default. Transports
		/// that cannot time a read out say so.

	[[nodiscard]] Poco::Timespan getTimeout() const;
		/// Returns the timeout of a request.

	void setMaxPages(int maxPages);
		/// Sets how many pages a list call fetches at most.

	[[nodiscard]] int getMaxPages() const;
		/// Returns how many pages a list call fetches at most.

	void setMaxInputRounds(int maxRounds);
		/// Sets how many times a request is sent at most while the server
		/// answers it with input_required.

	[[nodiscard]] int getMaxInputRounds() const;
		/// Returns how many times a request is sent at most while the server
		/// answers it with input_required.

	[[nodiscard]] ServerDescription discover();
		/// Sends server/discover.
		///
		/// Throws MCPException with code UNSUPPORTED_PROTOCOL_VERSION if the
		/// server does not implement this client's protocol revision, and
		/// Poco::ProtocolException if the result has no supportedVersions array
		/// of strings.

	[[nodiscard]] std::vector<Tool> listTools();
		/// Returns the server's tools, all pages. Entries without a name or an
		/// input schema object are left out.

	[[nodiscard]] ToolResult callTool(const std::string& name, const Poco::JSON::Object::Ptr& arguments = nullptr);
		/// Calls a tool. A tool that fails returns a result with isError set;
		/// an MCPException means the call itself failed (an unknown tool, for
		/// instance).
		///
		/// A HEADER_MISMATCH error, which the Streamable HTTP transport gets
		/// when the definition of the tool it knows is missing or out of date,
		/// makes the client list the tools again and call once more.

	[[nodiscard]] std::vector<Resource> listResources();
		/// Returns the server's resources, all pages.

	[[nodiscard]] std::vector<ResourceContents> readResource(const std::string& uri);
		/// Reads a resource.

	[[nodiscard]] std::vector<Prompt> listPrompts();
		/// Returns the server's prompts, all pages.

	[[nodiscard]] Poco::JSON::Object::Ptr getPrompt(const std::string& name, const Poco::JSON::Object::Ptr& arguments = nullptr);
		/// Renders a prompt; returns the result object with its messages array.

	[[nodiscard]] Poco::JSON::Object::Ptr call(const std::string& method, const Poco::JSON::Object::Ptr& params = nullptr);
		/// Sends a request and returns its result object; for methods the
		/// class does not wrap. The request carries a copy of params whose
		/// _meta object keeps what the caller put in it (a progressToken, for
		/// instance) and gets the protocol revision, the client capabilities
		/// and the client info from the client.
		///
		/// Throws MCPException for an error reply, Poco::ProtocolException for
		/// a reply that breaks the protocol, Poco::InvalidArgumentException if
		/// params has a _meta member that is not an object, and the transport's
		/// exceptions, e.g. Poco::TimeoutException.

protected:
	Client();
		/// Creates the client.

	virtual Poco::JSON::Object::Ptr exchange(const Poco::JSON::Object::Ptr& request) = 0;
		/// Sends the request and returns its reply, the first message for
		/// which isReply() is true. Messages the server sends before it are
		/// passed over. Implemented by the transport.

	virtual void toolsListed(std::vector<Tool>& tools);
		/// Called by listTools() with the tools of all pages before they are
		/// returned. A transport may drop tools it cannot call, or remember
		/// what it needs to call them. Does nothing by default.

	[[nodiscard]] static bool isReply(const Poco::JSON::Object::Ptr& message, const Id& id);
		/// Returns true if message is the reply to the request with the given
		/// id: a message with that id that is not a request, or an error
		/// response without an id (or with a null one). call() refuses a reply
		/// that has neither a result nor an error.

private:
	std::vector<Poco::JSON::Object::Ptr> listAll(const std::string& method, const std::string& key);

	Client(const Client&) = delete;
	Client& operator = (const Client&) = delete;

	mutable Poco::FastMutex _mutex;
	Poco::FastMutex _callMutex;
	Implementation _clientInfo;
	Poco::Timespan _timeout;
	int _maxPages;
	int _maxInputRounds;
	Poco::Int64 _nextId;
};


} // namespace Poco::AI::MCP


#endif // AIMCP_Client_INCLUDED
