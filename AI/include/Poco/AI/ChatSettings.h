//
// ChatSettings.h
//
// Library: AI
// Package: Chat
// Module:  ChatSettings
//
// Definition of the ProviderSettings, ChatSettings and ProviderStatus structs.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ChatSettings_INCLUDED
#define AI_ChatSettings_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/JSON/Object.h"
#include "Poco/Timespan.h"
#include <cstddef>
#include <string>
#include <vector>


namespace Poco::AI {


struct AI_API ProviderSettings
	/// A provider a chat may use, as an administrator configures it.
	///
	/// Its kind tells the API it speaks: "anthropic" for the Anthropic
	/// Messages API, and "openai", "ollama" and "openai-compatible" for
	/// servers of the OpenAI Chat Completions API. The kinds anthropic,
	/// openai and ollama have default base URLs; an openai-compatible server
	/// needs its base URL set. Without a kind, the name gives it: the name
	/// when it is one of kinds(), else openai-compatible.
{
	std::string name;
		/// The name of the provider in the chat.

	std::string kind;
		/// The kind of the provider; empty for the one the name gives.

	bool enabled = false;
		/// Whether the chat offers the provider.

	std::string baseUrl;
		/// The base URL of the API, e.g. https://api.anthropic.com/v1; empty
		/// for the default of the kind.

	std::vector<std::string> servers;
		/// Hosts tried in turn in place of a placeholder in the base URL, a
		/// name in brackets such as [server], e.g. http://[server]:11434/v1:
		/// the first that answers serves the chat.

	std::vector<std::string> models;
		/// The models the chat offers; empty to offer what the provider lists.

	int maxTokens = 0;
		/// The limit of an answer, in tokens; 0 for that of the service.

	Poco::Timespan timeout;
		/// The limit of a call of the API; zero for that of the service.
		/// Stored and sent in whole seconds.

	Poco::Timespan probeTimeout;
		/// How long a server has to answer the question for its models; zero
		/// for that of the service. Stored and sent in whole seconds.

	bool tools = true;
		/// Whether the model is given the tools of the chat; off for a model
		/// that cannot call tools.

	std::string apiKey;
		/// The key of the API. Empty for a provider that needs none, or one
		/// whose key is in the environment variable of its kind:
		/// ANTHROPIC_API_KEY or OPENAI_API_KEY.

	[[nodiscard]] Poco::JSON::Object::Ptr toJSON() const;
		/// Returns the settings as a JSON object, the key included: {"name",
		/// "kind", "enabled", "baseUrl", "servers", "models", "maxTokens",
		/// "timeout", "probeTimeout", "tools", "apiKey"}, kind only when it
		/// is set.

	[[nodiscard]] static ProviderSettings fromJSON(const Poco::JSON::Object::Ptr& pObject);
		/// Reads settings that toJSON() wrote, or that a client sent. A member
		/// that is missing gets its default; strings are strings, booleans
		/// booleans and numbers integers, and servers and models are arrays of
		/// strings, or one string of comma-separated items.
		///
		/// Throws Poco::DataFormatException if a member has the wrong type,
		/// or a number is beyond the range of its member.

	[[nodiscard]] std::string effectiveKind() const;
		/// The kind, or the one the name gives when it is empty.

	[[nodiscard]] static const std::vector<std::string>& kinds();
		/// The kinds with defaults: anthropic, openai and ollama.

	[[nodiscard]] static bool isKind(const std::string& kind);
		/// Returns true for anthropic, openai, ollama and openai-compatible.

	[[nodiscard]] static std::string defaultBaseUrl(const std::string& kind);
		/// The base URL of a kind when none is set, or an empty string for a
		/// kind without one.

	[[nodiscard]] static bool usesApiKey(const std::string& kind);
		/// Returns true for the kinds that authenticate with a key: anthropic
		/// and openai.

	[[nodiscard]] static bool usesServers(const std::string& kind);
		/// Returns true for the kinds whose servers usually run on hosts of
		/// their own, among which the chat fails over: ollama.

	[[nodiscard]] static std::string keyVariable(const std::string& kind);
		/// The environment variable that holds the key of a kind, or an empty
		/// string.
};


struct AI_API ChatSettings
	/// What an administrator configures of a chat: the providers, and the
	/// provider and model in use.
{
	std::vector<ProviderSettings> providers;
		/// The providers, in the order the chat offers the enabled ones.

	std::string provider;
		/// The provider in use; empty for the first enabled one.

	std::string model;
		/// The model in use; empty for the first the provider offers.

	[[nodiscard]] Poco::JSON::Object::Ptr toJSON() const;
		/// Returns the settings as a JSON object, keys included, for the host
		/// to store: {"provider", "model", "providers": [...]}, each provider
		/// as ProviderSettings::toJSON() writes it.

	[[nodiscard]] static ChatSettings fromJSON(const Poco::JSON::Object::Ptr& pObject);
		/// Reads settings toJSON() wrote, each provider as
		/// ProviderSettings::fromJSON() does. What is missing gets its default.
		///
		/// Throws Poco::DataFormatException if a member has the wrong type,
		/// or a number is beyond the range of its member.
};


struct AI_API ProviderStatus
	/// What the chat knows of a provider's connection.
{
	std::string name;
		/// The provider.

	bool probed = false;
		/// Whether the provider was asked for its models since its settings
		/// last changed.

	bool reachable = false;
		/// Whether it answered (when probed).

	std::string detail;
		/// Why not, or which server answered.

	std::size_t modelCount = 0;
		/// The number of models it listed.
};


} // namespace Poco::AI


#endif // AI_ChatSettings_INCLUDED
