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
#include <libssh/server.h>

#ifndef POCO_OS_FAMILY_WINDOWS
#include <poll.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif


namespace Poco {
namespace SSH {


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
	stop();
}


void SSHServer::start()
{
	_sshBind = ssh_bind_new();
	if (!_sshBind)
		throw SSHException("Failed to create SSH bind");

	int port = _config.port;
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_BINDADDR, _config.bindAddress.c_str());
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_BINDPORT, &port);
	ssh_bind_options_set(_sshBind, SSH_BIND_OPTIONS_HOSTKEY, _config.hostKeyPath.c_str());

	if (ssh_bind_listen(_sshBind) < 0)
	{
		std::string err = ssh_get_error(_sshBind);
		ssh_bind_free(_sshBind);
		_sshBind = nullptr;
		throw SSHConnectionException("SSH bind listen failed: " + err);
	}

	_listenFd = ssh_bind_get_fd(_sshBind);

	_logger.information("SSH server listening on %s:%d", _config.bindAddress, (int)_config.port);

	_stopped = false;
	_acceptThread.start(_acceptAdapter);
}


void SSHServer::stop()
{
	if (_stopped.exchange(true))
		return;

	_logger.information("SSH server stopping");

	// 1. Close the listening socket to unblock poll()/accept()
	ssh_socket_t fd = _listenFd.exchange(SSH_INVALID_FD);
	if (fd != SSH_INVALID_FD)
	{
#ifdef POCO_OS_FAMILY_WINDOWS
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

	// 3. Force-close active session sockets so session threads unblock
	disconnectAllSessions();

	// 4. Wait for all session threads to finish cleanup and unregister
	{
		bool hasSessions = false;
		{
			Poco::FastMutex::ScopedLock lock(_sessionsMutex);
			hasSessions = !_activeSessions.empty();
			if (hasSessions)
				_noSessions.reset();
		}
		if (hasSessions)
			_noSessions.wait();
	}

	// 5. Wait for thread pool threads to return
	_threadPool.joinAll();

	// 6. Free the bind (safe — no threads reference it)
	if (_sshBind)
	{
		ssh_bind_free(_sshBind);
		_sshBind = nullptr;
	}

	_logger.information("SSH server stopped");
}


void SSHServer::registerSession(ssh_session session)
{
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	_activeSessions.insert(session);
}


void SSHServer::unregisterSession(ssh_session session)
{
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	_activeSessions.erase(session);
	if (_activeSessions.empty())
		_noSessions.set();
}


void SSHServer::disconnectAllSessions()
{
	Poco::FastMutex::ScopedLock lock(_sessionsMutex);
	if (!_activeSessions.empty())
		_logger.information("Disconnecting %z active SSH session(s)", _activeSessions.size());
	for (ssh_session session : _activeSessions)
	{
		socket_t fd = ssh_get_fd(session);
		if (fd != SSH_INVALID_SOCKET)
		{
			::shutdown(fd, 2);
#ifdef POCO_OS_FAMILY_WINDOWS
			::closesocket(fd);
#endif
		}
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

#ifdef POCO_OS_FAMILY_WINDOWS
		WSAPOLLFD pfd;
#else
		struct pollfd pfd;
#endif
		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

#ifdef POCO_OS_FAMILY_WINDOWS
		int pollRc = WSAPoll(&pfd, 1, 500);
#else
		int pollRc = ::poll(&pfd, 1, 500);
#endif
		if (pollRc == 0) // timeout
			continue;
		if (pollRc < 0 || _stopped)
			break;
		if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL))
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
		try
		{
			pSession = _sessionFactory(session, _config, *this);
			_threadPool.start(*pSession);
		}
		catch (Poco::NoThreadAvailableException&)
		{
			_logger.warning("SSH connection rejected: max connections reached");
			delete pSession; // SSHSession destructor does not free ssh_session (run() does)
			ssh_disconnect(session);
			ssh_free(session);
		}
		catch (...)
		{
			delete pSession;
			ssh_disconnect(session);
			ssh_free(session);
		}
	}

	if (_logger.trace())
		_logger.trace("Accept loop ended");
}


} } // namespace Poco::SSH
