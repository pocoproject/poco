//
// SSHServer.h
//
// Library: SSH
// Package: SSH
// Module:  SSHServer
//
// Definition of the SSHServer class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHServer_INCLUDED
#define SSH_SSHServer_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/SSH/SSHServerConfig.h"
#include "Poco/Logger.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <atomic>
#include <set>
#include <functional>

#if defined(_WIN32)
#include <winsock2.h>
using ssh_socket_t = SOCKET;
#define SSH_INVALID_FD INVALID_SOCKET
#else
using ssh_socket_t = int;
#define SSH_INVALID_FD (-1)
#endif


namespace Poco {
namespace SSH {


class SSHSession;


class SSH_API SSHServer
	/// SSH server that listens for connections and delegates
	/// each session to an SSHSession subclass via a factory.
	///
	/// The server handles the ssh_bind lifecycle, the accept loop,
	/// and a thread pool for concurrent sessions.
{
public:
	using SessionFactory = std::function<SSHSession*(ssh_session, const SSHServerConfig&, SSHServer&)>;
		/// Factory function that creates an SSHSession for each accepted connection.

	SSHServer(const SSHServerConfig& config, SessionFactory factory);
	~SSHServer();

	void start();
		/// Starts listening for SSH connections.

	void stop();
		/// Stops the server and waits for active sessions to finish.

	void registerSession(ssh_session session);
		/// Register an active session for tracking.

	void unregisterSession(ssh_session session);
		/// Unregister a session on exit.

private:
	void acceptLoop();
	void disconnectAllSessions();

	SSHServerConfig _config;
	SessionFactory _sessionFactory;
	ssh_bind _sshBind;
	std::atomic<ssh_socket_t> _listenFd;
	Poco::RunnableAdapter<SSHServer> _acceptAdapter;
	Poco::Thread _acceptThread;
	Poco::ThreadPool _threadPool;
	std::atomic<bool> _stopped;
	Poco::FastMutex _sessionsMutex;
	std::set<ssh_session> _activeSessions;
	Poco::Event _noSessions;
	Poco::Logger& _logger;
};


} } // namespace Poco::SSH


#endif // SSH_SSHServer_INCLUDED
