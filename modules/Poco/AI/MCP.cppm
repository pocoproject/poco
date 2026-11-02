//
// MCP.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_AI_MCP
#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/Implementation.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/ProcessClient.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/ToolResult.h"
#endif

export module Poco:AI.MCP;

export namespace Poco::AI::MCP {
	#ifdef ENABLE_AI_MCP
	using Poco::AI::MCP::Client;
	using Poco::AI::MCP::ErrorCode;
	using Poco::AI::MCP::Identity;
	using Poco::AI::MCP::Implementation;
	using Poco::AI::MCP::MCPException;
	using Poco::AI::MCP::MessageTooLargeException;
	using Poco::AI::MCP::ProcessClient;
	using Poco::AI::MCP::Prompt;
	using Poco::AI::MCP::PromptArgument;
	using Poco::AI::MCP::Request;
	using Poco::AI::MCP::RequestContext;
	using Poco::AI::MCP::Resource;
	using Poco::AI::MCP::ResourceContents;
	using Poco::AI::MCP::Response;
	using Poco::AI::MCP::Server;
	using Poco::AI::MCP::ServerDescription;
	using Poco::AI::MCP::StreamClient;
	using Poco::AI::MCP::Tool;
	using Poco::AI::MCP::ToolResult;

	using Poco::AI::MCP::ContextToolHandler;
	using Poco::AI::MCP::Id;
	using Poco::AI::MCP::PromptRenderer;
	using Poco::AI::MCP::ResourceReader;
	using Poco::AI::MCP::ToolHandler;

	using Poco::AI::MCP::DEFAULT_MAX_MESSAGE_SIZE;
	using Poco::AI::MCP::JSONRPC_VERSION;
	using Poco::AI::MCP::META_CLIENT_CAPABILITIES;
	using Poco::AI::MCP::META_CLIENT_INFO;
	using Poco::AI::MCP::META_PROTOCOL_VERSION;
	using Poco::AI::MCP::META_SERVER_INFO;
	using Poco::AI::MCP::PROTOCOL_VERSION;

	using Poco::AI::MCP::isResponseTo;
	using Poco::AI::MCP::isValidId;
	using Poco::AI::MCP::parseMessage;
	using Poco::AI::MCP::readMessage;
	using Poco::AI::MCP::sameId;
	using Poco::AI::MCP::serialize;
	using Poco::AI::MCP::writeMessage;
	#endif
}
