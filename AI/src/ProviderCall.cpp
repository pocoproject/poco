//
// ProviderCall.cpp
//
// Library: AI
// Package: Providers
// Module:  ProviderCall
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProviderCall.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <sstream>
#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <unistd.h>
#endif


namespace Poco::AI {


namespace
{
	poco_socket_t duplicate(poco_socket_t fd)
		/// Another descriptor of the same connection, which only its owner
		/// closes; invalid when there is none.
	{
		if (fd == POCO_INVALID_SOCKET) return POCO_INVALID_SOCKET;
#if defined(POCO_OS_FAMILY_WINDOWS)
		WSAPROTOCOL_INFOW info;
		if (::WSADuplicateSocketW(fd, ::GetCurrentProcessId(), &info) != 0) return POCO_INVALID_SOCKET;
		return ::WSASocketW(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, &info, 0, 0);
#else
		const int copy = ::dup(fd);
		return copy >= 0 ? copy : POCO_INVALID_SOCKET;
#endif
	}


	Poco::Dynamic::Var copyOf(const Poco::Dynamic::Var& value)
		/// A copy of a JSON value; objects and arrays recursively.
	{
		if (value.type() == typeid(Poco::JSON::Object::Ptr))
		{
			const Poco::JSON::Object::Ptr& pObject = value.extract<Poco::JSON::Object::Ptr>();
			if (pObject.isNull()) return value;
			Poco::JSON::Object::Ptr pCopy = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
			for (const std::string& name: pObject->getNames()) pCopy->set(name, copyOf(pObject->get(name)));
			return pCopy;
		}
		if (value.type() == typeid(Poco::JSON::Array::Ptr))
		{
			const Poco::JSON::Array::Ptr& pArray = value.extract<Poco::JSON::Array::Ptr>();
			if (pArray.isNull()) return value;
			Poco::JSON::Array::Ptr pCopy = new Poco::JSON::Array;
			for (const auto& element: *pArray) pCopy->add(copyOf(element));
			return pCopy;
		}
		return value;
	}


	Poco::Timespan retryAfterOf(const Poco::Net::HTTPResponse& response)
		/// How long the response asks to wait: retry-after-ms, or retry-after
		/// in seconds or as an HTTP date; zero when it does not say. The value
		/// comes from the server and is taken up to a day, before it is
		/// multiplied, so that no value overflows.
	{
		const Poco::Timespan::TimeDiff maxWait = 24*Poco::Timespan::HOURS;
		Poco::Int64 value = 0;
		if (Poco::NumberParser::tryParse64(Poco::trim(response.get("retry-after-ms", "")), value) && value > 0)
		{
			return Poco::Timespan(std::min(value, maxWait/Poco::Timespan::MILLISECONDS)*Poco::Timespan::MILLISECONDS);
		}
		const std::string text = Poco::trim(response.get("retry-after", ""));
		if (Poco::NumberParser::tryParse64(text, value))
		{
			return value > 0 ? Poco::Timespan(std::min(value, maxWait/Poco::Timespan::SECONDS)*Poco::Timespan::SECONDS) : Poco::Timespan();
		}
		Poco::DateTime date;
		int timeZone = 0;
		if (!text.empty() && Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::HTTP_FORMAT, text, date, timeZone))
		{
			const Poco::Timestamp::TimeDiff wait = date.timestamp() - Poco::Timestamp();
			return wait > 0 ? Poco::Timespan(std::min(wait, maxWait)) : Poco::Timespan();
		}
		return Poco::Timespan();
	}
}


ProviderCall::ProviderCall(const SessionFactory& sessionFactory, const Poco::URI& uri, const std::string& method, const Poco::Timespan& timeout, const Poco::Net::NameValueCollection& headers):
	_pSession(sessionFactory ? sessionFactory(uri) : createClientSession(uri)),
	_request(method, uri.getPathAndQuery().empty() ? std::string("/") : uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1),
	_descriptor(POCO_INVALID_SOCKET),
	_aborted(false)
{
	if (_pSession == nullptr)
	{
		throw Poco::NullPointerException("The session factory returned no session", uri.getHost());
	}
	if (timeout.totalMicroseconds() > 0)
	{
		_pSession->setTimeout(timeout);
	}
	// The Host header names the endpoint, whatever the session is connected to
	// (a factory may hand out a session through a proxy or a tunnel).
	_request.setHost(uri.getHost(), uri.getPort());
	for (const auto& header: headers)
	{
		_request.set(header.first, header.second);
	}
}


ProviderCall::~ProviderCall()
{
	// Once the abort is unregistered it neither runs nor will be called.
	_pAbort.reset();
	if (_descriptor != POCO_INVALID_SOCKET)
	{
		poco_closesocket(_descriptor);
	}
}


Poco::Net::HTTPRequest& ProviderCall::request()
{
	return _request;
}


void ProviderCall::send(const std::string& body, Cancellation& cancellation)
{
	_pSocket = Poco::AutoPtr<Poco::Net::SocketImpl>(_pSession->socket().impl(), true);
	_pAbort = std::make_unique<Cancellation::Request>(cancellation, [this]() { abort(); });
	{
		Poco::FastMutex::ScopedLock lock(_abortMutex);
		if (_aborted) throw Poco::IOException("The call was cancelled before it was sent");
	}
	if (!body.empty())
	{
		_request.setContentLength(static_cast<std::streamsize>(body.size()));
	}
	std::ostream& out = _pSession->sendRequest(_request);
	keepDescriptor();
	out << body;
}


std::istream& ProviderCall::receive()
{
	return _pSession->receiveResponse(_response);
}


const Poco::Net::HTTPResponse& ProviderCall::response() const
{
	return _response;
}


void ProviderCall::abort()
{
	Poco::FastMutex::ScopedLock lock(_abortMutex);
	_aborted = true;
	if (_descriptor != POCO_INVALID_SOCKET)
	{
		// 2 shuts both directions down, on every platform.
		(void) ::shutdown(_descriptor, 2);
		return;
	}
	// Still connecting or sending the head: through the socket the session
	// had when the call began. Should the session close it in a failure at
	// this very moment and another thread open a socket that gets the same
	// descriptor at once, that one would be shut down; a descriptor of its
	// own, from keepDescriptor() on, rules that out.
	if (!_pSocket.isNull())
	{
		const poco_socket_t fd = _pSocket->sockfd();
		if (fd != POCO_INVALID_SOCKET) (void) ::shutdown(fd, 2);
	}
}


void ProviderCall::keepDescriptor()
{
	const poco_socket_t copy = duplicate(_pSession->socket().impl()->sockfd());
	Poco::FastMutex::ScopedLock lock(_abortMutex);
	_descriptor = copy;
	if (_aborted && _descriptor != POCO_INVALID_SOCKET)
	{
		(void) ::shutdown(_descriptor, 2);
	}
}


std::string ProviderCall::networkError() const
{
	const Poco::Exception* pException = _pSession->networkException();
	return pException != nullptr ? pException->displayText() : std::string();
}


bool ProviderCall::brokeOff(std::size_t received) const
{
	if (_pSession->networkException() != nullptr) return true;
	return _response.hasContentLength() && static_cast<Poco::Int64>(received) < _response.getContentLength64();
}


Poco::URI joinURI(const Poco::URI& base, const std::string& segment)
{
	Poco::URI uri(base);
	std::string path = uri.getPath();
	if (path.empty() || path.back() != '/') path += '/';
	path += segment;
	uri.setPath(path);
	return uri;
}


FailureCode failureCodeOf(int status, const std::string& body)
{
	if (status == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED || status == Poco::Net::HTTPResponse::HTTP_FORBIDDEN) return FailureCode::UNAUTHENTICATED;
	if (status == Poco::Net::HTTPResponse::HTTP_PAYMENT_REQUIRED) return FailureCode::QUOTA_EXCEEDED;
	if (status == Poco::Net::HTTPResponse::HTTP_TOO_MANY_REQUESTS)
	{
		const Poco::JSON::Object::Ptr pRoot = parseObject(body);
		const Poco::JSON::Object::Ptr pError = pRoot.isNull() ? Poco::JSON::Object::Ptr() : pRoot->getObject("error");
		const std::string type = stringOf(pError, "type");
		const std::string code = stringOf(pError, "code");
		if (type == "insufficient_quota" || code == "insufficient_quota" || type == "billing_error") return FailureCode::QUOTA_EXCEEDED;
		return FailureCode::RATE_LIMITED;
	}
	if (status == Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE || status == 529) return FailureCode::OVERLOADED;
	if (status >= 500) return FailureCode::SERVER_ERROR;
	return FailureCode::REQUEST_REJECTED;
}


void reportFailure(const ContentEventCallback& onEvent, FailureCode code, const std::string& text, const Poco::Net::HTTPResponse* pResponse)
{
	ContentEvent event;
	event.type = ContentEvent::Type::FAILURE;
	event.code = code;
	event.text = text;
	if (pResponse != nullptr)
	{
		event.retryAfter = retryAfterOf(*pResponse);
		event.requestId = pResponse->get("request-id", pResponse->get("x-request-id", ""));
	}
	if (onEvent) onEvent(event);
}


std::string serializeJSON(const Poco::Dynamic::Var& value)
{
	std::ostringstream os;
	Poco::JSON::Stringifier::stringify(value, os);
	return os.str();
}


Poco::JSON::Object::Ptr parseOrderedObject(const std::string& text)
{
	try
	{
		Poco::JSON::Parser parser(new Poco::JSON::ParseHandler(true));
		const Poco::Dynamic::Var parsed = parser.parse(text);
		if (parsed.type() == typeid(Poco::JSON::Object::Ptr)) return parsed.extract<Poco::JSON::Object::Ptr>();
	}
	catch (const Poco::Exception&)
	{
	}
	return Poco::JSON::Object::Ptr();
}


Poco::Dynamic::Var memberOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
{
	return pObject.isNull() ? Poco::Dynamic::Var() : pObject->get(key);
}


std::string stringOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
{
	const Poco::Dynamic::Var value = memberOf(pObject, key);
	return value.isString() ? value.extract<std::string>() : std::string();
}


Poco::UInt64 countOf(const Poco::JSON::Object::Ptr& pObject, const std::string& key)
{
	const Poco::Dynamic::Var value = memberOf(pObject, key);
	if (!value.isInteger() || value.isBoolean()) return 0;
	try
	{
		const Poco::Int64 count = value.convert<Poco::Int64>();
		return count > 0 ? static_cast<Poco::UInt64>(count) : 0;
	}
	catch (const Poco::Exception&)
	{
		return 0;
	}
}


void mergeMembers(Poco::JSON::Object& body, const Poco::JSON::Object::Ptr& pMembers)
{
	if (pMembers.isNull()) return;
	// Copies: stringifying a JSON object writes into it, and calls that run
	// at once share the object of the settings.
	for (const std::string& name: pMembers->getNames())
	{
		body.set(name, copyOf(pMembers->get(name)));
	}
}


} // namespace Poco::AI
