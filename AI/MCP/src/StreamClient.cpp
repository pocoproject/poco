//
// StreamClient.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  StreamClient
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/Exception.h"
#include <istream>
#include <ostream>


namespace Poco::AI::MCP {


StreamClient::StreamClient(std::istream& in, std::ostream& out, std::size_t maxMessageSize):
	_in(in),
	_out(out),
	_maxMessageSize(maxMessageSize)
{
}


StreamClient::~StreamClient()
{
}


Poco::JSON::Object::Ptr StreamClient::exchange(const Poco::JSON::Object::Ptr& request)
{
	const Id id = request->get("id");
	writeMessage(_out, request);
	for (;;)
	{
		Poco::JSON::Object::Ptr pMessage;
		try
		{
			pMessage = readMessage(_in, _maxMessageSize);
		}
		catch (const MCPException&)
		{
			// Not a message: passed over.
			continue;
		}
		if (pMessage == nullptr)
		{
			throw Poco::IOException("The MCP stream ended before the reply to " + request->getValue<std::string>("method"));
		}
		if (isReply(pMessage, id))
		{
			return pMessage;
		}
	}
}


} // namespace Poco::AI::MCP
