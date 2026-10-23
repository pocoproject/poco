//
// ChatService.cpp
//
// Library: AI
// Package: Chat
// Module:  ChatService
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/ChatService.h"
#include "Poco/AI/AnthropicProvider.h"
#include "Poco/AI/OpenAIProvider.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/Ascii.h"
#include "Poco/Environment.h"
#include "Poco/Logger.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <set>
#include <utility>


namespace Poco::AI {


namespace
{
	const Poco::Timespan RETRY_INTERVAL(10, 0);
		/// How long a provider that did not answer is not asked for its models
		/// again, but by retry().


	bool findPlaceholder(const std::string& url, std::size_t& begin, std::size_t& end)
		/// Finds a name in brackets in url, such as [server]: begin at its '[',
		/// end after its ']'. An IPv6 address in brackets is no name.
	{
		for (std::size_t pos = url.find('['); pos != std::string::npos; pos = url.find('[', pos + 1))
		{
			std::size_t i = pos + 1;
			if (i >= url.size() || !(Poco::Ascii::isAlpha(url[i]) || url[i] == '_')) continue;
			while (i < url.size() && (Poco::Ascii::isAlphaNumeric(url[i]) || url[i] == '_')) ++i;
			if (i < url.size() && url[i] == ']')
			{
				begin = pos;
				end = i + 1;
				return true;
			}
		}
		return false;
	}


	std::string baseUrlOf(const ProviderSettings& settings)
	{
		return settings.baseUrl.empty() ? ProviderSettings::defaultBaseUrl(settings.effectiveKind()) : settings.baseUrl;
	}


	std::string withoutUserInfo(const std::string& url)
		/// The URL without the user name and password it may carry, for the
		/// status of a provider and the log.
	{
		try
		{
			Poco::URI uri(url);
			if (uri.getUserInfo().empty()) return url;
			uri.setUserInfo(std::string());
			return uri.toString();
		}
		catch (const Poco::SyntaxException&)
		{
			return "(a URL that cannot be parsed)";
		}
	}


	bool hasPlaceholder(const std::string& url)
	{
		std::size_t begin = 0;
		std::size_t end = 0;
		return findPlaceholder(url, begin, end);
	}
}


ChatService::ChatService(std::shared_ptr<ToolRegistry> pTools, const ChatSettings& settings, const Params& params):
	_pTools(std::move(pTools)),
	_params(params)
{
	if (_pTools == nullptr) throw Poco::InvalidArgumentException("A chat service needs a tool registry");
	if (_params.maxRounds < 1) throw Poco::InvalidArgumentException("A turn needs at least one round");
	ChatSettings initial = complete(settings);
	validate(initial, false);
	normalize(initial);
	for (const ProviderSettings& entry: initial.providers)
	{
		ProviderSettings address;
		address.name = entry.name;
		address.baseUrl = entry.baseUrl;
		address.servers = entry.servers;
		_startAddresses[entry.name] = address;
	}
	_settings = std::move(initial);
}


ChatService::~ChatService()
{
	cancelAll();
}


void ChatService::chat(Poco::JSON::Array& conversation, const Poco::AI::MCP::Identity& user, const std::string& chatId, const EventSink& onEvent, std::shared_ptr<Cancellation> pCancellation)
{
	if (pCancellation == nullptr) pCancellation = std::make_shared<Cancellation>();
	{
		Poco::FastMutex::ScopedLock lock(_turnsMutex);
		_turns.push_back(Turn{user.principal, chatId, pCancellation});
	}
	struct Deregistration
	{
		ChatService& service;
		const std::shared_ptr<Cancellation>& pCancellation;

		~Deregistration()
		{
			Poco::FastMutex::ScopedLock lock(service._turnsMutex);
			service._turns.erase(std::remove_if(service._turns.begin(), service._turns.end(),
				[this](const Turn& turn) { return turn.pCancellation == pCancellation; }), service._turns.end());
		}
	} deregistration{*this, pCancellation};

	const auto refuse = [&onEvent](const std::string& message)
	{
		AgentEvent failure;
		failure.type = AgentEvent::Type::FAILURE;
		failure.content = message;
		failure.code = FailureCode::PROVIDER_FAILED;
		onEvent(toJSON(failure));
		AgentEvent done;
		done.type = AgentEvent::Type::DONE;
		done.stopReason = StopReason::FAILURE;
		onEvent(toJSON(done));
	};

	std::optional<ProviderSettings> settings;
	std::string model;
	Poco::UInt64 generation = 0;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		settings = find(_settings, _settings.provider);
		model = _settings.model;
		generation = _generation;
	}
	if (!settings)
	{
		refuse("No provider is enabled");
		return;
	}
	if (model.empty())
	{
		const std::vector<std::string> offered = modelsOf(settings->name, false);
		if (!offered.empty()) model = offered.front();
	}
	if (model.empty())
	{
		refuse("The provider " + settings->name + " offers no model");
		return;
	}

	std::string baseUrl = baseUrlOf(*settings);
	if (hasPlaceholder(baseUrl))
	{
		std::string server;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			const auto it = _runtime.find(settings->name);
			if (it != _runtime.end() && generation == _generation) server = it->second.server;
		}
		if (server.empty())
		{
			std::vector<std::string> models;
			const std::optional<ProviderStatus> status = probeOnce(*settings, generation, server, models);
			if (!status)
			{
				refuse("The servers of the provider " + settings->name + " are being asked which one answers; try again");
				return;
			}
			if (!status->reachable)
			{
				refuse("No server of the provider " + settings->name + " answers: " + status->detail);
				return;
			}
		}
		baseUrl = server;
	}

	const Poco::Timespan timeout = settings->timeout > Poco::Timespan() ? settings->timeout : _params.timeout;
	std::shared_ptr<LLMProvider> pProvider;
	try
	{
		pProvider = createProvider(*settings, baseUrl, model, timeout);
	}
	catch (const Poco::Exception& exc)
	{
		refuse(exc.displayText());
		return;
	}
	const std::string name = settings->name;
	// A model that cannot call tools is given none.
	ToolRegistry noTools;
	AgentLoop loop(pProvider, settings->tools ? *_pTools : noTools, _params.maxRounds);
	Poco::AI::MCP::RequestContext context;
	context.identity = user;
	loop.setRequestContext(context);
	if (_params.approver)
	{
		loop.setApprover([this, &user](const ToolCall& call, const Poco::AI::MCP::Tool& tool)
			{
				return _params.approver(user, call, tool);
			});
	}
	loop.run(conversation, _params.systemPrompt, [this, &name, generation, &onEvent](const AgentEvent& event)
		{
			if (event.type == AgentEvent::Type::FAILURE && event.code == FailureCode::PROVIDER_FAILED)
			{
				connectionFailed(name, generation, event.content);
			}
			onEvent(toJSON(event));
		}, *pCancellation);
}


int ChatService::cancel(const std::string& principal, const std::string& chatId)
{
	if (principal.empty()) return 0;
	int cancelled = 0;
	Poco::FastMutex::ScopedLock lock(_turnsMutex);
	for (const Turn& turn: _turns)
	{
		if (turn.principal == principal && (chatId.empty() || turn.chatId == chatId))
		{
			turn.pCancellation->cancel();
			++cancelled;
		}
	}
	return cancelled;
}


void ChatService::cancelAll()
{
	Poco::FastMutex::ScopedLock lock(_turnsMutex);
	for (const Turn& turn: _turns) turn.pCancellation->cancel();
}


std::string ChatService::provider() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _settings.provider;
}


std::string ChatService::model() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	if (!_settings.model.empty()) return _settings.model;
	const std::optional<ProviderSettings> settings = find(_settings, _settings.provider);
	if (!settings) return std::string();
	if (!settings->models.empty()) return settings->models.front();
	const auto it = _runtime.find(settings->name);
	if (it != _runtime.end() && !it->second.models.empty()) return it->second.models.front();
	return std::string();
}


std::vector<std::string> ChatService::providers() const
{
	std::vector<std::string> result;
	Poco::FastMutex::ScopedLock lock(_mutex);
	for (const ProviderSettings& settings: _settings.providers)
	{
		if (settings.enabled) result.push_back(settings.name);
	}
	return result;
}


std::vector<std::string> ChatService::models()
{
	return modelsOf(provider(), false);
}


std::vector<std::string> ChatService::models(const std::string& provider)
{
	return modelsOf(provider, false);
}


std::vector<ProviderStatus> ChatService::providerStatus() const
{
	std::vector<ProviderStatus> result;
	Poco::FastMutex::ScopedLock lock(_mutex);
	for (const ProviderSettings& settings: _settings.providers)
	{
		if (!settings.enabled) continue;
		const auto it = _runtime.find(settings.name);
		ProviderStatus status = it != _runtime.end() ? it->second.status : ProviderStatus();
		status.name = settings.name;
		result.push_back(status);
	}
	return result;
}


void ChatService::selectProvider(const std::string& name)
{
	Poco::FastMutex::ScopedLock lock(_changeMutex);
	ChatSettings next = current();
	const std::optional<ProviderSettings> settings = find(next, name);
	if (!settings || !settings->enabled) throw Poco::NotFoundException("No enabled provider of that name", name);
	next.provider = name;
	next.model.clear();
	change(next);
}


void ChatService::selectModel(const std::string& model)
{
	if (model.empty()) throw Poco::InvalidArgumentException("A model needs a name");
	Poco::FastMutex::ScopedLock lock(_changeMutex);
	ChatSettings next = current();
	const std::optional<ProviderSettings> active = find(next, next.provider);
	if (active && !active->models.empty() && std::find(active->models.begin(), active->models.end(), model) == active->models.end())
	{
		throw Poco::NotFoundException("The provider in use lists no model of that name", model);
	}
	next.model = model;
	change(next);
}


ProviderStatus ChatService::retry(const std::string& name)
{
	std::optional<ProviderSettings> settings;
	Poco::UInt64 generation = 0;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		settings = find(_settings, name);
		generation = _generation;
	}
	if (!settings || !settings->enabled) throw Poco::NotFoundException("No enabled provider of that name", name);
	std::string server;
	std::vector<std::string> models;
	const std::optional<ProviderStatus> status = probeOnce(*settings, generation, server, models);
	if (status) return *status;
	// Another caller asks the provider now.
	Poco::FastMutex::ScopedLock lock(_mutex);
	const auto it = _runtime.find(name);
	if (it != _runtime.end()) return it->second.status;
	ProviderStatus known;
	known.name = name;
	return known;
}


ChatSettings ChatService::settings() const
{
	return current();
}


void ChatService::configure(const std::vector<ProviderSettings>& providers)
{
	Poco::FastMutex::ScopedLock lock(_changeMutex);
	const ChatSettings previous = current();
	ChatSettings next = previous;
	next.providers = providers;
	next = complete(next);
	for (ProviderSettings& settings: next.providers)
	{
		if (!settings.apiKey.empty()) continue;
		const std::optional<ProviderSettings> old = find(previous, settings.name);
		if (old && sameAddress(*old, settings)) settings.apiKey = old->apiKey;
	}
	validate(next, true);
	change(next);
}


void ChatService::setKey(const std::string& provider, const std::string& key)
{
	Poco::FastMutex::ScopedLock lock(_changeMutex);
	ChatSettings next = current();
	const auto it = std::find_if(next.providers.begin(), next.providers.end(),
		[&provider](const ProviderSettings& settings) { return settings.name == provider; });
	if (it == next.providers.end()) throw Poco::NotFoundException("No provider of that name", provider);
	it->apiKey = key;
	change(next);
}


bool ChatService::hasKey(const std::string& provider) const
{
	std::optional<ProviderSettings> settings;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		settings = find(_settings, provider);
	}
	return settings && !keyOf(*settings).empty();
}


bool ChatService::sameAddress(const ProviderSettings& first, const ProviderSettings& second)
{
	return baseUrlOf(first) == baseUrlOf(second) && first.servers == second.servers;
}


std::string ChatService::failureName(FailureCode code)
{
	switch (code)
	{
	case FailureCode::NONE: return std::string();
	case FailureCode::UNAUTHENTICATED: return "unauthenticated";
	case FailureCode::RATE_LIMITED: return "rate_limited";
	case FailureCode::QUOTA_EXCEEDED: return "quota_exceeded";
	case FailureCode::OVERLOADED: return "overloaded";
	case FailureCode::SERVER_ERROR: return "server_error";
	case FailureCode::REQUEST_REJECTED: return "request_rejected";
	case FailureCode::MALFORMED_TOOL_INPUT: return "malformed_tool_input";
	case FailureCode::STREAM_INTERRUPTED: return "stream_interrupted";
	case FailureCode::STREAM_ERROR: return "stream_error";
	case FailureCode::PROVIDER_FAILED: return "provider_unreachable";
	case FailureCode::NO_ANSWER: return "no_answer";
	case FailureCode::ROUND_LIMIT: return "round_limit";
	}
	return std::string();
}


std::string ChatService::stopReasonName(StopReason reason)
{
	switch (reason)
	{
	case StopReason::UNKNOWN: return "unknown";
	case StopReason::END_TURN: return "end_turn";
	case StopReason::TOOL_USE: return "tool_use";
	case StopReason::MAX_TOKENS: return "max_tokens";
	case StopReason::STOP_SEQUENCE: return "stop_sequence";
	case StopReason::REFUSAL: return "refusal";
	case StopReason::CONTENT_FILTER: return "content_filter";
	case StopReason::PAUSED: return "paused";
	case StopReason::CANCELLED: return "cancelled";
	case StopReason::FAILURE: return "failure";
	}
	return "unknown";
}


Poco::JSON::Object::Ptr ChatService::toJSON(const AgentEvent& event)
{
	Poco::JSON::Object::Ptr pEvent = new Poco::JSON::Object;
	switch (event.type)
	{
	case AgentEvent::Type::TEXT:
		pEvent->set("type", "text");
		pEvent->set("content", event.content);
		break;
	case AgentEvent::Type::TOOL_CALL:
		pEvent->set("type", "tool_call");
		pEvent->set("name", event.toolName);
		pEvent->set("input", event.toolInput);
		break;
	case AgentEvent::Type::TOOL_RESULT:
		pEvent->set("type", "tool_result");
		pEvent->set("name", event.toolName);
		pEvent->set("content", event.content);
		if (event.toolError) pEvent->set("error", true);
		break;
	case AgentEvent::Type::FAILURE:
		pEvent->set("type", "error");
		pEvent->set("content", event.content);
		if (event.code != FailureCode::NONE) pEvent->set("code", failureName(event.code));
		if (event.retryAfter.totalSeconds() > 0) pEvent->set("retryAfter", static_cast<Poco::Int64>(event.retryAfter.totalSeconds()));
		if (!event.requestId.empty()) pEvent->set("requestId", event.requestId);
		break;
	case AgentEvent::Type::DONE:
		pEvent->set("type", "done");
		pEvent->set("stopReason", stopReasonName(event.stopReason));
		break;
	}
	return pEvent;
}


ChatSettings ChatService::complete(ChatSettings settings)
{
	for (const std::string& kind: ProviderSettings::kinds())
	{
		if (find(settings, kind)) continue;
		ProviderSettings entry;
		entry.name = kind;
		settings.providers.push_back(entry);
	}
	return settings;
}


void ChatService::validate(const ChatSettings& settings, bool needsProvider)
{
	std::set<std::string> names;
	bool enabled = false;
	for (const ProviderSettings& entry: settings.providers)
	{
		if (entry.name.empty()) throw Poco::InvalidArgumentException("A provider needs a name");
		if (!names.insert(entry.name).second) throw Poco::InvalidArgumentException("A provider is listed twice", entry.name);
		if (!entry.kind.empty() && !ProviderSettings::isKind(entry.kind))
		{
			throw Poco::InvalidArgumentException("A provider has a kind that is not known", entry.name + ": " + entry.kind);
		}
		if (entry.baseUrl.empty() && ProviderSettings::defaultBaseUrl(entry.effectiveKind()).empty())
		{
			throw Poco::InvalidArgumentException("A provider of a kind without a default base URL needs one", entry.name);
		}
		if (entry.maxTokens < 0 || entry.timeout < Poco::Timespan() || entry.probeTimeout < Poco::Timespan())
		{
			throw Poco::InvalidArgumentException("A limit cannot be negative", entry.name);
		}
		enabled = enabled || entry.enabled;
	}
	if (needsProvider && !enabled) throw Poco::InvalidArgumentException("At least one provider must be enabled");
}


void ChatService::normalize(ChatSettings& settings)
{
	const std::optional<ProviderSettings> selected = find(settings, settings.provider);
	if (!selected || !selected->enabled)
	{
		const auto it = std::find_if(settings.providers.begin(), settings.providers.end(),
			[](const ProviderSettings& entry) { return entry.enabled; });
		settings.provider = it != settings.providers.end() ? it->name : std::string();
		settings.model.clear();
	}
	const std::optional<ProviderSettings> active = find(settings, settings.provider);
	if (active && !active->models.empty() && std::find(active->models.begin(), active->models.end(), settings.model) == active->models.end())
	{
		settings.model.clear();
	}
}


std::optional<ProviderSettings> ChatService::find(const ChatSettings& settings, const std::string& name)
{
	const auto it = std::find_if(settings.providers.begin(), settings.providers.end(),
		[&name](const ProviderSettings& entry) { return entry.name == name; });
	if (it == settings.providers.end()) return std::nullopt;
	return *it;
}


bool ChatService::sameConnection(const ProviderSettings& first, const ProviderSettings& second)
{
	return first.enabled == second.enabled
		&& first.effectiveKind() == second.effectiveKind()
		&& first.baseUrl == second.baseUrl
		&& first.servers == second.servers
		&& first.models == second.models
		&& first.maxTokens == second.maxTokens
		&& first.timeout == second.timeout
		&& first.probeTimeout == second.probeTimeout
		&& first.tools == second.tools
		&& first.apiKey == second.apiKey;
}


ChatSettings ChatService::current() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _settings;
}


void ChatService::change(const ChatSettings& settings)
{
	ChatSettings next = settings;
	normalize(next);
	if (_params.store) _params.store(next);

	Poco::FastMutex::ScopedLock lock(_mutex);
	// What was learnt of a provider whose connection changed no longer holds.
	bool changed = false;
	for (const ProviderSettings& entry: next.providers)
	{
		const std::optional<ProviderSettings> old = find(_settings, entry.name);
		if (!old || !sameConnection(*old, entry))
		{
			_runtime.erase(entry.name);
			changed = true;
		}
	}
	if (changed) ++_generation;
	_settings = std::move(next);
	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	if (logger.information())
	{
		logger.information("AI chat uses the provider %s, model %s", _settings.provider, _settings.model.empty() ? std::string("(the first offered)") : _settings.model);
	}
}


ProviderStatus ChatService::probe(const ProviderSettings& settings, std::string& server, std::vector<std::string>& models) const
{
	ProviderStatus status;
	status.name = settings.name;
	status.probed = true;
	server.clear();
	models.clear();

	const std::string base = baseUrlOf(settings);
	std::vector<std::string> candidates;
	std::size_t begin = 0;
	std::size_t end = 0;
	if (findPlaceholder(base, begin, end))
	{
		for (const std::string& host: settings.servers)
		{
			candidates.push_back(base.substr(0, begin) + host + base.substr(end));
		}
		if (candidates.empty())
		{
			status.detail = "the base URL has a placeholder, and the provider has no servers";
			return status;
		}
	}
	else candidates.push_back(base);

	Poco::Logger& logger = Poco::Logger::get(_params.loggerName);
	const Poco::Timespan timeout = probeTimeoutOf(settings);
	std::string failures;
	for (const std::string& candidate: candidates)
	{
		try
		{
			models = createProvider(settings, candidate, std::string(), timeout)->listModels();
			status.reachable = true;
			status.modelCount = models.size();
			if (candidates.size() > 1) status.detail = withoutUserInfo(candidate);
			server = candidate;
			return status;
		}
		catch (const Poco::Exception& exc)
		{
			if (logger.warning())
			{
				logger.warning("AI provider %s does not answer at %s: %s", settings.name, withoutUserInfo(candidate), exc.displayText());
			}
			if (!failures.empty()) failures += "; ";
			failures += (candidates.size() > 1 ? withoutUserInfo(candidate) + ": " : std::string()) + exc.displayText();
		}
	}
	models.clear();
	status.detail = failures;
	return status;
}


void ChatService::record(const std::string& name, Poco::UInt64 generation, const ProviderStatus& status, const std::string& server, const std::vector<std::string>& models)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	// An answer to settings that have changed since tells nothing of the
	// present ones.
	if (generation != _generation || !find(_settings, name)) return;
	Runtime& runtime = _runtime[name];
	runtime.status = status;
	runtime.server = status.reachable ? server : std::string();
	runtime.models = status.reachable ? models : std::vector<std::string>();
	runtime.hasModels = status.reachable;
	runtime.listed.update();
}


std::string ChatService::keyOf(const ProviderSettings& settings) const
	/// The key a provider of the settings gets: its own, else the key of the
	/// environment variable of its kind if the provider is where it was when
	/// the service was created, else none.
{
	if (!settings.apiKey.empty()) return settings.apiKey;
	const std::string variable = ProviderSettings::keyVariable(settings.effectiveKind());
	if (variable.empty()) return std::string();
	const auto it = _startAddresses.find(settings.name);
	if (it == _startAddresses.end() || !sameAddress(it->second, settings)) return std::string();
	return Poco::Environment::get(variable, "");
}


std::shared_ptr<LLMProvider> ChatService::createProvider(const ProviderSettings& settings, const std::string& baseUrl, const std::string& model, const Poco::Timespan& timeout) const
{
	ProviderSettings keyed = settings;
	keyed.apiKey = keyOf(settings);
	if (_params.providerFactory)
	{
		std::shared_ptr<LLMProvider> pProvider = _params.providerFactory(keyed, baseUrl, model, timeout);
		if (pProvider == nullptr) throw Poco::NullPointerException("The provider factory returned no provider", settings.name);
		return pProvider;
	}
	const std::string& key = keyed.apiKey;
	const int maxTokens = settings.maxTokens > 0 ? settings.maxTokens : _params.maxTokens;
	const std::string kind = settings.effectiveKind();
	if (kind == "anthropic")
	{
		AnthropicProvider::Params params;
		params.baseURI = Poco::URI(baseUrl);
		params.apiKey = key;
		params.model = model;
		if (maxTokens > 0) params.maxTokens = maxTokens;
		params.timeout = timeout;
		return std::make_shared<AnthropicProvider>(params, _params.sessionFactory);
	}
	OpenAIProvider::Params params;
	params.baseURI = Poco::URI(baseUrl);
	params.apiKey = key;
	params.model = model;
	if (maxTokens > 0) params.maxTokens = maxTokens;
	// OpenAI's reasoning models refuse max_tokens; other servers know only it.
	params.maxTokensField = kind == "openai" ? "max_completion_tokens" : "max_tokens";
	params.timeout = timeout;
	return std::make_shared<OpenAIProvider>(params, _params.sessionFactory);
}


std::vector<std::string> ChatService::modelsOf(const std::string& name, bool refresh)
{
	std::optional<ProviderSettings> settings;
	Poco::UInt64 generation = 0;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		settings = find(_settings, name);
		if (!settings) return std::vector<std::string>();
		if (!settings->models.empty()) return settings->models;
		const auto it = _runtime.find(name);
		if (!refresh && it != _runtime.end())
		{
			const Runtime& runtime = it->second;
			if (runtime.hasModels && !runtime.listed.isElapsed(_params.modelsLifetime.totalMicroseconds())) return runtime.models;
			if (runtime.status.probed && !runtime.status.reachable && !runtime.listed.isElapsed(RETRY_INTERVAL.totalMicroseconds()))
			{
				return std::vector<std::string>();
			}
		}
		generation = _generation;
	}
	std::string server;
	std::vector<std::string> models;
	if (!probeOnce(*settings, generation, server, models))
	{
		// Another caller asks the provider now: what is known meanwhile.
		Poco::FastMutex::ScopedLock lock(_mutex);
		const auto it = _runtime.find(name);
		return it != _runtime.end() ? it->second.models : std::vector<std::string>();
	}
	return models;
}


std::optional<ProviderStatus> ChatService::probeOnce(const ProviderSettings& settings, Poco::UInt64 generation, std::string& server, std::vector<std::string>& models)
	/// Asks the provider for its models and records the answer, unless
	/// another caller asks it already: then it returns nothing at once.
{
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (!_probing.insert(settings.name).second) return std::nullopt;
	}
	struct Release
	{
		ChatService& service;
		const std::string& name;

		~Release()
		{
			Poco::FastMutex::ScopedLock lock(service._mutex);
			service._probing.erase(name);
		}
	} release{*this, settings.name};
	const ProviderStatus status = probe(settings, server, models);
	record(settings.name, generation, status, server, models);
	return status;
}


void ChatService::connectionFailed(const std::string& name, Poco::UInt64 generation, const std::string& detail)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	// A turn that began before the settings changed tells nothing of the
	// present ones.
	if (generation != _generation || !find(_settings, name)) return;
	Runtime& runtime = _runtime[name];
	runtime.server.clear();
	runtime.status.name = name;
	runtime.status.probed = true;
	runtime.status.reachable = false;
	runtime.status.detail = detail;
}


Poco::Timespan ChatService::probeTimeoutOf(const ProviderSettings& settings) const
{
	return settings.probeTimeout > Poco::Timespan() ? settings.probeTimeout : _params.probeTimeout;
}


} // namespace Poco::AI
