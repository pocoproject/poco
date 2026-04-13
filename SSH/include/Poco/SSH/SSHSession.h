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
#include "Poco/Logger.h"
#include "Poco/Runnable.h"
#include <libssh/libssh.h>
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

	const SSHServerConfig& config() const;

private:
	bool authenticate();
	ssh_channel negotiateChannel();
	bool checkAuthorizedKey(const std::string& username, ssh_key clientKey);

	ssh_session _session;
	SSHServerConfig _config;
	SSHServer& _server;
	std::string _authenticatedUser;
	Poco::Logger& _logger;
};


inline const SSHServerConfig& SSHSession::config() const
{
	return _config;
}


} } // namespace Poco::SSH


#endif // SSH_SSHSession_INCLUDED
