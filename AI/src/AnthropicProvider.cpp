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
#include "Poco/Ascii.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <map>
#include <utility>


using Poco::JSON::Object;
using Poco::JSON::Array;


namespace Poco::AI {


namespace
{
	struct Block
		/// One content block of a streamed response: the block as its start
		/// event gave it, and what its deltas added.
	{
		Object::Ptr pStart;
		std::string type;
		std::string text;
		std::string arguments;
		bool hasArguments = false;
		std::string signature;
		Array::Ptr pCitations;
	};


	StopReason stopReasonOf(const std::string& value)
	{
		if (value == "end_turn") return StopReason::END_TURN;
		if (value == "tool_use") return StopReason::TOOL_USE;
		if (value == "max_tokens" || value == "model_context_window_exceeded") return StopReason::MAX_TOKENS;
		if (value == "stop_sequence") return StopReason::STOP_SEQUENCE;
		if (value == "refusal") return StopReason::REFUSAL;
		if (value == "pause_turn") return StopReason::PAUSED;
		return StopReason::UNKNOWN;
	}


	ChatResult failed()
	{
		ChatResult result;
		result.stopReason = StopReason::FAILURE;
		return result;
	}


	ChatResult cancelled()
	{
		ChatResult result;
		result.stopReason = StopReason::CANCELLED;
		return result;
	}


	Object::Ptr cacheMarker()
	{
		Object::Ptr pCacheControl = new Object;
		pCacheControl->set("type", "ephemeral");
		return pCacheControl;
	}


	Object::Ptr copyMembers(const Object::Ptr& pObject)
		/// A copy of an object with the same members, in their order.
	{
		Object::Ptr pCopy = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		if (!pObject.isNull())
		{
			for (const std::string& name: pObject->getNames()) pCopy->set(name, pObject->get(name));
		}
		return pCopy;
	}


	bool blank(const std::string& text)
		/// Returns true if text holds nothing but whitespace.
	{
		return std::all_of(text.begin(), text.end(), [](char c) { return Poco::Ascii::isSpace(c); });
	}


	Object::Ptr blockObject(const Block& block, const Object::Ptr& pInput)
		/// The block as the API sent it, for the assistant turn: what its start
		/// event carried, in that order, with what its deltas added; blocks of
		/// types this provider does not know as they came. Null for what the
		/// API refuses in a request: a thinking block without its signature, and
		/// a text block with nothing but whitespace.
	{
		Object::Ptr pBlock = copyMembers(block.pStart);
		pBlock->set("type", block.type);
		if (block.type == "text")
		{
			if (blank(block.text)) return Object::Ptr();
			pBlock->set("text", block.text);
			if (!block.pCitations.isNull()) pBlock->set("citations", block.pCitations);
		}
		else if (block.type == "thinking")
		{
			if (block.signature.empty()) return Object::Ptr();
			pBlock->set("thinking", block.text);
			pBlock->set("signature", block.signature);
		}
		else if (!pInput.isNull())
		{
			pBlock->set("input", pInput);
		}
		return pBlock;
	}


	Array::Ptr withCacheMarker(const Array& messages)
		/// The messages, with a cache marker on the last block of the last one,
		/// so that every call of an agent loop reads the conversation so far
		/// from the prompt cache instead of paying for all of it again. Only
		/// that message and its last block are copied; the caller's objects
		/// stay as they are.
	{
		Array::Ptr pMessages = new Array(messages);
		if (pMessages->size() == 0) return pMessages;
		const unsigned last = static_cast<unsigned>(pMessages->size() - 1);
		const Object::Ptr pLast = pMessages->getObject(last);
		if (pLast.isNull()) return pMessages;
		Array::Ptr pContent = new Array;
		const Poco::Dynamic::Var content = pLast->get("content");
		if (content.isString())
		{
			Object::Ptr pText = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
			pText->set("type", "text");
			pText->set("text", content.extract<std::string>());
			pContent->add(pText);
		}
		else if (pLast->isArray("content"))
		{
			for (const auto& block: *pLast->getArray("content")) pContent->add(block);
		}
		if (pContent->size() == 0) return pMessages;
		const unsigned lastBlock = static_cast<unsigned>(pContent->size() - 1);
		const Object::Ptr pBlock = pContent->getObject(lastBlock);
		const std::string type = stringOf(pBlock, "type");
		// Thinking blocks take no marker, nor does an empty text block.
		if (pBlock.isNull() || type == "thinking" || type == "redacted_thinking" || (type == "text" && stringOf(pBlock, "text").empty()))
		{
			return pMessages;
		}
		Object::Ptr pMarked = copyMembers(pBlock);
		pMarked->set("cache_control", cacheMarker());
		pContent->set(lastBlock, pMarked);
		Object::Ptr pCopy = copyMembers(pLast);
		pCopy->set("content", pContent);
		pMessages->set(last, pCopy);
		return pMessages;
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
		if (pContent->size() == 0 && result.hasStructuredContent())
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
	refuseRequestMembers(_params.requestMembers, {"model", "max_tokens", "messages", "system", "stream", "tools", "tool_choice"});
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
	Cancellation cancellation;
	ProviderCall call(_sessionFactory, uri, Poco::Net::HTTPRequest::HTTP_GET, current.timeout, current.headers);
	call.request().set("x-api-key", current.apiKey);
	call.request().set("anthropic-version", current.apiVersion);
	call.send(std::string(), cancellation);
	std::istream& stream = call.receive();
	const int status = static_cast<int>(call.response().getStatus());
	if (status != Poco::Net::HTTPResponse::HTTP_OK)
	{
		throw Poco::Net::HTTPException(Poco::format("Anthropic API error %d", status), excerpt(readAtMost(stream, 65536)), status);
	}
	const std::string text = readBody(stream, current.maxResponseSize);
	if (call.brokeOff(text.size()))
	{
		throw Poco::IOException("The model list of the Anthropic API broke off", call.networkError());
	}
	const Object::Ptr pRoot = parseObject(text);
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
		Object::Ptr pSystemBlock = new Object;
		pSystemBlock->set("type", "text");
		pSystemBlock->set("text", request.systemPrompt);
		pSystemBlock->set("cache_control", cacheMarker());
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
	pBody->set("messages", withCacheMarker(request.messages));
	mergeMembers(*pBody, current.requestMembers);
	return pBody;
}


ChatResult AnthropicProvider::chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation)
{
	if (cancellation.cancelled()) return cancelled();
	const Params current = params();
	const std::string body = serializeJSON(requestBody(request, current));

	ProviderCall call(_sessionFactory, joinURI(current.baseURI, "messages"), Poco::Net::HTTPRequest::HTTP_POST, current.timeout, current.headers);
	call.request().setContentType("application/json");
	call.request().set("Accept", "text/event-stream");
	call.request().set("x-api-key", current.apiKey);
	call.request().set("anthropic-version", current.apiVersion);
	ChatResult result;
	bool streaming = false;
	try
	{
		// From the send on, a cancel shuts the connection down, so that what
		// waits returns.
		call.send(body, cancellation);
		std::istream& stream = call.receive();
		const int status = static_cast<int>(call.response().getStatus());
		if (status != Poco::Net::HTTPResponse::HTTP_OK)
		{
			const std::string text = readAtMost(stream, 65536);
			if (cancellation.cancelled()) return cancelled();
			reportFailure(onEvent, failureCodeOf(status, text), Poco::format("Anthropic API error %d: %s", status, excerpt(text)), &call.response());
			return failed();
		}
		streaming = true;

		// The Messages API streams typed events. Text arrives in text_delta
		// events and is reported at once; every block is assembled by index
		// from its start event and its deltas: a tool call's input arrives as
		// pieces of JSON text, a thinking block as its text and then its
		// signature, citations one by one.
		std::map<int, Block> blocks;
		bool complete = false;
		Poco::AI::MCP::HTTP::EventStreamReader reader(stream, current.maxResponseSize);
		Poco::AI::MCP::HTTP::EventStreamReader::Event event;
		while (!complete && reader.next(event))
		{
			if (cancellation.cancelled()) return cancelled();
			const Object::Ptr pEvent = parseOrderedObject(event.data);
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
				block.pStart = pBlock;
				block.type = stringOf(pBlock, "type");
				block.signature = stringOf(pBlock, "signature");
				block.text = block.type == "thinking" ? stringOf(pBlock, "thinking") : stringOf(pBlock, "text");
				if (!pBlock.isNull() && pBlock->isArray("citations"))
				{
					block.pCitations = new Array(*pBlock->getArray("citations"));
				}
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
					block.hasArguments = true;
				}
				else if (deltaType == "thinking_delta")
				{
					block.text += stringOf(pDelta, "thinking");
				}
				else if (deltaType == "signature_delta")
				{
					block.signature += stringOf(pDelta, "signature");
				}
				else if (deltaType == "citations_delta")
				{
					if (block.pCitations.isNull()) block.pCitations = new Array;
					block.pCitations->add(pDelta->get("citation"));
				}
			}
			else if (type == "message_delta")
			{
				const Object::Ptr pDelta = pEvent->getObject("delta");
				result.stopDetail = stringOf(pDelta, "stop_reason");
				result.stopReason = stopReasonOf(result.stopDetail);
				result.explanation = stringOf(pDelta.isNull() ? Object::Ptr() : pDelta->getObject("stop_details"), "explanation");
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
				reportFailure(onEvent, errorType == "overloaded_error" ? FailureCode::OVERLOADED : FailureCode::STREAM_ERROR,
					"Anthropic stream error: " + (message.empty() ? errorType : message), &call.response());
				return failed();
			}
		}
		if (cancellation.cancelled()) return cancelled();
		if (!complete && result.stopDetail.empty())
		{
			// Without its end the response is incomplete: no tool call of it runs.
			const std::string reason = call.networkError();
			reportFailure(onEvent, FailureCode::STREAM_INTERRUPTED, "Anthropic response stream ended before its end" + (reason.empty() ? std::string() : ": " + reason));
			return failed();
		}
		if (result.stopReason == StopReason::REFUSAL)
		{
			// A refused answer is not kept: no turn to append, no tool to run.
			return result;
		}

		Array::Ptr pContent = new Array;
		for (auto& entry: blocks)
		{
			const Block& block = entry.second;
			Object::Ptr pInput;
			if (block.type == "tool_use" || block.hasArguments)
			{
				if (!parseToolInput(block.arguments, pInput))
				{
					pInput = nullptr;
					if (block.type == "tool_use")
					{
						// The tool does not run on input the model did not give.
						reportFailure(onEvent, FailureCode::MALFORMED_TOOL_INPUT, "Anthropic tool call '" + stringOf(block.pStart, "name") + "' has malformed input");
						continue;
					}
				}
			}
			const Object::Ptr pBlock = blockObject(block, pInput);
			if (pBlock.isNull()) continue;
			pContent->add(pBlock);
			if (block.type == "tool_use")
			{
				ToolCall toolCall;
				toolCall.id = stringOf(block.pStart, "id");
				toolCall.name = stringOf(block.pStart, "name");
				toolCall.input = pInput;
				result.toolCalls.push_back(toolCall);
				if (onEvent)
				{
					ContentEvent toolEvent;
					toolEvent.type = ContentEvent::Type::TOOL_USE;
					toolEvent.toolCall = toolCall;
					onEvent(toolEvent);
				}
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
		reportFailure(onEvent, FailureCode::STREAM_INTERRUPTED, "Anthropic response stream timed out");
		return failed();
	}
	catch (const Poco::Exception& exc)
	{
		if (cancellation.cancelled()) return cancelled();
		if (!streaming) throw;
		reportFailure(onEvent, FailureCode::STREAM_INTERRUPTED, "Anthropic response stream failed: " + exc.displayText());
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
			pTool->set("cache_control", cacheMarker());
		}
		pTools->add(pTool);
	}
	return pTools;
}


} // namespace Poco::AI
