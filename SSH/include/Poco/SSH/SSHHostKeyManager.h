//
// SSHHostKeyManager.h
//
// Library: SSH
// Package: SSH
// Module:  SSHHostKeyManager
//
// SSH host key generation and loading.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHHostKeyManager_INCLUDED
#define SSH_SSHHostKeyManager_INCLUDED


#include "Poco/SSH/SSH.h"
#include <string>


namespace Poco {
namespace SSH {


class SSH_API SSHHostKeyManager
	/// Manages SSH host key lifecycle.
	///
	/// Ensures a host key exists at the configured path,
	/// generating an Ed25519 key if none is found.
{
public:
	static std::string ensureHostKey(
		const std::string& keyDir,
		const std::string& keyName = "ssh_host_ed25519_key");
		/// Ensures a host key exists at keyDir/keyName.
		/// Generates an Ed25519 key if missing.
		/// Returns the full path to the key file.

	static void generateKey(const std::string& path);
		/// Generates an Ed25519 private key and writes it to the given path.
		/// Sets file permissions to 0600.
};


} } // namespace Poco::SSH


#endif // SSH_SSHHostKeyManager_INCLUDED
