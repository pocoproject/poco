//
// ChatEndpoint.h
//
// Library: AI
// Package: Chat
// Module:  ChatEndpoint
//
// Definition of the ChatEndpoint class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ChatEndpoint_INCLUDED
#define AI_ChatEndpoint_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/ChatService.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Timespan.h"
#include <cstddef>
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API ChatEndpoint
	/// The HTTP interface of a ChatService for a chat in a browser, below a
	/// base path the host chooses, e.g. /api/ai. Its routes, relative to the
	/// base:
	///
	///   - GET health: {"status": "ok", "provider", "model", "models",
	///     "providers", "providerStatus": [{"name", "reachable", "detail",
	///     "modelCount"}], "features": {"manage", "keys", ...}}, where
	///     reachable, detail and modelCount are there once the provider was
	///     asked for its models, manage tells whether the caller may manage the
	///     chat, keys whether keys are set here, and the other features are
	///     false. detail, which may quote the answer of a server of the
	///     provider, is what the service knows (see ProviderStatus) for a
	///     caller who may manage the chat; others get only "the provider does
	///     not answer" for a provider that does not;
	///   - POST chat/stream with {"messages": [{"role", "content"}], "chat"}:
	///     the events of the turn (see ChatService) as server-sent events, a
	///     data line with the JSON of each, comments while the model works,
	///     and a last data line [DONE]. The roles are user and assistant, the
	///     contents strings, and chat is the id by which the turn can be
	///     cancelled;
	///   - POST chat/cancel?chat=<id>: cancels the caller's turn in the chat,
	///     or all of the caller's turns without the parameter: {"status": "ok",
	///     "cancelled"}. A caller who is not signed in cancels none (see
	///     ChatService::cancel()); their turn stops when they close its stream;
	///   - GET models: {"status": "ok", "provider", "model", "models"};
	///   - POST models with {"model"}: puts the model in use; one that the
	///     provider in use does not list, where it has a list, gets 404;
	///   - GET providers: {"status": "ok", "provider", "providers",
	///     "providerStatus"};
	///   - POST providers with {"provider"}: puts the provider in use; with
	///     {"op": "retry", "provider"}: asks it for its models again;
	///   - GET config: {"current", "providers": [{"name", "kind", "enabled",
	///     "baseUrl", "defaultBaseUrl", "servers", "models", "maxTokens",
	///     "timeout", "probeTimeout", "tools", "hasApiKey", "usesApiKey",
	///     "usesServers"}]}, never a key. kind is the effective one (see
	///     ProviderSettings); hasApiKey tells whether the provider has a key,
	///     from the settings or the environment; usesApiKey whether one of its
	///     kind can be set here;
	///   - POST config with {"providers": [...]}: replaces the providers, as
	///     ChatService::configure() does, each read by
	///     ProviderSettings::fromJSON(), whose rules a wrong type breaks with
	///     400; an entry's apiKey sets its key, where keys may be set here;
	///   - POST token with {"provider", "token"}: sets the key of a provider,
	///     where keys may be set here.
	///
	/// Putting a provider or model in use, the configuration and the keys need
	/// a caller who may manage the chat; others get 403. Keys are set here
	/// only where the host lets them be (Params::keysFromRequests): by default
	/// they come from its configuration or the environment, and a request that
	/// sets one, or that gives a provider with a key another base URL or other
	/// servers, gets 403. Errors come as {"error", "detail"}, with 400, 403,
	/// 404, 405, 413, 415 or 500.
	///
	/// A page of another site cannot have the browser send what the endpoint
	/// takes without a preflight request, which the endpoint does not answer:
	/// a body that is not JSON (Content-Type application/json) gets 415, and a
	/// request with an Origin header gets 403 unless it names the host the
	/// request is sent to (its Host header) or one of Params::allowedOrigins.
	/// A host that signs users in with cookies protects its routes against
	/// requests of other sites as it does its other pages, e.g. with SameSite
	/// cookies or a token header.
	///
	/// The host signs the caller in before, and says who it is.
{
public:
	struct Caller
		/// Who calls, as the host signed them in.
	{
		std::string user;
			/// The user, whose turns cancel requests cancel, and whom the tools
			/// of a turn run for.
		std::string scope;
			/// What the user may do, in the terms of the host, e.g. the
			/// permissions; the tools of a turn get it with the user.
		bool mayManage = false;
			/// Whether the user may change the settings of the chat.
	};

	struct Params
		/// The limits of the endpoint.
	{
		std::size_t maxChatBodySize = 4*1024*1024;
			/// The limit of the body of a chat/stream request, in bytes.
		std::size_t maxBodySize = 64*1024;
			/// The limit of the body of any other request, in bytes.
		Poco::Timespan heartbeat = Poco::Timespan(5, 0);
			/// The interval of the comments of an event stream while nothing
			/// else comes, so that the client and the proxies between see the
			/// connection alive. Zero or less sends none; another interval is
			/// taken between 10 milliseconds and a day.
		bool keysFromRequests = false;
			/// Whether a caller who may manage the chat sets the keys of the
			/// providers, and the base URL and servers of a provider with a
			/// key; else they come from the host alone.
		std::vector<std::string> allowedOrigins;
			/// The origins of pages of other hosts that may call the endpoint,
			/// as a browser names them: scheme://host, and :port unless it is
			/// the default one. "*" allows every origin. A page of the host
			/// the request is sent to needs none, and neither does a request
			/// without an Origin header, from a client that is no browser.
		std::string loggerName = "AI.ChatEndpoint";
			/// The logger of the endpoint.
	};

	explicit ChatEndpoint(ChatService& service);
		/// Creates the endpoint of the service, which must outlive it.

	ChatEndpoint(ChatService& service, const Params& params);
		/// Creates the endpoint of the service, which must outlive it.

	~ChatEndpoint();
		/// Destroys the endpoint.

	[[nodiscard]] static bool isRoute(const std::string& route);
		/// Returns true if route is one of the endpoint's.

	void handle(const std::string& route, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
		/// Answers a request to the route, the path below the base without a
		/// leading slash, e.g. "chat/stream". An unknown route gets 404, a
		/// method the route does not take 405. Never throws: a reply that
		/// cannot be sent, because the client has gone away, is dropped.

private:
	void health(Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void stream(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void cancel(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void models(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void providers(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void config(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	void token(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, const Caller& caller) const;
	Poco::JSON::Object::Ptr readObject(Poco::Net::HTTPServerRequest& request, std::size_t maxSize) const;
	Poco::JSON::Array::Ptr providerStatus(const Caller& caller) const;

	ChatService& _service;
	const Params _params;
};


} // namespace Poco::AI


#endif // AI_ChatEndpoint_INCLUDED
