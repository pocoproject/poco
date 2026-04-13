//
// SSHClient.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHClient
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHClient.h"
#include "Poco/SSH/SSHException.h"


namespace Poco {
namespace SSH {


SSHClient::SSHClient():
	_session(ssh_new()),
	_connected(false)
{
	if (!_session)
		throw SSHException("Failed to create SSH session");
}


SSHClient::~SSHClient()
{
	disconnect();
	if (_session)
	{
		ssh_free(_session);
		_session = nullptr;
	}
}


void SSHClient::connect(const std::string& host, Poco::UInt16 port)
{
	if (_connected)
		disconnect();

	int p = port;
	ssh_options_set(_session, SSH_OPTIONS_HOST, host.c_str());
	ssh_options_set(_session, SSH_OPTIONS_PORT, &p);
	ssh_options_set(_session, SSH_OPTIONS_KNOWNHOSTS, "/dev/null");
	ssh_options_set(_session, SSH_OPTIONS_GLOBAL_KNOWNHOSTS, "/dev/null");

	int rc = ssh_connect(_session);
	if (rc != SSH_OK)
	{
		std::string err = ssh_get_error(_session);
		throw SSHConnectionException("SSH connect failed: " + err);
	}

	_connected = true;
}


bool SSHClient::authenticatePassword(const std::string& user, const std::string& password)
{
	if (!_connected)
		throw SSHConnectionException("Not connected");

	int rc = ssh_userauth_password(_session, user.c_str(), password.c_str());
	return rc == SSH_AUTH_SUCCESS;
}


bool SSHClient::authenticatePublicKey(const std::string& user, const std::string& keyFile)
{
	if (!_connected)
		throw SSHConnectionException("Not connected");

	int rc;
	if (keyFile.empty())
	{
		rc = ssh_userauth_publickey_auto(_session, user.c_str(), nullptr);
	}
	else
	{
		ssh_key key = nullptr;
		rc = ssh_pki_import_privkey_file(keyFile.c_str(), nullptr, nullptr, nullptr, &key);
		if (rc != SSH_OK)
			return false;
		rc = ssh_userauth_publickey(_session, user.c_str(), key);
		ssh_key_free(key);
	}

	return rc == SSH_AUTH_SUCCESS;
}


ssh_channel SSHClient::openShellChannel()
{
	if (!_connected)
		throw SSHConnectionException("Not connected");

	ssh_channel channel = ssh_channel_new(_session);
	if (!channel)
		return nullptr;

	if (ssh_channel_open_session(channel) != SSH_OK)
	{
		ssh_channel_free(channel);
		return nullptr;
	}

	if (ssh_channel_request_pty(channel) != SSH_OK)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return nullptr;
	}

	if (ssh_channel_request_shell(channel) != SSH_OK)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return nullptr;
	}

	return channel;
}


void SSHClient::disconnect()
{
	if (_connected)
	{
		ssh_disconnect(_session);
		_connected = false;
	}
}


} } // namespace Poco::SSH
