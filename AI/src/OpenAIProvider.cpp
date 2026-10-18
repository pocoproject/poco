//
// OpenAIProvider.cpp
//
// Library: AI
// Package: Providers
// Module:  OpenAIProvider
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/OpenAIProvider.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "ProviderCall.h"
#include "ToolInput.h"
#include "Poco/Net/NetException.h"
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
	struct ToolAccumulator
		/// One tool call of a streamed response, assembled from its pieces.
	{
		std::string id;
		std::string name;
		std::string arguments;
	};


	StopReason stopReasonOf(const std::string& value)
	{
		if (value == "stop") return StopReason::END_TURN;
		if (value == "tool_calls" || value == "function_call") return StopReason::TOOL_USE;
		if (value == "length") return StopReason::MAX_TOKENS;
		if (value == "content_filter") return StopReason::CONTENT_FILTER;
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


	void authorize(Poco::Net::HTTPRequest& request, const std::string& apiKey)
	{
		if (!apiKey.empty())
		{
			request.set("Authorization", "Bearer " + apiKey);
		}
	}


	std::string errorText(const Object::Ptr& pError)
		/// The message of an error object, which some servers send as a string.
	{
		const std::string message = stringOf(pError, "message");
		return message.empty() ? serializeJSON(Poco::Dynamic::Var(pError)) : message;
	}
}


OpenAIProvider::OpenAIProvider(const Params& params, SessionFactory sessionFactory):
	_params(params),
	_sessionFactory(std::move(sessionFactory))
{
}


OpenAIProvider::~OpenAIProvider()
{
}


OpenAIProvider::Params OpenAIProvider::params() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _params;
}


void OpenAIProvider::setModel(const std::string& model)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_params.model = model;
}


std::string OpenAIProvider::getModel() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _params.model;
}


Object::Ptr OpenAIProvider::requestBody(const ChatRequest& request, const Params& current) const
{
	Array::Ptr pMessages = new Array;
	if (!request.systemPrompt.empty())
	{
		Object::Ptr pSystem = new Object;
		pSystem->set("role", "system");
		pSystem->set("content", request.systemPrompt);
		pMessages->add(pSystem);
	}
	for (const auto& message: request.messages)
	{
		pMessages->add(message);
	}

	Object::Ptr pBody = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pBody->set("model", current.model);
	if (current.maxTokens > 0)
	{
		pBody->set(current.maxTokensField.empty() ? std::string("max_tokens") : current.maxTokensField, current.maxTokens);
	}
	pBody->set("messages", pMessages);
	pBody->set("stream", true);
	if (current.supportsTools && !request.tools.empty())
	{
		pBody->set("tools", toOpenAITools(request.tools));
		if (request.toolChoice == ToolChoice::NONE)
		{
			pBody->set("tool_choice", "none");
		}
	}
	return pBody;
}


ChatResult OpenAIProvider::chat(const ChatRequest& request, const ContentEventCallback& onEvent, Cancellation& cancellation)
{
	if (cancellation.cancelled()) return cancelled();
	const Params current = params();
	const std::string body = serializeJSON(requestBody(request, current));

	ProviderCall call(_sessionFactory, joinURI(current.baseURI, "chat/completions"), Poco::Net::HTTPRequest::HTTP_POST, current.timeout);
	call.request().setContentType("application/json");
	call.request().set("Accept", "text/event-stream");
	authorize(call.request(), current.apiKey);
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
			report(onEvent, errorCodeOf(status), Poco::format("OpenAI API error %d: %s", status, excerpt(text)));
			return failed();
		}
		streaming = true;

		// Each event is a chat.completion.chunk; the stream ends with [DONE].
		// Text arrives in choices[0].delta.content and is reported at once;
		// tool calls arrive in pieces keyed by tool_calls[].index (id and name
		// first, then the arguments as JSON text) and are reported once the
		// stream is complete.
		std::map<int, ToolAccumulator> toolCalls;
		bool complete = false;
		Poco::AI::MCP::HTTP::EventStreamReader reader(stream, current.maxResponseSize);
		Poco::AI::MCP::HTTP::EventStreamReader::Event event;
		while (!complete && reader.next(event))
		{
			if (cancellation.cancelled()) return cancelled();
			if (event.data == "[DONE]")
			{
				complete = true;
				break;
			}
			const Object::Ptr pChunk = parseObject(event.data);
			if (pChunk.isNull()) continue;
			if (pChunk->has("error"))
			{
				// An error after the 200, as vLLM and llama.cpp send it.
				const Object::Ptr pError = pChunk->getObject("error");
				report(onEvent, ErrorCode::STREAM_ERROR, "OpenAI stream error: " + (pError.isNull() ? pChunk->get("error").toString() : errorText(pError)));
				return failed();
			}
			const Object::Ptr pUsage = pChunk->getObject("usage");
			if (!pUsage.isNull())
			{
				result.usage.inputTokens = countOf(pUsage, "prompt_tokens");
				result.usage.outputTokens = countOf(pUsage, "completion_tokens");
				result.usage.cachedInputTokens = countOf(pUsage->getObject("prompt_tokens_details"), "cached_tokens");
			}
			const Array::Ptr pChoices = pChunk->getArray("choices");
			const Object::Ptr pChoice = (pChoices.isNull() || pChoices->size() == 0) ? Object::Ptr() : pChoices->getObject(0);
			if (pChoice.isNull()) continue;

			const Object::Ptr pDelta = pChoice->getObject("delta");
			const std::string text = stringOf(pDelta, "content");
			if (!text.empty())
			{
				result.text += text;
				if (onEvent)
				{
					ContentEvent textEvent;
					textEvent.text = text;
					onEvent(textEvent);
				}
			}
			const Array::Ptr pToolCalls = pDelta.isNull() ? Array::Ptr() : pDelta->getArray("tool_calls");
			for (std::size_t i = 0; !pToolCalls.isNull() && i < pToolCalls->size(); ++i)
			{
				const Object::Ptr pToolCall = pToolCalls->getObject(static_cast<unsigned>(i));
				if (pToolCall.isNull()) continue;
				const int index = memberOf(pToolCall, "index").isInteger() ? pToolCall->getValue<int>("index") : 0;
				ToolAccumulator& accumulator = toolCalls[index];
				// Continuation chunks may carry null or empty ids and names.
				const std::string id = stringOf(pToolCall, "id");
				if (!id.empty()) accumulator.id = id;
				const Object::Ptr pFunction = pToolCall->getObject("function");
				const std::string name = stringOf(pFunction, "name");
				if (!name.empty()) accumulator.name = name;
				accumulator.arguments += stringOf(pFunction, "arguments");
			}
			const std::string finishReason = stringOf(pChoice, "finish_reason");
			if (!finishReason.empty())
			{
				result.stopDetail = finishReason;
				result.stopReason = stopReasonOf(finishReason);
			}
		}
		if (cancellation.cancelled()) return cancelled();
		if (!complete && result.stopDetail.empty())
		{
			// Without its end the response is incomplete: no tool call of it runs.
			const std::string reason = call.networkError();
			report(onEvent, ErrorCode::STREAM_INTERRUPTED, "OpenAI response stream ended before its end" + (reason.empty() ? std::string() : ": " + reason));
			return failed();
		}

		Array::Ptr pCalls = new Array;
		for (const auto& entry: toolCalls)
		{
			const ToolAccumulator& accumulator = entry.second;
			ToolCall toolCall;
			if (!parseToolInput(accumulator.arguments, toolCall.input))
			{
				// The tool does not run on arguments the model did not give.
				report(onEvent, ErrorCode::MALFORMED_TOOL_INPUT, "OpenAI tool call '" + accumulator.name + "' has malformed arguments");
				continue;
			}
			toolCall.id = accumulator.id;
			toolCall.name = accumulator.name;
			Object::Ptr pFunction = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
			pFunction->set("name", accumulator.name);
			pFunction->set("arguments", serializeJSON(toolCall.input));
			Object::Ptr pCall = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
			pCall->set("id", accumulator.id);
			pCall->set("type", "function");
			pCall->set("function", pFunction);
			pCalls->add(pCall);
			result.toolCalls.push_back(toolCall);
			if (onEvent)
			{
				ContentEvent toolEvent;
				toolEvent.type = ContentEvent::Type::TOOL_USE;
				toolEvent.toolCall = toolCall;
				onEvent(toolEvent);
			}
		}

		result.assistantMessage = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		result.assistantMessage->set("role", "assistant");
		if (pCalls->size() > 0)
		{
			// The content is the text, or null alongside tool calls.
			result.assistantMessage->set("content", result.text.empty() ? Poco::Dynamic::Var() : Poco::Dynamic::Var(result.text));
			result.assistantMessage->set("tool_calls", pCalls);
		}
		else
		{
			result.assistantMessage->set("content", result.text);
		}
		return result;
	}
	catch (const Poco::Exception& exc)
	{
		if (cancellation.cancelled()) return cancelled();
		if (!streaming) throw;
		report(onEvent, ErrorCode::STREAM_INTERRUPTED, "OpenAI response stream failed: " + exc.displayText());
		return failed();
	}
}


std::vector<std::string> OpenAIProvider::listModels()
{
	const Params current = params();
	ProviderCall call(_sessionFactory, joinURI(current.baseURI, "models"), Poco::Net::HTTPRequest::HTTP_GET, current.timeout);
	authorize(call.request(), current.apiKey);
	call.send(std::string());
	std::istream& stream = call.receive();
	const int status = static_cast<int>(call.response().getStatus());
	if (status != Poco::Net::HTTPResponse::HTTP_OK)
	{
		throw Poco::Net::HTTPException(Poco::format("OpenAI API error %d", status), excerpt(readAtMost(stream, 65536)));
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
		// Embedding models share this listing but cannot serve a chat request.
		if (id.empty() || id.find("embed") != std::string::npos) continue;
		models.push_back(id);
	}
	std::sort(models.begin(), models.end());
	return models;
}


std::vector<std::vector<float>> OpenAIProvider::embed(const std::vector<std::string>& texts, const std::string& model, int dimensions)
{
	std::vector<std::vector<float>> result;
	if (texts.empty()) return result;

	const Params current = params();
	Object::Ptr pBody = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pBody->set("model", model);
	Array::Ptr pInput = new Array;
	for (const std::string& text: texts) pInput->add(text);
	pBody->set("input", pInput);
	if (dimensions > 0) pBody->set("dimensions", dimensions);

	ProviderCall call(_sessionFactory, joinURI(current.baseURI, "embeddings"), Poco::Net::HTTPRequest::HTTP_POST, current.timeout);
	call.request().setContentType("application/json");
	authorize(call.request(), current.apiKey);
	call.send(serializeJSON(pBody));
	std::istream& stream = call.receive();
	const int status = static_cast<int>(call.response().getStatus());
	if (status != Poco::Net::HTTPResponse::HTTP_OK)
	{
		throw Poco::Net::HTTPException(Poco::format("embeddings API error %d", status), excerpt(readAtMost(stream, 65536)));
	}

	const Object::Ptr pRoot = parseObject(readBody(stream, current.maxResponseSize));
	const Array::Ptr pData = pRoot.isNull() ? Array::Ptr() : pRoot->getArray("data");
	if (pData.isNull() || pData->size() != texts.size())
	{
		throw Poco::DataFormatException(Poco::format("embeddings response has %z vectors for %z inputs", std::size_t(pData.isNull() ? 0 : pData->size()), texts.size()));
	}
	try
	{
		// The API is free to reorder entries; index restores input order.
		result.resize(texts.size());
		for (std::size_t i = 0; i < pData->size(); ++i)
		{
			const Object::Ptr pEntry = pData->getObject(static_cast<unsigned>(i));
			if (pEntry.isNull()) throw Poco::DataFormatException("embeddings response entry is not an object");
			const std::size_t index = pEntry->optValue<std::size_t>("index", i);
			if (index >= result.size()) throw Poco::DataFormatException("embeddings response index out of range");
			const Array::Ptr pVector = pEntry->getArray("embedding");
			if (pVector.isNull() || pVector->size() == 0) throw Poco::DataFormatException("embeddings response entry has no embedding");
			std::vector<float>& out = result[index];
			if (!out.empty()) throw Poco::DataFormatException(Poco::format("embeddings response repeats index %z", index));
			out.reserve(pVector->size());
			for (std::size_t j = 0; j < pVector->size(); ++j)
			{
				// Converts through Dynamic::Var, which refuses a value outside the float range.
				out.push_back(pVector->getElement<float>(static_cast<unsigned>(j)));
			}
		}
		for (const std::vector<float>& vector: result)
		{
			if (vector.size() != result[0].size()) throw Poco::DataFormatException("embeddings response has vectors of different lengths");
		}
	}
	catch (const Poco::DataFormatException&)
	{
		throw;
	}
	catch (const Poco::Exception& exc)
	{
		throw Poco::DataFormatException("malformed embeddings response: " + exc.displayText());
	}
	return result;
}


void OpenAIProvider::appendToolResult(Array& messages, const std::string& toolCallId, const Poco::AI::MCP::ToolResult& result)
{
	Object::Ptr pMessage = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
	pMessage->set("role", "tool");
	pMessage->set("tool_call_id", toolCallId);
	pMessage->set("content", toolResultText(result));
	messages.add(pMessage);
}


Array::Ptr OpenAIProvider::toOpenAITools(const std::vector<Poco::AI::MCP::Tool>& tools)
{
	Array::Ptr pTools = new Array;
	for (const Poco::AI::MCP::Tool& tool: tools)
	{
		// toObject() copies the schema, so that requests serialized on several
		// threads share no object.
		const Object::Ptr pListed = tool.toObject();
		Object::Ptr pFunction = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pFunction->set("name", tool.name);
		if (!tool.description.empty()) pFunction->set("description", tool.description);
		pFunction->set("parameters", pListed->getObject("inputSchema"));
		Object::Ptr pTool = new Object(Poco::JSON_PRESERVE_KEY_ORDER);
		pTool->set("type", "function");
		pTool->set("function", pFunction);
		pTools->add(pTool);
	}
	return pTools;
}


} // namespace Poco::AI
