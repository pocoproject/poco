//
// AnthropicProvider.cpp
//
// Library: AI
// Package: Providers
// Module:  AnthropicProvider
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/AnthropicProvider.h"
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "ProviderCall.h"
#include "ToolInput.h"
#include "Poco/Net/NetException.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <map>
#include <utility>


using Poco::JSON::Object;
using Poco::JSON::Array;


namespace Poco::AI {


namespace
{
	struct Block
		/// One content block of a streamed response, assembled from its events.
	{
		std::string type;
		std::string text;
		std::string id;
		std::string name;
		std::string arguments;
		std::string signature;
		std::string data;
		bool dropped = false;
	};


	StopReason stopReasonOf(const std::string& value)
	{
		if (value == "end_turn") return StopReason::END_TURN;
		if (value == "tool_use") return StopReason::TOOL_USE;
		if (value == "max_tokens") return StopReason::MAX_TOKENS;
		if (value == "stop_sequence") return StopReason::STOP_SEQUENCE;
		if (value == "refusal") return StopReason::REFUSAL;
		if (value == "pause_turn") return StopReason::PAUSED;
		return StopReason::UNKNOWN;
	}


	void report(const ContentEventCallback& onEvent, ErrorCode code, const std::string& text)
	{
		ContentEvent event;
		event.type = ContentEvent::Type::ERROR;
		event.code = code;
		event.text = text;
		if (onEvent) onEvent(event);
	}


	ChatResult failed()
	{
		ChatResult result;
		result.stopReason = StopReason::ERROR;
		return result;
	}


	ChatResult cancelled()
	{
		ChatResult result;
		result.stopReason = StopReason::CANCELLED;
		return result;
	}


	Object::Ptr blockObject(const Block& block)
		/// The block as the API sent it, for the assistant turn; null for a
		/// block that is left out.
	{
		Object::Ptr pBlock = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pBlock->set("type", block.type);
		if (block.type == "text")
		{
			if (block.text.empty()) return Object::Ptr();
			pBlock->set("text", block.text);
		}
		else if (block.type == "thinking")
		{
			pBlock->set("thinking", block.text);
			pBlock->set("signature", block.signature);
		}
		else if (block.type == "redacted_thinking")
		{
			pBlock->set("data", block.data);
		}
		else
		{
			return Object::Ptr();
		}
		return pBlock;
	}


	Array::Ptr resultContent(const Poco::AI::MCP::ToolResult& result)
		/// The content of a tool_result block: text and image blocks as they
		/// are, other blocks and structured content as JSON text.
	{
		Array::Ptr pContent = new Array;
		const Array::Ptr pBlocks = result.content();
		for (std::size_t i = 0; pBlocks != nullptr && i < pBlocks->size(); ++i)
		{
			const Object::Ptr pBlock = pBlocks->getObject(static_cast<unsigned>(i));
			const std::string type = stringOf(pBlock, "type");
			Object::Ptr pOut = new Object;
			if (type == "text")
			{
				const std::string text = stringOf(pBlock, "text");
				if (text.empty()) continue;
				pOut->set("type", "text");
				pOut->set("text", text);
			}
			else if (type == "image")
			{
				Object::Ptr pSource = new Object;
				pSource->set("type", "base64");
				pSource->set("media_type", stringOf(pBlock, "mimeType"));
				pSource->set("data", stringOf(pBlock, "data"));
				pOut->set("type", "image");
				pOut->set("source", pSource);
			}
			else
			{
				pOut->set("type", "text");
				pOut->set("text", serializeJSON(pBlock));
			}
			pContent->add(pOut);
		}
		if (pContent->size() == 0 && !result.structuredContent().isEmpty())
		{
			Object::Ptr pOut = new Object;
			pOut->set("type", "text");
			pOut->set("text", serializeJSON(result.structuredContent()));
			pContent->add(pOut);
		}
		return pContent;
	}
}


AnthropicProvider::AnthropicProvider(const Params& params, SessionFactory sessionFactory):
	_params(params),
	_sessionFactory(std::move(sessionFactory))
{
}


AnthropicProvider::~AnthropicProvider()
{
}


AnthropicProvider::Params AnthropicProvider::params() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _params;
}


void AnthropicProvider::setModel(const std::string& model)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_params.model = model;
}


std::string AnthropicProvider::getModel() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _params.model;
}


std::vector<std::string> AnthropicProvider::listModels()
{
	const Params current = params();
	Poco::URI uri = joinURI(current.baseURI, "models");
	uri.addQueryParameter("limit", "1000");
	ProviderCall call(_sessionFactory, uri, Poco::Net::HTTPRequest::HTTP_GET, current.timeout);
	call.request().set("x-api-key", current.apiKey);
	call.request().set("anthropic-version", current.apiVersion);
	call.send(std::string());
	std::istream& stream = call.receive();
	const int status = static_cast<int>(call.response().getStatus());
	if (status != Poco::Net::HTTPResponse::HTTP_OK)
	{
		throw Poco::Net::HTTPException(Poco::format("Anthropic API error %d", status), excerpt(readAtMost(stream, 65536)));
	}
	const Object::Ptr pRoot = parseObject(readBody(stream, current.maxResponseSize));
	if (pRoot.isNull() || !pRoot->isArray("data"))
	{
		throw Poco::DataFormatException("The model list is not an object with a data array");
	}
	std::vector<std::string> models;
	const Array::Ptr pData = pRoot->getArray("data");
	for (std::size_t i = 0; i < pData->size(); ++i)
	{
		const std::string id = stringOf(pData->getObject(static_cast<unsigned>(i)), "id");
		if (!id.empty()) models.push_back(id);
	}
	return models;
}


Object::Ptr AnthropicProvider::requestBody(const ChatRequest& request, const Params& current) const
{
	Object::Ptr pBody = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pBody->set("model", current.model);
	pBody->set("max_tokens", current.maxTokens);
	pBody->set("stream", true);
	if (!request.systemPrompt.empty())
	{
		// The system prompt is one text block with the cache marker.
		Object::Ptr pCacheControl = new Object;
		pCacheControl->set("type", "ephemeral");
		Object::Ptr pSystemBlock = new Object;
		pSystemBlock->set("type", "text");
		pSystemBlock->set("text", request.systemPrompt);
		pSystemBlock->set("cache_control", pCacheControl);
		Array::Ptr pSystem = new Array;
		pSystem->add(pSystemBlock);
		pBody->set("system", pSystem);
	}
	if (!request.tools.empty())
	{
		pBody->set("tools", toAnthropicTools(request.tools));
		if (request.toolChoice == ToolChoice::NONE)
		{
			Object::Ptr pChoice = new Object;
			pChoice->set("type", "none");
			pBody->set("tool_choice", pChoice);
		}
	}
	pBody->set("messages", Array::Ptr(new Array(request.messages)));
	return pBody;
}


ChatResult AnthropicProvider::chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation)
{
	if (cancellation.cancelled()) return cancelled();
	const Params current = params();
	const std::string body = serializeJSON(requestBody(request, current));

	ProviderCall call(_sessionFactory, joinURI(current.baseURI, "messages"), Poco::Net::HTTPRequest::HTTP_POST, current.timeout);
	call.request().setContentType("application/json");
	call.request().set("Accept", "text/event-stream");
	call.request().set("x-api-key", current.apiKey);
	call.request().set("anthropic-version", current.apiVersion);
	try
	{
		call.send(body);
	}
	catch (const Poco::Exception&)
	{
		if (cancellation.cancelled()) return cancelled();
		throw;
	}

	// From here on a cancel shuts the connection down, so that the read that
	// waits returns.
	Cancellation::Request whileReceiving(cancellation, [&call]() { call.abort(); });
	ChatResult result;
	bool streaming = false;
	try
	{
		std::istream& stream = call.receive();
		const int status = static_cast<int>(call.response().getStatus());
		if (status != Poco::Net::HTTPResponse::HTTP_OK)
		{
			const std::string text = readAtMost(stream, 65536);
			if (cancellation.cancelled()) return cancelled();
			report(onEvent, errorCodeOf(status), Poco::format("Anthropic API error %d: %s", status, excerpt(text)));
			return failed();
		}
		streaming = true;

		// The Messages API streams typed events. Text arrives in text_delta
		// events and is reported at once; the other blocks are assembled by
		// index: a tool call's arguments arrive as pieces of JSON text, a
		// thinking block as its text and then its signature.
		std::map<int, Block> blocks;
		bool complete = false;
		Poco::AI::MCP::HTTP::EventStreamReader reader(stream, current.maxResponseSize);
		Poco::AI::MCP::HTTP::EventStreamReader::Event event;
		while (!complete && reader.next(event))
		{
			if (cancellation.cancelled()) return cancelled();
			const Object::Ptr pEvent = parseObject(event.data);
			const std::string type = stringOf(pEvent, "type");
			const int index = memberOf(pEvent, "index").isInteger() ? pEvent->getValue<int>("index") : 0;

			if (type == "message_start")
			{
				const Object::Ptr pMessage = pEvent->getObject("message");
				const Object::Ptr pUsage = pMessage.isNull() ? Object::Ptr() : pMessage->getObject("usage");
				result.usage.inputTokens = countOf(pUsage, "input_tokens") + countOf(pUsage, "cache_creation_input_tokens") + countOf(pUsage, "cache_read_input_tokens");
				result.usage.cachedInputTokens = countOf(pUsage, "cache_read_input_tokens");
			}
			else if (type == "content_block_start")
			{
				const Object::Ptr pBlock = pEvent->getObject("content_block");
				Block& block = blocks[index];
				block.type = stringOf(pBlock, "type");
				block.id = stringOf(pBlock, "id");
				block.name = stringOf(pBlock, "name");
				block.data = stringOf(pBlock, "data");
				block.signature = stringOf(pBlock, "signature");
				block.text = block.type == "thinking" ? stringOf(pBlock, "thinking") : stringOf(pBlock, "text");
			}
			else if (type == "content_block_delta")
			{
				const Object::Ptr pDelta = pEvent->getObject("delta");
				const std::string deltaType = stringOf(pDelta, "type");
				Block& block = blocks[index];
				if (deltaType == "text_delta")
				{
					const std::string text = stringOf(pDelta, "text");
					block.text += text;
					result.text += text;
					if (!text.empty() && onEvent)
					{
						ContentEvent textEvent;
						textEvent.text = text;
						onEvent(textEvent);
					}
				}
				else if (deltaType == "input_json_delta")
				{
					block.arguments += stringOf(pDelta, "partial_json");
				}
				else if (deltaType == "thinking_delta")
				{
					block.text += stringOf(pDelta, "thinking");
				}
				else if (deltaType == "signature_delta")
				{
					block.signature += stringOf(pDelta, "signature");
				}
			}
			else if (type == "message_delta")
			{
				const Object::Ptr pDelta = pEvent->getObject("delta");
				result.stopDetail = stringOf(pDelta, "stop_reason");
				result.stopReason = stopReasonOf(result.stopDetail);
				result.usage.outputTokens = countOf(pEvent->getObject("usage"), "output_tokens");
			}
			else if (type == "message_stop")
			{
				complete = true;
			}
			else if (type == "error")
			{
				const Object::Ptr pError = pEvent->getObject("error");
				const std::string errorType = stringOf(pError, "type");
				const std::string message = stringOf(pError, "message");
				report(onEvent, errorType == "overloaded_error" ? ErrorCode::OVERLOADED : ErrorCode::STREAM_ERROR,
					"Anthropic stream error: " + (message.empty() ? errorType : message));
				return failed();
			}
		}
		if (cancellation.cancelled()) return cancelled();
		if (!complete && result.stopDetail.empty())
		{
			// Without its end the response is incomplete: no tool call of it runs.
			const std::string reason = call.networkError();
			report(onEvent, ErrorCode::STREAM_INTERRUPTED, "Anthropic response stream ended before its end" + (reason.empty() ? std::string() : ": " + reason));
			return failed();
		}

		Array::Ptr pContent = new Array;
		for (auto& entry: blocks)
		{
			Block& block = entry.second;
			if (block.type == "tool_use")
			{
				ToolCall toolCall;
				if (!parseToolInput(block.arguments, toolCall.input))
				{
					// The tool does not run on input the model did not give.
					report(onEvent, ErrorCode::MALFORMED_TOOL_INPUT, "Anthropic tool call '" + block.name + "' has malformed input");
					continue;
				}
				toolCall.id = block.id;
				toolCall.name = block.name;
				Object::Ptr pToolUse = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
				pToolUse->set("type", "tool_use");
				pToolUse->set("id", block.id);
				pToolUse->set("name", block.name);
				pToolUse->set("input", toolCall.input);
				pContent->add(pToolUse);
				result.toolCalls.push_back(toolCall);
				if (onEvent)
				{
					ContentEvent toolEvent;
					toolEvent.type = ContentEvent::Type::TOOL_USE;
					toolEvent.toolCall = toolCall;
					onEvent(toolEvent);
				}
			}
			else
			{
				const Object::Ptr pBlock = blockObject(block);
				if (!pBlock.isNull()) pContent->add(pBlock);
			}
		}
		if (pContent->size() > 0)
		{
			result.assistantMessage = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
			result.assistantMessage->set("role", "assistant");
			result.assistantMessage->set("content", pContent);
		}
		return result;
	}
	catch (const Poco::TimeoutException&)
	{
		if (cancellation.cancelled()) return cancelled();
		if (!streaming) throw;
		report(onEvent, ErrorCode::STREAM_INTERRUPTED, "Anthropic response stream timed out");
		return failed();
	}
	catch (const Poco::Exception& exc)
	{
		if (cancellation.cancelled()) return cancelled();
		if (!streaming) throw;
		report(onEvent, ErrorCode::STREAM_INTERRUPTED, "Anthropic response stream failed: " + exc.displayText());
		return failed();
	}
}


void AnthropicProvider::appendToolResult(Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result)
{
	Object::Ptr pToolResult = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pToolResult->set("type", "tool_result");
	pToolResult->set("tool_use_id", toolCallId);
	const Array::Ptr pContent = resultContent(result);
	if (pContent->size() > 0)
	{
		pToolResult->set("content", pContent);
	}
	else
	{
		pToolResult->set("content", std::string());
	}
	if (result.isError())
	{
		pToolResult->set("is_error", true);
	}

	// The results of one turn's tool calls go together in one user message.
	if (messages.size() > 0)
	{
		const Object::Ptr pLast = messages.getObject(static_cast<unsigned>(messages.size() - 1));
		if (stringOf(pLast, "role") == "user" && pLast->isArray("content"))
		{
			Array::Ptr pLastContent = pLast->getArray("content");
			if (pLastContent->size() > 0 && stringOf(pLastContent->getObject(0), "type") == "tool_result")
			{
				pLastContent->add(pToolResult);
				return;
			}
		}
	}
	Array::Ptr pUserContent = new Array;
	pUserContent->add(pToolResult);
	Object::Ptr pMessage = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pMessage->set("role", "user");
	pMessage->set("content", pUserContent);
	messages.add(pMessage);
}


Array::Ptr AnthropicProvider::toAnthropicTools(const std::vector<Poco::AI::MCP::Tool>& tools)
{
	Array::Ptr pTools = new Array;
	for (std::size_t i = 0; i < tools.size(); ++i)
	{
		// toObject() copies the schema, so that requests serialized on several
		// threads share no object.
		const Object::Ptr pListed = tools[i].toObject();
		Object::Ptr pTool = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pTool->set("name", tools[i].name);
		if (!tools[i].description.empty()) pTool->set("description", tools[i].description);
		pTool->set("input_schema", pListed->getObject("inputSchema"));
		if (i + 1 == tools.size())
		{
			// The cache marker on the last tool ends the cacheable prefix.
			Object::Ptr pCacheControl = new Object;
			pCacheControl->set("type", "ephemeral");
			pTool->set("cache_control", pCacheControl);
		}
		pTools->add(pTool);
	}
	return pTools;
}


} // namespace Poco::AI
