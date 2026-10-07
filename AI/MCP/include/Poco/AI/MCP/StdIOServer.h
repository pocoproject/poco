//
// StdIOServer.h
//
// Library: AIMCP
// Package: Transport
// Module:  StdIOServer
//
// The MCP stdio transport: pump an MCP Server over a process's standard input
// and output (newline-delimited JSON). This is how local MCP clients launch
// and talk to a server subprocess.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_StdIOServer_INCLUDED
#define AIMCP_StdIOServer_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include <iosfwd>


namespace Poco {
namespace AI {
namespace MCP {


class Server;


class AIMCP_API StdIOServer
	/// Serves an MCP Server over standard input/output. In stdio mode stdout is
	/// reserved for protocol messages, so run() routes the root logger to stderr.
{
public:
	explicit StdIOServer(Server& server);
		/// Creates a stdio transport for the given server.

	~StdIOServer();
		/// Destroys the transport.

	void run();
		/// Routes logging to stderr, then serves over std::cin/std::cout until EOF.

	void run(std::istream& in, std::ostream& out);
		/// Serves over the given streams until EOF (does not touch logging). Used
		/// for testing and for embedding the transport over non-standard streams.

private:
	StdIOServer(const StdIOServer&) = delete;
	StdIOServer& operator = (const StdIOServer&) = delete;

	Server& _server;
};


} } } // namespace Poco::AI::MCP


#endif // AIMCP_StdIOServer_INCLUDED
