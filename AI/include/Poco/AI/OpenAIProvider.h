//
// OpenAIProvider.h
//
// Library: AI
// Package: Providers
// Module:  OpenAIProvider
//
// A provider for the OpenAI Chat Completions API and the servers that
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
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API OpenAIProvider: public LLMProvider
	/// A provider for the OpenAI Chat Completions API, as served by OpenAI
	/// and by the servers that implement the same wire format (ollama,
	/// llama.cpp, vLLM and others). Streams the response (server-sent
	/// events) and reports function calls as tool calls; also lists the
	/// models the server offers and computes embeddings.
	///
	/// Chat Completions carries no reasoning items across tool turns, which
	/// OpenAI's reasoning models keep in its Responses API; with those models
	/// a tool turn starts its reasoning anew.
	///
	/// Sessions come from the session factory, createClientSession() by
	/// default; an https base URI needs Poco::Net::HTTPSSessionInstantiator
	/// registered by the application.
	///
	/// This library is not affiliated with or endorsed by OpenAI; the name
	/// identifies the API format it speaks.
{
public:
	struct Params
		/// The settings of the provider.
	{
		Poco::URI baseURI{"https://api.openai.com/v1"};
			/// The API base, with its version path, e.g.
			/// "http://localhost:11434/v1" for ollama.
		std::string apiKey;
			/// When empty, no Authorization header is sent (local servers).
		std::string model;
		int maxTokens = 4096;
			/// The most output tokens a call asks for; 0 sends no limit.
		std::string maxTokensField = "max_tokens";
			/// The request field of the limit. OpenAI's reasoning models take
			/// only "max_completion_tokens", which some compatible servers
			/// ignore.
		Poco::Timespan timeout;
			/// How long a read may wait for the server; zero leaves the
			/// session's own timeout.
		bool supportsTools = true;
			/// When false, no tools are sent, for servers that reject them.
		std::size_t maxResponseSize = 32*1024*1024;
			/// The limit of a response body, and of one event of a stream.
	};

	explicit OpenAIProvider(const Params& params, SessionFactory sessionFactory = SessionFactory());
		/// Creates the provider.

	~OpenAIProvider() override;
		/// Destroys the provider.

	void setModel(const std::string& model) override;

	[[nodiscard]] std::string getModel() const override;

	[[nodiscard]] std::vector<std::string> listModels() override;
		/// Asks the server what it serves (GET {baseURI}/models, the discovery
		/// endpoint the compatible servers implement too) and returns the model
		/// ids, sorted. Models whose id contains "embed" are left out: they
		/// appear in the same list but cannot answer a chat request.
		///
		/// Throws Poco::Net::HTTPException for an HTTP error status and
		/// Poco::DataFormatException for a malformed response.

	ChatResult chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation) override;

	void appendToolResult(Poco::JSON::Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result) override;
		/// Appends a message with role "tool". Its content is the text of the
		/// result (see toolResultText()); the format has no error flag, so an
		/// error result is passed as its text.

	[[nodiscard]] std::vector<std::vector<float>> embed(const std::vector<std::string>& texts, const std::string& model, int dimensions = 0);
		/// Returns one embedding vector per input text via the embeddings
		/// endpoint (POST {baseURI}/embeddings), in the order of the texts. The
		/// model is passed explicitly because embedding models are distinct
		/// from chat models. A dimensions value above 0 is forwarded as the
		/// request parameter of the same name (only some servers honour it).
		/// All vectors have the same length.
		///
		/// Throws Poco::Net::HTTPException for an HTTP error status and
		/// Poco::DataFormatException for a malformed response.

	[[nodiscard]] static Poco::JSON::Array::Ptr toOpenAITools(const std::vector<Poco::AI::MCP::Tool>& tools);
		/// Converts tools to the Chat Completions function tool format. The
		/// schemas are copies.

private:
	Params params() const;
	Poco::JSON::Object::Ptr requestBody(const ChatRequest& request, const Params& params) const;

	mutable Poco::FastMutex _mutex;
	Params _params;
	const SessionFactory _sessionFactory;
};


} // namespace Poco::AI


#endif // AI_OpenAIProvider_INCLUDED
