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
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include "Poco/URI.h"
#include <memory>
#include <sstream>
#include <map>
#include <utility>


using namespace Poco::Net;
using namespace Poco::JSON;
using Poco::Dynamic::Var;


namespace Poco {
namespace AI {




AnthropicProvider::AnthropicProvider(
	const std::string& baseUrl,
	const std::string& apiKey,
	const std::string& model,
	int maxTokens,
	int timeoutSeconds,
	SessionFactory sessionFactory):
	_baseUrl(baseUrl),
	_apiKey(apiKey),
	_model(model),
	_maxTokens(maxTokens),
	_timeoutSeconds(timeoutSeconds),
	_sessionFactory(std::move(sessionFactory)),
	_apiVersion(DEFAULT_API_VERSION)
{
}


void AnthropicProvider::setApiVersion(const std::string& version)
{
	_apiVersion = version;
}


void AnthropicProvider::setModel(const std::string& model)
{
	_model = model;
}


void AnthropicProvider::chat(
	const Poco::JSON::Array& messages,
	const std::string& systemPrompt,
	const Poco::JSON::Array& tools,
	ContentEventCallback onEvent)
{
	// Build request body
	Object body;
	body.set("model", _model);
	body.set("max_tokens", _maxTokens);
	body.set("stream", true);

	// System prompt as structured array with cache_control for prompt caching
	{
		Object::Ptr pCacheControl = new Object;
		pCacheControl->set("type", "ephemeral");

		Object::Ptr pSystemBlock = new Object;
		pSystemBlock->set("type", "text");
		pSystemBlock->set("text", systemPrompt);
		pSystemBlock->set("cache_control", pCacheControl);

		Array::Ptr pSystem = new Array;
		pSystem->add(pSystemBlock);
		body.set("system", pSystem);
	}

	// Convert tool definitions to the Messages API format with cache_control on the last tool
	if (tools.size() > 0)
	{
		body.set("tools", toAnthropicTools(tools));
	}

	body.set("messages", messages);

	std::ostringstream bodyStream;
	Stringifier::stringify(body, bodyStream);
	std::string bodyStr = bodyStream.str();

	Poco::URI uri(_baseUrl + "/messages");
	std::unique_ptr<HTTPClientSession> pSession = _sessionFactory ? _sessionFactory(uri) : createClientSession(uri);
	pSession->setTimeout(Poco::Timespan(_timeoutSeconds, 0));

	{
		std::lock_guard<std::mutex> lock(_sessionMutex);
		_pActiveSession = pSession.get();
	}

	// Guard to clear active session on exit
	struct SessionGuard
	{
		AnthropicProvider& self;
		~SessionGuard()
		{
			std::lock_guard<std::mutex> lock(self._sessionMutex);
			self._pActiveSession = nullptr;
		}
	} guard{*this};

	if (cancelled()) return;

	HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery(), HTTPMessage::HTTP_1_1);
	// The Host header names the endpoint, whatever the session is connected to
	// (a factory may hand out a session through a proxy or a tunnel).
	request.setHost(uri.getHost(), uri.getPort());
	request.setContentType("application/json");
	request.set("x-api-key", _apiKey);
	request.set("anthropic-version", _apiVersion);
	request.setContentLength(bodyStr.size());

	std::ostream& os = pSession->sendRequest(request);
	os << bodyStr;

	HTTPResponse response;
	std::istream& rs = pSession->receiveResponse(response);

	if (response.getStatus() != HTTPResponse::HTTP_OK)
	{
		std::string responseBody;
		Poco::StreamCopier::copyToString(rs, responseBody);
		ContentEvent evt;
		evt.type = ContentEvent::TYPE_ERROR;
		evt.text = Poco::format("Anthropic API error %d: %s",
			static_cast<int>(response.getStatus()), responseBody.substr(0, 500));
		if (response.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED
			|| response.getStatus() == HTTPResponse::HTTP_FORBIDDEN)
			evt.code = "unauthenticated";
		onEvent(evt);
		return;
	}

	// Stream the response as Messages API SSE. Frames are pairs of
	//   event: <type>
	//   data: {json}
	// lines; the event: line is ignored and we switch on the data payload's
	// own "type" field. Text arrives as content_block_delta / text_delta and
	// is emitted immediately. A tool call arrives as a content_block_start
	// carrying id + name, followed by input_json_delta fragments (the
	// arguments streamed as a JSON string) accumulated by block index, then
	// assembled and emitted at that block's content_block_stop.
	struct ToolAccum { std::string id; std::string name; std::string args; };
	std::map<int, ToolAccum> toolCalls;

	std::string line;
	try
	{
		while (std::getline(rs, line))
		{
			if (cancelled()) return;
			if (!line.empty() && line.back() == '\r') line.pop_back();
			if (line.rfind("data:", 0) != 0) continue;

			std::string payload = line.substr(5);
			std::size_t start = payload.find_first_not_of(" \t");
			if (start == std::string::npos) continue;
			payload = payload.substr(start);

			Object::Ptr pEvent;
			try
			{
				Parser parser;
				pEvent = parser.parse(payload).extract<Object::Ptr>();
			}
			catch (...) { continue; }  // skip a malformed frame
			if (!pEvent || !pEvent->has("type")) continue;
			std::string type = pEvent->getValue<std::string>("type");

			if (type == "content_block_delta")
			{
				Object::Ptr pDelta = pEvent->getObject("delta");
				if (!pDelta || !pDelta->has("type")) continue;
				std::string deltaType = pDelta->getValue<std::string>("type");

				// Text delta - emit immediately.
				if (deltaType == "text_delta" && pDelta->has("text"))
				{
					std::string text = pDelta->getValue<std::string>("text");
					if (!text.empty())
					{
						ContentEvent evt;
						evt.type = ContentEvent::TYPE_TEXT;
						evt.text = text;
						onEvent(evt);
					}
				}
				// Tool-argument delta - accumulate the JSON string by index.
				else if (deltaType == "input_json_delta" && pDelta->has("partial_json"))
				{
					int idx = pEvent->has("index") ? pEvent->getValue<int>("index") : 0;
					toolCalls[idx].args += pDelta->getValue<std::string>("partial_json");
				}
				// thinking_delta and other delta types are ignored.
			}
			else if (type == "content_block_start")
			{
				// A tool_use block opens with its id and name; text blocks need
				// no setup (their content arrives as text_delta).
				Object::Ptr pBlock = pEvent->getObject("content_block");
				if (pBlock && pBlock->has("type")
					&& pBlock->getValue<std::string>("type") == "tool_use")
				{
					int idx = pEvent->has("index") ? pEvent->getValue<int>("index") : 0;
					ToolAccum& acc = toolCalls[idx];
					if (pBlock->has("id")) acc.id = pBlock->getValue<std::string>("id");
					if (pBlock->has("name")) acc.name = pBlock->getValue<std::string>("name");
				}
			}
			else if (type == "content_block_stop")
			{
				// If this index accumulated a tool call, its arguments are now
				// complete - parse and emit it.
				int idx = pEvent->has("index") ? pEvent->getValue<int>("index") : 0;
				auto it = toolCalls.find(idx);
				if (it != toolCalls.end())
				{
					const ToolAccum& acc = it->second;
					ContentEvent evt;
					evt.type = ContentEvent::TYPE_TOOL_USE;
					evt.id = acc.id;
					evt.name = acc.name;
					if (!acc.args.empty())
					{
						try
						{
							Parser argParser;
							Var argResult = argParser.parse(acc.args);
							auto pArgs = argResult.extract<Object::Ptr>();
							if (pArgs) evt.input = *pArgs;
						}
						catch (...) { /* invalid JSON - pass empty input */ }
					}
					onEvent(evt);
					toolCalls.erase(it);
				}
			}
			else if (type == "error")
			{
				Object::Ptr pErr = pEvent->getObject("error");
				std::string msg = "unknown error";
				if (pErr && pErr->has("message")) msg = pErr->getValue<std::string>("message");
				ContentEvent evt;
				evt.type = ContentEvent::TYPE_ERROR;
				evt.text = "Anthropic stream error: " + msg;
				onEvent(evt);
				return;
			}
			else if (type == "message_stop")
			{
				break;
			}
			// message_start, message_delta, ping: nothing to emit.
		}
	}
	catch (const Poco::Exception& e)
	{
		if (cancelled()) return;  // user aborted mid-stream - stop quietly
		ContentEvent evt;
		evt.type = ContentEvent::TYPE_ERROR;
		evt.text = "Anthropic stream error: " + e.displayText();
		onEvent(evt);
		return;
	}
}


void AnthropicProvider::cancel()
{
	LLMProvider::cancel();
	std::lock_guard<std::mutex> lock(_sessionMutex);
	if (_pActiveSession)
	{
		_pActiveSession->abort();
	}
}


void AnthropicProvider::appendAssistantMessage(
	Poco::JSON::Array& messages,
	const std::string& text,
	const std::vector<ContentEvent>& toolCalls)
{
	Array::Ptr pContent = new Array;

	if (!text.empty())
	{
		Object::Ptr pTextBlock = new Object;
		pTextBlock->set("type", "text");
		pTextBlock->set("text", text);
		pContent->add(pTextBlock);
	}

	for (const auto& tc : toolCalls)
	{
		Object::Ptr pToolUseBlock = new Object;
		pToolUseBlock->set("type", "tool_use");
		pToolUseBlock->set("id", tc.id);
		pToolUseBlock->set("name", tc.name);
		pToolUseBlock->set("input", tc.input);
		pContent->add(pToolUseBlock);
	}

	Object::Ptr pMsg = new Object;
	pMsg->set("role", "assistant");
	pMsg->set("content", pContent);
	messages.add(pMsg);
}


void AnthropicProvider::appendToolResult(
	Poco::JSON::Array& messages,
	const std::string& toolCallId,
	const std::string& result)
{
	// Tool results go as a user message with tool_result blocks.
	// Check if last message is already a user message with tool_results.
	if (messages.size() > 0)
	{
		auto pLast = messages.getObject(messages.size() - 1);
		if (pLast && pLast->getValue<std::string>("role") == "user")
		{
			auto pContent = pLast->getArray("content");
			if (pContent && pContent->size() > 0)
			{
				auto pFirst = pContent->getObject(0);
				if (pFirst && pFirst->getValue<std::string>("type") == "tool_result")
				{
					Object::Ptr pToolResult = new Object;
					pToolResult->set("type", "tool_result");
					pToolResult->set("tool_use_id", toolCallId);
					pToolResult->set("content", result);
					pContent->add(pToolResult);
					return;
				}
			}
		}
	}

	Array::Ptr pContent = new Array;
	Object::Ptr pToolResult = new Object;
	pToolResult->set("type", "tool_result");
	pToolResult->set("tool_use_id", toolCallId);
	pToolResult->set("content", result);
	pContent->add(pToolResult);

	Object::Ptr pMsg = new Object;
	pMsg->set("role", "user");
	pMsg->set("content", pContent);
	messages.add(pMsg);
}


Poco::JSON::Array AnthropicProvider::toAnthropicTools(const Poco::JSON::Array& tools)
{
	Array result;
	for (std::size_t i = 0; i < tools.size(); ++i)
	{
		auto pTool = tools.getObject(i);
		Object::Ptr pAnthropicTool = new Object;
		pAnthropicTool->set("name", pTool->getValue<std::string>("name"));
		pAnthropicTool->set("description", pTool->getValue<std::string>("description"));
		if (pTool->has("parameters"))
		{
			pAnthropicTool->set("input_schema", pTool->get("parameters"));
		}
		// Cache control on the last tool marks the end of the cacheable prefix
		if (i == tools.size() - 1)
		{
			Object::Ptr pCacheControl = new Object;
			pCacheControl->set("type", "ephemeral");
			pAnthropicTool->set("cache_control", pCacheControl);
		}
		result.add(pAnthropicTool);
	}
	return result;
}


} } // namespace Poco::AI
