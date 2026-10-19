//
// HTTPClient.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  HTTPClient
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/Socket.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Array.h"
#include "Poco/Base64Encoder.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Logger.h"
#include "Poco/Format.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <utility>


namespace Poco::AI::MCP::HTTP {


namespace
{
	const std::string SENTINEL_PREFIX("=?base64?");
	const std::string SENTINEL_SUFFIX("?=");
	constexpr Poco::Int64 MAX_SAFE_INTEGER = 9007199254740991;
		/// The largest integer a header may carry: the safe range of JavaScript.
	constexpr std::size_t MAX_LOGGED_NAME = 128;


	bool isToken(const std::string& name)
		/// Returns true if name is an HTTP field name token (RFC 9110).
	{
		static const std::string SPECIALS("!#$%&'*+-.^_`|~");
		if (name.empty()) return false;
		for (const char c: name)
		{
			const bool alnum = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
			if (!alnum && SPECIALS.find(c) == std::string::npos) return false;
		}
		return true;
	}


	bool isPlain(const std::string& value)
		/// Returns true if value can be sent as it is: printable ASCII without
		/// leading or trailing spaces, and not looking like the base64 form.
	{
		for (const char c: value)
		{
			const unsigned char u = static_cast<unsigned char>(c);
			if (u < 0x20 || u > 0x7E) return false;
		}
		if (!value.empty() && (value.front() == ' ' || value.back() == ' ')) return false;
		// Starts like the base64 form and ends like it, even where the two
		// overlap, as in "=?base64?=".
		const bool sentinel = value.compare(0, SENTINEL_PREFIX.size(), SENTINEL_PREFIX) == 0
			&& value.size() >= SENTINEL_SUFFIX.size()
			&& value.compare(value.size() - SENTINEL_SUFFIX.size(), SENTINEL_SUFFIX.size(), SENTINEL_SUFFIX) == 0;
		return !sentinel;
	}


	std::string headerValue(const std::string& value)
		/// The value as a header carries it: as it is, or in the base64 form.
	{
		if (isPlain(value)) return value;
		std::ostringstream encoded;
		Poco::Base64Encoder encoder(encoded);
		encoder.rdbuf()->setLineLength(0);
		encoder << value;
		encoder.close();
		return SENTINEL_PREFIX + encoded.str() + SENTINEL_SUFFIX;
	}


	bool hasAnnotation(const Poco::Dynamic::Var& value)
		/// Returns true if an x-mcp-header annotation sits anywhere in value.
	{
		if (value.type() == typeid(Poco::JSON::Object::Ptr))
		{
			const Poco::JSON::Object::Ptr& pObject = value.extract<Poco::JSON::Object::Ptr>();
			if (pObject.isNull()) return false;
			for (const std::string& key: pObject->getNames())
			{
				if (key == "x-mcp-header" || hasAnnotation(pObject->get(key))) return true;
			}
		}
		else if (value.type() == typeid(Poco::JSON::Array::Ptr))
		{
			const Poco::JSON::Array::Ptr& pArray = value.extract<Poco::JSON::Array::Ptr>();
			if (pArray.isNull()) return false;
			for (const auto& element: *pArray)
			{
				if (hasAnnotation(element)) return true;
			}
		}
		return false;
	}


	struct Annotation
	{
		std::string name;
		std::vector<std::string> path;
	};


	void collectAnnotations(const Poco::JSON::Object::Ptr& pSchema, std::vector<std::string>& path, std::vector<Annotation>& annotations)
		/// Collects the x-mcp-header annotations of a schema object reached from
		/// the root through properties keys only. Throws Poco::DataFormatException
		/// for an invalid annotation, or one anywhere else.
	{
		for (const std::string& key: pSchema->getNames())
		{
			const Poco::Dynamic::Var value = pSchema->get(key);
			if (key == "x-mcp-header")
			{
				if (path.empty())
				{
					throw Poco::DataFormatException("x-mcp-header at the root of the input schema");
				}
				if (!value.isString() || !isToken(value.extract<std::string>()))
				{
					throw Poco::DataFormatException("x-mcp-header that is not a header name");
				}
				const Poco::Dynamic::Var type = pSchema->get("type");
				const std::string typeName = type.isString() ? type.extract<std::string>() : std::string();
				if (typeName != "string" && typeName != "integer" && typeName != "boolean")
				{
					throw Poco::DataFormatException("x-mcp-header on a parameter that is not a string, an integer or a boolean");
				}
				const std::string& name = value.extract<std::string>();
				for (const Annotation& annotation: annotations)
				{
					if (Poco::icompare(annotation.name, name) == 0)
					{
						throw Poco::DataFormatException("x-mcp-header name used twice", name);
					}
				}
				annotations.push_back(Annotation{name, path});
			}
			else if (key == "properties" && pSchema->isObject(key))
			{
				const Poco::JSON::Object::Ptr pProperties = pSchema->getObject(key);
				for (const std::string& property: pProperties->getNames())
				{
					if (!pProperties->isObject(property)) continue;
					path.push_back(property);
					collectAnnotations(pProperties->getObject(property), path, annotations);
					path.pop_back();
				}
			}
			else if (hasAnnotation(value))
			{
				// Below items, the composition and conditional keywords and $ref,
				// a parameter is not statically reachable.
				throw Poco::DataFormatException("x-mcp-header on a parameter that is not reached through properties alone");
			}
		}
	}


	bool render(const Poco::Dynamic::Var& value, std::string& text)
		/// Renders a parameter value as a header value; false for no value.
	{
		if (value.isEmpty()) return false;
		if (value.isString())
		{
			text = value.extract<std::string>();
			return true;
		}
		if (value.isBoolean())
		{
			text = value.extract<bool>() ? "true" : "false";
			return true;
		}
		// An integer outside the safe range has no header, whether it came as
		// an integer or as a number with no fraction.
		if (value.isInteger())
		{
			Poco::Int64 number = 0;
			try
			{
				number = value.convert<Poco::Int64>();
			}
			catch (const Poco::RangeException&)
			{
				return false;
			}
			if (number < -MAX_SAFE_INTEGER || number > MAX_SAFE_INTEGER) return false;
			text = Poco::NumberFormatter::format(number);
			return true;
		}
		if (value.isNumeric())
		{
			const double number = value.convert<double>();
			if (std::fabs(number) > static_cast<double>(MAX_SAFE_INTEGER) || std::floor(number) != number) return false;
			text = Poco::NumberFormatter::format(static_cast<Poco::Int64>(number));
			return true;
		}
		return false;
	}


	std::string loggable(const std::string& text)
		/// Text from the server as a log line may carry it: control bytes
		/// escaped, at most MAX_LOGGED_NAME bytes.
	{
		static const char HEX[] = "0123456789abcdef";
		std::string result;
		for (const char c: text)
		{
			const unsigned char u = static_cast<unsigned char>(c);
			if (u < 0x20 || u == 0x7F)
			{
				result += "\\x";
				result += HEX[u >> 4];
				result += HEX[u & 0x0F];
			}
			else result += c;
			if (result.size() > MAX_LOGGED_NAME) break;
		}
		return excerpt(result, MAX_LOGGED_NAME);
	}


	bool readsOnly(const std::string& method)
		/// The requests that change nothing on the server, which may be sent
		/// again after an answer was lost.
	{
		return method == "server/discover" || (method.size() > 5 && method.compare(method.size() - 5, 5, "/list") == 0);
	}


	Poco::Timespan left(const Poco::Timestamp& deadline, const std::string& method)
		/// The time left until deadline, at least a millisecond; throws
		/// Poco::TimeoutException when it has passed.
	{
		const Poco::Timespan remaining(deadline - Poco::Timestamp());
		if (remaining.totalMilliseconds() <= 0)
		{
			throw Poco::TimeoutException("No reply from the MCP endpoint to " + method + " within the timeout");
		}
		return std::max(remaining, Poco::Timespan(0, 1000));
	}


	Poco::Dynamic::Var valueAt(const Poco::JSON::Object::Ptr& pArguments, const std::vector<std::string>& path)
		/// The value at the property path, or an empty Var.
	{
		Poco::JSON::Object::Ptr pObject = pArguments;
		for (std::size_t i = 0; i + 1 < path.size(); ++i)
		{
			if (pObject.isNull() || !pObject->isObject(path[i])) return Poco::Dynamic::Var();
			pObject = pObject->getObject(path[i]);
		}
		if (pObject.isNull() || path.empty()) return Poco::Dynamic::Var();
		return pObject->get(path.back());
	}
}


HTTPClient::HTTPClient(const Poco::URI& endpoint, SessionFactory sessionFactory):
	_uri(endpoint),
	_sessionFactory(std::move(sessionFactory)),
	_maxResponseSize(DEFAULT_MAX_MESSAGE_SIZE),
	_sessionUsed(false)
{
}


HTTPClient::~HTTPClient()
{
}


void HTTPClient::setRequestHook(RequestHook hook)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_hook = std::move(hook);
}


void HTTPClient::setMaxResponseSize(std::size_t maxSize)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_maxResponseSize = maxSize;
}


std::size_t HTTPClient::getMaxResponseSize() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _maxResponseSize;
}


void HTTPClient::toolsListed(std::vector<Tool>& tools)
{
	std::map<std::string, std::vector<ParamHeader>> paramHeaders;
	std::vector<Tool> valid;
	for (Tool& tool: tools)
	{
		std::vector<Annotation> annotations;
		std::vector<std::string> path;
		try
		{
			collectAnnotations(tool.inputSchema, path, annotations);
		}
		catch (const Poco::DataFormatException& exc)
		{
			Poco::Logger& logger = Poco::Logger::get("AI.MCP.HTTPClient");
			if (logger.warning())
			{
				logger.warning("Tool %s of %s left out: %s", loggable(tool.name), redactedEndpoint(), exc.displayText());
			}
			continue;
		}
		std::vector<ParamHeader>& headers = paramHeaders[tool.name];
		for (Annotation& annotation: annotations)
		{
			headers.push_back(ParamHeader{"Mcp-Param-" + annotation.name, std::move(annotation.path)});
		}
		valid.push_back(std::move(tool));
	}
	tools = std::move(valid);
	Poco::FastMutex::ScopedLock lock(_mutex);
	_paramHeaders = std::move(paramHeaders);
}


void HTTPClient::addHeaders(Poco::Net::HTTPRequest& httpRequest, const Poco::JSON::Object::Ptr& request)
{
	const std::string method = request->getValue<std::string>("method");
	if (!isPlain(method) || method.empty())
	{
		throw Poco::InvalidArgumentException("An MCP method name must be printable ASCII", method);
	}
	httpRequest.set("MCP-Protocol-Version", PROTOCOL_VERSION);
	httpRequest.set("Mcp-Method", method);

	const Poco::JSON::Object::Ptr pParams = request->isObject("params") ? request->getObject("params") : Poco::JSON::Object::Ptr();
	if (pParams.isNull()) return;
	const std::string nameKey = method == "resources/read" ? "uri" : "name";
	if (method == "tools/call" || method == "prompts/get" || method == "resources/read")
	{
		const Poco::Dynamic::Var name = pParams->get(nameKey);
		if (name.isString())
		{
			httpRequest.set("Mcp-Name", headerValue(name.extract<std::string>()));
		}
	}
	if (method == "tools/call" && pParams->get("name").isString())
	{
		std::vector<ParamHeader> headers;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			const auto it = _paramHeaders.find(pParams->getValue<std::string>("name"));
			if (it != _paramHeaders.end()) headers = it->second;
		}
		const Poco::JSON::Object::Ptr pArguments = pParams->isObject("arguments") ? pParams->getObject("arguments") : Poco::JSON::Object::Ptr();
		for (const ParamHeader& header: headers)
		{
			std::string text;
			if (render(valueAt(pArguments, header.path), text))
			{
				httpRequest.set(header.name, headerValue(text));
			}
		}
	}
}


std::string HTTPClient::redactedEndpoint() const
{
	// Without user info, query or fragment, which may carry credentials.
	std::string text = _uri.getScheme() + "://" + _uri.getHost();
	if (_uri.getSpecifiedPort() != 0) text += ":" + Poco::NumberFormatter::format(_uri.getSpecifiedPort());
	return text + _uri.getPath();
}


Poco::Net::HTTPClientSession& HTTPClient::session(bool& reused)
{
	// A session without a host cannot reconnect by itself, e.g. one attached
	// to a tunnel: it serves one request.
	if (_pSession != nullptr && _sessionUsed && _pSession->getHost().empty())
	{
		_pSession.reset();
	}
	// A kept connection the server has closed, which shows as readable while
	// no answer is due, is not used again.
	if (_pSession != nullptr && _sessionUsed && _pSession->connected())
	{
		try
		{
			if (_pSession->socket().poll(Poco::Timespan(0), Poco::Net::Socket::SELECT_READ | Poco::Net::Socket::SELECT_ERROR))
			{
				_pSession.reset();
			}
		}
		catch (const Poco::Exception&)
		{
			_pSession.reset();
		}
	}
	reused = _pSession != nullptr && _sessionUsed;
	if (_pSession == nullptr)
	{
		_pSession = _sessionFactory ? _sessionFactory(_uri) : createClientSession(_uri);
		if (_pSession == nullptr)
		{
			throw Poco::NullPointerException("The session factory returned no session", redactedEndpoint());
		}
		_pSession->setKeepAlive(true);
		_sessionUsed = false;
	}
	return *_pSession;
}


Poco::JSON::Object::Ptr HTTPClient::exchange(const Poco::JSON::Object::Ptr& request)
{
	// One deadline for the request and the one time it may be sent again.
	const Poco::Timestamp deadline = Poco::Timestamp() + getTimeout().totalMicroseconds();
	bool retry = false;
	Poco::JSON::Object::Ptr pReply = post(request, deadline, retry);
	if (retry)
	{
		pReply = post(request, deadline, retry);
	}
	return pReply;
}


Poco::JSON::Object::Ptr HTTPClient::post(const Poco::JSON::Object::Ptr& request, const Poco::Timestamp& deadline, bool& retry)
{
	retry = false;
	const Id id = request->get("id");
	const std::string method = request->getValue<std::string>("method");
	const std::string body = serialize(request);
	std::string path = _uri.getPathAndQuery();
	if (path.empty()) path = "/";

	Poco::Net::HTTPRequest httpRequest(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);
	// The Host header names the endpoint, whatever the session is connected to
	// (a factory may hand out a session through a proxy or a tunnel).
	httpRequest.setHost(_uri.getHost(), _uri.getPort());
	httpRequest.setContentType("application/json");
	httpRequest.set("Accept", "application/json, text/event-stream");
	addHeaders(httpRequest, request);
	httpRequest.setContentLength(static_cast<std::streamsize>(body.size()));

	RequestHook hook;
	std::size_t maxSize = 0;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		hook = _hook;
		maxSize = _maxResponseSize;
	}
	if (hook) hook(httpRequest);

	bool reused = false;
	Poco::Net::HTTPClientSession& httpSession = session(reused);
	// Every step that waits gets the time that is left: the connection, the
	// request, the head of the answer and each read of its body.
	const Poco::Timespan sendTime = left(deadline, method);
	httpSession.setTimeout(sendTime);
	const auto bound = [&httpSession, &deadline, &method]()
	{
		const Poco::Timespan remaining = left(deadline, method);
		if (httpSession.connected()) httpSession.socket().setReceiveTimeout(remaining);
	};
	if (httpSession.connected())
	{
		httpSession.socket().setSendTimeout(sendTime);
		httpSession.socket().setReceiveTimeout(sendTime);
	}
	Poco::Net::HTTPResponse response;
	std::istream* pStream = nullptr;
	bool sent = false;
	try
	{
		std::ostream& os = httpSession.sendRequest(httpRequest);
		os << body;
		os.flush();
		if (os.bad())
		{
			if (httpSession.networkException() != nullptr) httpSession.networkException()->rethrow();
			throw Poco::IOException("Cannot send the request to the MCP endpoint");
		}
		sent = true;
		bound();
		pStream = &httpSession.receiveResponse(response);
	}
	catch (const Poco::IOException&)
	{
		_pSession.reset();
		// Once more, on a new connection: a request that cannot have reached
		// the server, or one that only reads. A tool call the server may have
		// run is not repeated.
		if (reused && (!sent || readsOnly(method)) && Poco::Timestamp() < deadline)
		{
			retry = true;
			return Poco::JSON::Object::Ptr();
		}
		throw;
	}
	catch (...)
	{
		_pSession.reset();
		throw;
	}
	_sessionUsed = true;

	try
	{
		const int status = static_cast<int>(response.getStatus());
		const bool success = status >= 200 && status < 300 && status != Poco::Net::HTTPResponse::HTTP_ACCEPTED;
		const bool eventStream = response.has(Poco::Net::HTTPMessage::CONTENT_TYPE)
			&& Poco::Net::MediaType(response.getContentType()).matches("text", "event-stream");
		if (success && eventStream)
		{
			EventStreamReader reader(*pStream, maxSize);
			reader.setReadGuard(bound);
			EventStreamReader::Event event;
			while (reader.next(event))
			{
				if (!event.type.empty() && event.type != "message") continue;
				const Poco::JSON::Object::Ptr pMessage = parseObject(event.data);
				if (isReply(pMessage, id))
				{
					// The server may keep the stream open: the next request takes a new session.
					_pSession.reset();
					return pMessage;
				}
			}
			// A chunked stream that broke off ends as if it were complete; the
			// session knows better.
			if (httpSession.networkException() != nullptr) httpSession.networkException()->rethrow();
			_pSession.reset();
			throw Poco::IOException("The MCP event stream ended before the reply to " + method);
		}

		const std::string text = readBody(*pStream, maxSize, bound);
		// A body that broke off reads as a shorter one: the session or the
		// length the answer promised tells.
		if (httpSession.networkException() != nullptr) httpSession.networkException()->rethrow();
		if (response.hasContentLength() && static_cast<Poco::Int64>(text.size()) < response.getContentLength64())
		{
			throw Poco::IOException("The answer of the MCP endpoint to " + method + " broke off");
		}
		if (status == Poco::Net::HTTPResponse::HTTP_ACCEPTED)
		{
			throw Poco::ProtocolException("The MCP endpoint accepted the request without a reply");
		}
		const Poco::JSON::Object::Ptr pMessage = parseObject(text);
		if (success)
		{
			if (pMessage.isNull())
			{
				throw Poco::ProtocolException("The MCP endpoint did not answer with a JSON object", excerpt(text, 200));
			}
			if (!isReply(pMessage, id))
			{
				throw Poco::ProtocolException("The MCP endpoint answered another request");
			}
			return pMessage;
		}
		if ((status == Poco::Net::HTTPResponse::HTTP_BAD_REQUEST || status == Poco::Net::HTTPResponse::HTTP_NOT_FOUND)
			&& !pMessage.isNull() && pMessage->isObject("error"))
		{
			if (!isReply(pMessage, id))
			{
				throw Poco::ProtocolException("The MCP endpoint answered another request");
			}
			return pMessage;
		}
		throw Poco::Net::HTTPException(Poco::format("The MCP endpoint answered HTTP %d %s", status, response.getReason()), excerpt(text, 200));
	}
	catch (...)
	{
		_pSession.reset();
		throw;
	}
}


} // namespace Poco::AI::MCP::HTTP
