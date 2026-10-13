//
// ToolDispatchProvider.cpp
//
// Library: AI
// Package: Providers
// Module:  ToolDispatchProvider
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/ToolDispatchProvider.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/JSON/Stringifier.h"
#include <sstream>


using namespace Poco::JSON;


namespace Poco {
namespace AI {


ToolDispatchProvider::ToolDispatchProvider(
	std::shared_ptr<LLMProvider> pChatProvider,
	ToolRegistry& toolRegistry,
	int contextBudget):
	_pChatProvider(std::move(pChatProvider)),
	_toolRegistry(toolRegistry),
	_contextBudget(contextBudget)
{
}


void ToolDispatchProvider::setModel(const std::string& model)
{
	_pChatProvider->setModel(model);
}


void ToolDispatchProvider::chat(
	const Poco::JSON::Array& messages,
	const std::string& systemPrompt,
	const Poco::JSON::Array& tools,
	ContentEventCallback onEvent)
{
	// The tools of this turn have run: the wrapped provider answers from
	// their results.
	if (hasToolResults(messages))
	{
		std::string userMsg = extractLastUserMessage(messages);
		std::string toolSummary = extractToolResults(messages);
		Poco::JSON::Array chatMessages = buildChatContext(userMsg, toolSummary);
		Poco::JSON::Array noTools;
		_pChatProvider->chat(chatMessages, systemPrompt, noTools, onEvent);
		return;
	}

	// First round of a turn: match keywords to tools
	if (tools.size() > 0)
	{
		auto matches = matchTools(extractLastUserMessage(messages));
		if (!matches.empty())
		{
			// Emit tool calls for AgentLoop to execute
			for (auto& tc : matches)
			{
				onEvent(tc);
			}
			return;
		}
	}

	// No keyword match, or no tools to match: the wrapped provider gets the
	// conversation as it is, with its native tool calling if it has any.
	// Providers that don't support tools ignore the tools array.
	_pChatProvider->chat(messages, systemPrompt, tools, onEvent);
}


void ToolDispatchProvider::appendAssistantMessage(
	Poco::JSON::Array& messages,
	const std::string& text,
	const std::vector<ContentEvent>& toolCalls)
{
	// Chat Completions message format (same as _pChatProvider)
	Object::Ptr pMsg = new Object;
	pMsg->set("role", "assistant");

	if (toolCalls.empty())
	{
		pMsg->set("content", text);
	}
	else
	{
		if (!text.empty())
			pMsg->set("content", text);
		else
			pMsg->set("content", Poco::Dynamic::Var()); // null

		Array::Ptr pToolCalls = new Array;
		for (const auto& tc : toolCalls)
		{
			Object::Ptr pToolCall = new Object;
			pToolCall->set("id", tc.id);
			pToolCall->set("type", "function");

			Object::Ptr pFunction = new Object;
			pFunction->set("name", tc.name);
			std::ostringstream oss;
			Stringifier::stringify(tc.input, oss);
			pFunction->set("arguments", oss.str());

			pToolCall->set("function", pFunction);
			pToolCalls->add(pToolCall);
		}
		pMsg->set("tool_calls", pToolCalls);
	}

	messages.add(pMsg);
}


void ToolDispatchProvider::appendToolResult(
	Poco::JSON::Array& messages,
	const std::string& toolCallId,
	const std::string& result)
{
	// Chat Completions format: role "tool" with tool_call_id
	Object::Ptr pMsg = new Object;
	pMsg->set("role", "tool");
	pMsg->set("tool_call_id", toolCallId);
	pMsg->set("content", result);
	messages.add(pMsg);
}


void ToolDispatchProvider::cancel()
{
	LLMProvider::cancel();
	_pChatProvider->cancel();
}


void ToolDispatchProvider::addRule(DispatchRule rule)
{
	_rules.push_back(std::move(rule));
}


std::vector<ContentEvent> ToolDispatchProvider::matchTools(const std::string& userMessage)
{
	std::vector<ContentEvent> matches;

	for (const auto& rule : _rules)
	{
		// Skip rules for tools not in the registry
		if (!_toolRegistry.hasTool(rule.toolName))
			continue;

		bool ruleMatched = false;
		for (const auto& group : rule.keywordGroups)
		{
			// All keywords in the group must be present (AND)
			bool groupMatched = true;
			for (const auto& keyword : group)
			{
				if (Poco::isubstr(userMessage, keyword) == std::string::npos)
				{
					groupMatched = false;
					break;
				}
			}
			if (groupMatched)
			{
				ruleMatched = true;
				break; // Any group matching is sufficient (OR)
			}
		}

		if (ruleMatched)
		{
			ContentEvent evt;
			evt.type = ContentEvent::TYPE_TOOL_USE;
			evt.id = Poco::format("dispatch-%d", _nextCallId++);
			evt.name = rule.toolName;
			if (rule.buildArgs)
				evt.input = rule.buildArgs(userMessage);
			matches.push_back(std::move(evt));
		}
	}

	return matches;
}


unsigned int ToolDispatchProvider::currentTurn(const Poco::JSON::Array& messages) const
{
	// The current turn starts after the last user message.
	for (unsigned int i = static_cast<unsigned int>(messages.size()); i > 0; --i)
	{
		auto pMsg = messages.getObject(i - 1);
		if (pMsg && pMsg->optValue<std::string>("role", "") == "user")
			return i;
	}
	return 0;
}


bool ToolDispatchProvider::hasToolResults(const Poco::JSON::Array& messages) const
{
	for (unsigned int i = currentTurn(messages); i < messages.size(); ++i)
	{
		auto pMsg = messages.getObject(i);
		if (pMsg && pMsg->optValue<std::string>("role", "") == "tool")
			return true;
	}
	return false;
}


std::string ToolDispatchProvider::extractToolResults(const Poco::JSON::Array& messages) const
{
	std::string summary;
	const std::size_t maxChars = static_cast<std::size_t>(_contextBudget) * 4;

	for (unsigned int i = currentTurn(messages); i < messages.size(); ++i)
	{
		auto pMsg = messages.getObject(i);
		if (!pMsg || pMsg->optValue<std::string>("role", "") != "tool")
			continue;

		std::string content = pMsg->optValue<std::string>("content", "");
		if (content.empty()) continue;

		if (!summary.empty())
			summary += "\n\n";
		summary += content;

		if (summary.size() > maxChars)
		{
			summary.resize(maxChars);
			summary += "\n...(truncated)";
			break;
		}
	}

	return summary;
}


std::string ToolDispatchProvider::extractLastUserMessage(const Poco::JSON::Array& messages) const
{
	for (int i = static_cast<int>(messages.size()) - 1; i >= 0; --i)
	{
		auto pMsg = messages.getObject(i);
		if (pMsg && pMsg->optValue<std::string>("role", "") == "user")
			return pMsg->optValue<std::string>("content", "");
	}
	return {};
}


Poco::JSON::Array ToolDispatchProvider::buildChatContext(
	const std::string& userMessage,
	const std::string& toolSummary) const
{
	Poco::JSON::Array result;
	Object::Ptr pMsg = new Object;
	pMsg->set("role", "user");

	if (!toolSummary.empty())
	{
		pMsg->set("content",
			"Based on the following data:\n" + toolSummary +
			"\n\nAnswer this question: " + userMessage);
	}
	else
	{
		pMsg->set("content", userMessage);
	}

	result.add(pMsg);
	return result;
}


} } // namespace Poco::AI
