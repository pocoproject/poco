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
#include "ProviderCall.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include <exception>
#include <utility>


namespace Poco::AI {


namespace
{
	void emitError(const AgentEventCallback& onEvent, ErrorCode code, const std::string& text)
	{
		AgentEvent event;
		event.type = AgentEvent::Type::FAILURE;
		event.code = code;
		event.content = text;
		if (onEvent) onEvent(event);
	}


	const Poco::AI::MCP::Tool* findTool(const std::vector<Poco::AI::MCP::Tool>& tools, const std::string& name)
	{
		for (const Poco::AI::MCP::Tool& tool: tools)
		{
			if (tool.name == name) return &tool;
		}
		return nullptr;
	}
}


AgentLoop::AgentLoop(std::shared_ptr<LLMProvider> pProvider, ToolRegistry& toolRegistry, int maxRounds):
	_pProvider(std::move(pProvider)),
	_toolRegistry(toolRegistry),
	_maxRounds(maxRounds),
	_maxToolResultSize(DEFAULT_MAX_TOOL_RESULT_SIZE)
{
	if (_pProvider == nullptr)
	{
		throw Poco::InvalidArgumentException("An agent loop needs a provider");
	}
	if (_maxRounds < 1)
	{
		throw Poco::InvalidArgumentException("An agent loop needs at least one round");
	}
}


AgentLoop::~AgentLoop()
{
}


void AgentLoop::setMaxToolResultSize(std::size_t maxSize)
{
	_maxToolResultSize = maxSize;
}


void AgentLoop::setApprover(ToolApprover approver)
{
	_approver = std::move(approver);
}


void AgentLoop::setRequestContext(const Poco::AI::MCP::RequestContext& context)
{
	_context = context;
}


void AgentLoop::run(Poco::JSON::Array& conversation, const std::string& systemPrompt, const AgentEventCallback& onEvent)
{
	Cancellation cancellation;
	run(conversation, systemPrompt, onEvent, cancellation);
}


void AgentLoop::run(Poco::JSON::Array& conversation, const std::string& systemPrompt, const AgentEventCallback& onEvent, Cancellation& cancellation)
{
	ChatRequest request;
	request.systemPrompt = systemPrompt;
	request.tools = _toolRegistry.tools();

	const auto onContent = [&onEvent](const ContentEvent& contentEvent)
	{
		if (contentEvent.type == ContentEvent::Type::TEXT)
		{
			AgentEvent event;
			event.content = contentEvent.text;
			if (onEvent) onEvent(event);
		}
		else if (contentEvent.type == ContentEvent::Type::FAILURE)
		{
			emitError(onEvent, contentEvent.code, contentEvent.text);
		}
		// A tool call is reported when the loop gets to it.
	};

	for (int round = 0; round <= _maxRounds && !cancellation.cancelled(); ++round)
	{
		// Rounds 0 to maxRounds - 1 may run tools; one more call asks for text.
		const bool finalRound = round == _maxRounds;
		request.messages = conversation;
		request.toolChoice = finalRound ? ToolChoice::NONE : ToolChoice::AUTO;

		ChatResult result;
		try
		{
			result = _pProvider->chat(request, onContent, cancellation);
		}
		catch (const Poco::Exception& exc)
		{
			emitError(onEvent, ErrorCode::PROVIDER_FAILED, "LLM error: " + exc.displayText());
			break;
		}
		catch (const std::exception& exc)
		{
			emitError(onEvent, ErrorCode::PROVIDER_FAILED, std::string("LLM error: ") + exc.what());
			break;
		}
		if (result.stopReason == StopReason::CANCELLED || result.stopReason == StopReason::FAILURE) break;
		if (result.assistantMessage != nullptr)
		{
			conversation.add(result.assistantMessage);
		}
		if (result.toolCalls.empty()) break;

		if (finalRound)
		{
			emitError(onEvent, ErrorCode::ROUND_LIMIT, "The model still called tools after " + Poco::NumberFormatter::format(_maxRounds) + " rounds");
		}
		for (const ToolCall& call: result.toolCalls)
		{
			Poco::AI::MCP::ToolResult toolResult;
			if (finalRound)
			{
				toolResult = Poco::AI::MCP::ToolResult::error("The tool was not run: the round limit was reached.");
			}
			else if (cancellation.cancelled())
			{
				toolResult = Poco::AI::MCP::ToolResult::error("The tool was not run: the run was cancelled.");
			}
			else
			{
				AgentEvent callEvent;
				callEvent.type = AgentEvent::Type::TOOL_CALL;
				callEvent.toolName = call.name;
				callEvent.toolInput = serializeJSON(call.input);
				if (onEvent) onEvent(callEvent);

				const Poco::AI::MCP::Tool* pTool = findTool(request.tools, call.name);
				if (_approver && pTool != nullptr && !_approver(call, *pTool))
				{
					toolResult = Poco::AI::MCP::ToolResult::error("The tool call was refused.");
				}
				else
				{
					toolResult = _toolRegistry.executeTool(call.name, call.input, _context);
				}
			}

			std::string text = toolResultText(toolResult);
			if (text.size() > _maxToolResultSize)
			{
				// Cut at a character boundary: a partial UTF-8 sequence would make
				// the next request body invalid.
				cutAtCharacter(text, _maxToolResultSize);
				text += "\n...(truncated)";
				const bool isError = toolResult.isError();
				toolResult = Poco::AI::MCP::ToolResult();
				toolResult.addText(text).setError(isError);
			}

			AgentEvent resultEvent;
			resultEvent.type = AgentEvent::Type::TOOL_RESULT;
			resultEvent.toolName = call.name;
			resultEvent.content = text;
			resultEvent.toolError = toolResult.isError();
			if (onEvent) onEvent(resultEvent);

			_pProvider->appendToolResult(conversation, call.id, toolResult);
		}
		if (finalRound) break;
	}

	AgentEvent done;
	done.type = AgentEvent::Type::DONE;
	if (onEvent) onEvent(done);
}


} // namespace Poco::AI
