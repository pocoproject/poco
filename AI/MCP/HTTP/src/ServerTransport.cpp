//
// ServerTransport.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  ServerTransport
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/ServerTransport.h"
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Headers.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include "Poco/Logger.h"
#include "Poco/Exception.h"
#include <optional>
#include <set>


namespace Poco::AI::MCP::HTTP {


namespace
{
	const std::string PROTOCOL_VERSION_HEADER("MCP-Protocol-Version");
	const std::string METHOD_HEADER("Mcp-Method");
	const std::string NAME_HEADER("Mcp-Name");
	const std::string PARAM_HEADER_PREFIX("Mcp-Param-");


	ServerTransport::Reply refusal(Poco::Net::HTTPResponse::HTTPStatus status)
		/// A refusal of the request as a whole, without a message.
	{
		ServerTransport::Reply reply;
		reply.status = status;
		return reply;
	}


	ServerTransport::Reply errorReply(Poco::Net::HTTPResponse::HTTPStatus status, const Id& id, ErrorCode code, const std::string& message)
	{
		ServerTransport::Reply reply;
		reply.status = status;
		reply.message = Response::error(id, code, message);
		return reply;
	}


	ServerTransport::Reply closing(ServerTransport::Reply reply)
		/// The reply to a request whose body is left unread: the connection
		/// cannot carry another request.
	{
		reply.headers.set("Connection", "close");
		return reply;
	}


	bool header(const Poco::Net::HTTPServerRequest& request, const std::string& name, std::string& value)
		/// Sets value to the header without the whitespace around it and
		/// returns true, if the request has the header.
	{
		const auto it = request.find(name);
		if (it == request.end()) return false;
		value = Poco::trim(it->second);
		return true;
	}


	std::string repeatedHeader(const Poco::Net::HTTPServerRequest& request)
		/// The name of an MCP header that the request has more than once, or
		/// an empty string.
	{
		std::set<std::string> seen;
		for (const auto& field: request)
		{
			const std::string name = Poco::toLower(field.first);
			const bool mcp = name == Poco::toLower(PROTOCOL_VERSION_HEADER) || name == Poco::toLower(METHOD_HEADER) || name == Poco::toLower(NAME_HEADER)
				|| name.compare(0, PARAM_HEADER_PREFIX.size(), Poco::toLower(PARAM_HEADER_PREFIX)) == 0;
			if (mcp && !seen.insert(name).second) return field.first;
		}
		return std::string();
	}


	bool isJSON(const std::string& contentType)
	{
		return !contentType.empty() && Poco::Net::MediaType(contentType).matches("application", "json");
	}


	bool acceptsJSON(const std::string& accept)
		/// Returns true if the Accept header has a range that takes
		/// application/json with a quality above zero.
	{
		std::vector<std::string> ranges;
		Poco::Net::MessageHeader::splitElements(accept, ranges);
		for (const std::string& text: ranges)
		{
			const Poco::Net::MediaType range(text);
			if (!range.matchesRange("application", "json")) continue;
			double quality = 1;
			if (range.hasParameter("q") && !Poco::NumberParser::tryParseFloat(range.getParameter("q"), quality)) continue;
			if (quality > 0) return true;
		}
		return false;
	}


	bool isDecimal(const std::string& text)
		/// Returns true if text is a decimal number: digits, with a sign and
		/// a fraction or not.
	{
		std::size_t i = (!text.empty() && text[0] == '-') ? 1 : 0;
		const std::size_t integral = i;
		while (i < text.size() && text[i] >= '0' && text[i] <= '9') ++i;
		if (i == integral) return false;
		if (i == text.size()) return true;
		if (text[i] != '.') return false;
		const std::size_t fraction = ++i;
		while (i < text.size() && text[i] >= '0' && text[i] <= '9') ++i;
		return i > fraction && i == text.size();
	}


	bool sameParam(const Annotation& annotation, const Poco::Dynamic::Var& value, const std::string& rendered, const std::string& received)
		/// Returns true if a header received carries the value of the
		/// parameter: an integer as the same number, 42.0 as well as 42, any
		/// other value as the same text.
	{
		if (annotation.type == "integer" && !value.isBoolean() && !value.isString() && value.isNumeric() && isDecimal(received))
		{
			double number = 0;
			return Poco::NumberParser::tryParseFloat(received, number) && number == value.convert<double>();
		}
		return received == rendered;
	}


	bool hasEnvelope(const Poco::JSON::Object::Ptr& pMessage)
		/// Returns true if the request carries the _meta object every request
		/// of the protocol needs, with a revision and the client capabilities.
	{
		if (!pMessage->isObject("params")) return false;
		const Poco::JSON::Object::Ptr pParams = pMessage->getObject("params");
		if (!pParams->isObject("_meta")) return false;
		const Poco::JSON::Object::Ptr pMeta = pParams->getObject("_meta");
		return pMeta->get(META_PROTOCOL_VERSION).isString() && pMeta->isObject(META_CLIENT_CAPABILITIES);
	}


	Poco::Net::HTTPResponse::HTTPStatus statusOf(const Poco::JSON::Object::Ptr& pReply, const Poco::JSON::Object::Ptr& pMessage)
		/// The status of a reply of the server: 200, but for the errors that
		/// refuse a request before its method runs.
	{
		if (!pReply->isObject("error")) return Poco::Net::HTTPResponse::HTTP_OK;
		const Poco::Dynamic::Var code = pReply->getObject("error")->get("code");
		if (!code.isInteger()) return Poco::Net::HTTPResponse::HTTP_OK;
		const int value = code.convert<int>();
		if (value == static_cast<int>(ErrorCode::METHOD_NOT_FOUND))
		{
			return Poco::Net::HTTPResponse::HTTP_NOT_FOUND;
		}
		if (value == static_cast<int>(ErrorCode::INVALID_REQUEST)
			|| value == static_cast<int>(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION)
			|| value == static_cast<int>(ErrorCode::MISSING_REQUIRED_CLIENT_CAPABILITY)
			|| !hasEnvelope(pMessage))
		{
			return Poco::Net::HTTPResponse::HTTP_BAD_REQUEST;
		}
		return Poco::Net::HTTPResponse::HTTP_OK;
	}


	std::string nameMismatch(const Poco::Net::HTTPServerRequest& request, const std::string& method, const Poco::JSON::Object::Ptr& pParams)
		/// Checks the Mcp-Name header of a request that names a tool, a
		/// prompt or a resource.
	{
		std::string key;
		if (method == "tools/call" || method == "prompts/get") key = "name";
		else if (method == "resources/read") key = "uri";
		else return std::string();

		const Poco::Dynamic::Var name = pParams.isNull() ? Poco::Dynamic::Var() : pParams->get(key);
		std::string received;
		if (!header(request, NAME_HEADER, received))
		{
			return name.isString() ? "the request has no Mcp-Name header" : std::string();
		}
		std::string decoded;
		if (!decodeHeaderValue(received, decoded))
		{
			return "the Mcp-Name header is not valid base64";
		}
		if (name.isString() && decoded != name.extract<std::string>())
		{
			return "the Mcp-Name header does not match the " + key + " of the request";
		}
		return std::string();
	}


	std::string paramMismatch(const Server& server, const Poco::Net::HTTPServerRequest& request, const Poco::JSON::Object::Ptr& pParams)
		/// Checks the Mcp-Param headers of a tool call against the arguments
		/// the input schema of the tool marks with x-mcp-header. A parameter
		/// without a value needs no header, and a header for it is ignored.
	{
		if (pParams.isNull() || !pParams->get("name").isString()) return std::string();
		const std::optional<Tool> tool = server.findTool(pParams->getValue<std::string>("name"));
		if (!tool || tool->inputSchema.isNull()) return std::string();

		std::vector<Annotation> annotations;
		std::vector<std::string> path;
		try
		{
			collectAnnotations(tool->inputSchema, path, annotations);
		}
		catch (const Poco::DataFormatException&)
		{
			// Clients leave a tool whose annotations are not valid out; one
			// that calls it nevertheless sends no header for them.
			return std::string();
		}
		const Poco::JSON::Object::Ptr pArguments = pParams->isObject("arguments") ? pParams->getObject("arguments") : Poco::JSON::Object::Ptr();
		for (const Annotation& annotation: annotations)
		{
			const Poco::Dynamic::Var value = valueAt(pArguments, annotation.path);
			std::string rendered;
			if (!renderParam(value, rendered)) continue;
			const std::string name = PARAM_HEADER_PREFIX + annotation.name;
			std::string received;
			if (!header(request, name, received))
			{
				return "the request has no " + name + " header";
			}
			std::string decoded;
			if (!decodeHeaderValue(received, decoded))
			{
				return "the " + name + " header is not valid base64";
			}
			if (!sameParam(annotation, value, rendered, decoded))
			{
				return "the " + name + " header does not match the argument";
			}
		}
		return std::string();
	}


	std::string headerMismatch(const Server& server, const Poco::Net::HTTPServerRequest& request, const Poco::JSON::Object::Ptr& pMessage)
		/// What in the MCP headers of the request does not match its message,
		/// or an empty string.
	{
		// A message that is neither a request nor a notification gets its
		// answer from the server.
		const Poco::Dynamic::Var method = pMessage->get("method");
		if (pMessage->optValue<std::string>("jsonrpc", "") != JSONRPC_VERSION || !method.isString()) return std::string();
		const bool isRequest = pMessage->has("id");
		if (isRequest && !isValidId(pMessage->get("id"))) return std::string();

		const std::string& methodName = method.extract<std::string>();
		const Poco::JSON::Object::Ptr pParams = pMessage->isObject("params") ? pMessage->getObject("params") : Poco::JSON::Object::Ptr();
		const Poco::JSON::Object::Ptr pMeta = (!pParams.isNull() && pParams->isObject("_meta")) ? pParams->getObject("_meta") : Poco::JSON::Object::Ptr();
		const Poco::Dynamic::Var revision = pMeta.isNull() ? Poco::Dynamic::Var() : pMeta->get(META_PROTOCOL_VERSION);

		// One field of each: an intermediary that routes on another of
		// several, or on all of them joined, would see another request.
		const std::string repeated = repeatedHeader(request);
		if (!repeated.empty()) return "the request has more than one " + repeated + " header";

		// A header that is sent matches the message, whatever it is.
		std::string version;
		const bool hasVersion = header(request, PROTOCOL_VERSION_HEADER, version);
		if (hasVersion && revision.isString() && version != revision.extract<std::string>())
		{
			return "the MCP-Protocol-Version header does not match the revision of the request";
		}
		std::string methodHeader;
		const bool hasMethod = header(request, METHOD_HEADER, methodHeader);
		if (hasMethod && methodHeader != methodName)
		{
			return "the Mcp-Method header does not match the method of the request";
		}

		// A request of this revision carries the headers it needs. Any other
		// request, e.g. one without a _meta object or of another revision,
		// gets the answer of the server, which names what it is missing.
		if (!isRequest || !revision.isString() || revision.extract<std::string>() != PROTOCOL_VERSION) return std::string();
		if (!hasVersion) return "the request has no MCP-Protocol-Version header";
		if (!hasMethod) return "the request has no Mcp-Method header";
		std::string mismatch = nameMismatch(request, methodName, pParams);
		if (mismatch.empty() && methodName == "tools/call")
		{
			mismatch = paramMismatch(server, request, pParams);
		}
		return mismatch;
	}
}


ServerTransport::ServerTransport()
{
}


ServerTransport::ServerTransport(const Params& params):
	_params(params)
{
}


ServerTransport::~ServerTransport()
{
}


ServerTransport::Reply ServerTransport::answer(Server& server, Poco::Net::HTTPServerRequest& request, const RequestContext& context) const
{
	try
	{
		if (request.getAutoDecode()) return closing(decodedHeaders());
		Reply reply;
		if (refuse(request, reply)) return closing(reply);
		if (request.hasContentLength() && request.getContentLength64() > static_cast<Poco::Int64>(_params.maxMessageSize))
		{
			return closing(tooLarge());
		}
		std::string body;
		try
		{
			body = readBody(request.stream(), _params.maxMessageSize);
		}
		catch (const Poco::DataFormatException&)
		{
			return closing(tooLarge());
		}
		catch (const Poco::Exception& exc)
		{
			Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
			if (logger.debug())
			{
				logger.debug("Cannot read the body of an MCP request: %s", exc.displayText());
			}
			return closing(errorReply(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, Id(), ErrorCode::PARSE_ERROR, "Parse error: the body cannot be read"));
		}
		return answerMessage(server, request, body, context);
	}
	catch (const Poco::Exception& exc)
	{
		return failed(exc.displayText());
	}
	catch (const std::exception& exc)
	{
		return failed(exc.what());
	}
	catch (...)
	{
		return failed("unknown exception");
	}
}


ServerTransport::Reply ServerTransport::answer(Server& server, const Poco::Net::HTTPServerRequest& request, const std::string& body, const RequestContext& context) const
{
	try
	{
		if (request.getAutoDecode()) return decodedHeaders();
		Reply reply;
		if (refuse(request, reply)) return reply;
		if (body.size() > _params.maxMessageSize) return tooLarge();
		return answerMessage(server, request, body, context);
	}
	catch (const Poco::Exception& exc)
	{
		return failed(exc.displayText());
	}
	catch (const std::exception& exc)
	{
		return failed(exc.what());
	}
	catch (...)
	{
		return failed("unknown exception");
	}
}


void ServerTransport::send(const Reply& reply, Poco::Net::HTTPServerResponse& response) const
{
	response.setStatusAndReason(reply.status);
	for (const auto& header: reply.headers)
	{
		response.set(header.first, header.second);
	}
	if (reply.message.isNull())
	{
		response.setContentLength(0);
		response.send();
		return;
	}
	const std::string body = serialize(reply.message);
	response.setContentType("application/json");
	response.sendBuffer(body.data(), body.size());
}


void ServerTransport::handle(Server& server, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const RequestContext& context) const
{
	const Reply reply = answer(server, request, context);
	try
	{
		send(reply, response);
	}
	catch (const Poco::Exception& exc)
	{
		// The client has gone away; the connection ends with the request.
		Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
		if (logger.debug())
		{
			logger.debug("Cannot send the reply to an MCP request: %s", exc.displayText());
		}
	}
}


bool ServerTransport::allowsOrigin(const std::string& origin) const
{
	for (const std::string& allowed: _params.allowedOrigins)
	{
		if (allowed == "*" || Poco::icompare(allowed, origin) == 0) return true;
	}
	return false;
}


ServerTransport::Reply ServerTransport::unavailable()
{
	return errorReply(Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE, Id(), ErrorCode::INTERNAL_ERROR, "The MCP server is not available");
}


bool ServerTransport::refuse(const Poco::Net::HTTPServerRequest& request, Reply& reply) const
{
	std::string reason;
	std::string value;
	if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST)
	{
		reply = refusal(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		reply.headers.set("Allow", Poco::Net::HTTPRequest::HTTP_POST);
		reason = "the method is not POST";
	}
	else if (header(request, "Origin", value) && !allowsOrigin(value))
	{
		reply = refusal(Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
		reason = "the origin is not allowed";
	}
	else if (!isJSON(request.getContentType()))
	{
		reply = refusal(Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE);
		reason = "the body is not JSON";
	}
	else if (header(request, "Accept", value) && !acceptsJSON(value))
	{
		reply = refusal(Poco::Net::HTTPResponse::HTTP_NOT_ACCEPTABLE);
		reason = "the client does not accept JSON";
	}
	else return false;

	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	if (logger.debug())
	{
		logger.debug("MCP request refused with %d: %s", static_cast<int>(reply.status), reason);
	}
	return true;
}


ServerTransport::Reply ServerTransport::answerMessage(Server& server, const Poco::Net::HTTPServerRequest& request, const std::string& body, const RequestContext& context) const
{
	Poco::JSON::Object::Ptr pMessage;
	try
	{
		pMessage = parseMessage(body);
	}
	catch (const MCPException& exc)
	{
		// Not JSON, or a batch or another value that is no message.
		const ErrorCode code = exc.code() == static_cast<int>(ErrorCode::PARSE_ERROR) ? ErrorCode::PARSE_ERROR : ErrorCode::INVALID_REQUEST;
		return errorReply(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, Id(), code, exc.message());
	}

	const std::string mismatch = headerMismatch(server, request, pMessage);
	if (!mismatch.empty())
	{
		Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
		if (logger.debug())
		{
			logger.debug("MCP request refused: %s", mismatch);
		}
		const Poco::Dynamic::Var id = pMessage->get("id");
		return errorReply(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, isValidId(id) ? id : Id(), ErrorCode::HEADER_MISMATCH, "Header mismatch: " + mismatch);
	}

	Reply reply;
	reply.message = server.handle(pMessage, context);
	if (reply.message.isNull())
	{
		reply.status = Poco::Net::HTTPResponse::HTTP_ACCEPTED;
		return reply;
	}
	reply.status = statusOf(reply.message, pMessage);
	return reply;
}


ServerTransport::Reply ServerTransport::tooLarge() const
{
	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	if (logger.debug())
	{
		logger.debug("MCP request refused with 413: the body is larger than %z bytes", _params.maxMessageSize);
	}
	return refusal(Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
}


ServerTransport::Reply ServerTransport::decodedHeaders() const
{
	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	if (logger.error())
	{
		logger.error("MCP request refused: the HTTP server decodes RFC 2047 encoded words in request headers, "
			"so the MCP headers cannot be checked as they were sent; turn it off with "
			"Poco::Net::HTTPServerParams::setAutoDecodeHeaders(false)");
	}
	return errorReply(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, Id(), ErrorCode::INTERNAL_ERROR, "Internal error");
}


ServerTransport::Reply ServerTransport::failed(const std::string& what) const
{
	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	if (logger.error())
	{
		logger.error("MCP request failed: %s", what);
	}
	return errorReply(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, Id(), ErrorCode::INTERNAL_ERROR, "Internal error");
}


} // namespace Poco::AI::MCP::HTTP
