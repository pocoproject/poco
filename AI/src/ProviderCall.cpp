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
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <sstream>


namespace Poco::AI {


ProviderCall::ProviderCall(const SessionFactory& sessionFactory, const Poco::URI& uri, const std::string& method, const Poco::Timespan& timeout):
	_pSession(sessionFactory ? sessionFactory(uri) : createClientSession(uri)),
	_request(method, uri.getPathAndQuery().empty() ? std::string("/") : uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1)
{
	if (_pSession == nullptr)
	{
		throw Poco::NullPointerException("The session factory returned no session", uri.toString());
	}
	if (timeout.totalMicroseconds() > 0)
	{
		_pSession->setTimeout(timeout);
	}
	// The Host header names the endpoint, whatever the session is connected to
	// (a factory may hand out a session through a proxy or a tunnel).
	_request.setHost(uri.getHost(), uri.getPort());
}


ProviderCall::~ProviderCall()
{
}


Poco::Net::HTTPRequest& ProviderCall::request()
{
	return _request;
}


void ProviderCall::send(const std::string& body)
{
	if (!body.empty())
	{
		_request.setContentLength(static_cast<std::streamsize>(body.size()));
	}
	std::ostream& out = _pSession->sendRequest(_request);
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
	const poco_socket_t fd = _pSession->socket().impl()->sockfd();
	if (fd != POCO_INVALID_SOCKET)
	{
		// 2 shuts both directions down, on every platform.
		(void) ::shutdown(fd, 2);
	}
}


std::string ProviderCall::networkError() const
{
	const Poco::Exception* pException = _pSession->networkException();
	return pException != nullptr ? pException->displayText() : std::string();
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


std::string readAtMost(std::istream& stream, std::size_t maxSize)
{
	std::string text(maxSize, '\0');
	stream.read(&text[0], static_cast<std::streamsize>(maxSize));
	text.resize(static_cast<std::size_t>(stream.gcount()));
	return text;
}


std::string readBody(std::istream& stream, std::size_t maxSize)
{
	std::string body;
	char buffer[8192];
	while (stream.read(buffer, sizeof(buffer)) || stream.gcount() > 0)
	{
		if (body.size() + static_cast<std::size_t>(stream.gcount()) > maxSize)
		{
			throw Poco::DataFormatException("The response exceeds the size limit");
		}
		body.append(buffer, static_cast<std::size_t>(stream.gcount()));
	}
	return body;
}


void cutAtCharacter(std::string& text, std::size_t maxSize)
{
	if (text.size() <= maxSize) return;
	std::size_t n = maxSize;
	// A continuation byte (10xxxxxx) belongs to the character before it.
	while (n > 0 && (static_cast<unsigned char>(text[n]) & 0xC0) == 0x80) --n;
	text.resize(n);
}


std::string excerpt(const std::string& text, std::size_t maxSize)
{
	std::string result(text);
	cutAtCharacter(result, maxSize);
	return result;
}


ErrorCode errorCodeOf(int status)
{
	if (status == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED || status == Poco::Net::HTTPResponse::HTTP_FORBIDDEN) return ErrorCode::UNAUTHENTICATED;
	if (status == Poco::Net::HTTPResponse::HTTP_TOO_MANY_REQUESTS) return ErrorCode::RATE_LIMITED;
	if (status == Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE || status == 529) return ErrorCode::OVERLOADED;
	if (status >= 500) return ErrorCode::SERVER_ERROR;
	return ErrorCode::REQUEST_REJECTED;
}


std::string serializeJSON(const Poco::Dynamic::Var& value)
{
	std::ostringstream os;
	Poco::JSON::Stringifier::stringify(value, os);
	return os.str();
}


Poco::JSON::Object::Ptr parseObject(const std::string& text)
{
	try
	{
		Poco::JSON::Parser parser;
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


} // namespace Poco::AI
