//
// AnthropicProvider.h
//
// Library: AI
// Package: Providers
// Module:  AnthropicProvider
//
// A provider for the Anthropic Messages API.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_AnthropicProvider_INCLUDED
#define AI_AnthropicProvider_INCLUDED


#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include <string>
#include <mutex>


namespace Poco {
namespace AI {


class AI_API AnthropicProvider: public LLMProvider
	/// A provider for the Anthropic Messages API: streams the response
	/// (server-sent events) and reports tool use blocks as tool calls.
	/// The system prompt and the last tool definition carry prompt
	/// caching markers.
	///
	/// Sessions come from the session factory, createClientSession() by
	/// default; an https base URL needs Poco::Net::HTTPSSessionInstantiator
	/// registered by the application. The base URL includes the API version
	/// path, e.g. "https://api.anthropic.com/v1".
	///
	/// This library is not affiliated with or endorsed by Anthropic; the
	/// name identifies the API it speaks.
{
public:
	static constexpr char DEFAULT_API_VERSION[] = "2023-06-01";
		/// The value of the anthropic-version header sent by default.

	AnthropicProvider(
		const std::string& baseUrl,
		const std::string& apiKey,
		const std::string& model,
		int maxTokens,
		int timeoutSeconds,
		SessionFactory sessionFactory = SessionFactory());
		/// Creates the provider for the API at baseUrl, authenticating with
		/// apiKey. A call asks for at most maxTokens output tokens and waits
		/// at most timeoutSeconds for the server.

	~AnthropicProvider() = default;

	void setApiVersion(const std::string& version);
		/// Sets the value of the anthropic-version header.

	const std::string& apiVersion() const;
		/// Returns the value of the anthropic-version header.

	void setModel(const std::string& model) override;

	void chat(
		const Poco::JSON::Array& messages,
		const std::string& systemPrompt,
		const Poco::JSON::Array& tools,
		ContentEventCallback onEvent) override;

	void appendAssistantMessage(
		Poco::JSON::Array& messages,
		const std::string& text,
		const std::vector<ContentEvent>& toolCalls) override;

	void appendToolResult(
		Poco::JSON::Array& messages,
		const std::string& toolCallId,
		const std::string& result) override;

	static Poco::JSON::Array toAnthropicTools(const Poco::JSON::Array& tools);
		/// Converts tool definitions ({name, description, parameters}) to the
		/// Messages API tool format and marks the last one for prompt caching.

	void cancel() override;

private:
	std::string _baseUrl;
	std::string _apiKey;
	std::string _model;
	int _maxTokens;
	int _timeoutSeconds;
	SessionFactory _sessionFactory;
	std::string _apiVersion;
	std::mutex _sessionMutex;
	Poco::Net::HTTPClientSession* _pActiveSession = nullptr;
		/// The session of the in-flight call, for cancel(); not owned.
};


//
// inlines
//
inline const std::string& AnthropicProvider::apiVersion() const
{
	return _apiVersion;
}


} } // namespace Poco::AI


#endif // AI_AnthropicProvider_INCLUDED
