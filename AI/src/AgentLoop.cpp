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
	void emitFailure(const AgentEventCallback& onEvent, FailureCode code, const std::string& text, const Poco::Timespan& retryAfter = Poco::Timespan(), const std::string& requestId = std::string())
	{
		AgentEvent event;
		event.type = AgentEvent::Type::FAILURE;
		event.code = code;
		event.content = text;
		event.retryAfter = retryAfter;
		event.requestId = requestId;
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


	bool takenImage(const std::string& mimeType)
		/// The image types the model APIs take in a tool result.
	{
		return mimeType == "image/png" || mimeType == "image/jpeg" || mimeType == "image/gif" || mimeType == "image/webp";
	}


	std::string leftOut(const std::string& what, const std::string& mimeType, std::size_t size)
	{
		return "[" + what + (mimeType.empty() ? std::string() : " " + mimeType) + " of " + std::to_string(size) + " bytes left out]";
	}


	Poco::AI::MCP::ToolResult limited(const Poco::AI::MCP::ToolResult& result, std::size_t maxText, std::size_t maxImage)
		/// The result as the model gets it: its text (text blocks, embedded
		/// text resources, structured content) cut to maxText bytes in all, at
		/// a character boundary, and marked as cut; images of the types the
		/// APIs take kept whole up to maxImage bytes each; other images, audio
		/// and binary resources replaced by a line that says what was left out.
	{
		Poco::AI::MCP::ToolResult out;
		out.setError(result.isError());
		std::size_t left = maxText;
		bool cut = false;
		bool hasText = false;
		const auto addText = [&](std::string text)
		{
			if (text.size() > left)
			{
				cutAtCharacter(text, left);
				cut = true;
			}
			left -= text.size();
			if (!text.empty())
			{
				out.addText(text);
				hasText = true;
			}
		};
		const Poco::JSON::Array::Ptr pContent = result.content();
		for (std::size_t i = 0; pContent != nullptr && i < pContent->size(); ++i)
		{
			const Poco::JSON::Object::Ptr pBlock = pContent->getObject(static_cast<unsigned>(i));
			const std::string type = stringOf(pBlock, "type");
			if (type == "text")
			{
				addText(stringOf(pBlock, "text"));
			}
			else if (type == "image")
			{
				const std::string mimeType = stringOf(pBlock, "mimeType");
				const std::size_t size = stringOf(pBlock, "data").size();
				if (takenImage(mimeType) && size <= maxImage) out.addContent(pBlock);
				else addText(leftOut("image", mimeType, size));
			}
			else if (type == "audio")
			{
				addText(leftOut("audio", stringOf(pBlock, "mimeType"), stringOf(pBlock, "data").size()));
			}
			else if (type == "resource")
			{
				const Poco::JSON::Object::Ptr pResource = pBlock->getObject("resource");
				if (!pResource.isNull() && pResource->has("text")) addText(stringOf(pResource, "text"));
				else addText(leftOut("resource " + stringOf(pResource, "uri"), stringOf(pResource, "mimeType"), stringOf(pResource, "blob").size()));
			}
			else if (type == "resource_link")
			{
				out.addContent(pBlock);
			}
			else if (!pBlock.isNull())
			{
				addText(serializeJSON(pBlock));
			}
		}
		if (!result.structuredContent().isEmpty())
		{
			const std::string text = serializeJSON(result.structuredContent());
			if (text.size() <= left)
			{
				left -= text.size();
				out.setStructuredContent(result.structuredContent());
			}
			else if (!hasText)
			{
				// What the model reads without text blocks, as text.
				addText(text);
			}
			else cut = true;
		}
		if (cut) out.addText("...(truncated)");
		return out;
	}
}


AgentLoop::AgentLoop(std::shared_ptr<LLMProvider> pProvider, ToolRegistry& toolRegistry, int maxRounds):
	_pProvider(std::move(pProvider)),
	_toolRegistry(toolRegistry),
	_maxRounds(maxRounds),
	_maxToolResultSize(DEFAULT_MAX_TOOL_RESULT_SIZE),
	_maxImageSize(DEFAULT_MAX_IMAGE_SIZE)
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


void AgentLoop::setMaxImageSize(std::size_t maxSize)
{
	_maxImageSize = maxSize;
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
	// Definitions and handlers: only these run in this run.
	request.tools = _toolRegistry.tools();
	Poco::AI::MCP::RequestContext context(_context);
	context.cancelled = [&cancellation]() { return cancellation.cancelled(); };

	AgentEvent done;
	done.type = AgentEvent::Type::DONE;

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
			emitFailure(onEvent, contentEvent.code, contentEvent.text, contentEvent.retryAfter, contentEvent.requestId);
		}
		// A tool call is reported when the loop gets to it.
	};

	try
	{
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
				done.stopReason = StopReason::FAILURE;
				emitFailure(onEvent, FailureCode::PROVIDER_FAILED, "LLM error: " + exc.displayText());
				break;
			}
			catch (const std::exception& exc)
			{
				done.stopReason = StopReason::FAILURE;
				emitFailure(onEvent, FailureCode::PROVIDER_FAILED, std::string("LLM error: ") + exc.what());
				break;
			}
			done.stopReason = result.stopReason;
			done.stopDetail = result.stopDetail;
			done.explanation = result.explanation;
			done.usage.inputTokens += result.usage.inputTokens;
			done.usage.outputTokens += result.usage.outputTokens;
			done.usage.cachedInputTokens += result.usage.cachedInputTokens;
			if (result.stopReason == StopReason::CANCELLED || result.stopReason == StopReason::FAILURE) break;
			if (result.assistantMessage != nullptr)
			{
				conversation.add(result.assistantMessage);
			}
			if (result.toolCalls.empty()) break;

			// A call cut by the token limit may have lost part of its input:
			// no tool call of that turn runs, and the run ends.
			const bool cut = result.stopReason == StopReason::MAX_TOKENS;
			if (finalRound)
			{
				emitFailure(onEvent, FailureCode::ROUND_LIMIT, "The model still called tools after " + Poco::NumberFormatter::format(_maxRounds) + " rounds");
			}
			std::size_t answered = 0;
			try
			{
				for (const ToolCall& call: result.toolCalls)
				{
					AgentEvent callEvent;
					callEvent.type = AgentEvent::Type::TOOL_CALL;
					callEvent.toolName = call.name;
					callEvent.toolInput = serializeJSON(call.input);
					if (onEvent) onEvent(callEvent);

					Poco::AI::MCP::ToolResult toolResult;
					const Poco::AI::MCP::Tool* pTool = findTool(request.tools, call.name);
					if (finalRound)
					{
						toolResult = Poco::AI::MCP::ToolResult::error("The tool was not run: the round limit was reached.");
					}
					else if (cut)
					{
						toolResult = Poco::AI::MCP::ToolResult::error("The tool was not run: the answer was cut off by the token limit, and its input may be incomplete.");
					}
					else if (cancellation.cancelled())
					{
						toolResult = Poco::AI::MCP::ToolResult::error("The tool was not run: the run was cancelled.");
					}
					else if (pTool == nullptr)
					{
						toolResult = Poco::AI::MCP::ToolResult::error("Unknown tool: " + call.name);
					}
					else
					{
						bool approved = true;
						if (_approver)
						{
							try
							{
								approved = _approver(call, *pTool);
							}
							catch (...)
							{
								approved = false;
							}
						}
						toolResult = approved
							? ToolRegistry::execute(*pTool, call.input, context)
							: Poco::AI::MCP::ToolResult::error("The tool call was refused.");
					}
					toolResult = limited(toolResult, _maxToolResultSize, _maxImageSize);

					_pProvider->appendToolResult(conversation, call.id, toolResult);
					++answered;

					AgentEvent resultEvent;
					resultEvent.type = AgentEvent::Type::TOOL_RESULT;
					resultEvent.toolName = call.name;
					resultEvent.content = toolResultText(toolResult);
					resultEvent.toolError = toolResult.isError();
					if (onEvent) onEvent(resultEvent);
				}
			}
			catch (...)
			{
				// Every tool call keeps its result, so that the conversation
				// stays one the API takes.
				for (std::size_t i = answered; i < result.toolCalls.size(); ++i)
				{
					_pProvider->appendToolResult(conversation, result.toolCalls[i].id, Poco::AI::MCP::ToolResult::error("The tool was not run: the run ended with an error."));
				}
				throw;
			}
			if (finalRound || cut) break;
		}
		// A run cancelled before its first call, or between the tools and the
		// next call, got no answer.
		if (cancellation.cancelled() && (done.stopReason == StopReason::UNKNOWN || done.stopReason == StopReason::TOOL_USE))
		{
			done.stopReason = StopReason::CANCELLED;
		}
	}
	catch (...)
	{
		try
		{
			if (onEvent) onEvent(done);
		}
		catch (...)
		{
		}
		throw;
	}
	if (onEvent) onEvent(done);
}


} // namespace Poco::AI
