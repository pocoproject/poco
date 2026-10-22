//
// SSHServer.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHServer
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHServer.h"
#include "Poco/SSH/SSHSession.h"
#include "Poco/SSH/SSHException.h"
#include "SSHDeadline.h"
#include "Poco/Error.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <libssh/libssh.h>
#include <libssh/server.h>

#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


namespace Poco {
namespace SSH {


namespace {


[[nodiscard]] Poco::UInt16 boundPort(ssh_socket_t fd)
{
	struct sockaddr_storage addr{};
	socklen_t length = sizeof(addr);
	if (::getsockname(fd, reinterpret_cast<struct sockaddr*>(&addr), &length) != 0)
		throw SSHConnectionException("Cannot determine the port of the SSH server socket");
	if (addr.ss_family == AF_INET6)
		return ntohs(reinterpret_cast<const struct sockaddr_in6*>(&addr)->sin6_port);
	return ntohs(reinterpret_cast<const struct sockaddr_in*>(&addr)->sin_port);
}


void closeWakeFd([[maybe_unused]] ssh_socket_t wakeFd)
	/// Closes the descriptor with which stop() wakes a session. There is none on Windows.
{
#if !defined(POCO_OS_FAMILY_WINDOWS)
	if (wakeFd != SSH_INVALID_FD)
		::close(wakeFd);
#endif
}


void setNonBlocking(ssh_socket_t fd)
	/// Makes the socket non-blocking. Throws an SSHConnectionException if that fails.
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	u_long nonBlocking = 1;
	const bool done = ::ioctlsocket(fd, FIONBIO, &nonBlocking) == 0;
#else
	const int flags = ::fcntl(fd, F_GETFL);
	const bool done = flags != -1 && ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
#endif
	if (!done)
		throw SSHConnectionException("cannot make its socket non-blocking: " + Poco::Error::getLastMessage());
}


void discardSession(ssh_session session, SSHSession* pSession, ssh_socket_t wakeFd)
	/// Closes and frees a connection that the server rejects, with its session object and wake descriptor.
{
	closeWakeFd(wakeFd);
	delete pSession; // SSHSession destructor does not free ssh_session (run() does)
	ssh_disconnect(session);
	ssh_free(session);
}


} // namespace


SSHServer::SSHServer(const SSHServerConfig& config, SessionFactory factory):
	_config(config),
	_sessionFactory(std::move(factory)),
	_sshBind(nullptr),
	_listenFd(SSH_INVALID_FD),
	_acceptAdapter(*this, &SSHServer::acceptLoop),
	_threadPool(1, config.maxConnections),
	_stopped(false),
	_logger(Poco::Logger::get("Poco.SSH.SSHServer"))
{
}


SSHServer::~SSHServer()
{
	// Sessions that are still running use this object, so it is not destroyed before they have ended.
	bool warned = false;
	while (true)
	{
		try
		{
			stop();
			break;
		}
		catch (Poco::TimeoutException& exc)
		{
			// Every round reports the same sessions, so only the first report is a warning.
			if (warned)
				_logger.debug("%s; waiting for them to end", exc.message());
			else
				_logger.warning("%s; waiting for them to end", exc.message());
			warned = true;
		}
		catch (...)
		{
			poco_unexpected();
			break;
		}
	}
}


void SSHServer::start()
{
	_sshBind = ssh_bind_new();
	if (!_sshBind)
		throw SSHException("Failed to create SSH bind");

	int bindPort = _config.port;
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_BINDADDR, _config.bindAddress.c_str());
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_BINDPORT, &bindPort);
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_HOSTKEY, _config.hostKeyPath.c_str());

	if (ssh_bind_listen(_sshBind) < 0)
	{
		std::string err = ssh_get_error(_sshBind);
		ssh_bind_free(_sshBind);
		_sshBind = nullptr;
		throw SSHConnectionException("SSH bind listen failed: " + err);
	}

	_listenFd = ssh_bind_get_fd(_sshBind);
	_port = boundPort(_listenFd);

	_logger.information("SSH server listening on %s:%hu", _config.bindAddress, _port);

	_stopped = false;
	_acceptThread.start(_acceptAdapter);
}


void SSHServer::stop(std::chrono::milliseconds timeout)
{
	Poco::FastMutex::ScopedLock stopLock(_stopMutex);

	// After a stop that timed out, the bind is kept until the remaining sessions have ended.
	const bool wasStopped = _stopped.exchange(true);
	if (wasStopped && _sshBind == nullptr)
		return;
	if (!wasStopped)
		_logger.information("SSH server stopping");

	// 1. Close the listening socket to unblock poll()/accept()
	ssh_socket_t fd = _listenFd.exchange(SSH_INVALID_FD);
	if (fd != SSH_INVALID_FD)
	{
#if defined(POCO_OS_FAMILY_WINDOWS)
		::closesocket(fd);
#else
		::close(fd);
#endif
		// Prevent ssh_bind_free from double-closing the fd
		ssh_bind_set_fd(_sshBind, SSH_INVALID_SOCKET);
	}

	// 2. Wait for the accept loop thread to exit
	if (_acceptThread.isRunning())
	{
		_acceptThread.join();
	}

	// 3. Ask the active sessions to end.
	// A repeated stop() finds only sessions that the first one asked.
	if (!wasStopped)
		requestStopAll();

	// 4. Wait for all session threads to finish cleanup and unregister
	const auto deadline = deadlineAfter(timeout);
	while (true)
	{
		std::size_t sessionCount = 0;
		{
			Poco::FastMutex::ScopedLock lock(_sessionsMutex);
			sessionCount = _activeSessions.size();
			if (sessionCount > 0)
				_noSessions.reset();
		}
		if (sessionCount == 0)
			break;

		const auto waitTime = remainingWait(deadline);
		if (waitTime == std::chrono::milliseconds::zero())
		{
			throw Poco::TimeoutException(Poco::format(
				"SSH server stop: %z session(s) still running after %?d ms", sessionCount, timeout.count()));
		}
		// Event::tryWait() takes a long, which has 32 bits on Windows; a longer wait takes several iterations.
		_noSessions.tryWait(static_cast<long>(waitTime.count()));
	}

	// 5. Wait for thread pool threads to return
	_threadPool.joinAll();

	// 6. Free the bind (safe - no threads reference it)
	if (_sshBind)
	{
		ssh_bind_free(_sshBind);
		_sshBind = nullptr;
	}

	_logger.information("SSH server stopped");
}


void SSHServer::registerSession(ssh_session session, SSHSession* pSession, ssh_socket_t wakeFd)
{
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	_activeSessions.emplace(session, ActiveSession{pSession, wakeFd});
}


void SSHServer::unregisterSession(ssh_session session)
{
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	// Closed under the lock, so that requestStopAll() never shuts down a closed or reused descriptor
	if (const auto it = _activeSessions.find(session); it != _activeSessions.end())
	{
		closeWakeFd(it->second.wakeFd);
		_activeSessions.erase(it);
	}
	if (_activeSessions.empty())
		_noSessions.set();
}


void SSHServer::requestStopAll()
{
	// Only the thread of a session closes its socket, which libssh uses without a lock
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	if (!_activeSessions.empty())
		_logger.information("Stopping %z active SSH session(s)", _activeSessions.size());
	for (const auto& [session, activeSession]: _activeSessions)
	{
		activeSession.pSession->requestStop();
#if !defined(POCO_OS_FAMILY_WINDOWS)
		::shutdown(activeSession.wakeFd, SHUT_RDWR);
#endif
	}
}


void SSHServer::acceptLoop()
{
	if (_logger.trace())
		_logger.trace("Accept loop started");

	while (!_stopped)
	{
		// Poll the listening socket so we can check _stopped periodically
		ssh_socket_t fd = _listenFd.load();
		if (fd == SSH_INVALID_FD)
			break;

#if defined(POCO_OS_FAMILY_WINDOWS)
		WSAPOLLFD pfd{};
#else
		struct pollfd pfd{};
#endif
		pfd.fd = fd;
		pfd.events = POLLIN;

#if defined(POCO_OS_FAMILY_WINDOWS)
		int pollRc = WSAPoll(&pfd, 1, 500);
#else
		int pollRc = ::poll(&pfd, 1, 500);
#endif
		if (pollRc == 0) // timeout
			continue;
		if (pollRc < 0)
		{
#if !defined(POCO_OS_FAMILY_WINDOWS)
			if (errno == EINTR) continue; // restart on signal
#endif
			break;
		}
		if (_stopped)
			break;
		if ((pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0)
			break;

		ssh_session session = ssh_new();
		if (!session)
		{
			if (!_stopped)
				_logger.error("Failed to create SSH session");
			break;
		}

		if (_logger.trace())
			_logger.trace("Waiting for connection");

		int rc = ssh_bind_accept(_sshBind, session);
		if (rc != SSH_OK)
		{
			ssh_free(session);
			if (!_stopped)
				_logger.error("SSH accept failed: %s", std::string(ssh_get_error(_sshBind)));
			break;
		}

		if (_logger.trace())
			_logger.trace("Connection accepted");

		SSHSession* pSession = nullptr;
		ssh_socket_t wakeFd = SSH_INVALID_FD;
		try
		{
			// libssh then keeps what the socket does not take and waits for the socket with the session timeout,
			// instead of in send() without a time limit. No other thread uses the session yet.
			setNonBlocking(ssh_get_fd(session));
			pSession = _sessionFactory(session, _config, *this);
#if !defined(POCO_OS_FAMILY_WINDOWS)
			// Shutting down this duplicate ends every wait of the session thread for its client at once. libssh
			// never closes it, so its number is not reused while the session is registered.
			wakeFd = ::fcntl(ssh_get_fd(session), F_DUPFD_CLOEXEC, 0);
			if (wakeFd == SSH_INVALID_FD)
				throw SSHConnectionException("cannot duplicate its socket: " + Poco::Error::getLastMessage());
#endif
			// Registered before its thread starts: stop() ends this loop and then asks the registered sessions to end
			registerSession(session, pSession, wakeFd);
			// unregisterSession() closes it from here on
			wakeFd = SSH_INVALID_FD;
			_threadPool.start(*pSession);
		}
		catch (Poco::NoThreadAvailableException&)
		{
			_logger.warning("SSH connection rejected: max connections reached");
			unregisterSession(session);
			discardSession(session, pSession, wakeFd);
		}
		catch (SSHConnectionException& exc)
		{
			_logger.warning("SSH connection rejected: %s", exc.message());
			unregisterSession(session);
			discardSession(session, pSession, wakeFd);
		}
		catch (...)
		{
			unregisterSession(session);
			discardSession(session, pSession, wakeFd);
		}
	}

	if (_logger.trace())
		_logger.trace("Accept loop ended");
}


} } // namespace Poco::SSH
