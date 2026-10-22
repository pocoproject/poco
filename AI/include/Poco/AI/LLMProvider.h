//
// LLMProvider.h
//
// Library: AI
// Package: Providers
// Module:  LLMProvider
//
// The provider interface: one call of a large language model API with
// streamed content events, the turn it returns in the provider's own
// message format, and cancellation through a per-call token.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_LLMProvider_INCLUDED
#define AI_LLMProvider_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Timespan.h"
#include <functional>
#include <string>
#include <vector>


namespace Poco::AI {


enum class FailureCode
	/// What a failure event reports. Named apart from the JSON-RPC
	/// Poco::AI::MCP::ErrorCode, as code with both namespaces in use must
	/// tell them apart.
{
	NONE,
	UNAUTHENTICATED,      /// HTTP 401 or 403: the key is missing, wrong or lacks the permission.
	RATE_LIMITED,         /// HTTP 429 for too many requests: worth a retry after retryAfter.
	QUOTA_EXCEEDED,       /// HTTP 402, or a 429 for a used-up quota or a reached spend cap: no retry helps until it is raised or renewed.
	OVERLOADED,           /// HTTP 503 or 529, or an overloaded error in the stream; worth a retry later.
	SERVER_ERROR,         /// Another HTTP 5xx.
	REQUEST_REJECTED,     /// Another HTTP 4xx: the request itself is wrong, or a spend limit the organization set itself is reached, which Anthropic answers with a 400; the text says which.
	MALFORMED_TOOL_INPUT, /// A tool call whose input is not a JSON object; the tool is not called.
	STREAM_INTERRUPTED,   /// The response stream ended before its end; what arrived is incomplete.
	STREAM_ERROR,         /// The API reported an error inside the response stream.
	PROVIDER_FAILED,      /// The provider could not be reached: connecting, the TLS handshake or sending the request failed, or the connection broke before an answer.
	NO_ANSWER,            /// The request went out, but the call failed before an answer came, e.g. the server did not answer in time.
	ROUND_LIMIT           /// The model still called tools when the agent loop had no rounds left.
};


enum class StopReason
	/// Why the model stopped.
{
	UNKNOWN,        /// The provider did not say, or said something else.
	END_TURN,       /// The model finished its answer.
	TOOL_USE,       /// The model wants tool results.
	MAX_TOKENS,     /// The answer reached the token limit and is cut.
	STOP_SEQUENCE,  /// The answer reached a stop sequence.
	REFUSAL,        /// The model declined to answer.
	CONTENT_FILTER, /// A content filter cut the answer.
	PAUSED,         /// The model paused a long turn; send the turn back to continue.
	CANCELLED,      /// The call was cancelled.
	FAILURE         /// The call failed; a FAILURE event says why.
};


enum class ToolChoice
	/// Whether the model may call tools.
{
	AUTO, /// The model decides.
	NONE  /// The model must answer in text; the tools are still sent, unchanged.
};


struct AI_API ToolCall
	/// A tool call the model requests.
{
	std::string id;
	std::string name;
	Poco::JSON::Object::Ptr input;
		/// The input; never null, empty when the model gave none.
};


struct AI_API Usage
	/// The tokens a call used, as far as the provider reports them.
{
	Poco::UInt64 inputTokens = 0;
	Poco::UInt64 outputTokens = 0;
	Poco::UInt64 cachedInputTokens = 0;
		/// The input tokens read from the provider's prompt cache.
};


struct AI_API ChatRequest
	/// One call of a model.
{
	Poco::JSON::Array messages;
		/// The conversation in the provider's message format.
	std::string systemPrompt;
		/// The system prompt; not sent when empty.
	std::vector<Poco::AI::MCP::Tool> tools;
		/// The tools the model may call; only name, description and input
		/// schema are sent.
	ToolChoice toolChoice = ToolChoice::AUTO;
};


struct AI_API ChatResult
	/// What one call of a model returned.
{
	StopReason stopReason = StopReason::UNKNOWN;
	std::string stopDetail;
		/// The provider's own stop value, e.g. "end_turn" or "length".
	Usage usage;
	std::string text;
		/// The text of the answer.
	std::vector<ToolCall> toolCalls;
		/// The tool calls the model requests.
	std::string explanation;
		/// Why the model stopped, where the provider says it, e.g. the
		/// explanation of a refusal.
	Poco::JSON::Object::Ptr assistantMessage;
		/// The model's turn in the provider's message format, with every block
		/// the model returned, as it returned them: reasoning blocks and their
		/// signatures included, which some models require back. Append it to
		/// the conversation as it is, and send every turn of the conversation
		/// back unchanged, with the same system prompt and tools: a change
		/// invalidates replayed reasoning. Null when the call failed, was
		/// cancelled or refused, or returned nothing; a refused answer is not
		/// to be kept, so the text that streamed in before is to be dropped.
};


struct AI_API ContentEvent
	/// One event of a model call, delivered as the response streams in.
{
	enum class Type
	{
		TEXT,     /// A piece of text.
		TOOL_USE, /// A complete tool call.
		FAILURE   /// A failure; text says what, code what kind.
	};

	Type type = Type::TEXT;
	std::string text;
	ToolCall toolCall;
		/// The tool call (TOOL_USE).
	FailureCode code = FailureCode::NONE;
		/// The kind of failure (FAILURE).
	Poco::Timespan retryAfter;
		/// How long to wait before a retry, when the API says it (FAILURE).
	std::string requestId;
		/// The API's id of the request, for its support (FAILURE).
};


using ContentEventCallback = std::function<void (const ContentEvent& event)>;


class AI_API LLMProvider
	/// The interface of a large language model API.
	///
	/// A provider makes one API call per chat(), delivers what the model
	/// returns as ContentEvents while it streams in, and returns the model's
	/// turn in the provider's own message format; appendToolResult() adds a
	/// tool result in that format. An AgentLoop thus drives any provider
	/// without knowing the format.
	///
	/// A provider may serve several calls at once; each call has its own
	/// Cancellation.
	///
	/// What a model returns is untrusted input: its text, its tool calls and
	/// their input.
{
public:
	virtual ~LLMProvider();
		/// Destroys the provider.

	virtual void setModel(const std::string& model) = 0;
		/// Sets the model of the calls that start after it returns.

	[[nodiscard]] virtual std::string getModel() const = 0;
		/// Returns the model.

	[[nodiscard]] virtual std::vector<std::string> listModels();
		/// Returns the models the provider serves.
		///
		/// Throws Poco::NotImplementedException when the provider has no model
		/// listing, and the provider's exception when the listing fails.

	virtual ChatResult chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation) = 0;
		/// Makes one call of the model and delivers its content as events.
		///
		/// An error the API reports, before or within the response stream,
		/// arrives as a FAILURE event, and the result has stop reason FAILURE; so
		/// does a response stream that ends before its end. A cancelled call
		/// returns quietly with stop reason CANCELLED.
		///
		/// Throws when the call fails before an answer arrives: a
		/// Poco::Net::NetException when the endpoint cannot be reached or the
		/// connection breaks, another exception otherwise, e.g. a
		/// Poco::TimeoutException when the server does not answer in time.

	virtual void appendToolResult(Poco::JSON::Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result) = 0;
		/// Appends a tool result to the conversation in the provider's format.

protected:
	LLMProvider();
		/// Creates the provider.

private:
	LLMProvider(const LLMProvider&) = delete;
	LLMProvider& operator = (const LLMProvider&) = delete;
};


} // namespace Poco::AI


#endif // AI_LLMProvider_INCLUDED
