//
// SSHDeadline.h
//
// Library: SSH
// Package: SSH
// Module:  SSHDeadline
//
// Definition of the deadlineAfter(), remainingWait() and setSessionTimeout() functions.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHDeadline_INCLUDED
#define SSH_SSHDeadline_INCLUDED


#include "Poco/SSH/SSH.h"
#include <libssh/libssh.h>
#include <algorithm>
#include <chrono>
#include <limits>


POCO_CHECK_MINMAX_MACROS


namespace Poco::SSH {


[[nodiscard]] std::chrono::steady_clock::time_point deadlineAfter(std::chrono::milliseconds timeout);
	/// Returns the time point that is the given timeout after now, or
	/// std::chrono::steady_clock::time_point::max() if the clock cannot
	/// represent that time point. A zero or negative timeout returns now.
	///
	/// This function is internal to the SSH library.


[[nodiscard]] std::chrono::milliseconds remainingWait(std::chrono::steady_clock::time_point deadline);
	/// Returns the time from now until the given deadline, rounded up to
	/// whole milliseconds and limited to std::numeric_limits<int>::max()
	/// milliseconds, the longest wait that ssh_channel_read_timeout() and
	/// Poco::Event::tryWait() accept on every platform. Returns zero if the
	/// deadline has passed.
	///
	/// This function is internal to the SSH library.


[[nodiscard]] bool setSessionTimeout(ssh_session session, std::chrono::milliseconds timeout);
	/// Sets the timeout of the session, which limits the waits of libssh for
	/// the peer. Zero is the timeout of a new session. Returns false if libssh
	/// rejects the timeout.
	///
	/// This function is internal to the SSH library.


//
// inlines
//
inline std::chrono::steady_clock::time_point deadlineAfter(std::chrono::milliseconds timeout)
{
	const auto now = std::chrono::steady_clock::now();
	if (timeout <= std::chrono::milliseconds::zero())
		return now;
	const auto maxTimeout =
		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::time_point::max() - now);
	if (timeout >= maxTimeout)
		return std::chrono::steady_clock::time_point::max();
	return now + timeout;
}


inline std::chrono::milliseconds remainingWait(std::chrono::steady_clock::time_point deadline)
{
	const auto remaining = std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
	return std::clamp(remaining, std::chrono::milliseconds::zero(),
		std::chrono::milliseconds(std::numeric_limits<int>::max()));
}


inline bool setSessionTimeout(ssh_session session, std::chrono::milliseconds timeout)
{
	const auto seconds = std::chrono::floor<std::chrono::seconds>(timeout);
	const long timeoutSec = static_cast<long>(seconds.count());
	const long timeoutUsec = static_cast<long>(std::chrono::microseconds(timeout - seconds).count());
	return ssh_options_set(session, SSH_OPTIONS_TIMEOUT, &timeoutSec) == SSH_OK &&
		ssh_options_set(session, SSH_OPTIONS_TIMEOUT_USEC, &timeoutUsec) == SSH_OK;
}


} // namespace Poco::SSH


#endif // SSH_SSHDeadline_INCLUDED
