//
// SSHSession.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHSession
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


// Suppress deprecation warnings for libssh message-based auth API
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996) // deprecated
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif


#include "Poco/SSH/SSHSession.h"
#include "Poco/SSH/SSHServer.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <fstream>
#include <sstream>


namespace Poco {
namespace SSH {


SSHSession::SSHSession(
	ssh_session session,
	const SSHServerConfig& config,
	SSHServer& server):
	_session(session),
	_config(config),
	_server(server),
	_logger(Poco::Logger::get("Poco.SSH.SSHSession"))
{
}


SSHSession::~SSHSession() = default;


void SSHSession::run()
{
	_server.registerSession(_session);

	if (_logger.trace())
		_logger.trace("Session started");

	try
	{
		if (_logger.trace())
			_logger.trace("Starting key exchange");

		if (ssh_handle_key_exchange(_session) == SSH_OK)
		{
			if (_logger.trace())
				_logger.trace("Key exchange succeeded, authenticating");

			if (authenticate())
			{
				_logger.information("Authenticated user: %s", _authenticatedUser);

				ssh_channel channel = negotiateChannel();
				if (channel)
				{
					if (_logger.trace())
						_logger.trace("Channel open, starting session handler");

					handleSession(channel, _authenticatedUser);
				}
				else
				{
					if (_logger.trace())
						_logger.trace("Channel negotiation failed");
				}
			}
			else
			{
				if (_logger.trace())
					_logger.trace("Authentication failed");
			}
		}
		else
		{
			if (_logger.trace())
				_logger.trace("Key exchange failed: %s", std::string(ssh_get_error(_session)));
		}
	}
	catch (Poco::Exception& exc)
	{
		_logger.error("SSH session error: %s", exc.displayText());
	}
	catch (std::exception& exc)
	{
		_logger.error("SSH session error: %s", std::string(exc.what()));
	}
	catch (...)
	{
		_logger.error("SSH session: unknown exception");
	}

	if (_logger.trace())
		_logger.trace("Session cleanup");

	_server.unregisterSession(_session);
	ssh_disconnect(_session);
	ssh_free(_session);
	_session = nullptr;
	delete this;
}


bool SSHSession::authenticate()
{
	int authAttempts = 0;
	int authMethods = 0;
	if (_config.enablePasswordAuth && _config.passwordAuthenticator)
		authMethods |= SSH_AUTH_METHOD_PASSWORD;
	if (_config.enablePubkeyAuth && !_config.authorizedKeysDir.empty())
		authMethods |= SSH_AUTH_METHOD_PUBLICKEY;

	// Cap total messages received to bound loop work even when a misbehaving
	// client sends non-AUTH messages (which otherwise do not increment
	// authAttempts). 10x maxAuthAttempts is a generous multiplier that still
	// denies indefinite loops.
	const int maxMessages = _config.maxAuthAttempts * 10;
	int messagesSeen = 0;

	while (authAttempts < _config.maxAuthAttempts && messagesSeen < maxMessages)
	{
		++messagesSeen;
		ssh_message msg = ssh_message_get(_session);
		if (!msg)
			return false;

		int type = ssh_message_type(msg);
		int subtype = ssh_message_subtype(msg);

		if (type == SSH_REQUEST_AUTH)
		{
			if (subtype == SSH_AUTH_METHOD_PASSWORD && (_config.enablePasswordAuth && _config.passwordAuthenticator))
			{
				const char* user = ssh_message_auth_user(msg);
				const char* pass = ssh_message_auth_password(msg);
				if (user && pass && _config.passwordAuthenticator(user, pass))
				{
					_authenticatedUser = user;
					ssh_message_auth_reply_success(msg, 0);
					ssh_message_free(msg);
					return true;
				}
				authAttempts++;
				if (_logger.trace())
					_logger.trace("Password auth failed for user: %s (attempt %d)", std::string(user ? user : ""), authAttempts);
				ssh_message_reply_default(msg);
			}
			else if (subtype == SSH_AUTH_METHOD_PUBLICKEY && _config.enablePubkeyAuth)
			{
				const char* user = ssh_message_auth_user(msg);
				ssh_key clientKey = ssh_message_auth_pubkey(msg);

				if (user && clientKey)
				{
					enum ssh_publickey_state_e keyState = ssh_message_auth_publickey_state(msg);
					if (keyState == SSH_PUBLICKEY_STATE_NONE)
					{
						if (checkAuthorizedKey(user, clientKey))
							ssh_message_auth_reply_pk_ok_simple(msg);
						else
						{
							authAttempts++;
							ssh_message_reply_default(msg);
						}
					}
					else if (keyState == SSH_PUBLICKEY_STATE_VALID)
					{
						if (checkAuthorizedKey(user, clientKey))
						{
							_authenticatedUser = user;
							ssh_message_auth_reply_success(msg, 0);
							ssh_message_free(msg);
							return true;
						}
						authAttempts++;
						ssh_message_reply_default(msg);
					}
					else
					{
						ssh_message_reply_default(msg);
					}
				}
				else
				{
					ssh_message_reply_default(msg);
				}
			}
			else
			{
				ssh_message_auth_set_methods(msg, authMethods);
				ssh_message_reply_default(msg);
			}
		}
		else
		{
			ssh_message_reply_default(msg);
		}

		ssh_message_free(msg);
	}

	return false;
}


ssh_channel SSHSession::negotiateChannel()
{
	ssh_channel channel = nullptr;

	for (int i = 0; i < 10; ++i)
	{
		ssh_message msg = ssh_message_get(_session);
		if (!msg) return nullptr;

		if (ssh_message_type(msg) == SSH_REQUEST_CHANNEL_OPEN &&
		    ssh_message_subtype(msg) == SSH_CHANNEL_SESSION)
		{
			channel = ssh_message_channel_request_open_reply_accept(msg);
			ssh_message_free(msg);
			break;
		}

		ssh_message_reply_default(msg);
		ssh_message_free(msg);
	}

	if (!channel) return nullptr;

	for (int i = 0; i < 10; ++i)
	{
		ssh_message msg = ssh_message_get(_session);
		if (!msg)
		{
			ssh_channel_free(channel);
			return nullptr;
		}

		int type = ssh_message_type(msg);
		int subtype = ssh_message_subtype(msg);

		if (type == SSH_REQUEST_CHANNEL)
		{
			if (subtype == SSH_CHANNEL_REQUEST_SHELL ||
			    subtype == SSH_CHANNEL_REQUEST_EXEC)
			{
				ssh_message_channel_request_reply_success(msg);
				ssh_message_free(msg);
				return channel;
			}
			else if (subtype == SSH_CHANNEL_REQUEST_PTY)
			{
				ssh_message_channel_request_reply_success(msg);
				ssh_message_free(msg);
				continue;
			}
		}

		ssh_message_reply_default(msg);
		ssh_message_free(msg);
	}

	ssh_channel_free(channel);
	return nullptr;
}


bool SSHSession::checkAuthorizedKey(const std::string& username, ssh_key clientKey)
{
	if (_config.authorizedKeysDir.empty())
		return false;

	Poco::Path keyFilePath(_config.authorizedKeysDir);
	keyFilePath.append(username);
	std::string keyFile = keyFilePath.toString();

	Poco::File f(keyFile);
	if (!f.exists()) return false;

	std::ifstream ifs(keyFile);
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		std::string keytype, b64key;
		iss >> keytype >> b64key;
		if (keytype.empty() || b64key.empty()) continue;

		ssh_key candidate = nullptr;
		int rc = ssh_pki_import_pubkey_base64(
			b64key.c_str(),
			ssh_key_type_from_name(keytype.c_str()),
			&candidate);

		if (rc == SSH_OK && candidate)
		{
			bool match = (ssh_key_cmp(clientKey, candidate, SSH_KEY_CMP_PUBLIC) == 0);
			ssh_key_free(candidate);
			if (match) return true;
		}
		else if (candidate)
		{
			ssh_key_free(candidate);
		}
	}

	return false;
}


} } // namespace Poco::SSH


#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
