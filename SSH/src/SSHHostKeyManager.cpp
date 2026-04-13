//
// SSHHostKeyManager.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHHostKeyManager
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHHostKeyManager.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <libssh/libssh.h>

#ifndef POCO_OS_FAMILY_WINDOWS
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


void SSHHostKeyManager::generateKey(const std::string& path)
{
	ssh_key key = nullptr;
	int rc = ssh_pki_generate(SSH_KEYTYPE_ED25519, 0, &key);
	if (rc != SSH_OK || !key)
		throw SSHException("Failed to generate SSH host key");

	rc = ssh_pki_export_privkey_file(key, nullptr, nullptr, nullptr, path.c_str());
	ssh_key_free(key);
	if (rc != SSH_OK)
		throw SSHException("Failed to write SSH host key to " + path);

#ifndef POCO_OS_FAMILY_WINDOWS
	chmod(path.c_str(), 0600);
#endif
}


} } // namespace Poco::SSH
