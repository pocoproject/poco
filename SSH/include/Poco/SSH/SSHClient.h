//
// SSHClient.h
//
// Library: SSH
// Package: SSH
// Module:  SSHClient
//
// Definition of the SSHClient class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHClient_INCLUDED
#define SSH_SSHClient_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/Types.h"
#include <libssh/libssh.h>
#include <string>


namespace Poco {
namespace SSH {


class SSH_API SSHClient
	/// SSH client for connecting to an SSH server.
	///
	/// Provides connect, authenticate (password or public key),
	/// and channel management. RAII — disconnects on destruction.
{
public:
	SSHClient();
	~SSHClient();

	void connect(const std::string& host, Poco::UInt16 port = 22);
		/// Connects to the SSH server at host:port.
		/// Performs key exchange automatically.

	bool authenticatePassword(const std::string& user, const std::string& password);
		/// Authenticates using username and password.
		/// Returns true on success.

	bool authenticatePublicKey(const std::string& user, const std::string& keyFile = "");
		/// Authenticates using public key.
		/// If keyFile is empty, tries default keys from ~/.ssh/.
		/// Returns true on success.

	ssh_channel openShellChannel();
		/// Opens a session channel and requests a shell.
		/// Returns the channel on success, nullptr on failure.
		/// Caller owns the channel and must free it.

	void disconnect();
		/// Disconnects from the server.

	bool isConnected() const;
		/// Returns true if connected.

	ssh_session session() const;
		/// Returns the underlying ssh_session.

private:
	SSHClient(const SSHClient&) = delete;
	SSHClient& operator=(const SSHClient&) = delete;

	ssh_session _session;
	bool _connected;
};


inline bool SSHClient::isConnected() const
{
	return _connected;
}


inline ssh_session SSHClient::session() const
{
	return _session;
}


} } // namespace Poco::SSH


#endif // SSH_SSHClient_INCLUDED
