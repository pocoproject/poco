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
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Array.h"
#include "Poco/Base64Encoder.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Logger.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
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
		const bool sentinel = value.size() >= SENTINEL_PREFIX.size() + SENTINEL_SUFFIX.size()
			&& value.compare(0, SENTINEL_PREFIX.size(), SENTINEL_PREFIX) == 0
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
			const Poco::JSON::Object::Ptr pObject = value.extract<Poco::JSON::Object::Ptr>();
			if (pObject.isNull()) return false;
			for (const std::string& key: pObject->getNames())
			{
				if (key == "x-mcp-header" || hasAnnotation(pObject->get(key))) return true;
			}
		}
		else if (value.type() == typeid(Poco::JSON::Array::Ptr))
		{
			const Poco::JSON::Array::Ptr pArray = value.extract<Poco::JSON::Array::Ptr>();
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
				const std::string name = value.extract<std::string>();
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
		if (value.isInteger())
		{
			text = value.toString();
			return true;
		}
		if (value.isNumeric())
		{
			const double number = value.convert<double>();
			if (std::floor(number) == number && std::fabs(number) <= 9007199254740991.0)
			{
				text = Poco::NumberFormatter::format(static_cast<Poco::Int64>(number));
				return true;
			}
		}
		return false;
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


	Poco::JSON::Object::Ptr parseObject(const std::string& text)
		/// The JSON object in text, or null.
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


	std::string readBody(std::istream& stream, std::size_t maxSize)
		/// Reads the body, throwing when it exceeds maxSize bytes.
	{
		std::string body;
		char buffer[8192];
		while (stream.read(buffer, sizeof(buffer)) || stream.gcount() > 0)
		{
			if (body.size() + static_cast<std::size_t>(stream.gcount()) > maxSize)
			{
				throw Poco::DataFormatException("The MCP reply exceeds the size limit");
			}
			body.append(buffer, static_cast<std::size_t>(stream.gcount()));
		}
		return body;
	}


	std::string excerpt(const std::string& body)
		/// The start of a body, cut at a character boundary.
	{
		static const std::size_t MAX_EXCERPT = 200;
		if (body.size() <= MAX_EXCERPT) return body;
		std::size_t n = MAX_EXCERPT;
		while (n > 0 && (static_cast<unsigned char>(body[n]) & 0xC0) == 0x80) --n;
		return body.substr(0, n) + "...";
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
				logger.warning("Tool %s of %s left out: %s", tool.name, _uri.toString(), exc.displayText());
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


Poco::Net::HTTPClientSession& HTTPClient::session(bool& reused)
{
	// A session without a host cannot reconnect by itself, e.g. one attached
	// to a tunnel: it serves one request.
	if (_pSession != nullptr && _sessionUsed && _pSession->getHost().empty())
	{
		_pSession.reset();
	}
	reused = _pSession != nullptr && _sessionUsed;
	if (_pSession == nullptr)
	{
		_pSession = _sessionFactory ? _sessionFactory(_uri) : createClientSession(_uri);
		if (_pSession == nullptr)
		{
			throw Poco::NullPointerException("The session factory returned no session", _uri.toString());
		}
		_pSession->setKeepAlive(true);
		_sessionUsed = false;
	}
	return *_pSession;
}


Poco::JSON::Object::Ptr HTTPClient::exchange(const Poco::JSON::Object::Ptr& request)
{
	bool retry = false;
	Poco::JSON::Object::Ptr pReply = post(request, retry);
	if (retry)
	{
		pReply = post(request, retry);
	}
	return pReply;
}


Poco::JSON::Object::Ptr HTTPClient::post(const Poco::JSON::Object::Ptr& request, bool& retry)
{
	retry = false;
	const Id id = request->get("id");
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
	const Poco::Timespan timeout = getTimeout();
	httpSession.setTimeout(timeout);
	Poco::Net::HTTPResponse response;
	std::istream* pStream = nullptr;
	try
	{
		std::ostream& os = httpSession.sendRequest(httpRequest);
		os << body;
		pStream = &httpSession.receiveResponse(response);
	}
	catch (const Poco::IOException&)
	{
		_pSession.reset();
		if (reused)
		{
			// The server may have closed the idle connection: once more, on a new one.
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
			reader.setDeadline(Poco::Timestamp() + timeout.totalMicroseconds());
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
			_pSession.reset();
			throw Poco::IOException("The MCP event stream ended before the reply to " + request->getValue<std::string>("method"));
		}

		const std::string text = readBody(*pStream, maxSize);
		if (status == Poco::Net::HTTPResponse::HTTP_ACCEPTED)
		{
			throw Poco::ProtocolException("The MCP endpoint accepted the request without a reply");
		}
		const Poco::JSON::Object::Ptr pMessage = parseObject(text);
		if (success)
		{
			if (pMessage.isNull())
			{
				throw Poco::ProtocolException("The MCP endpoint did not answer with a JSON object", excerpt(text));
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
			return pMessage;
		}
		throw Poco::Net::HTTPException(Poco::format("The MCP endpoint answered HTTP %d %s", status, response.getReason()), excerpt(text));
	}
	catch (...)
	{
		_pSession.reset();
		throw;
	}
}


} // namespace Poco::AI::MCP::HTTP
