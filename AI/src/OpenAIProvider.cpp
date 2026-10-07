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
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Format.h"
#include <algorithm>
#include <sstream>
#include <map>
#include <memory>
#include <utility>


using namespace Poco::Net;
using namespace Poco::JSON;
using Poco::Dynamic::Var;


namespace Poco {
namespace AI {


OpenAIProvider::OpenAIProvider(
	const std::string& baseUrl,
	const std::string& apiKey,
	const std::string& model,
	int maxTokens,
	int timeoutSeconds,
	bool supportsTools,
	SessionFactory sessionFactory):
	_baseUrl(baseUrl),
	_apiKey(apiKey),
	_model(model),
	_maxTokens(maxTokens),
	_timeoutSeconds(timeoutSeconds),
	_supportsTools(supportsTools),
	_sessionFactory(std::move(sessionFactory))
{
}


void OpenAIProvider::setModel(const std::string& model)
{
	_model = model;
}


std::unique_ptr<HTTPClientSession> OpenAIProvider::openSession(const Poco::URI& uri, long timeoutSeconds) const
{
	std::unique_ptr<HTTPClientSession> pSession = _sessionFactory ? _sessionFactory(uri) : createClientSession(uri);
	pSession->setTimeout(Poco::Timespan(timeoutSeconds, 0));
	return pSession;
}


void OpenAIProvider::chat(
	const Poco::JSON::Array& messages,
	const std::string& systemPrompt,
	const Poco::JSON::Array& tools,
	ContentEventCallback onEvent)
{
	// Build messages array with system prompt prepended
	Array apiMessages;
	if (!systemPrompt.empty())
	{
		Object::Ptr pSystemMsg = new Object;
		pSystemMsg->set("role", "system");
		pSystemMsg->set("content", systemPrompt);
		apiMessages.add(pSystemMsg);
	}
	for (std::size_t i = 0; i < messages.size(); ++i)
	{
		apiMessages.add(messages.get(i));
	}

	// Build request body
	Object body;
	body.set("model", _model);
	body.set("max_tokens", _maxTokens);
	body.set("messages", apiMessages);
	body.set("stream", true);

	if (_supportsTools && tools.size() > 0)
	{
		body.set("tools", toOpenAITools(tools));
	}

	std::ostringstream bodyStream;
	Stringifier::stringify(body, bodyStream);
	std::string bodyStr = bodyStream.str();

	Poco::URI uri(_baseUrl + "/chat/completions");
	std::unique_ptr<HTTPClientSession> pSession = openSession(uri, _timeoutSeconds);

	{
		std::lock_guard<std::mutex> lock(_sessionMutex);
		_pActiveSession = pSession.get();
	}

	// Guard to clear active session on exit
	struct SessionGuard
	{
		OpenAIProvider& self;
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
	if (!_apiKey.empty())
	{
		request.set("Authorization", "Bearer " + _apiKey);
	}
	request.setContentLength(bodyStr.size());

	std::ostream& os = pSession->sendRequest(request);
	os << bodyStr;

	HTTPResponse response;
	std::istream& rs = pSession->receiveResponse(response);

	if (response.getStatus() != HTTPResponse::HTTP_OK)
	{
		std::string responseBody;
		Poco::StreamCopier::copyToString(rs, responseBody);

		// If server doesn't support tools, retry without them
		if (response.getStatus() == HTTPResponse::HTTP_INTERNAL_SERVER_ERROR
			&& responseBody.find("Unsupported param: tools") != std::string::npos
			&& tools.size() > 0)
		{
			Poco::JSON::Array emptyTools;
			chat(messages, systemPrompt, emptyTools, onEvent);
			return;
		}
		ContentEvent evt;
		evt.type = ContentEvent::TYPE_ERROR;
		evt.text = Poco::format("OpenAI API error %d: %s",
			static_cast<int>(response.getStatus()), responseBody.substr(0, 500));
		if (response.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED
			|| response.getStatus() == HTTPResponse::HTTP_FORBIDDEN)
			evt.code = "unauthenticated";
		onEvent(evt);
		return;
	}

	// Stream the response as Chat Completions SSE. Each frame is a
	//   data: {chat.completion.chunk}
	// line, ending with data: [DONE]. Text arrives in choices[0].delta.content
	// and is emitted immediately as a text event; tool calls arrive fragmented
	// across deltas keyed by tool_calls[].index (id + function.name first, then
	// function.arguments streamed in pieces), so accumulate and emit each once
	// the stream ends.
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
			if (payload == "[DONE]") break;

			Object::Ptr pChunk;
			try
			{
				Parser parser;
				pChunk = parser.parse(payload).extract<Object::Ptr>();
			}
			catch (...) { continue; }  // skip a malformed frame
			if (!pChunk) continue;

			Array::Ptr pChoices = pChunk->getArray("choices");
			if (!pChoices || pChoices->size() == 0) continue;
			Object::Ptr pChoice = pChoices->getObject(0);
			if (!pChoice) continue;
			Object::Ptr pDelta = pChoice->getObject("delta");
			if (!pDelta) continue;

			// Text delta - emit immediately.
			if (pDelta->has("content") && !pDelta->isNull("content"))
			{
				std::string content = pDelta->getValue<std::string>("content");
				if (!content.empty())
				{
					ContentEvent evt;
					evt.type = ContentEvent::TYPE_TEXT;
					evt.text = content;
					onEvent(evt);
				}
			}

			// Tool-call deltas - accumulate by index.
			if (pDelta->has("tool_calls"))
			{
				Array::Ptr pTcs = pDelta->getArray("tool_calls");
				if (pTcs)
				{
					for (std::size_t i = 0; i < pTcs->size(); ++i)
					{
						Object::Ptr pTc = pTcs->getObject(i);
						if (!pTc) continue;
						int idx = pTc->has("index") ? pTc->getValue<int>("index") : 0;
						ToolAccum& acc = toolCalls[idx];
						if (pTc->has("id")) acc.id = pTc->getValue<std::string>("id");
						Object::Ptr pFn = pTc->getObject("function");
						if (pFn)
						{
							if (pFn->has("name")) acc.name = pFn->getValue<std::string>("name");
							if (pFn->has("arguments")) acc.args += pFn->getValue<std::string>("arguments");
						}
					}
				}
			}
		}
	}
	catch (const Poco::Exception& e)
	{
		if (cancelled()) return;  // user aborted mid-stream - stop quietly
		ContentEvent evt;
		evt.type = ContentEvent::TYPE_ERROR;
		evt.text = "OpenAI stream error: " + e.displayText();
		onEvent(evt);
		return;
	}

	if (cancelled()) return;  // aborted after a clean read - drop partial tool calls

	// Emit the assembled tool calls (arguments were streamed as a JSON string).
	for (const auto& kv : toolCalls)
	{
		const ToolAccum& acc = kv.second;
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
	}
}


std::vector<std::string> OpenAIProvider::listModels()
{
	std::vector<std::string> models;
	try
	{
		Poco::URI uri(_baseUrl + "/models");
		// Discovery runs while a user waits for the model list, so it uses
		// a short deadline of its own rather than the chat timeout.
		std::unique_ptr<HTTPClientSession> pSession = openSession(uri, 10);

		HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery(), HTTPMessage::HTTP_1_1);
		request.setHost(uri.getHost(), uri.getPort());
		if (!_apiKey.empty()) request.set("Authorization", "Bearer " + _apiKey);

		pSession->sendRequest(request);

		HTTPResponse response;
		std::istream& rs = pSession->receiveResponse(response);
		std::string responseBody;
		Poco::StreamCopier::copyToString(rs, responseBody);
		if (response.getStatus() != HTTPResponse::HTTP_OK) return models;

		Parser parser;
		Var parsed = parser.parse(responseBody);
		Object::Ptr pRoot = parsed.extract<Object::Ptr>();
		Poco::JSON::Array::Ptr pData = pRoot->getArray("data");
		if (!pData) return models;

		for (std::size_t i = 0; i < pData->size(); ++i)
		{
			Object::Ptr pModel = pData->getObject(static_cast<unsigned int>(i));
			if (!pModel) continue;
			const std::string id = pModel->optValue<std::string>("id", "");
			if (id.empty()) continue;
			// Embedding models share this listing but cannot serve a chat
			// request, and picking one would only produce a puzzling error.
			if (id.find("embed") != std::string::npos) continue;
			models.push_back(id);
		}
		std::sort(models.begin(), models.end());
	}
	catch (Poco::Exception&)
	{
		models.clear();
	}
	catch (std::exception&)
	{
		models.clear();
	}
	return models;
}


std::vector<std::vector<float>> OpenAIProvider::embed(
	const std::vector<std::string>& texts,
	const std::string& model,
	int dimensions)
{
	std::vector<std::vector<float>> result;
	if (texts.empty()) return result;

	Object body;
	body.set("model", model);
	Poco::JSON::Array input;
	for (const auto& t: texts) input.add(t);
	body.set("input", input);
	if (dimensions > 0) body.set("dimensions", dimensions);

	std::ostringstream bodyStream;
	Stringifier::stringify(body, bodyStream);
	const std::string bodyStr = bodyStream.str();

	Poco::URI uri(_baseUrl + "/embeddings");
	std::unique_ptr<HTTPClientSession> pSession = openSession(uri, _timeoutSeconds);

	HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery(), HTTPMessage::HTTP_1_1);
	request.setHost(uri.getHost(), uri.getPort());
	request.setContentType("application/json");
	if (!_apiKey.empty())
	{
		request.set("Authorization", "Bearer " + _apiKey);
	}
	request.setContentLength(bodyStr.size());

	pSession->sendRequest(request) << bodyStr;

	HTTPResponse response;
	std::istream& rs = pSession->receiveResponse(response);
	std::string responseBody;
	Poco::StreamCopier::copyToString(rs, responseBody);

	if (response.getStatus() != HTTPResponse::HTTP_OK)
	{
		throw Poco::RuntimeException(Poco::format("embeddings API error %d: %s",
			static_cast<int>(response.getStatus()), responseBody.substr(0, 500)));
	}

	try
	{
		Parser parser;
		Var parsed = parser.parse(responseBody);
		Object::Ptr pRoot = parsed.extract<Object::Ptr>();
		Poco::JSON::Array::Ptr pData = pRoot->getArray("data");
		if (!pData || pData->size() != texts.size())
			throw Poco::DataFormatException(Poco::format(
				"embeddings response has %z vectors for %z inputs",
				std::size_t(pData ? pData->size() : 0), texts.size()));

		// The API is free to reorder entries; index restores input order.
		result.resize(texts.size());
		for (std::size_t i = 0; i < pData->size(); ++i)
		{
			Object::Ptr pEntry = pData->getObject(static_cast<unsigned>(i));
			const std::size_t idx = pEntry->optValue<std::size_t>("index", i);
			if (idx >= result.size())
				throw Poco::DataFormatException("embeddings response index out of range");
			Poco::JSON::Array::Ptr pVec = pEntry->getArray("embedding");
			if (!pVec || pVec->size() == 0)
				throw Poco::DataFormatException("embeddings response entry has no embedding");
			std::vector<float>& out = result[idx];
			out.reserve(pVec->size());
			for (std::size_t j = 0; j < pVec->size(); ++j)
				out.push_back(static_cast<float>(pVec->getElement<double>(static_cast<unsigned>(j))));
		}
	}
	catch (Poco::DataFormatException&)
	{
		throw;
	}
	catch (Poco::Exception& ex)
	{
		throw Poco::DataFormatException("malformed embeddings response: " + ex.displayText());
	}
	return result;
}


void OpenAIProvider::cancel()
{
	LLMProvider::cancel();
	std::lock_guard<std::mutex> lock(_sessionMutex);
	if (_pActiveSession)
	{
		_pActiveSession->abort();
	}
}


void OpenAIProvider::appendAssistantMessage(
	Poco::JSON::Array& messages,
	const std::string& text,
	const std::vector<ContentEvent>& toolCalls)
{
	Object::Ptr pMsg = new Object;
	pMsg->set("role", "assistant");

	if (toolCalls.empty())
	{
		pMsg->set("content", text);
	}
	else
	{
		// content can be text or null alongside tool_calls
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

			// Serialize input object to JSON string
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


void OpenAIProvider::appendToolResult(
	Poco::JSON::Array& messages,
	const std::string& toolCallId,
	const std::string& result)
{
	// Each tool result is a separate message with role "tool"
	Object::Ptr pMsg = new Object;
	pMsg->set("role", "tool");
	pMsg->set("tool_call_id", toolCallId);
	pMsg->set("content", result);
	messages.add(pMsg);
}


Poco::JSON::Array OpenAIProvider::toOpenAITools(const Poco::JSON::Array& tools)
{
	Array result;
	for (std::size_t i = 0; i < tools.size(); ++i)
	{
		auto pTool = tools.getObject(i);

		Object::Ptr pFunction = new Object;
		pFunction->set("name", pTool->getValue<std::string>("name"));
		pFunction->set("description", pTool->getValue<std::string>("description"));
		if (pTool->has("parameters"))
		{
			pFunction->set("parameters", pTool->get("parameters"));
		}

		Object::Ptr pOpenAITool = new Object;
		pOpenAITool->set("type", "function");
		pOpenAITool->set("function", pFunction);
		result.add(pOpenAITool);
	}
	return result;
}


} } // namespace Poco::AI
