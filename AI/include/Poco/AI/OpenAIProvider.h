//
// OpenAIProvider.h
//
// Library: AI
// Package: Providers
// Module:  OpenAIProvider
//
// A provider for the OpenAI Chat Completions API and for the servers that
// implement the same wire format.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_OpenAIProvider_INCLUDED
#define AI_OpenAIProvider_INCLUDED


#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include <string>
#include <vector>
#include <mutex>


namespace Poco {
namespace AI {


class AI_API OpenAIProvider: public LLMProvider
	/// A provider for the OpenAI Chat Completions API, as served by OpenAI
	/// and by the servers that implement the same wire format (ollama,
	/// llama.cpp, vLLM and others). Streams the response (server-sent
	/// events) and reports function calls as tool calls; also lists the
	/// models the server offers and computes embeddings.
	///
	/// Sessions come from the session factory, createClientSession() by
	/// default; an https base URL needs Poco::Net::HTTPSSessionInstantiator
	/// registered by the application. The base URL includes the API version
	/// path, e.g. "https://api.openai.com/v1" or "http://localhost:11434/v1".
	///
	/// This library is not affiliated with or endorsed by OpenAI; the name
	/// identifies the API format it speaks.
{
public:
	OpenAIProvider(
		const std::string& baseUrl,
		const std::string& apiKey,
		const std::string& model,
		int maxTokens,
		int timeoutSeconds,
		bool supportsTools = true,
		SessionFactory sessionFactory = SessionFactory());
		/// Creates the provider for the API at baseUrl. An empty apiKey sends
		/// no Authorization header (local servers). A call asks for at most
		/// maxTokens output tokens and waits at most timeoutSeconds for the
		/// server. With supportsTools false no tool definitions are sent,
		/// for servers that reject them.

	~OpenAIProvider() = default;

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

	static Poco::JSON::Array toOpenAITools(const Poco::JSON::Array& tools);
		/// Converts tool definitions ({name, description, parameters}) to the
		/// Chat Completions function tool format.

	std::vector<std::string> listModels() override;
		/// Asks the server what it serves (GET {baseUrl}/models, the discovery
		/// endpoint the compatible servers implement too) and returns the model
		/// ids, sorted. Models whose id contains "embed" are left out: they
		/// appear in the same list but cannot answer a chat request. Returns
		/// an empty vector on any failure; discovery is never a reason to fail
		/// a request. Uses a short deadline of its own, not the chat timeout.

	std::vector<std::vector<float>> embed(
		const std::vector<std::string>& texts,
		const std::string& model,
		int dimensions = 0);
		/// Returns one embedding vector per input text via the embeddings
		/// endpoint (POST {baseUrl}/embeddings). Not streamed; uses the chat
		/// timeout and, when set, the API key. The model is passed explicitly
		/// because embedding models are distinct from chat models. A
		/// dimensions value > 0 is forwarded as the request parameter of the
		/// same name (only some servers honour it; size storage from the
		/// returned vectors, not from this parameter).
		///
		/// Throws Poco::RuntimeException on a non-OK response and
		/// Poco::DataFormatException on a malformed response body.

	void cancel() override;

private:
	std::unique_ptr<Poco::Net::HTTPClientSession> openSession(const Poco::URI& uri, long timeoutSeconds) const;

	std::string _baseUrl;
	std::string _apiKey;
	std::string _model;
	int _maxTokens;
	int _timeoutSeconds;
	bool _supportsTools;
	SessionFactory _sessionFactory;
	std::mutex _sessionMutex;
	Poco::Net::HTTPClientSession* _pActiveSession = nullptr;
		/// The session of the in-flight chat call, for cancel(); not owned.
};


} } // namespace Poco::AI


#endif // AI_OpenAIProvider_INCLUDED
