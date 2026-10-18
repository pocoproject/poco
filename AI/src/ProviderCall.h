//
// ProviderCall.h
//
// Library: AI
// Package: Providers
// Module:  ProviderCall
//
// The HTTP plumbing the providers share. Not installed.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ProviderCall_INCLUDED
#define AI_ProviderCall_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"
#include "Poco/Timespan.h"
#include "Poco/URI.h"
#include <istream>
#include <memory>
#include <string>


namespace Poco::AI {


class ProviderCall
	/// One HTTP request of a provider: the session, the request, the abort a
	/// Cancellation may call while the response is awaited and read, and the
	/// response.
{
public:
	ProviderCall(const SessionFactory& sessionFactory, const Poco::URI& uri, const std::string& method, const Poco::Timespan& timeout);
		/// Opens a session for uri. A zero timeout leaves the session's own.
		///
		/// Throws Poco::NullPointerException if the factory returns no session.

	~ProviderCall();

	Poco::Net::HTTPRequest& request();
		/// The request, with its method, path, host and HTTP version set.

	void send(const std::string& body);
		/// Sends the request, with body unless it is empty.

	std::istream& receive();
		/// Receives the response head and returns the body stream.

	[[nodiscard]] const Poco::Net::HTTPResponse& response() const;
		/// The response, once received.

	void abort();
		/// Shuts the connection's descriptor down, so that a read or write the
		/// calling thread blocks in returns. Only the descriptor: the socket
		/// object and, for TLS, its state stay with the calling thread, which
		/// also closes the socket. For the thread of a Cancellation, once the
		/// request is sent.

	[[nodiscard]] std::string networkError() const;
		/// The text of the network error the session met while reading, if any.

private:
	std::unique_ptr<Poco::Net::HTTPClientSession> _pSession;
	Poco::Net::HTTPRequest _request;
	Poco::Net::HTTPResponse _response;
};


[[nodiscard]] Poco::URI joinURI(const Poco::URI& base, const std::string& segment);
	/// Appends a path segment to base, with exactly one slash between; the
	/// query stays.


[[nodiscard]] std::string readAtMost(std::istream& stream, std::size_t maxSize);
	/// Reads at most maxSize bytes of stream.


[[nodiscard]] std::string readBody(std::istream& stream, std::size_t maxSize);
	/// Reads the whole stream; throws Poco::DataFormatException beyond maxSize bytes.


void cutAtCharacter(std::string& text, std::size_t maxSize);
	/// Cuts text to at most maxSize bytes, at a UTF-8 character boundary.


[[nodiscard]] std::string excerpt(const std::string& text, std::size_t maxSize = 500);
	/// The start of text, cut at a character boundary.


[[nodiscard]] ErrorCode errorCodeOf(int status);
	/// The error code of an HTTP status that is not 200.


[[nodiscard]] std::string serializeJSON(const Poco::Dynamic::Var& value);
	/// Compact JSON text of a value.


[[nodiscard]] Poco::JSON::Object::Ptr parseObject(const std::string& text);
	/// The JSON object in text, or null.


[[nodiscard]] Poco::Dynamic::Var memberOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A member of an object that may be null; empty when absent.


[[nodiscard]] std::string stringOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A string member of an object that may be null; empty when absent,
	/// null or not a string.


[[nodiscard]] Poco::UInt64 countOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A count member of an object that may be null; 0 when absent or not a
	/// non-negative integer.


} // namespace Poco::AI


#endif // AI_ProviderCall_INCLUDED
