//
// MCPHost.h
//
// Library: AI
// Package: Agent
// Module:  MCPHost
//
// Connects to MCP servers over HTTP and mirrors their tools into a
// ToolRegistry, so an AgentLoop calls them like native tools.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_MCPHost_INCLUDED
#define AI_MCPHost_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/ToolRegistry.h"
#include "Poco/AI/SessionFactory.h"
#include "Poco/URI.h"
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace Poco {
namespace AI {


class AI_API MCPHost
	/// Owns live connections to MCP servers and registers their tools into
	/// ToolRegistry instances. Each registered tool's executor is bound to
	/// its connection, so dispatch needs no name-to-server lookup; an owner
	/// map is kept for diagnostics. The connections live as long as the
	/// host, so the host must outlive every registry it populated.
{
public:
	MCPHost();
		/// Creates a host with no connections.

	~MCPHost();
		/// Destroys the host and closes all connections.

	void addServer(const std::string& label, const Poco::URI& endpoint,
		const std::string& clientName, const std::string& clientVersion,
		SessionFactory sessionFactory = SessionFactory());
		/// Connects to the MCP server at endpoint, performs the handshake as
		/// clientName/clientVersion and caches the server's tool list. The
		/// sessions come from sessionFactory, or from createClientSession()
		/// when none is given (an https endpoint then needs
		/// Poco::Net::HTTPSSessionInstantiator registered by the application).
		///
		/// Throws a Poco::Exception when the server cannot be reached or
		/// refuses the handshake; the host is then unchanged, so one dead
		/// server need not sink the others.

	std::size_t registerInto(ToolRegistry& registry, std::vector<std::string>* pShadowed = nullptr);
		/// For every connected server, registers each of its tools under its
		/// bare name into registry, with an executor bound to that server. A
		/// name already present in registry (a native tool or another
		/// server's tool) is skipped and, when pShadowed is given, appended
		/// to it. Returns the number of tools registered.

	const std::map<std::string, std::string>& toolOwners() const;
		/// Maps each registered tool name to the label of the server that owns it.

	std::size_t serverCount() const;
		/// Returns the number of connected servers.

private:
	struct Connection;

	MCPHost(const MCPHost&) = delete;
	MCPHost& operator = (const MCPHost&) = delete;

	std::vector<std::shared_ptr<Connection>> _connections;
	std::map<std::string, std::string> _owners;
};


//
// inlines
//
inline const std::map<std::string, std::string>& MCPHost::toolOwners() const
{
	return _owners;
}


inline std::size_t MCPHost::serverCount() const
{
	return _connections.size();
}


} } // namespace Poco::AI


#endif // AI_MCPHost_INCLUDED
