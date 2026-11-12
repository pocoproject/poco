//
// MCP.h
//
// Library: AI/MCP
// Package: Core
// Module:  MCP
//
// Basic definitions for the AI MCP (Model Context Protocol) library.
// This file must be the first file included by every other AI MCP header.
//
// The core library is the protocol implementation of MCP revision 2026-07-28:
// the server with its message and stream ingress, the client over any
// transport, the stream client and the stdio transport to a server process.
// It depends only on Poco::JSON and Foundation (no Poco::Net). The
// Streamable HTTP transport, client and server, lives in the separate
// PocoAIMCPHTTP library.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_MCP_INCLUDED
#define AIMCP_MCP_INCLUDED


#include "Poco/Foundation.h"
#include <cstddef>


//
// AIMCP_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define AIMCP_API POCO_API(AIMCP_EXPORTS)


//
// Automatically link MCP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AIMCP_EXPORTS)
		#pragma comment(lib, "PocoAIMCP" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::AI::MCP {


//
// Protocol constants. Compile-time (inline constexpr): a const std::string here
// would be dynamically initialized once per translation unit including this
// header. Character arrays rather than std::string_view so every existing use
// (Dynamic::Var construction, CppUnit assertEqual, string comparisons) keeps
// working unchanged.
//
inline constexpr char JSONRPC_VERSION[] = "2.0";
	/// The JSON-RPC version every MCP message carries.

inline constexpr char PROTOCOL_VERSION[] = "2026-07-28";
	/// The MCP protocol revision this library implements: the only one it
	/// sends and the only one it accepts. The revision is stateless: every
	/// request names the revision and the client's capabilities in its _meta
	/// object, and there is no initialization handshake and no session.

inline constexpr char META_PROTOCOL_VERSION[] = "io.modelcontextprotocol/protocolVersion";
	/// The key of the protocol revision in the _meta object of a request.

inline constexpr char META_CLIENT_CAPABILITIES[] = "io.modelcontextprotocol/clientCapabilities";
	/// The key of the client's capabilities in the _meta object of a request.

inline constexpr char META_CLIENT_INFO[] = "io.modelcontextprotocol/clientInfo";
	/// The key of the client's name and version in the _meta object of a request.

inline constexpr char META_SERVER_INFO[] = "io.modelcontextprotocol/serverInfo";
	/// The key of the server's name and version in the _meta object of a result.

inline constexpr std::size_t DEFAULT_MAX_MESSAGE_SIZE = 16*1024*1024;
	/// The default limit, in bytes, of one message read from a peer.


} // namespace Poco::AI::MCP


#endif // AIMCP_MCP_INCLUDED
