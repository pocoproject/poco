//
// SSHServerConfig.h
//
// Library: SSH
// Package: SSH
// Module:  SSHServerConfig
//
// Configuration for the SSH server.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHServerConfig_INCLUDED
#define SSH_SSHServerConfig_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/Types.h"
#include <chrono>
#include <string>
#include <functional>


namespace Poco {
namespace SSH {


struct SSH_API SSHServerConfig
	/// Configuration for the SSH server.
{
	std::string bindAddress = "localhost";
		/// Network interface to listen on.

	Poco::UInt16 port = 22023;
		/// SSH listen port.

	std::string hostKeyPath;
		/// Path to the SSH host key file (Ed25519).
		/// Generated automatically if missing.

	std::string authorizedKeysDir;
		/// Directory containing per-user authorized_keys files.
		/// The key file of a user is the entry of this directory whose name
		/// equals the user name exactly (e.g., "admin"), compared byte for
		/// byte with the UTF-8 entry names, so names that the file system
		/// treats as the same file (other case, other Unicode normalisation,
		/// short names) do not share a key file. Public key authentication
		/// refuses a user name that is not a single file name: an empty
		/// name, "." and "..", and a name that contains "/", "\", ":", NUL
		/// or another ASCII control character.
		/// Empty string disables public key authentication.

	int maxConnections = 4;
		/// Maximum concurrent SSH sessions.

	int maxAuthAttempts = 3;
		/// Maximum failed authentication attempts before disconnect.

	std::chrono::milliseconds loginGraceTime = std::chrono::seconds(120);
		/// Time limit for a client to complete the key exchange, the
		/// authentication, the opening of the session channel and its
		/// shell or exec request. It starts when the thread of the session
		/// starts, right after the server has accepted the connection. The
		/// server closes a session that has not opened its session channel
		/// and sent its shell or exec request within this time. A password
		/// authenticator that has not returned delays the close until it returns.
		/// Zero or a negative value sets no time limit.

	bool enablePasswordAuth = true;
		/// Allow SSH password authentication.

	bool enablePubkeyAuth = true;
		/// Allow SSH public key authentication.

	using PasswordAuthenticator = std::function<bool(const std::string& username, const std::string& password)>;
		/// Callback for password authentication.
		/// Returns true if credentials are valid.

	PasswordAuthenticator passwordAuthenticator;
		/// The password authenticator callback.
		/// If not set, password auth is rejected even if enablePasswordAuth is true.
};


} } // namespace Poco::SSH


#endif // SSH_SSHServerConfig_INCLUDED
