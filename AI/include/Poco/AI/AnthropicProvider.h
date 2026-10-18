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
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API AnthropicProvider: public LLMProvider
	/// A provider for the Anthropic Messages API: the response streams in as
	/// server-sent events; tool use, thinking blocks and prompt caching
	/// markers are supported.
	///
	/// The assistant turn a call returns carries the thinking and redacted
	/// thinking blocks the model produced, with their signatures and in their
	/// order, as the API requires them back in a tool-use turn. Keep the tools
	/// unchanged across the calls of one turn: a changed tool list makes the
	/// replayed thinking invalid.
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
		int maxTokens = 4096;
			/// The most output tokens a call asks for.
		Poco::Timespan timeout;
			/// How long a read may wait for the server; zero leaves the
			/// session's own timeout.
		std::string apiVersion = DEFAULT_API_VERSION;
		std::size_t maxResponseSize = 32*1024*1024;
			/// The limit of a response body, and of one event of a stream.
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
