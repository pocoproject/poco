//
// LLMProvider.h
//
// Library: AI
// Package: Providers
// Module:  LLMProvider
//
// The provider interface: one chat call of a large language model API with
// streamed content events, the helpers that keep a message history in the
// provider's own format, and cancellation.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_LLMProvider_INCLUDED
#define AI_LLMProvider_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <string>
#include <functional>
#include <vector>
#include <atomic>


namespace Poco {
namespace AI {


struct AI_API ContentEvent
	/// One event of a single model API call.
{
	enum Type
	{
		TYPE_TEXT,     /// A piece of text
		TYPE_TOOL_USE, /// A tool call the model requests
		TYPE_ERROR     /// An error reported by the provider
	};

	Type type;
	std::string text;         /// The text (TYPE_TEXT) or the error message (TYPE_ERROR)
	std::string id;           /// The tool call id (TYPE_TOOL_USE)
	std::string name;         /// The tool name (TYPE_TOOL_USE)
	Poco::JSON::Object input; /// The tool input (TYPE_TOOL_USE)
	std::string code;         /// A machine-readable error code (TYPE_ERROR), e.g. "unauthenticated"
};


using ContentEventCallback = std::function<void(const ContentEvent&)>;


class AI_API LLMProvider
	/// Abstract base class for large language model API providers.
	///
	/// A provider makes one API call per chat() and delivers what the model
	/// returns as ContentEvents. The message history passed to chat() is in
	/// the provider's own wire format; appendAssistantMessage() and
	/// appendToolResult() extend it in that format, so an AgentLoop drives
	/// any provider without knowing the format.
{
public:
	virtual ~LLMProvider() = default;

	virtual void setModel(const std::string& model) = 0;
		/// Changes the model used for subsequent API calls.

	virtual std::vector<std::string> listModels()
		/// Returns the models the provider currently serves, so that an
		/// application need not keep a hand-written list in step with the
		/// server. Returns an empty vector when the provider cannot be
		/// reached or offers no discovery; the caller decides what to fall
		/// back to. Implementations must not throw.
	{
		return {};
	}

	virtual void chat(
		const Poco::JSON::Array& messages,
		const std::string& systemPrompt,
		const Poco::JSON::Array& tools,
		ContentEventCallback onEvent) = 0;
		/// Makes a single API call and delivers the content events through
		/// onEvent. Transport failures propagate as exceptions; errors the
		/// API reports arrive as a TYPE_ERROR event.

	virtual void appendAssistantMessage(
		Poco::JSON::Array& messages,
		const std::string& text,
		const std::vector<ContentEvent>& toolCalls) = 0;
		/// Appends the assistant response to the message array
		/// in the provider's format.

	virtual void appendToolResult(
		Poco::JSON::Array& messages,
		const std::string& toolCallId,
		const std::string& result) = 0;
		/// Appends a tool result to the message array
		/// in the provider's format.

	virtual void cancel()
		/// Cancels the in-flight API call. Thread-safe.
	{
		_cancelled = true;
	}

	bool cancelled() const
		/// Returns true if cancel() has been called.
	{
		return _cancelled.load();
	}

	void resetCancelled()
		/// Resets the cancelled flag. Call before starting
		/// a new request.
	{
		_cancelled = false;
	}

private:
	std::atomic<bool> _cancelled{false};
};


} } // namespace Poco::AI


#endif // AI_LLMProvider_INCLUDED
