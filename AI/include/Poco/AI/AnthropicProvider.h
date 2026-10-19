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
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API AnthropicProvider: public LLMProvider
	/// A provider for the Anthropic Messages API: the response streams in as
	/// server-sent events; tool use, thinking blocks and prompt caching
	/// markers are supported.
	///
	/// The assistant turn a call returns carries every block the model
	/// produced, in its order and with its members as they came: thinking and
	/// redacted thinking with their signatures, citations, and blocks of types
	/// this provider does not know. The API requires the turns back as they
	/// came. Send the whole conversation back unchanged, with the same system
	/// prompt and tools: a change makes replayed thinking invalid. Left out
	/// is only what the API would refuse back: a thinking block without its
	/// signature, and a text block with nothing but whitespace.
	///
	/// Requests carry cache markers on the system prompt, on the last tool
	/// and on the last block of the last message, so that each call of an
	/// agent loop reads the conversation so far from the prompt cache. The
	/// API takes four markers at most; messages and request members that
	/// bring their own count towards that.
	///
	/// Sessions come from the session factory, createClientSession() by
	/// default; an https base URI needs Poco::Net::HTTPSSessionInstantiator
	/// registered by the application.
	///
	/// This library is not affiliated with or endorsed by Anthropic; the name
	/// identifies the API it speaks.
{
public:
	static constexpr char DEFAULT_API_VERSION[] = "2023-06-01";
		/// The value of the anthropic-version header sent by default.

	struct Params
		/// The settings of the provider.
	{
		Poco::URI baseURI{"https://api.anthropic.com/v1"};
			/// The API base, with its version path.
		std::string apiKey;
		std::string model;
		int maxTokens = 16000;
			/// The most output tokens a call asks for. Thinking counts towards
			/// it.
		Poco::Timespan timeout;
			/// How long connecting, sending and each read may take; zero
			/// leaves the session's own timeouts.
		std::string apiVersion = DEFAULT_API_VERSION;
		std::size_t maxResponseSize = 32*1024*1024;
			/// The limit of a response body, and of one event of a stream.
		Poco::JSON::Object::Ptr requestMembers;
			/// Members set in every request body, replacing those of the same
			/// name, e.g. {"thinking": {"type": "adaptive"}}.
		Poco::Net::NameValueCollection headers;
			/// Headers sent with every request, e.g. anthropic-beta.
	};

	explicit AnthropicProvider(const Params& params, SessionFactory sessionFactory = SessionFactory());
		/// Creates the provider.

	~AnthropicProvider() override;
		/// Destroys the provider.

	void setModel(const std::string& model) override;

	[[nodiscard]] std::string getModel() const override;

	[[nodiscard]] std::vector<std::string> listModels() override;
		/// Returns the ids of the models the API serves (GET {baseURI}/models).
		///
		/// Throws Poco::Net::HTTPException for an HTTP error status and
		/// Poco::DataFormatException for a malformed response.

	ChatResult chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation) override;

	void appendToolResult(Poco::JSON::Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result) override;
		/// Appends a tool_result block, with is_error set for an error result,
		/// to the user message of tool results that ends the conversation, or
		/// in a new one. Text and image blocks are passed on; other blocks and
		/// structured content as JSON text.

	[[nodiscard]] static Poco::JSON::Array::Ptr toAnthropicTools(const std::vector<Poco::AI::MCP::Tool>& tools);
		/// Converts tools to the Messages API tool format, with the cache
		/// marker on the last one. The schemas are copies.

private:
	Params params() const;
	Poco::JSON::Object::Ptr requestBody(const ChatRequest& request, const Params& params) const;

	mutable Poco::FastMutex _mutex;
	Params _params;
	const SessionFactory _sessionFactory;
};


} // namespace Poco::AI


#endif // AI_AnthropicProvider_INCLUDED
