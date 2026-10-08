//
// ToolDispatchProvider.h
//
// Library: AI
// Package: Providers
// Module:  ToolDispatchProvider
//
// Keyword-rule tool dispatch for models without native tool calling.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ToolDispatchProvider_INCLUDED
#define AI_ToolDispatchProvider_INCLUDED


#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>


namespace Poco {
namespace AI {


class AI_API ToolDispatchProvider: public LLMProvider
	/// Wraps a provider and adds rule-based tool dispatch, for models that
	/// cannot call tools themselves. Keyword rules match the user's message
	/// to tools; a matched tool is reported as a tool call, the AgentLoop
	/// executes it, and the results are given to the wrapped provider as
	/// context for the answer. Only the tool results of the current turn,
	/// those after the last user message, count: results of earlier turns
	/// in the history neither stop the rules from matching nor reach the
	/// answer.
	///
	/// When no rule matches, or when there are no tools at all, the request
	/// goes to the wrapped provider with the conversation as it is, and
	/// with its native tool calling if it has any, so rules handle the
	/// well-known questions and the model handles the rest.
	///
	/// The rules come from the application through addRule(); the provider
	/// ships none. The wrapped provider must use the Chat Completions
	/// message format (OpenAIProvider), which this class writes.
{
public:
	struct DispatchRule
	{
		std::vector<std::vector<std::string>> keywordGroups;
			/// Outer vector: OR groups. Inner vector: AND keywords.
			/// A rule matches if ANY group matches, where a group
			/// matches if ALL its keywords appear in the message
			/// (case-insensitive).

		std::string toolName;
			/// The tool to call; rules for tools that are not registered
			/// are skipped.

		std::function<Poco::JSON::Object(const std::string& message)> buildArgs;
			/// Builds the tool input from the user's message.
			/// If null, an empty JSON object is used.
	};

	ToolDispatchProvider(
		std::shared_ptr<LLMProvider> pChatProvider,
		ToolRegistry& toolRegistry,
		int contextBudget = 1500);
		/// Wraps pChatProvider. contextBudget, in tokens (four characters
		/// each), limits the tool results handed to the model as context.

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

	void cancel() override;

	void addRule(DispatchRule rule);
		/// Adds a dispatch rule.

private:
	std::vector<ContentEvent> matchTools(const std::string& userMessage);
	unsigned int currentTurn(const Poco::JSON::Array& messages) const;
	bool hasToolResults(const Poco::JSON::Array& messages) const;
	std::string extractToolResults(const Poco::JSON::Array& messages) const;
	std::string extractLastUserMessage(const Poco::JSON::Array& messages) const;
	Poco::JSON::Array buildChatContext(
		const std::string& userMessage,
		const std::string& toolSummary) const;

	std::shared_ptr<LLMProvider> _pChatProvider;
	ToolRegistry& _toolRegistry;
	std::vector<DispatchRule> _rules;
	int _contextBudget;
	int _nextCallId = 0;
};


} } // namespace Poco::AI


#endif // AI_ToolDispatchProvider_INCLUDED
