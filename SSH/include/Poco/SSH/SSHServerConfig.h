//
// SSHServerConfig.h
//
// Library: SSH
// Package: SSH
// Module:  SSHServerConfig
//
// Configuration for the SSH server.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHServerConfig_INCLUDED
#define SSH_SSHServerConfig_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/Types.h"
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
		/// Each file is named by username (e.g., "admin").
		/// Empty string disables public key authentication.

	int maxConnections = 4;
		/// Maximum concurrent SSH sessions.

	int maxAuthAttempts = 3;
		/// Maximum failed authentication attempts before disconnect.

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
