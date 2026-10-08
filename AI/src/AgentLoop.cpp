//
// AgentLoop.cpp
//
// Library: AI
// Package: Agent
// Module:  AgentLoop
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/AgentLoop.h"
#include "Poco/JSON/Stringifier.h"
#include <sstream>


namespace Poco {
namespace AI {


AgentLoop::AgentLoop(
	std::shared_ptr<LLMProvider> pProvider,
	ToolRegistry& toolRegistry,
	int maxRounds):
	_pProvider(std::move(pProvider)),
	_toolRegistry(toolRegistry),
	_maxRounds(maxRounds)
{
}


void AgentLoop::setMaxToolResultSize(std::size_t size)
{
	_maxToolResultSize = size;
}


void AgentLoop::run(
	const Poco::JSON::Array& messages,
	const std::string& systemPrompt,
	AgentEventCallback onEvent)
{
	Poco::JSON::Array apiMessages(messages);
	Poco::JSON::Array tools = _toolRegistry.getToolDefinitions();

	_pProvider->resetCancelled();

	bool hadToolCalls = false;
	for (int round = 0; round < _maxRounds; ++round)
	{
		if (_pProvider->cancelled()) break;

		std::vector<ContentEvent> toolCalls;
		std::string textParts;

		try
		{
			_pProvider->chat(apiMessages, systemPrompt, tools,
				[&](const ContentEvent& evt)
				{
					if (evt.type == ContentEvent::TYPE_TEXT)
					{
						textParts += evt.text;
						AgentEvent ae;
						ae.type = AgentEvent::TYPE_TEXT;
						ae.content = evt.text;
						onEvent(ae);
					}
					else if (evt.type == ContentEvent::TYPE_TOOL_USE)
					{
						toolCalls.push_back(evt);
						AgentEvent ae;
						ae.type = AgentEvent::TYPE_TOOL_CALL;
						ae.toolName = evt.name;
						std::ostringstream oss;
						Poco::JSON::Stringifier::stringify(evt.input, oss);
						ae.toolInput = oss.str();
						onEvent(ae);
					}
					else if (evt.type == ContentEvent::TYPE_ERROR)
					{
						AgentEvent ae;
						ae.type = AgentEvent::TYPE_ERROR;
						ae.content = evt.text;
						ae.code = evt.code;
						onEvent(ae);
					}
				});
		}
		catch (const Poco::Exception& e)
		{
			AgentEvent ae;
			ae.type = AgentEvent::TYPE_ERROR;
			ae.content = "LLM error: " + e.displayText();
			onEvent(ae);
			return;
		}
		catch (const std::exception& e)
		{
			AgentEvent ae;
			ae.type = AgentEvent::TYPE_ERROR;
			ae.content = std::string("LLM error: ") + e.what();
			onEvent(ae);
			return;
		}

		hadToolCalls = !toolCalls.empty();
		if (!hadToolCalls)
			break;

		// Append assistant response
		_pProvider->appendAssistantMessage(apiMessages, textParts, toolCalls);

		// Execute tools and append results
		for (const auto& tc : toolCalls)
		{
			if (_pProvider->cancelled()) break;
			std::string result = _toolRegistry.executeTool(tc.name, tc.input);

			if (result.size() > _maxToolResultSize)
				result.resize(_maxToolResultSize);

			AgentEvent ae;
			ae.type = AgentEvent::TYPE_TOOL_RESULT;
			ae.toolName = tc.name;
			ae.content = result;
			onEvent(ae);

			_pProvider->appendToolResult(apiMessages, tc.id, result);
		}
	}

	// If the loop used up its rounds while still making tool calls,
	// one final call without tools asks the model for a text answer.
	if (!_pProvider->cancelled() && hadToolCalls)
	{
		try
		{
			Poco::JSON::Array noTools;
			_pProvider->chat(apiMessages, systemPrompt, noTools,
				[&](const ContentEvent& evt)
				{
					if (evt.type == ContentEvent::TYPE_TEXT)
					{
						AgentEvent ae;
						ae.type = AgentEvent::TYPE_TEXT;
						ae.content = evt.text;
						onEvent(ae);
					}
					else if (evt.type == ContentEvent::TYPE_ERROR)
					{
						AgentEvent ae;
						ae.type = AgentEvent::TYPE_ERROR;
						ae.content = evt.text;
						ae.code = evt.code;
						onEvent(ae);
					}
				});
		}
		catch (const Poco::Exception& e)
		{
			AgentEvent ae;
			ae.type = AgentEvent::TYPE_ERROR;
			ae.content = "LLM error: " + e.displayText();
			onEvent(ae);
		}
		catch (const std::exception& e)
		{
			AgentEvent ae;
			ae.type = AgentEvent::TYPE_ERROR;
			ae.content = std::string("LLM error: ") + e.what();
			onEvent(ae);
		}
	}

	AgentEvent ae;
	ae.type = AgentEvent::TYPE_DONE;
	onEvent(ae);
}


} } // namespace Poco::AI
