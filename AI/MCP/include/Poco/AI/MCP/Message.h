//
// Message.h
//
// Library: AI/MCP
// Package: Core
// Module:  Message
//
// JSON-RPC 2.0 envelope builders and the newline-delimited JSON framing
// used by the stream transports.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Message_INCLUDED
#define AIMCP_Message_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/JSON/Object.h"
#include "Poco/Dynamic/Var.h"
#include <iosfwd>
#include <string>


namespace Poco::AI::MCP {


using Id = Poco::Dynamic::Var;
	/// A JSON-RPC request id: a string or an integer. An empty Var stands for
	/// no id: a notification, or the null id of an error reply to a message
	/// whose id cannot be read.


enum class ErrorCode
	/// The JSON-RPC error codes MCP uses. Tool execution failures are not
	/// reported with these: they come back as a result with isError set.
{
	PARSE_ERROR                        = -32700,
	INVALID_REQUEST                    = -32600,
	METHOD_NOT_FOUND                   = -32601,
	INVALID_PARAMS                     = -32602,
	INTERNAL_ERROR                     = -32603,
	HEADER_MISMATCH                    = -32020,
		/// An HTTP header does not match the request it carries.
	MISSING_REQUIRED_CLIENT_CAPABILITY = -32021,
		/// The request needs a capability the client did not declare.
	UNSUPPORTED_PROTOCOL_VERSION       = -32022
		/// The request names a protocol revision the server does not implement.
};


class AIMCP_API Request
	/// Builders for outbound JSON-RPC requests and notifications.
{
public:
	[[nodiscard]] static Poco::JSON::Object::Ptr make(const Id& id, const std::string& method, Poco::JSON::Object::Ptr params = nullptr);
		/// Builds a request envelope {jsonrpc, id, method, params?}.

	[[nodiscard]] static Poco::JSON::Object::Ptr makeNotification(const std::string& method, Poco::JSON::Object::Ptr params = nullptr);
		/// Builds a notification envelope {jsonrpc, method, params?} (no id).
};


class AIMCP_API Response
	/// Builders for JSON-RPC responses.
{
public:
	[[nodiscard]] static Poco::JSON::Object::Ptr result(const Id& id, const Poco::Dynamic::Var& result);
		/// Builds a success envelope {jsonrpc, id, result}.

	[[nodiscard]] static Poco::JSON::Object::Ptr error(const Id& id, ErrorCode code, const std::string& message, Poco::JSON::Object::Ptr data = nullptr);
		/// Builds an error envelope {jsonrpc, id, error: {code, message, data?}}.
		/// An empty id is written as null.
};


[[nodiscard]] AIMCP_API bool isValidId(const Poco::Dynamic::Var& id);
	/// Returns true if id is a string or an integer, the two kinds of id a
	/// request may carry. Null, booleans, fractions, arrays and objects are not.


[[nodiscard]] AIMCP_API bool isResponseTo(const Poco::JSON::Object::Ptr& message, const Id& id);
	/// Returns true if message is a response (it carries a result or an
	/// error) whose id equals id: a string id equals the same string, an
	/// integer id the same integer.


[[nodiscard]] AIMCP_API std::string serialize(const Poco::JSON::Object::Ptr& message);
	/// Serializes a message to a single compact JSON line. Control characters
	/// in strings are escaped, so the line holds no newline.


AIMCP_API void writeMessage(std::ostream& out, const Poco::JSON::Object::Ptr& message);
	/// Writes a message as one newline-terminated JSON line and flushes.
	///
	/// Throws Poco::WriteFileException if the stream fails, so that a
	/// transport whose peer has gone away stops serving.


[[nodiscard]] AIMCP_API Poco::JSON::Object::Ptr readMessage(std::istream& in, std::size_t maxSize = DEFAULT_MAX_MESSAGE_SIZE);
	/// Reads the next newline-delimited JSON object from the stream, skipping
	/// blank lines. Returns null at the end of the stream; a last line without
	/// its newline is read as well.
	///
	/// Throws MCPException with code PARSE_ERROR if the line is not JSON, and
	/// with code INVALID_REQUEST if it is JSON but not an object, or longer than
	/// maxSize bytes; in that case the rest of the line is skipped, so the
	/// stream stays usable.


} // namespace Poco::AI::MCP


#endif // AIMCP_Message_INCLUDED
