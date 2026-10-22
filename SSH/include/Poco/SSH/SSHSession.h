//
// SSHSession.h
//
// Library: SSH
// Package: SSH
// Module:  SSHSession
//
// Definition of the SSHSession class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHSession_INCLUDED
#define SSH_SSHSession_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/SSH/SSHServerConfig.h"
#include "Poco/SSH/SSHTypes.h"
#include "Poco/Logger.h"
#include "Poco/Runnable.h"
#include <atomic>
#include <chrono>
#include <optional>
#include <string>


namespace Poco {
namespace SSH {


class SSHServer;


class SSH_API SSHSession: public Poco::Runnable
	/// Abstract base class for SSH session handlers.
	///
	/// Subclasses implement handleSession() which is called after
	/// successful SSH authentication with an open channel.
	///
	/// The base class handles the SSH lifecycle: key exchange,
	/// authentication, channel negotiation, and cleanup.
	///
	/// Instances are created by SSHServer and run in a thread pool.
	/// Self-deletes at the end of run().
{
public:
	static constexpr std::chrono::milliseconds STOP_CHECK_INTERVAL = std::chrono::milliseconds(200);
		/// The longest time that a wait of the login, or of sshReadLine() or an
		/// SSHChannelStream given a stop check, lasts before the session checks
		/// whether the server stops it.

	SSHSession(
		ssh_session session,
		const SSHServerConfig& config,
		SSHServer& server);

	virtual ~SSHSession();

	void run() override;

protected:
	virtual void handleSession(ssh_channel channel, const std::string& username) = 0;
		/// Called after successful SSH authentication and channel setup.
		/// The channel is open and ready for I/O.
		/// The username is the authenticated user.
		/// Override in subclasses to implement session logic.
		/// SSHServer::stop() asks the handler to return, see stopRequested().

	const SSHServerConfig& config() const;

	[[nodiscard]] bool stopRequested() const;
		/// Returns true after SSHServer::stop() has asked the session to end.
		///
		/// A handleSession() that reads with sshReadLine() or writes through an
		/// SSHChannelStream passes it to them as the stop check. A handler that
		/// waits in another way checks it at least every STOP_CHECK_INTERVAL and
		/// returns when it is true. After sshReadLine() or a write of the stream
		/// has ended because of a stop, each wait of libssh for the client that
		/// uses the timeout of the session lasts at most STOP_CHECK_INTERVAL.
		/// Closing or freeing the channel makes up to two such waits.

private:
	bool authenticate(const std::optional<std::chrono::steady_clock::time_point>& loginDeadline);
	ssh_channel negotiateChannel(const std::optional<std::chrono::steady_clock::time_point>& loginDeadline);
	bool checkAuthorizedKey(const std::string& username, ssh_key clientKey);
	void requestStop();

	ssh_session _session;
	SSHServerConfig _config;
	SSHServer& _server;
	std::string _authenticatedUser;
	Poco::Logger& _logger;
	std::atomic<bool> _stopRequested{false};

	friend class SSHServer;
};


inline const SSHServerConfig& SSHSession::config() const
{
	return _config;
}


inline bool SSHSession::stopRequested() const
{
	return _stopRequested;
}


inline void SSHSession::requestStop()
{
	_stopRequested = true;
}


} } // namespace Poco::SSH


#endif // SSH_SSHSession_INCLUDED
