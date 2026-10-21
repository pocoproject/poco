//
// ChatEndpoint.cpp
//
// Library: AI
// Package: Chat
// Module:  ChatEndpoint
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/ChatEndpoint.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/MediaType.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Logger.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/URI.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <functional>
#include <ostream>
#include <utility>


namespace Poco::AI {


namespace
{
	const std::vector<std::string> ROUTES = {"health", "chat/stream", "chat/cancel", "models", "providers", "config", "token"};


	struct Refusal
		/// A request the endpoint refuses: the status of its answer, and the
		/// error and detail the answer carries.
	{
		Poco::Net::HTTPResponse::HTTPStatus status;
		std::string error;
		std::string detail;
	};


	void sendJSON(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus status, const Poco::JSON::Object::Ptr& pBody)
	{
		std::ostringstream body;
		pBody->stringify(body);
		const std::string text = body.str();
		response.setStatusAndReason(status);
		response.setContentType("application/json");
		response.set("Cache-Control", "no-store");
		response.sendBuffer(text.data(), text.size());
	}


	void sendError(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus status, const std::string& error, const std::string& detail)
	{
		Poco::JSON::Object::Ptr pBody = new Poco::JSON::Object;
		pBody->set("error", error);
		if (!detail.empty()) pBody->set("detail", detail);
		sendJSON(response, status, pBody);
	}


	Poco::JSON::Object::Ptr ok()
	{
		Poco::JSON::Object::Ptr pBody = new Poco::JSON::Object;
		pBody->set("status", "ok");
		return pBody;
	}


	Poco::JSON::Array::Ptr toArray(const std::vector<std::string>& strings)
	{
		Poco::JSON::Array::Ptr pArray = new Poco::JSON::Array;
		for (const std::string& text: strings) pArray->add(text);
		return pArray;
	}


	void needsMethod(const Poco::Net::HTTPServerRequest& request, const std::string& method)
	{
		if (request.getMethod() != method)
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED, "method not allowed", "the route takes " + method};
		}
	}


	void needsManager(const ChatEndpoint::Caller& caller)
	{
		if (!caller.mayManage)
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_FORBIDDEN, "forbidden", "the caller may not manage the chat"};
		}
	}


	void needsKeysFromRequests(const ChatEndpoint::Params& params)
	{
		if (!params.keysFromRequests)
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_FORBIDDEN, "forbidden", "the keys of the providers come from the configuration of the host"};
		}
	}


	bool isJSON(const std::string& contentType)
	{
		return !contentType.empty() && Poco::Net::MediaType(contentType).matches("application", "json");
	}


	bool allowsOrigin(const ChatEndpoint::Params& params, const std::string& origin, const std::string& host)
		/// Returns true if a page of origin may call the endpoint that a request
		/// reaches at host, its Host header: a page of that host, or one of an
		/// allowed origin.
	{
		const std::string::size_type scheme = origin.find("://");
		if (scheme != std::string::npos && !host.empty() && Poco::icompare(origin.substr(scheme + 3), host) == 0) return true;
		for (const std::string& allowed: params.allowedOrigins)
		{
			if (allowed == "*" || Poco::icompare(allowed, origin) == 0) return true;
		}
		return false;
	}


	void needsSameAddress(const ChatSettings& settings, const ProviderSettings& entry)
		/// The address of a provider whose key comes from the host goes with
		/// the key.
	{
		for (const ProviderSettings& current: settings.providers)
		{
			if (current.name == entry.name && !ChatService::sameAddress(current, entry))
			{
				throw Refusal{Poco::Net::HTTPResponse::HTTP_FORBIDDEN, "forbidden", "the base URL and the servers of a provider with a key come from the configuration of the host"};
			}
		}
	}


	std::string stringMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return std::string();
		if (!value.isString()) throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", name + " is not a string"};
		return value.extract<std::string>();
	}


	std::vector<std::string> listMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
		/// A list of strings, or the comma-separated items of one string.
	{
		std::vector<std::string> result;
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return result;
		if (value.isString())
		{
			const Poco::StringTokenizer tokens(value.extract<std::string>(), ",", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
			result.assign(tokens.begin(), tokens.end());
			return result;
		}
		if (!pObject->isArray(name)) throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", name + " is not a list"};
		const Poco::JSON::Array::Ptr pArray = pObject->getArray(name);
		for (std::size_t i = 0; i < pArray->size(); ++i)
		{
			const Poco::Dynamic::Var element = pArray->get(static_cast<unsigned>(i));
			if (!element.isString()) throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", name + " holds something else than strings"};
			result.push_back(element.extract<std::string>());
		}
		return result;
	}


	int intMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return 0;
		if (!value.isNumeric() || value.isBoolean() || value.isString()) throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", name + " is not a number"};
		return value.convert<int>();
	}


	class Heartbeat: public Poco::Runnable
		/// Writes a comment to an event stream at an interval, until it is
		/// stopped or the stream fails.
	{
	public:
		Heartbeat(std::ostream& out, Poco::FastMutex& mutex, const Poco::Timespan& interval, std::function<void ()> onFailure):
			_out(out),
			_mutex(mutex),
			_interval(static_cast<long>(interval.totalMilliseconds())),
			_onFailure(std::move(onFailure))
		{
			_thread.start(*this);
		}

		~Heartbeat()
		{
			stop();
		}

		void stop()
		{
			if (_stopped) return;
			_stopped = true;
			_stop.set();
			_thread.join();
		}

		void run() override
		{
			try
			{
				while (!_stop.tryWait(_interval))
				{
					Poco::FastMutex::ScopedLock lock(_mutex);
					_out << ": working\n\n";
					_out.flush();
					if (!_out.good())
					{
						_onFailure();
						return;
					}
				}
			}
			catch (...)
			{
				_onFailure();
			}
		}

	private:
		std::ostream& _out;
		Poco::FastMutex& _mutex;
		long _interval;
		std::function<void ()> _onFailure;
		Poco::Event _stop;
		Poco::Thread _thread;
		bool _stopped = false;
	};
}


ChatEndpoint::ChatEndpoint(ChatService& service):
	_service(service)
{
}


ChatEndpoint::ChatEndpoint(ChatService& service, const Params& params):
	_service(service),
	_params(params)
{
}


ChatEndpoint::~ChatEndpoint()
{
}


bool ChatEndpoint::isRoute(const std::string& route)
{
	return std::find(ROUTES.begin(), ROUTES.end(), route) != ROUTES.end();
}


void ChatEndpoint::handle(const std::string& route, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	try
	{
		if (request.has("Origin") && !allowsOrigin(_params, request.get("Origin"), request.get(Poco::Net::HTTPRequest::HOST, "")))
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_FORBIDDEN, "forbidden", "the origin of the page is not allowed"};
		}
		if (route == "health")
		{
			needsMethod(request, Poco::Net::HTTPRequest::HTTP_GET);
			health(response, caller);
		}
		else if (route == "chat/stream") stream(request, response, caller);
		else if (route == "chat/cancel") cancel(request, response, caller);
		else if (route == "models") models(request, response, caller);
		else if (route == "providers") providers(request, response, caller);
		else if (route == "config") config(request, response, caller);
		else if (route == "token") token(request, response, caller);
		else sendError(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "not found", "no route " + route);
	}
	catch (const Refusal& refusal)
	{
		if (!response.sent()) sendError(response, refusal.status, refusal.error, refusal.detail);
	}
	catch (const Poco::InvalidArgumentException& exc)
	{
		if (!response.sent()) sendError(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", exc.message());
	}
	catch (const Poco::NotFoundException& exc)
	{
		if (!response.sent()) sendError(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "not found", exc.message());
	}
	catch (const Poco::Exception& exc)
	{
		if (logger.error()) logger.error("AI chat request %s failed: %s", route, exc.displayText());
		try
		{
			if (!response.sent()) sendError(response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "internal error", exc.displayText());
		}
		catch (const Poco::Exception&)
		{
			// The client has gone away.
		}
	}
	catch (const std::exception& exc)
	{
		if (logger.error()) logger.error("AI chat request %s failed: %s", route, std::string(exc.what()));
		try
		{
			if (!response.sent()) sendError(response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "internal error", exc.what());
		}
		catch (const Poco::Exception&)
		{
		}
	}
}


void ChatEndpoint::health(Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	Poco::JSON::Object::Ptr pBody = ok();
	pBody->set("provider", _service.provider());
	// The models first: asking the provider for them may tell the model in use.
	const std::vector<std::string> models = _service.models();
	pBody->set("model", _service.model());
	pBody->set("models", toArray(models));
	pBody->set("providers", toArray(_service.providers()));
	pBody->set("providerStatus", providerStatus());
	Poco::JSON::Object::Ptr pFeatures = new Poco::JSON::Object;
	pFeatures->set("manage", caller.mayManage);
	pFeatures->set("sessions", false);
	pFeatures->set("prompts", false);
	pFeatures->set("tools", false);
	pFeatures->set("documents", false);
	pFeatures->set("documentsWrite", false);
	pFeatures->set("memory", false);
	pFeatures->set("keys", _params.keysFromRequests);
	pBody->set("features", pFeatures);
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


void ChatEndpoint::stream(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxChatBodySize);
	if (!pRequest->isArray("messages"))
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "the request has no messages"};
	}
	// Only text messages of the user and the assistant: what the provider
	// gets beyond them is the service's.
	const Poco::JSON::Array::Ptr pMessages = pRequest->getArray("messages");
	Poco::JSON::Array conversation;
	for (std::size_t i = 0; i < pMessages->size(); ++i)
	{
		const Poco::JSON::Object::Ptr pMessage = pMessages->isObject(static_cast<unsigned>(i)) ? pMessages->getObject(static_cast<unsigned>(i)) : Poco::JSON::Object::Ptr();
		const std::string role = pMessage.isNull() ? std::string() : pMessage->optValue<std::string>("role", "");
		if (pMessage.isNull() || (role != "user" && role != "assistant") || !pMessage->get("content").isString())
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "a message is not an object with the role user or assistant and a text"};
		}
		Poco::JSON::Object::Ptr pCopy = new Poco::JSON::Object;
		pCopy->set("role", role);
		pCopy->set("content", pMessage->getValue<std::string>("content"));
		conversation.add(pCopy);
	}
	if (conversation.size() == 0)
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "the request has no messages"};
	}
	std::string chatId = stringMember(pRequest, "chat");
	if (chatId.empty()) chatId = Poco::UUIDGenerator::defaultGenerator().createRandom().toString();

	response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
	response.setContentType("text/event-stream");
	response.set("Cache-Control", "no-cache");
	response.set("X-Accel-Buffering", "no");
	response.setChunkedTransferEncoding(true);
	std::ostream& out = response.send();

	Poco::FastMutex mutex;
	bool gone = false;
	const std::shared_ptr<Cancellation> pCancellation = std::make_shared<Cancellation>();
	const auto leave = [&pCancellation]()
	{
		// The client has gone away: nobody reads the rest of this turn. Other
		// turns of the user in the chat, e.g. in another tab, go on.
		pCancellation->cancel();
	};
	{
		Poco::FastMutex::ScopedLock lock(mutex);
		out << ": connected\n\n";
		out.flush();
		if (!out.good()) return;
	}
	Heartbeat heartbeat(out, mutex, _params.heartbeat, leave);
	Poco::AI::MCP::Identity identity;
	identity.principal = caller.user;
	identity.scope = caller.scope;
	identity.authenticated = !caller.user.empty();
	_service.chat(conversation, identity, chatId, [&](const Poco::JSON::Object::Ptr& pEvent)
		{
			std::ostringstream text;
			pEvent->stringify(text);
			Poco::FastMutex::ScopedLock lock(mutex);
			if (gone) return;
			out << "data: " << text.str() << "\n\n";
			out.flush();
			if (!out.good())
			{
				gone = true;
				leave();
			}
		}, pCancellation);
	heartbeat.stop();
	Poco::FastMutex::ScopedLock lock(mutex);
	if (gone) return;
	out << "data: [DONE]\n\n";
	out.flush();
}


void ChatEndpoint::cancel(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	std::string chatId;
	for (const auto& parameter: Poco::URI(request.getURI()).getQueryParameters())
	{
		if (parameter.first == "chat") chatId = parameter.second;
	}
	Poco::JSON::Object::Ptr pBody = ok();
	pBody->set("cancelled", _service.cancel(caller.user, chatId));
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


void ChatEndpoint::models(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
	{
		Poco::JSON::Object::Ptr pBody = ok();
		const std::vector<std::string> models = _service.models();
		pBody->set("provider", _service.provider());
		pBody->set("model", _service.model());
		pBody->set("models", toArray(models));
		sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
		return;
	}
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	needsManager(caller);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxBodySize);
	_service.selectModel(stringMember(pRequest, "model"));
	Poco::JSON::Object::Ptr pBody = ok();
	pBody->set("model", _service.model());
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


void ChatEndpoint::providers(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
	{
		Poco::JSON::Object::Ptr pBody = ok();
		pBody->set("provider", _service.provider());
		pBody->set("providers", toArray(_service.providers()));
		pBody->set("providerStatus", providerStatus());
		sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
		return;
	}
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxBodySize);
	const std::string provider = stringMember(pRequest, "provider");
	Poco::JSON::Object::Ptr pBody = ok();
	if (stringMember(pRequest, "op") == "retry")
	{
		// Asking a provider again changes nothing of the settings: anybody may.
		const ProviderStatus status = _service.retry(provider);
		pBody->set("provider", provider);
		pBody->set("reachable", status.reachable);
		if (!status.detail.empty()) pBody->set("detail", status.detail);
		if (status.reachable) pBody->set("models", toArray(_service.models(provider)));
		sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
		return;
	}
	needsManager(caller);
	_service.selectProvider(provider);
	const std::vector<std::string> models = _service.models();
	pBody->set("provider", _service.provider());
	pBody->set("model", _service.model());
	pBody->set("models", toArray(models));
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


void ChatEndpoint::config(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	needsManager(caller);
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)
	{
		const ChatSettings settings = _service.settings();
		Poco::JSON::Object::Ptr pBody = new Poco::JSON::Object;
		pBody->set("current", settings.provider);
		Poco::JSON::Array::Ptr pProviders = new Poco::JSON::Array;
		for (const ProviderSettings& entry: settings.providers)
		{
			Poco::JSON::Object::Ptr pEntry = new Poco::JSON::Object;
			pEntry->set("name", entry.name);
			pEntry->set("enabled", entry.enabled);
			pEntry->set("baseUrl", entry.baseUrl);
			pEntry->set("defaultBaseUrl", ProviderSettings::defaultBaseUrl(entry.name));
			pEntry->set("servers", toArray(entry.servers));
			pEntry->set("models", toArray(entry.models));
			pEntry->set("maxTokens", entry.maxTokens);
			pEntry->set("timeout", entry.timeout);
			pEntry->set("probeTimeout", entry.probeTimeout);
			pEntry->set("tools", entry.tools);
			pEntry->set("hasApiKey", _service.hasKey(entry.name));
			pEntry->set("usesApiKey", _params.keysFromRequests && ProviderSettings::usesApiKey(entry.name));
			pEntry->set("usesServers", ProviderSettings::usesServers(entry.name));
			pProviders->add(pEntry);
		}
		pBody->set("providers", pProviders);
		sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
		return;
	}
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxBodySize);
	if (!pRequest->isArray("providers"))
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "the request has no providers"};
	}
	const Poco::JSON::Array::Ptr pProviders = pRequest->getArray("providers");
	const ChatSettings current = _service.settings();
	std::vector<ProviderSettings> providers;
	for (std::size_t i = 0; i < pProviders->size(); ++i)
	{
		if (!pProviders->isObject(static_cast<unsigned>(i)))
		{
			throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "a provider is not an object"};
		}
		const Poco::JSON::Object::Ptr pEntry = pProviders->getObject(static_cast<unsigned>(i));
		ProviderSettings entry;
		entry.name = stringMember(pEntry, "name");
		const Poco::Dynamic::Var enabled = pEntry->get("enabled");
		entry.enabled = enabled.isBoolean() ? enabled.extract<bool>() : true;
		entry.baseUrl = stringMember(pEntry, "baseUrl");
		entry.servers = listMember(pEntry, "servers");
		entry.models = listMember(pEntry, "models");
		entry.maxTokens = intMember(pEntry, "maxTokens");
		entry.timeout = intMember(pEntry, "timeout");
		entry.probeTimeout = intMember(pEntry, "probeTimeout");
		const Poco::Dynamic::Var tools = pEntry->get("tools");
		entry.tools = tools.isBoolean() ? tools.extract<bool>() : true;
		entry.apiKey = stringMember(pEntry, "apiKey");
		if (!entry.apiKey.empty()) needsKeysFromRequests(_params);
		if (!_params.keysFromRequests && ProviderSettings::usesApiKey(entry.name)) needsSameAddress(current, entry);
		providers.push_back(std::move(entry));
	}
	_service.configure(providers);
	Poco::JSON::Object::Ptr pBody = ok();
	const std::vector<std::string> models = _service.models();
	pBody->set("provider", _service.provider());
	pBody->set("model", _service.model());
	pBody->set("models", toArray(models));
	pBody->set("providers", toArray(_service.providers()));
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


void ChatEndpoint::token(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	needsManager(caller);
	needsKeysFromRequests(_params);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxBodySize);
	const std::string provider = stringMember(pRequest, "provider");
	_service.setKey(provider, stringMember(pRequest, "token"));
	Poco::JSON::Object::Ptr pBody = ok();
	pBody->set("provider", provider);
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


Poco::JSON::Object::Ptr ChatEndpoint::readObject(Poco::Net::HTTPServerRequest& request, std::size_t maxSize) const
{
	if (!isJSON(request.getContentType()))
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE, "unsupported media type", "the body is not JSON"};
	}
	if (request.hasContentLength() && request.getContentLength64() > static_cast<Poco::Int64>(maxSize))
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE, "too large", "the body is larger than the limit"};
	}
	std::string body;
	try
	{
		body = Poco::AI::MCP::HTTP::readBody(request.stream(), maxSize);
	}
	catch (const Poco::DataFormatException&)
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE, "too large", "the body is larger than the limit"};
	}
	const Poco::JSON::Object::Ptr pObject = Poco::AI::MCP::HTTP::parseObject(body);
	if (pObject.isNull())
	{
		throw Refusal{Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "bad request", "the body is not a JSON object"};
	}
	return pObject;
}


Poco::JSON::Array::Ptr ChatEndpoint::providerStatus() const
{
	Poco::JSON::Array::Ptr pStatus = new Poco::JSON::Array;
	for (const ProviderStatus& status: _service.providerStatus())
	{
		Poco::JSON::Object::Ptr pEntry = new Poco::JSON::Object;
		pEntry->set("name", status.name);
		if (status.probed)
		{
			pEntry->set("reachable", status.reachable);
			if (!status.detail.empty()) pEntry->set("detail", status.detail);
			pEntry->set("modelCount", static_cast<Poco::UInt64>(status.modelCount));
		}
		pStatus->add(pEntry);
	}
	return pStatus;
}


} // namespace Poco::AI
