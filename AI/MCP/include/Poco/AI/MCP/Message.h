//
// Message.h
//
// Library: AIMCP
// Package: Core
// Module:  Message
//
// JSON-RPC 2.0 envelope helpers shared by the server, the client and the
// transports: request/notification builders, response (result/error) builders,
// the standard error codes, and stream (de)serialization.
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


namespace Poco {
namespace AI {
namespace MCP {


using Id = Poco::Dynamic::Var;
	/// A JSON-RPC request id: a string, a number, or null. An empty Var denotes
	/// "no id" (a notification) or the null id used for pre-dispatch errors.


namespace ErrorCode
	/// The standard JSON-RPC 2.0 error codes. Tool *execution* failures are NOT
	/// reported with these - they come back as a normal result with isError set.
{
	enum
	{
		ParseError     = -32700,
		InvalidRequest = -32600,
		MethodNotFound = -32601,
		InvalidParams  = -32602,
		InternalError  = -32603
	};
}


class AIMCP_API Request
	/// Builders for outbound JSON-RPC requests and notifications.
{
public:
	static Poco::JSON::Object::Ptr make(const Id& id, const std::string& method,
		Poco::JSON::Object::Ptr params = nullptr);
		/// Builds a request envelope {jsonrpc, id, method, params?}.

	static Poco::JSON::Object::Ptr makeNotification(const std::string& method,
		Poco::JSON::Object::Ptr params = nullptr);
		/// Builds a notification envelope {jsonrpc, method, params?} (no id).
};


class AIMCP_API Response
	/// Builders for JSON-RPC responses.
{
public:
	static Poco::JSON::Object::Ptr result(const Id& id, const Poco::Dynamic::Var& result);
		/// Builds a success envelope {jsonrpc, id, result}.

	static Poco::JSON::Object::Ptr error(const Id& id, int code, const std::string& message,
		Poco::JSON::Object::Ptr data = nullptr);
		/// Builds an error envelope {jsonrpc, id, error:{code, message, data?}}.
};


AIMCP_API std::string serialize(const Poco::JSON::Object::Ptr& message);
	/// Serializes a message to a single compact JSON line (no embedded newlines).

AIMCP_API void writeMessage(std::ostream& out, const Poco::JSON::Object::Ptr& message);
	/// Writes a message as one newline-terminated JSON line and flushes.

AIMCP_API Poco::JSON::Object::Ptr readMessage(std::istream& in);
	/// Reads the next newline-delimited JSON object from the stream, skipping
	/// blank lines. Returns null at end of stream. Throws on malformed JSON.


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Message_INCLUDED
