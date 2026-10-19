//
// MCPException.cpp
//
// Library: AI/MCP
// Package: Core
// Module:  MCPException
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/MCPException.h"
#include <typeinfo>


namespace Poco::AI::MCP {


POCO_IMPLEMENT_EXCEPTION(MCPException, Poco::RuntimeException, "MCP error")
POCO_IMPLEMENT_EXCEPTION(MessageTooLargeException, MCPException, "MCP message too large")


} // namespace Poco::AI::MCP
