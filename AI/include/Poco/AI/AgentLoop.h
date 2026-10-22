//
// AgentLoop.h
//
// Library: AI
// Package: Agent
// Module:  AgentLoop
//
// The agent loop: call the model, execute the tools it requests, feed the
// results back, repeat.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_AgentLoop_INCLUDED
#define AI_AgentLoop_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Array.h"
#include <functional>
#include <memory>
#include <string>


namespace Poco::AI {


struct AI_API AgentEvent
	/// An event of the agent loop.
{
	enum class Type
	{
		TEXT,        /// A piece of text from the model.
		TOOL_CALL,   /// The model calls a tool.
		TOOL_RESULT, /// A tool call has its result.
		DONE,        /// The loop has finished; the last event of every run.
		FAILURE      /// A failure; content says what, code what kind.
	};

	Type type = Type::TEXT;
	std::string content;
		/// The text, the text of the tool result, or the error message.
	std::string toolName;
		/// The tool (TOOL_CALL, TOOL_RESULT).
	std::string toolInput;
		/// The tool input as JSON text (TOOL_CALL).
	bool toolError = false;
		/// Whether the tool result is an error (TOOL_RESULT).
	FailureCode code = FailureCode::NONE;
		/// The kind of failure (FAILURE).
	Poco::Timespan retryAfter;
		/// How long to wait before a retry, when the API says it (FAILURE).
	std::string requestId;
		/// The API's id of the failed request (FAILURE).
	StopReason stopReason = StopReason::UNKNOWN;
		/// Why the last call of the run stopped (DONE): END_TURN for a
		/// complete answer, MAX_TOKENS, REFUSAL, CONTENT_FILTER or PAUSED for
		/// one that is not, CANCELLED or FAILURE for a run that got none.
		/// UNKNOWN when the run made no call.
	std::string stopDetail;
		/// The provider's own stop value of that call (DONE).
	std::string explanation;
		/// Why that call stopped, where the provider says it (DONE).
	Usage usage;
		/// The tokens of all calls of the run, summed (DONE).
};


using AgentEventCallback = std::function<void (const AgentEvent& event)>;


using ToolApprover = std::function<bool (const ToolCall& call, const Poco::AI::MCP::Tool& tool)>;
	/// Decides whether a tool call of the model runs; a refusal becomes an
	/// error result the model sees. It gets the call with its input and the
	/// tool with its annotations, which for tools of a remote server are
	/// untrusted hints.


class AI_API AgentLoop
	/// The agent loop: call the model, run the tools it calls, feed the
	/// results back, repeat. The loop ends when the model answers without
	/// tool calls, when the run is cancelled, when a call fails, when an
	/// answer is cut by the token limit, or when the round limit is reached;
	/// in the last case one final call, with the same tools and tool choice
	/// none, asks the model for a text answer. A call that throws is a
	/// FAILURE event: PROVIDER_FAILED for a Poco::Net::NetException, as the
	/// provider could not be reached, NO_ANSWER for another exception (see
	/// LLMProvider::chat()). Every step is reported through
	/// the event callback, and DONE, with the stop reason of the last call and
	/// the usage of all, ends every run.
	///
	/// The tools are taken once per run, definitions and handlers, and sent
	/// unchanged in every call: some models invalidate their replayed
	/// reasoning when they change. Only those tools run, with those handlers,
	/// and only after the approver allowed them: a tool registered during the
	/// run is unknown to it.
	///
	/// Every tool call of the conversation gets its result, also one the loop
	/// does not run: when the run is cancelled, when the round limit is
	/// reached, when the call was cut by the token limit (its input may be
	/// incomplete), and when the event callback throws, in which case the
	/// exception leaves run() after the results and DONE.
	///
	/// The tools run with the request context of the loop, whose cancelled()
	/// tells once the run is cancelled: a tool that runs long asks it.
	///
	/// Tool results, the descriptions of tools from remote servers and the
	/// input the model gives a tool are untrusted input: a tool runs with
	/// arguments the model chose, maybe after reading text an attacker wrote.
	/// Use an approver to let a person, or a policy, decide which calls run.
	///
	/// A loop serves one run at a time, and is not configured while it runs.
{
public:
	static constexpr std::size_t DEFAULT_MAX_TOOL_RESULT_SIZE = 20000;
		/// The default limit, in bytes, of the text of a tool result.

	static constexpr std::size_t DEFAULT_MAX_IMAGE_SIZE = 5*1024*1024;
		/// The default limit, in bytes of base64, of one image of a tool result.

	AgentLoop(std::shared_ptr<LLMProvider> pProvider, ToolRegistry& toolRegistry, int maxRounds = 10);
		/// Creates the loop over pProvider and toolRegistry, which must outlive
		/// it. maxRounds limits the number of model calls that may run tools.
		///
		/// Throws Poco::InvalidArgumentException if pProvider is null or
		/// maxRounds is below 1.

	~AgentLoop();
		/// Destroys the loop.

	void setMaxToolResultSize(std::size_t maxSize);
		/// Sets the limit, in bytes, of the text of a tool result that is
		/// reported and passed back to the model: its text blocks, embedded
		/// text resources and structured content together. Text beyond it is
		/// cut at a character boundary, and the result says so.

	[[nodiscard]] std::size_t getMaxToolResultSize() const;
		/// Returns the limit of the text of a tool result.

	void setMaxImageSize(std::size_t maxSize);
		/// Sets the limit, in bytes of base64, of one image of a tool result.
		/// An image of a type the APIs take (PNG, JPEG, GIF, WebP) is passed
		/// back whole up to it; a larger one, one of another type, audio and
		/// binary resources are replaced by a line that says what was left out.

	[[nodiscard]] std::size_t getMaxImageSize() const;
		/// Returns the limit of one image of a tool result.

	void setApprover(ToolApprover approver);
		/// Sets the approver of tool calls; without one every call runs. An
		/// approver that throws refuses the call.

	void setRequestContext(const Poco::AI::MCP::RequestContext& context);
		/// Sets the request context the tools run with, e.g. the identity of
		/// the user the run serves.

	void run(Poco::JSON::Array& conversation, const std::string& systemPrompt, const AgentEventCallback& onEvent, Cancellation& cancellation);
		/// Runs the loop on conversation, in the provider's message format,
		/// and appends every turn of the run to it: the model's turns as the
		/// provider returned them, and the tool results. Every tool call in it
		/// has its result, also when the run is cancelled or the round limit
		/// is reached. Blocks until the loop finishes; cancellation stops it
		/// from another thread.

	void run(Poco::JSON::Array& conversation, const std::string& systemPrompt, const AgentEventCallback& onEvent);
		/// Runs the loop without a way to cancel it.

private:
	AgentLoop(const AgentLoop&) = delete;
	AgentLoop& operator = (const AgentLoop&) = delete;

	std::shared_ptr<LLMProvider> _pProvider;
	ToolRegistry& _toolRegistry;
	int _maxRounds;
	std::size_t _maxToolResultSize;
	std::size_t _maxImageSize;
	ToolApprover _approver;
	Poco::AI::MCP::RequestContext _context;
};


//
// inlines
//
inline std::size_t AgentLoop::getMaxToolResultSize() const
{
	return _maxToolResultSize;
}


inline std::size_t AgentLoop::getMaxImageSize() const
{
	return _maxImageSize;
}


} // namespace Poco::AI


#endif // AI_AgentLoop_INCLUDED
