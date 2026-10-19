//
// MCPException.h
//
// Library: AI/MCP
// Package: Core
// Module:  MCPException
//
// Definition of the MCPException class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_MCPException_INCLUDED
#define AIMCP_MCPException_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/Exception.h"


namespace Poco::AI::MCP {


POCO_DECLARE_EXCEPTION(AIMCP_API, MCPException, Poco::RuntimeException)
	/// An MCP error. code() is the JSON-RPC error code (see ErrorCode):
	/// the code of an error reply a peer sent, or the code a reply
	/// to the message that caused the exception would carry.


POCO_DECLARE_EXCEPTION(AIMCP_API, MessageTooLargeException, MCPException)
	/// A message longer than the size limit of the reader. code() is
	/// INVALID_REQUEST, what a server replies to it.


} // namespace Poco::AI::MCP


#endif // AIMCP_MCPException_INCLUDED
