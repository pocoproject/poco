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
#include "Poco/SSH/SSHTypes.h"
#include "Poco/Logger.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include <atomic>
#include <chrono>
#include <map>
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

	static constexpr std::chrono::milliseconds DEFAULT_STOP_TIMEOUT = std::chrono::seconds(10);
		/// Default time limit for stop() to wait for active sessions.

	SSHServer(const SSHServerConfig& config, SessionFactory factory);
	~SSHServer();
		/// Stops the server. Waits without a time limit for sessions
		/// that are still running, because they use this object.

	void start();
		/// Starts listening for SSH connections.

	void stop(std::chrono::milliseconds timeout = DEFAULT_STOP_TIMEOUT);
		/// Stops the server, asks the active sessions to end and waits
		/// up to the given timeout for them to finish. A timeout that is
		/// too long for std::chrono::steady_clock, such as
		/// std::chrono::milliseconds::max(), sets no time limit.
		/// A zero or negative timeout does not wait for the sessions.
		///
		/// A session ends within SSHSession::STOP_CHECK_INTERVAL while it
		/// logs in, and while its handler waits only as
		/// SSHSession::stopRequested() describes. On POSIX systems, stop()
		/// also ends every other wait of a session for its client at once,
		/// for example a read without a timeout in its handler; on Windows,
		/// stop() itself does not end such a wait. On every system, a
		/// session does not end while its handler or the password
		/// authenticator of the configuration waits for something other
		/// than the client.
		///
		/// Throws a Poco::TimeoutException if sessions are still running
		/// after the timeout. Such sessions still use this object; calling
		/// stop() again waits for them again.

	[[nodiscard]] Poco::UInt16 port() const;
		/// Returns the port the server listens on, valid after start().
		/// If the configured port is 0, this is the port that the
		/// system chose.

private:
	struct ActiveSession
	{
		SSHSession* pSession;
		ssh_socket_t wakeFd;
	};

	void acceptLoop();
	void registerSession(ssh_session session, SSHSession* pSession, ssh_socket_t wakeFd);
	void unregisterSession(ssh_session session);
	void requestStopAll();

	SSHServerConfig _config;
	SessionFactory _sessionFactory;
	ssh_bind _sshBind;
	std::atomic<ssh_socket_t> _listenFd;
	Poco::UInt16 _port = 0;
	Poco::RunnableAdapter<SSHServer> _acceptAdapter;
	Poco::Thread _acceptThread;
	Poco::ThreadPool _threadPool;
	std::atomic<bool> _stopped;
	Poco::FastMutex _stopMutex;
	Poco::FastMutex _sessionsMutex;
	std::map<ssh_session, ActiveSession> _activeSessions;
	Poco::Event _noSessions;
	Poco::Logger& _logger;

	friend class SSHSession;
};


//
// inlines
//
inline Poco::UInt16 SSHServer::port() const
{
	return _port;
}


} } // namespace Poco::SSH


#endif // SSH_SSHServer_INCLUDED
