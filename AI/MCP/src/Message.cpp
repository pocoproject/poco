//
// Message.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  Message
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Exception.h"
#include <istream>
#include <ostream>
#include <sstream>


namespace Poco::AI::MCP {


namespace
{
	enum class LineResult
	{
		LINE,
		TOO_LONG,
		END
	};


	LineResult readLine(std::istream& in, std::string& line, std::size_t maxSize)
		/// Reads one line without its terminator. A line longer than maxSize
		/// is skipped up to its end and reported as TOO_LONG.
	{
		line.clear();
		std::streambuf* pBuf = in.rdbuf();
		if (pBuf == nullptr || !in.good())
		{
			return LineResult::END;
		}
		bool tooLong = false;
		bool any = false;
		for (;;)
		{
			const int c = pBuf->sbumpc();
			if (c == std::char_traits<char>::eof())
			{
				in.setstate(std::ios::eofbit);
				if (!any) return LineResult::END;
				break;
			}
			any = true;
			if (c == '\n') break;
			if (tooLong) continue;
			if (line.size() >= maxSize)
			{
				tooLong = true;
				line.clear();
				continue;
			}
			line += static_cast<char>(c);
		}
		return tooLong ? LineResult::TOO_LONG : LineResult::LINE;
	}
}


Poco::JSON::Object::Ptr Request::make(const Id& id, const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr pRequest = new Poco::JSON::Object;
	pRequest->set("jsonrpc", JSONRPC_VERSION);
	pRequest->set("id", id);
	pRequest->set("method", method);
	if (params != nullptr)
	{
		pRequest->set("params", params);
	}
	return pRequest;
}


Poco::JSON::Object::Ptr Request::makeNotification(const std::string& method, Poco::JSON::Object::Ptr params)
{
	Poco::JSON::Object::Ptr pNotification = new Poco::JSON::Object;
	pNotification->set("jsonrpc", JSONRPC_VERSION);
	pNotification->set("method", method);
	if (params != nullptr)
	{
		pNotification->set("params", params);
	}
	return pNotification;
}


Poco::JSON::Object::Ptr Response::result(const Id& id, const Poco::Dynamic::Var& result)
{
	Poco::JSON::Object::Ptr pResponse = new Poco::JSON::Object;
	pResponse->set("jsonrpc", JSONRPC_VERSION);
	pResponse->set("id", id);
	pResponse->set("result", result);
	return pResponse;
}


Poco::JSON::Object::Ptr Response::error(const Id& id, ErrorCode code, const std::string& message, Poco::JSON::Object::Ptr data)
{
	Poco::JSON::Object::Ptr pError = new Poco::JSON::Object;
	pError->set("code", static_cast<int>(code));
	pError->set("message", message);
	if (data != nullptr)
	{
		pError->set("data", data);
	}

	Poco::JSON::Object::Ptr pResponse = new Poco::JSON::Object;
	pResponse->set("jsonrpc", JSONRPC_VERSION);
	pResponse->set("id", id);
	pResponse->set("error", pError);
	return pResponse;
}


bool isValidId(const Poco::Dynamic::Var& id)
{
	if (id.isEmpty()) return false;
	if (id.isString()) return true;
	// A boolean counts as an integer type to Dynamic::Var.
	return id.isInteger() && !id.isBoolean();
}


bool isResponseTo(const Poco::JSON::Object::Ptr& message, const Id& id)
{
	if (message == nullptr || !(message->has("result") || message->has("error"))) return false;
	const Poco::Dynamic::Var value = message->get("id");
	if (!isValidId(value) || !isValidId(id)) return false;
	try
	{
		if (id.isString()) return value.isString() && value.extract<std::string>() == id.extract<std::string>();
		return !value.isString() && value.convert<Poco::Int64>() == id.convert<Poco::Int64>();
	}
	catch (const Poco::Exception&)
	{
		return false;
	}
}


std::string serialize(const Poco::JSON::Object::Ptr& message)
{
	std::ostringstream os;
	// Default indent 0 produces a single compact line with no embedded newlines.
	Poco::JSON::Stringifier::stringify(message, os);
	return os.str();
}


void writeMessage(std::ostream& out, const Poco::JSON::Object::Ptr& message)
{
	out << serialize(message) << '\n';
	out.flush();
	if (!out.good())
	{
		throw Poco::WriteFileException("The MCP message stream is closed");
	}
}


Poco::JSON::Object::Ptr readMessage(std::istream& in, std::size_t maxSize)
{
	std::string line;
	for (;;)
	{
		const LineResult result = readLine(in, line, maxSize);
		if (result == LineResult::END)
		{
			return Poco::JSON::Object::Ptr();
		}
		if (result == LineResult::TOO_LONG)
		{
			throw MCPException("Message exceeds the size limit", static_cast<int>(ErrorCode::INVALID_REQUEST));
		}
		if (line.find_first_not_of(" \t\r") == std::string::npos)
		{
			continue;
		}
		Poco::Dynamic::Var parsed;
		try
		{
			Poco::JSON::Parser parser;
			parsed = parser.parse(line);
		}
		catch (const Poco::Exception& exc)
		{
			throw MCPException("Parse error", exc.message(), static_cast<int>(ErrorCode::PARSE_ERROR));
		}
		if (parsed.type() != typeid(Poco::JSON::Object::Ptr))
		{
			throw MCPException("Message is not a JSON object", static_cast<int>(ErrorCode::INVALID_REQUEST));
		}
		return parsed.extract<Poco::JSON::Object::Ptr>();
	}
}


} // namespace Poco::AI::MCP
