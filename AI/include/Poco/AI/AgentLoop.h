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


#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include <cstddef>
#include <memory>
#include <string>
#include <functional>


namespace Poco {
namespace AI {


struct AI_API AgentEvent
	/// An event emitted by the agent loop.
{
	enum Type
	{
		TYPE_TEXT,        /// A piece of text from the model
		TYPE_TOOL_CALL,   /// A tool is about to be executed
		TYPE_TOOL_RESULT, /// A tool has returned
		TYPE_DONE,        /// The loop has finished
		TYPE_ERROR        /// An error occurred
	};

	Type type;
	std::string content;   /// The text, the tool result or the error message
	std::string toolName;  /// The tool (TYPE_TOOL_CALL, TYPE_TOOL_RESULT)
	std::string toolInput; /// The tool input as a JSON string (TYPE_TOOL_CALL)
	std::string code;      /// A machine-readable error code (TYPE_ERROR), e.g. "unauthenticated"
};


using AgentEventCallback = std::function<void(const AgentEvent&)>;


class AI_API AgentLoop
	/// The agent loop: call the model, execute the tools it requests, feed
	/// the results back, repeat. The loop ends when the model answers without
	/// tool calls, when the provider is cancelled, or when the round limit is
	/// reached; in the last case a final call without tools asks the model
	/// for a text answer. Every step is reported through the event callback.
{
public:
	static constexpr std::size_t DEFAULT_MAX_TOOL_RESULT_SIZE = 20000;
		/// The default number of characters of a tool result passed on.

	AgentLoop(
		std::shared_ptr<LLMProvider> pProvider,
		ToolRegistry& toolRegistry,
		int maxRounds = 10);
		/// Creates the loop over pProvider and toolRegistry, which must
		/// outlive it. maxRounds limits the number of model calls that may
		/// request tools.

	~AgentLoop() = default;

	void setMaxToolResultSize(std::size_t size);
		/// Sets the number of characters of a tool result that are reported
		/// and passed back to the model; longer results are cut.

	std::size_t maxToolResultSize() const;
		/// Returns the number of characters of a tool result passed on.

	void run(
		const Poco::JSON::Array& messages,
		const std::string& systemPrompt,
		AgentEventCallback onEvent);
		/// Runs the loop over a copy of messages and calls onEvent for each
		/// event. Blocks until the loop finishes.

private:
	std::shared_ptr<LLMProvider> _pProvider;
	ToolRegistry& _toolRegistry;
	int _maxRounds;
	std::size_t _maxToolResultSize = DEFAULT_MAX_TOOL_RESULT_SIZE;
};


//
// inlines
//
inline std::size_t AgentLoop::maxToolResultSize() const
{
	return _maxToolResultSize;
}


} } // namespace Poco::AI


#endif // AI_AgentLoop_INCLUDED
