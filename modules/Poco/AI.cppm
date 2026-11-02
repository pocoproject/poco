//
// AI.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_AI
#include "Poco/AI/AgentLoop.h"
#include "Poco/AI/AI.h"
#include "Poco/AI/AnthropicProvider.h"
#include "Poco/AI/Cancellation.h"
#include "Poco/AI/ChatEndpoint.h"
#include "Poco/AI/ChatService.h"
#include "Poco/AI/ChatSettings.h"
#include "Poco/AI/LLMProvider.h"
#include "Poco/AI/MCPHost.h"
#include "Poco/AI/OpenAIProvider.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/AI/ToolRegistry.h"
#endif

export module Poco:AI;

export namespace Poco::AI {
	#ifdef ENABLE_AI
	using Poco::AI::AgentEvent;
	using Poco::AI::AgentLoop;
	using Poco::AI::AnthropicProvider;
	using Poco::AI::Cancellation;
	using Poco::AI::ChatEndpoint;
	using Poco::AI::ChatRequest;
	using Poco::AI::ChatResult;
	using Poco::AI::ChatService;
	using Poco::AI::ChatSettings;
	using Poco::AI::ContentEvent;
	using Poco::AI::FailureCode;
	using Poco::AI::LLMProvider;
	using Poco::AI::MCPHost;
	using Poco::AI::OpenAIProvider;
	using Poco::AI::ProviderSettings;
	using Poco::AI::ProviderStatus;
	using Poco::AI::StopReason;
	using Poco::AI::ToolCall;
	using Poco::AI::ToolChoice;
	using Poco::AI::ToolRegistry;
	using Poco::AI::Usage;

	using Poco::AI::AgentEventCallback;
	using Poco::AI::ContentEventCallback;
	using Poco::AI::SessionFactory;
	using Poco::AI::ToolApprover;

	using Poco::AI::createClientSession;
	using Poco::AI::toolResultText;
	#endif
}
