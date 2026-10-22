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
#include "SSHDeadline.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <optional>
#include <sstream>

#if defined(POCO_OS_FAMILY_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#endif


namespace Poco {
namespace SSH {


namespace {


[[nodiscard]] bool replyAuthSuccess(ssh_session session, ssh_message msg)
	/// Accepts the authentication request and frees msg.
	/// Returns false if the reply could not be sent.
{
	// libssh ignores a failed write of the reply: it closes the socket and marks the session as not connected,
	// but returns SSH_OK. A later ssh_message_get() then waits in poll() without a timeout for the closed socket
	// and never returns, so ssh_is_connected() is checked after the reply.
	const bool sent = ssh_message_auth_reply_success(msg, 0) == SSH_OK && ssh_is_connected(session) != 0;
	ssh_message_free(msg);
	return sent;
}


[[nodiscard]] bool limitWait(ssh_session session, const std::optional<std::chrono::steady_clock::time_point>& deadline)
	/// Limits the time that the next libssh call waits for the client to SSHSession::STOP_CHECK_INTERVAL, or to
	/// the time left until the deadline if that is shorter. Returns false if the deadline has passed.
	///
	/// The session checks after each interval whether it is stopped, and libssh can end a wait shortly before its
	/// timeout (its clock is coarse on Windows), so a call that timed out is repeated until this function returns
	/// false.
{
	auto wait = SSHSession::STOP_CHECK_INTERVAL;
	if (deadline.has_value())
	{
		const auto remaining = remainingWait(*deadline);
		if (remaining == std::chrono::milliseconds::zero())
			return false;
		wait = std::min(wait, remaining);
	}
	return setSessionTimeout(session, wait);
}


[[nodiscard]] ssh_message nextMessage(ssh_session session,
	const std::optional<std::chrono::steady_clock::time_point>& deadline, const std::atomic<bool>& stopRequested)
	/// Waits for the next message from the client until the deadline. Returns nullptr if the connection has
	/// ended, the session is stopped or the deadline has passed.
{
	while (!stopRequested && limitWait(session, deadline))
	{
		ssh_message msg = ssh_message_get(session);
		// A wait that ends without a message while the connection is open timed out
		if (msg != nullptr || ssh_is_connected(session) == 0 ||
			(ssh_get_status(session) & (SSH_CLOSED | SSH_CLOSED_ERROR)) != 0)
		{
			return msg;
		}
	}
	return nullptr;
}


[[nodiscard]] std::string clientAddress(ssh_session session)
	/// Returns the address and port of the client, for log messages.
{
	struct sockaddr_storage addr{};
	socklen_t length = sizeof(addr);
	std::array<char, NI_MAXHOST> host{};
	std::array<char, NI_MAXSERV> port{};
	if (::getpeername(ssh_get_fd(session), reinterpret_cast<struct sockaddr*>(&addr), &length) != 0 ||
		::getnameinfo(reinterpret_cast<const struct sockaddr*>(&addr), length, host.data(), NI_MAXHOST,
			port.data(), NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) != 0)
	{
		return "an unknown address";
	}
	return Poco::format("%s port %s", std::string(host.data()), std::string(port.data()));
}


} // namespace


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
	if (_logger.trace())
		_logger.trace("Session started");

	// libssh closes the socket when the connection ends, so the address is taken while it is open
	const std::string address = clientAddress(_session);

	// The login grace time limits the waits of libssh in this thread: closing the socket from another thread
	// would close a handle that libssh still uses.
	std::optional<std::chrono::steady_clock::time_point> loginDeadline;
	if (_config.loginGraceTime > std::chrono::milliseconds::zero())
		loginDeadline = deadlineAfter(_config.loginGraceTime);
	bool loggedIn = false;
	try
	{
		if (_logger.trace())
			_logger.trace("Starting key exchange");

		int rc = SSH_AGAIN;
		while (rc == SSH_AGAIN && !stopRequested() && limitWait(_session, loginDeadline))
			rc = ssh_handle_key_exchange(_session);
		if (rc == SSH_OK)
		{
			if (_logger.trace())
				_logger.trace("Key exchange succeeded, authenticating");

			if (authenticate(loginDeadline))
			{
				_logger.information("Authenticated user: %s", _authenticatedUser);

				ssh_channel channel = negotiateChannel(loginDeadline);
				if (channel)
				{
					loggedIn = true;
					// Zero is the timeout of a new session: the login grace time does not limit the rest of the session
					// (libssh rejects only a negative timeout, so the result needs no check)
					static_cast<void>(setSessionTimeout(_session, std::chrono::milliseconds::zero()));
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

	if (stopRequested())
	{
		_logger.information("SSH session from %s stopped by the server", address);
	}
	else if (!loggedIn && loginDeadline.has_value() &&
		remainingWait(*loginDeadline) == std::chrono::milliseconds::zero())
	{
		_logger.information("Closing the SSH connection from %s: login not completed within %?d ms",
			address, _config.loginGraceTime.count());
	}

	if (_logger.trace())
		_logger.trace("Session cleanup");

	_server.unregisterSession(_session);
	ssh_disconnect(_session);
	ssh_free(_session);
	_session = nullptr;
	delete this;
}


bool SSHSession::authenticate(const std::optional<std::chrono::steady_clock::time_point>& loginDeadline)
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
		ssh_message msg = nextMessage(_session, loginDeadline, _stopRequested);
		if (msg == nullptr)
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
					return replyAuthSuccess(_session, msg);
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
							return replyAuthSuccess(_session, msg);
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


ssh_channel SSHSession::negotiateChannel(const std::optional<std::chrono::steady_clock::time_point>& loginDeadline)
{
	ssh_channel channel = nullptr;

	for (int i = 0; i < 10; ++i)
	{
		ssh_message msg = nextMessage(_session, loginDeadline, _stopRequested);
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
		ssh_message msg = nextMessage(_session, loginDeadline, _stopRequested);
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
