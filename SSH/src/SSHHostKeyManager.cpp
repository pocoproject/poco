//
// SSHHostKeyManager.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHHostKeyManager
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHHostKeyManager.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <libssh/libssh.h>
#include <libssh/libssh_version.h>

#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <sys/stat.h>
#endif


namespace Poco {
namespace SSH {


std::string SSHHostKeyManager::ensureHostKey(const std::string& keyDir, const std::string& keyName)
{
	Poco::Path keyPath(keyDir);
	keyPath.append(keyName);
	std::string fullPath = keyPath.toString();

	Poco::File keyFile(fullPath);
	if (keyFile.exists() && keyFile.getSize() > 0)
	{
		// Verify the key loads successfully
		ssh_key key = nullptr;
		int rc = ssh_pki_import_privkey_file(fullPath.c_str(), nullptr, nullptr, nullptr, &key);
		if (rc == SSH_OK && key)
		{
			ssh_key_free(key);
			return fullPath;
		}
		if (key) ssh_key_free(key);
		// Key file is corrupt — regenerate
	}

	// Ensure directory exists
	Poco::File dir(keyDir);
	dir.createDirectories();

	generateKey(fullPath);
	return fullPath;
}


namespace
{
#if !defined(POCO_OS_FAMILY_WINDOWS)
	/// RAII guard that restores the process umask on scope exit.
	/// Ensures the umask is restored even if ssh_pki_* throws or exits early.
	class UmaskGuard
	{
	public:
		explicit UmaskGuard(mode_t newMask): _oldMask(umask(newMask)) {}
		~UmaskGuard() { umask(_oldMask); }
		UmaskGuard(const UmaskGuard&) = delete;
		UmaskGuard& operator=(const UmaskGuard&) = delete;
	private:
		mode_t _oldMask;
	};
#endif
}


void SSHHostKeyManager::generateKey(const std::string& path)
{
#if !defined(POCO_OS_FAMILY_WINDOWS)
	// Set restrictive umask before writing private key to ensure
	// the file is never accessible by group/others, even briefly.
	UmaskGuard umaskGuard(0077);
#endif

	ssh_key key = nullptr;
#if LIBSSH_VERSION_INT >= SSH_VERSION_INT(0, 12, 0)
	int rc = ssh_pki_generate_key(SSH_KEYTYPE_ED25519, nullptr, &key);
#else
	int rc = ssh_pki_generate(SSH_KEYTYPE_ED25519, 0, &key);
#endif
	if (rc != SSH_OK || key == nullptr)
		throw SSHException("Failed to generate SSH host key");

	rc = ssh_pki_export_privkey_file(key, nullptr, nullptr, nullptr, path.c_str());
	ssh_key_free(key);

	if (rc != SSH_OK)
		throw SSHException("Failed to write SSH host key to " + path);
}


} } // namespace Poco::SSH
