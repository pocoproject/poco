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
#include "Poco/Net/NetException.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Logger.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/URI.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <functional>
#include <ostream>
#include <utility>


namespace Poco::AI {


namespace
{
	const std::vector<std::string> ROUTES = {"health", "chat/stream", "chat/cancel", "models", "providers", "config", "key"};


	POCO_DECLARE_EXCEPTION(, Refusal, Poco::Net::HTTPException)
		/// A request the endpoint refuses: code() is the status of the answer,
		/// message() the detail it carries.

	POCO_IMPLEMENT_EXCEPTION(Refusal, Poco::Net::HTTPException, "Request refused")


	std::string errorOf(Poco::Net::HTTPResponse::HTTPStatus status)
		/// The error of an answer with the status.
	{
		switch (status)
		{
		case Poco::Net::HTTPResponse::HTTP_BAD_REQUEST: return "bad request";
		case Poco::Net::HTTPResponse::HTTP_FORBIDDEN: return "forbidden";
		case Poco::Net::HTTPResponse::HTTP_NOT_FOUND: return "not found";
		case Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED: return "method not allowed";
		case Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE: return "too large";
		case Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE: return "unsupported media type";
		default: return "internal error";
		}
	}


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
		/// Sends an error reply unless a reply has gone out already. A reply
		/// that cannot be sent, as the client has gone away, is dropped.
	{
		if (response.sent()) return;
		try
		{
			Poco::JSON::Object::Ptr pBody = new Poco::JSON::Object;
			pBody->set("error", error);
			if (!detail.empty()) pBody->set("detail", detail);
			sendJSON(response, status, pBody);
		}
		catch (const std::exception&)
		{
		}
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
			throw Refusal("the route takes " + method, Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		}
	}


	void needsManager(const ChatEndpoint::Caller& caller)
	{
		if (!caller.mayManage)
		{
			throw Refusal("the caller may not manage the chat", Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
		}
	}


	void needsKeysFromRequests(const ChatEndpoint::Params& params)
	{
		if (!params.keysFromRequests)
		{
			throw Refusal("the keys of the providers come from the configuration of the host", Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
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


	std::string detailFor(const ProviderStatus& status, const ChatEndpoint::Caller& caller)
		/// The detail of the status of a provider as the caller gets it: what
		/// the service knows for one who may manage the chat, which may quote
		/// the answers of the servers of the provider, and a fixed reason for
		/// others.
	{
		if (caller.mayManage) return status.detail;
		return status.reachable ? std::string() : std::string("the provider does not answer");
	}


	void needsSameAddress(const ChatSettings& settings, const ProviderSettings& entry)
		/// The address of a provider whose key comes from the host goes with
		/// the key.
	{
		for (const ProviderSettings& current: settings.providers)
		{
			if (current.name == entry.name && !ChatService::sameAddress(current, entry))
			{
				throw Refusal("the base URL and the servers of a provider with a key come from the configuration of the host", Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
			}
		}
	}


	std::string stringMember(const Poco::JSON::Object::Ptr& pObject, const std::string& name)
	{
		const Poco::Dynamic::Var value = pObject->get(name);
		if (value.isEmpty()) return std::string();
		if (!value.isString()) throw Refusal(name + " is not a string", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		return value.extract<std::string>();
	}


	class Heartbeat: public Poco::Runnable
		/// Writes a comment to an event stream at an interval, until it is
		/// stopped or the stream fails. An interval of zero or less writes
		/// none; another is taken between 10 milliseconds and a day.
	{
	public:
		Heartbeat(std::ostream& out, Poco::FastMutex& mutex, const Poco::Timespan& interval, std::function<void ()> onFailure):
			_out(out),
			_mutex(mutex),
			_interval(static_cast<long>(std::clamp<Poco::Timespan::TimeDiff>(interval.totalMilliseconds(), 10, 24*60*60*1000))),
			_onFailure(std::move(onFailure)),
			_stopped(interval <= Poco::Timespan())
		{
			if (!_stopped) _thread.start(*this);
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
			throw Refusal("the origin of the page is not allowed", Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
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
		else if (route == "key") key(request, response, caller);
		else throw Refusal("no route " + route, Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
	}
	catch (const Refusal& refusal)
	{
		const auto status = static_cast<Poco::Net::HTTPResponse::HTTPStatus>(refusal.code());
		sendError(response, status, errorOf(status), refusal.message());
	}
	catch (const Poco::InvalidArgumentException& exc)
	{
		sendError(response, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, errorOf(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST), exc.message());
	}
	catch (const Poco::NotFoundException& exc)
	{
		sendError(response, Poco::Net::HTTPResponse::HTTP_NOT_FOUND, errorOf(Poco::Net::HTTPResponse::HTTP_NOT_FOUND), exc.message());
	}
	catch (const Poco::Exception& exc)
	{
		if (logger.error()) logger.error("AI chat request %s failed: %s", route, exc.displayText());
		sendError(response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "internal error", exc.displayText());
	}
	catch (const std::exception& exc)
	{
		if (logger.error()) logger.error("AI chat request %s failed: %s", route, std::string(exc.what()));
		sendError(response, Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "internal error", exc.what());
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
	pBody->set("providerStatus", providerStatus(caller));
	Poco::JSON::Object::Ptr pFeatures = new Poco::JSON::Object;
	pFeatures->set("manage", caller.mayManage);
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
		throw Refusal("the request has no messages", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
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
			throw Refusal("a message is not an object with the role user or assistant and a text", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		}
		Poco::JSON::Object::Ptr pCopy = new Poco::JSON::Object;
		pCopy->set("role", role);
		pCopy->set("content", pMessage->getValue<std::string>("content"));
		conversation.add(pCopy);
	}
	if (conversation.size() == 0)
	{
		throw Refusal("the request has no messages", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
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
	_service.chat(conversation, caller.identity, chatId, [&](const Poco::JSON::Object::Ptr& pEvent)
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
	pBody->set("cancelled", _service.cancel(caller.identity.principal, chatId));
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
		pBody->set("providerStatus", providerStatus(caller));
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
		const std::string detail = detailFor(status, caller);
		if (!detail.empty()) pBody->set("detail", detail);
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
			Poco::JSON::Object::Ptr pEntry = entry.toJSON();
			// Never a key.
			pEntry->remove("apiKey");
			pEntry->set("kind", entry.effectiveKind());
			pEntry->set("defaultBaseUrl", ProviderSettings::defaultBaseUrl(entry.effectiveKind()));
			pEntry->set("hasApiKey", _service.hasKey(entry.name));
			pEntry->set("usesApiKey", _params.keysFromRequests && ProviderSettings::usesApiKey(entry.effectiveKind()));
			pEntry->set("usesServers", ProviderSettings::usesServers(entry.effectiveKind()));
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
		throw Refusal("the request has no providers", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	}
	const Poco::JSON::Array::Ptr pProviders = pRequest->getArray("providers");
	const ChatSettings current = _service.settings();
	std::vector<ProviderSettings> providers;
	for (std::size_t i = 0; i < pProviders->size(); ++i)
	{
		if (!pProviders->isObject(static_cast<unsigned>(i)))
		{
			throw Refusal("a provider is not an object", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		}
		ProviderSettings entry;
		try
		{
			entry = ProviderSettings::fromJSON(pProviders->getObject(static_cast<unsigned>(i)));
		}
		catch (const Poco::DataFormatException& exc)
		{
			throw Refusal(exc.message(), Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		}
		if (!entry.apiKey.empty()) needsKeysFromRequests(_params);
		if (!_params.keysFromRequests && ProviderSettings::usesApiKey(entry.effectiveKind())) needsSameAddress(current, entry);
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


void ChatEndpoint::key(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const
{
	needsMethod(request, Poco::Net::HTTPRequest::HTTP_POST);
	needsManager(caller);
	needsKeysFromRequests(_params);
	const Poco::JSON::Object::Ptr pRequest = readObject(request, _params.maxBodySize);
	const std::string provider = stringMember(pRequest, "provider");
	_service.setKey(provider, stringMember(pRequest, "apiKey"));
	Poco::JSON::Object::Ptr pBody = ok();
	pBody->set("provider", provider);
	sendJSON(response, Poco::Net::HTTPResponse::HTTP_OK, pBody);
}


Poco::JSON::Object::Ptr ChatEndpoint::readObject(Poco::Net::HTTPServerRequest& request, std::size_t maxSize) const
{
	if (!isJSON(request.getContentType()))
	{
		throw Refusal("the body is not JSON", Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE);
	}
	if (request.hasContentLength() && request.getContentLength64() > static_cast<Poco::Int64>(maxSize))
	{
		throw Refusal("the body is larger than the limit", Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
	}
	std::string body;
	try
	{
		body = Poco::AI::MCP::HTTP::Impl::readBody(request.stream(), maxSize);
	}
	catch (const Poco::DataFormatException&)
	{
		throw Refusal("the body is larger than the limit", Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
	}
	if (Poco::AI::MCP::HTTP::Impl::brokeOff(request, body.size()))
	{
		throw Refusal("the body broke off", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	}
	const Poco::JSON::Object::Ptr pObject = Poco::AI::MCP::HTTP::Impl::parseObject(body);
	if (pObject.isNull())
	{
		throw Refusal("the body is not a JSON object", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	}
	return pObject;
}


Poco::JSON::Array::Ptr ChatEndpoint::providerStatus(const Caller& caller) const
{
	Poco::JSON::Array::Ptr pStatus = new Poco::JSON::Array;
	for (const ProviderStatus& status: _service.providerStatus())
	{
		Poco::JSON::Object::Ptr pEntry = new Poco::JSON::Object;
		pEntry->set("name", status.name);
		if (status.probed)
		{
			pEntry->set("reachable", status.reachable);
			const std::string detail = detailFor(status, caller);
			if (!detail.empty()) pEntry->set("detail", detail);
			pEntry->set("modelCount", static_cast<Poco::UInt64>(status.modelCount));
		}
		pStatus->add(pEntry);
	}
	return pStatus;
}


} // namespace Poco::AI
