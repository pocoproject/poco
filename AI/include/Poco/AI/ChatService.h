//
// ChatService.h
//
// Library: AI
// Package: Chat
// Module:  ChatService
//
// Definition of the ChatService class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ChatService_INCLUDED
#define AI_ChatService_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/AgentLoop.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/ChatSettings.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API ChatService
	/// The chat of an application with a language model, for any number of
	/// users at once. A turn of a conversation runs through the agent loop,
	/// with the tools of the application, and its events come as JSON objects,
	/// which a host streams to its user interface (see ChatEndpoint). A turn
	/// can be stopped from another thread.
	///
	/// The providers the chat may use, and the provider and model in use, are
	/// its settings, which an administrator changes while it runs. The service
	/// keeps them in memory and hands every change to the store of the host
	/// before it takes effect.
	///
	/// A provider without a list of models offers what it lists itself; the
	/// list is asked for when it is needed and kept for a while. A provider
	/// whose base URL has a placeholder for its server, e.g.
	/// http://[server]:11434/v1, is reached at the first of its servers that
	/// answers, which the service keeps until a call fails to connect.
	///
	/// A key belongs to the address of its provider, the base URL and the
	/// servers: configure() keeps the key of a provider only while its address
	/// stays the same, and the key of the environment variable of a kind
	/// (ANTHROPIC_API_KEY, OPENAI_API_KEY) goes only to the address the
	/// provider has in the settings the service is created with.
	///
	/// The events of a turn:
	///
	///   - {"type": "text", "content"}: a piece of the answer;
	///   - {"type": "tool_call", "name", "input"}: the model calls a tool, with
	///     the input as JSON text;
	///   - {"type": "tool_result", "name", "content", "error"}: its result;
	///     error only when it is true;
	///   - {"type": "error", "content", "code", "retryAfter"}: a failure, with
	///     the name of its FailureCode (see failureName()) and the seconds to
	///     wait before a retry, when they are known;
	///   - {"type": "done", "stopReason"}: the end of the turn, which every
	///     turn has, with the name of the stop reason (see stopReasonName()).
	///
	/// All members are thread-safe.
{
public:
	using EventSink = std::function<void (const Poco::JSON::Object::Ptr& event)>;
		/// Gets the events of a turn. Throwing stops the turn: the exception
		/// leaves chat().

	using Store = std::function<void (const ChatSettings& settings)>;
		/// Stores the settings a change makes, keys included. Throwing refuses
		/// the change.

	using ProviderFactory = std::function<std::shared_ptr<LLMProvider> (const ProviderSettings& settings, const std::string& baseUrl, const std::string& model, const Poco::Timespan& timeout)>;
		/// Creates a provider for a turn, or for the question for its models:
		/// of the kind and with the key and limits of settings, at baseUrl,
		/// with the model (empty for the question for models), and timeout as
		/// the limit of a call. The key of settings is the one the provider
		/// gets: its own, or the key of the environment where that goes, else
		/// none.

	using Approver = std::function<bool (const Poco::AI::MCP::Identity& user, const ToolCall& call, const Poco::AI::MCP::Tool& tool)>;
		/// Decides whether a tool call the model makes in a turn of the user
		/// runs, as AgentLoop::setApprover() does: a refused call gets an
		/// error result, which the model reads, and throwing refuses it.

	struct Params
		/// The settings of the service that are not an administrator's.
	{
		std::string systemPrompt;
			/// The system prompt of every turn.
		int maxRounds = AgentLoop::DEFAULT_MAX_ROUNDS;
			/// The rounds of tool calls of a turn; see AgentLoop.
		int maxTokens = 0;
			/// The limit of an answer, for a provider without its own; 0 for
			/// the default of the provider class (see AnthropicProvider and
			/// OpenAIProvider).
		Poco::Timespan timeout = Poco::Timespan(60, 0);
			/// The limit of a call, for a provider without its own.
		Poco::Timespan probeTimeout = Poco::Timespan(5, 0);
			/// How long a server has to list its models, for a provider without
			/// its own limit.
		Poco::Timespan modelsLifetime = Poco::Timespan(60, 0);
			/// How long the models a provider listed are kept.
		SessionFactory sessionFactory;
			/// The sessions of the default provider factory; empty for
			/// createClientSession().
		ProviderFactory providerFactory;
			/// Creates the providers; empty for Anthropic and OpenAI providers
			/// as the kind says.
		Store store;
			/// Stores the settings; empty to keep them in memory only.
		Approver approver;
			/// Decides which tool calls of a turn run; empty for all. The tools
			/// run with the identity of the user, and the input the model gives
			/// them is untrusted (see AgentLoop): a host whose users may not run
			/// every call a model makes for them decides here.
		std::string loggerName = "AI.ChatService";
			/// The logger of the service.
	};

	ChatService(std::shared_ptr<ToolRegistry> pTools, const ChatSettings& settings, const Params& params);
		/// Creates the service with the tools of the application and the
		/// settings the host stored.
		///
		/// Throws Poco::InvalidArgumentException if pTools is null, maxRounds
		/// is below 1, or the settings are not valid (see configure()).

	~ChatService();
		/// Destroys the service. No turn may run any more: a host cancels the
		/// turns with cancelAll() and waits until its request handlers have
		/// ended.

	void chat(Poco::JSON::Array& conversation, const Poco::AI::MCP::Identity& user, const std::string& chatId, const EventSink& onEvent, std::shared_ptr<Cancellation> pCancellation = nullptr);
		/// Runs a turn on conversation, the messages of the user and the
		/// answers so far as {"role", "content"} objects, and appends the turn
		/// to it, in the format of the provider. The tools run with the
		/// identity of the user in their request context, and decide with it
		/// what the user may do. The turn can be cancelled by the principal of
		/// the user and the id of the chat, and through pCancellation, which
		/// stops this turn alone; without one the service makes its own. Every
		/// turn ends with a done event, also one that could not start, e.g.
		/// without a provider.

	int cancel(const std::string& user, const std::string& chatId = std::string());
		/// Cancels the turns of the user that run in the chat, or all of the
		/// user's for an empty chatId. Returns how many. A user who is not
		/// signed in, with an empty principal, cancels none: that principal
		/// is everybody's who is not signed in. Their turns stop through the
		/// cancellation they were started with, or cancelAll().

	void cancelAll();
		/// Cancels every turn that runs.

	[[nodiscard]] std::string provider() const;
		/// Returns the provider in use, or an empty string when none is
		/// enabled.

	[[nodiscard]] std::string model() const;
		/// Returns the model in use: the one selected, else the first the
		/// provider offers as far as the service knows; may be empty until
		/// models() has asked the provider.

	[[nodiscard]] std::vector<std::string> providers() const;
		/// Returns the enabled providers.

	[[nodiscard]] std::vector<std::string> models();
		/// Returns the models of the provider in use: its list, else what it
		/// listed itself, which is asked for when the service has none or the
		/// one it has is older than modelsLifetime. A provider that did not
		/// answer is asked again after ten seconds at the earliest. One caller
		/// at a time asks a provider: the others get what is known meanwhile,
		/// maybe nothing, without waiting.

	[[nodiscard]] std::vector<std::string> models(const std::string& provider);
		/// Returns the models of a provider, as models() does for the one in
		/// use; nothing for a provider that is not configured.

	[[nodiscard]] std::vector<ProviderStatus> providerStatus() const;
		/// Returns the status of every enabled provider.

	void selectProvider(const std::string& name);
		/// Puts the provider in use, with the first model it offers, and stores
		/// the settings.
		///
		/// Throws Poco::NotFoundException if the provider is not enabled.

	void selectModel(const std::string& model);
		/// Puts the model in use and stores the settings.
		///
		/// Throws Poco::InvalidArgumentException if model is empty, and
		/// Poco::NotFoundException if the provider in use has a list of
		/// models (ProviderSettings::models) that does not name it.

	ProviderStatus retry(const std::string& name);
		/// Asks the provider for its models now, with failover among its
		/// servers, and returns what came of it; while another caller asks
		/// the provider, the status known so far.
		///
		/// Throws Poco::NotFoundException if the provider is not enabled.

	[[nodiscard]] ChatSettings settings() const;
		/// Returns the settings, keys included: the configured providers in
		/// their order, then every kind with defaults that is not among them,
		/// disabled.

	void configure(const std::vector<ProviderSettings>& providers);
		/// Replaces the providers and stores the settings. An entry without a
		/// key keeps the key the provider had, as does a kind with defaults
		/// left out, which comes back disabled, unless its base URL or servers
		/// differ: a key does not go to another address. setKey() clears one.
		/// When the provider in use is no longer enabled, the first enabled one
		/// is used; when its model is no longer in its list, the first of the
		/// list.
		///
		/// Throws Poco::InvalidArgumentException if a name is empty or used
		/// twice, a provider of another kind than those with defaults has no
		/// base URL, a limit is negative, or no provider is enabled.

	void setKey(const std::string& provider, const std::string& key);
		/// Sets or, with an empty key, clears the key of a provider, and stores
		/// the settings.
		///
		/// Throws Poco::NotFoundException if the provider is not configured.

	[[nodiscard]] bool hasKey(const std::string& provider) const;
		/// Returns true if the provider has a key: its own, or the key of the
		/// environment at the address the service started with.

	[[nodiscard]] static bool sameAddress(const ProviderSettings& first, const ProviderSettings& second);
		/// Returns true if both settings reach their provider at the same
		/// base URL, the default of the kind for none, with the same servers.

	[[nodiscard]] static std::string failureName(FailureCode code);
		/// The name of a failure in an error event: unauthenticated,
		/// rate_limited, quota_exceeded, overloaded, server_error,
		/// request_rejected, malformed_tool_input, stream_interrupted,
		/// stream_error, provider_unreachable, no_answer or round_limit;
		/// empty for NONE.

	[[nodiscard]] static std::string stopReasonName(StopReason reason);
		/// The name of a stop reason in a done event: unknown, end_turn,
		/// tool_use, max_tokens, stop_sequence, refusal, content_filter,
		/// paused, cancelled or failure.

	[[nodiscard]] static Poco::JSON::Object::Ptr toJSON(const AgentEvent& event);
		/// Returns the event as the service reports it.

private:
	struct Runtime
		/// What the service learnt of a provider since its settings changed.
	{
		ProviderStatus status;
		std::string server;
			/// The base URL of the server that answered last, for a base URL
			/// with a placeholder.
		std::vector<std::string> models;
			/// What the provider listed.
		Poco::Timestamp listed;
		bool hasModels = false;
	};

	struct Turn
	{
		std::string user;
		std::string chatId;
		std::shared_ptr<Cancellation> pCancellation;
	};

	static ChatSettings complete(ChatSettings settings);
	static void validate(const ChatSettings& settings, bool needsProvider);
	static void normalize(ChatSettings& settings);
	static std::optional<ProviderSettings> find(const ChatSettings& settings, const std::string& name);
	static bool sameConnection(const ProviderSettings& first, const ProviderSettings& second);
	ChatSettings current() const;
	void change(const ChatSettings& settings);
	ProviderStatus probe(const ProviderSettings& settings, std::string& server, std::vector<std::string>& models) const;
	void record(const std::string& name, Poco::UInt64 generation, const ProviderStatus& status, const std::string& server, const std::vector<std::string>& models);
	std::string keyOf(const ProviderSettings& settings) const;
	std::shared_ptr<LLMProvider> createProvider(const ProviderSettings& settings, const std::string& baseUrl, const std::string& model, const Poco::Timespan& timeout) const;
	std::vector<std::string> modelsOf(const std::string& name, bool refresh);
	std::optional<ProviderStatus> probeOnce(const ProviderSettings& settings, Poco::UInt64 generation, std::string& server, std::vector<std::string>& models);
	void connectionFailed(const std::string& name, Poco::UInt64 generation, const std::string& detail);
	Poco::Timespan probeTimeoutOf(const ProviderSettings& settings) const;

	ChatService(const ChatService&) = delete;
	ChatService& operator = (const ChatService&) = delete;

	std::shared_ptr<ToolRegistry> _pTools;
	const Params _params;
	Poco::FastMutex _changeMutex;
	mutable Poco::FastMutex _mutex;
	ChatSettings _settings;
	std::map<std::string, ProviderSettings> _startAddresses;
		/// Of every provider of the settings the service was created with, the
		/// name, base URL and servers: where the key of its environment goes.
	Poco::UInt64 _generation = 0;
	std::map<std::string, Runtime> _runtime;
	std::set<std::string> _probing;
		/// The providers a caller asks for their models now: no other caller
		/// asks them meanwhile.
	Poco::FastMutex _turnsMutex;
	std::vector<Turn> _turns;
};


} // namespace Poco::AI


#endif // AI_ChatService_INCLUDED
