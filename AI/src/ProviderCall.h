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
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/AutoPtr.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/URI.h"
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>


namespace Poco::AI {


using Poco::AI::MCP::HTTP::readBody;
using Poco::AI::MCP::HTTP::readAtMost;
using Poco::AI::MCP::HTTP::cutAtCharacter;
using Poco::AI::MCP::HTTP::excerpt;
using Poco::AI::MCP::HTTP::parseObject;


class ProviderCall
	/// One HTTP request of a provider: the session, the request, the abort a
	/// Cancellation calls while the request is sent and its response awaited
	/// and read, and the response.
{
public:
	ProviderCall(const SessionFactory& sessionFactory, const Poco::URI& uri, const std::string& method, const Poco::Timespan& timeout, const Poco::Net::NameValueCollection& headers = Poco::Net::NameValueCollection());
		/// Opens a session for uri. A zero timeout leaves the session's own;
		/// another bounds the connection, the send and each read. The given
		/// headers are added to the request.
		///
		/// Throws Poco::NullPointerException if the factory returns no session.

	~ProviderCall();
		/// Unregisters the abort, then closes what it used.

	Poco::Net::HTTPRequest& request();
		/// The request, with its method, path, host and HTTP version set.

	void send(const std::string& body, Cancellation& cancellation);
		/// Registers the abort with cancellation, and sends the request, with
		/// body unless it is empty. The abort is registered first, so that a
		/// cancel ends the connect, the TLS handshake and the upload too.
		///
		/// Throws Poco::IOException when the call is cancelled before it is
		/// sent, and the exceptions of the session.

	std::istream& receive();
		/// Receives the response head and returns the body stream.

	[[nodiscard]] const Poco::Net::HTTPResponse& response() const;
		/// The response, once received.

	[[nodiscard]] std::string networkError() const;
		/// The text of the network error the session met while reading, if any.

	[[nodiscard]] bool brokeOff(std::size_t received) const;
		/// Returns true if a body of received bytes broke off: the session met
		/// an error, or the body is shorter than its Content-Length.

private:
	void abort();
		/// Shuts the connection down, so that a read or write the calling
		/// thread blocks in returns. Only the connection: the socket and, for
		/// TLS, its state stay with the calling thread, which closes them.

	void keepDescriptor();
		/// Once connected: the abort reaches the connection through a
		/// descriptor of its own from now on.

	ProviderCall(const ProviderCall&) = delete;
	ProviderCall& operator = (const ProviderCall&) = delete;

	std::unique_ptr<Poco::Net::HTTPClientSession> _pSession;
	Poco::Net::HTTPRequest _request;
	Poco::Net::HTTPResponse _response;
	Poco::FastMutex _abortMutex;
	Poco::AutoPtr<Poco::Net::SocketImpl> _pSocket;
	poco_socket_t _descriptor;
	bool _aborted;
	std::unique_ptr<Cancellation::Request> _pAbort;
};


[[nodiscard]] Poco::URI joinURI(const Poco::URI& base, const std::string& segment);
	/// Appends a path segment to base, with exactly one slash between; the
	/// query stays.


[[nodiscard]] FailureCode failureCodeOf(int status, const std::string& body);
	/// The kind of failure an HTTP status that is not 200 reports. A 429
	/// whose error says that the quota is used up (OpenAI's
	/// insufficient_quota) or that the spend cap is reached (Anthropic's
	/// enforced_spend_limit_reached in error.details.error_code) is
	/// QUOTA_EXCEEDED, not RATE_LIMITED: waiting does not help.


void reportFailure(const ContentEventCallback& onEvent, FailureCode code, const std::string& text, const Poco::Net::HTTPResponse* pResponse = nullptr);
	/// Delivers a FAILURE event; with the response, also how long to wait
	/// before a retry (retry-after-ms or retry-after) and the request id the
	/// API gave the request (request-id or x-request-id).


[[nodiscard]] std::string serializeJSON(const Poco::Dynamic::Var& value);
	/// Compact JSON text of a value.


[[nodiscard]] Poco::JSON::Object::Ptr parseOrderedObject(const std::string& text);
	/// The JSON object in text with its members in the order of the text, or
	/// null: for what is sent back as it came.


[[nodiscard]] Poco::Dynamic::Var memberOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A member of an object that may be null; empty when absent.


[[nodiscard]] std::string stringOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A string member of an object that may be null; empty when absent,
	/// null or not a string.


[[nodiscard]] Poco::UInt64 countOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key);
	/// A count member of an object that may be null; 0 when absent or not a
	/// non-negative integer.


void refuseRequestMembers(const Poco::JSON::Object::Ptr& pMembers, std::initializer_list<const char*> names);
	/// Throws a Poco::InvalidArgumentException naming the first of names
	/// that pMembers has: the members of the request body a provider builds
	/// itself.


void mergeMembers(Poco::JSON::Object& body, const Poco::JSON::Object::Ptr& pMembers);
	/// Sets the members of pMembers in body, replacing those of the same name.


} // namespace Poco::AI


#endif // AI_ProviderCall_INCLUDED
