//
// MCPHost.h
//
// Library: AI
// Package: Agent
// Module:  MCPHost
//
// Connects to MCP servers and mirrors their tools into a ToolRegistry, so
// an AgentLoop calls them like native tools.
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
#include "Poco/AI/MCP/Client.h"
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace Poco::AI {


class AI_API MCPHost
	/// Hosts the tools of MCP servers for an agent: addServer() takes a client
	/// of any transport (Poco::AI::MCP::ProcessClient for a server process,
	/// Poco::AI::MCP::HTTP::HTTPClient for a remote one), discovers the server
	/// and fetches its tools; registerInto() registers them in a ToolRegistry,
	/// each running its call through its server's client. A tool result keeps
	/// what the server returned: its content, its structured content and its
	/// error flag.
	///
	/// A tool checks the cancelled() of its request context before it calls
	/// its server; a call already sent runs to its end, or to the timeout of
	/// the client.
	///
	/// What a server returns is untrusted input: tool descriptions,
	/// annotations and results. A tool runs with input the model chose; see
	/// AgentLoop::setApprover().
	///
	/// A host is set up before its tools run: addServer(), setNaming() and
	/// registerInto() are not called while they may run.
{
public:
	enum class Naming
		/// The names tools are registered under.
	{
		REMOTE,  /// The names the servers give them.
		PREFIXED /// "<label>_<name>", with each character the model APIs refuse replaced by '_':
		         /// tools of several servers keep apart, and names with a dot become usable.
	};

	MCPHost();
		/// Creates a host without servers.

	~MCPHost();
		/// Destroys the host. Tools registered from it keep their clients alive.

	void addServer(const std::string& label, std::unique_ptr<Poco::AI::MCP::Client> pClient);
		/// Discovers the server behind pClient and fetches its tools, all
		/// pages, when it has the tools capability. On failure the host is
		/// unchanged and the exception is passed on, e.g. a
		/// Poco::AI::MCP::MCPException or the transport's.
		///
		/// Throws Poco::InvalidArgumentException if pClient is null or the
		/// label is taken.

	void setNaming(Naming naming);
		/// Sets the names registerInto() gives the tools; REMOTE by default.

	[[nodiscard]] Naming getNaming() const;
		/// Returns the names registerInto() gives the tools.

	std::size_t registerInto(ToolRegistry& registry, std::vector<std::string>* pSkipped = nullptr);
		/// Registers the tools of every server under the names of the naming
		/// and returns how many it registered. A tool is skipped when its name
		/// is not 1 to 64 characters from A-Z, a-z, 0-9, '_' and '-', which the
		/// model APIs refuse, or when the registry has a tool of that name;
		/// each skipped tool is added to pSkipped, if given, with its server
		/// and the reason, naming the owner of the name when it is a server
		/// of this host.

	[[nodiscard]] const std::map<std::string, std::string>& toolOwners() const;
		/// Returns the label of the server of every tool registerInto() registered.

	[[nodiscard]] std::size_t serverCount() const;
		/// Returns the number of servers.

private:
	struct Connection;

	MCPHost(const MCPHost&) = delete;
	MCPHost& operator = (const MCPHost&) = delete;

	std::vector<std::shared_ptr<Connection>> _connections;
	std::map<std::string, std::string> _owners;
	Naming _naming;
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


inline MCPHost::Naming MCPHost::getNaming() const
{
	return _naming;
}


} // namespace Poco::AI


#endif // AI_MCPHost_INCLUDED
