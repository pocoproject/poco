//
// SSHChannelStream.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHChannelStream
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHChannelStream.h"
#include "SSHDeadline.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/SSH/SSHSession.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <libssh/libssh.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <optional>
#include <utility>


namespace Poco {
namespace SSH {


namespace {


void limitWaitsAfterStop(ssh_session session)
	/// Sets the timeout of the session to SSHSession::STOP_CHECK_INTERVAL after the stop check has returned true,
	/// as SSHChannelStreamBuf describes.
{
	// libssh rejects only a negative timeout, so the result needs no check
	static_cast<void>(setSessionTimeout(session, SSHSession::STOP_CHECK_INTERVAL));
}


[[nodiscard]] int writeChannel(ssh_channel channel, const char* data, uint32_t length,
	const std::function<bool()>& stopRequested,
	const std::optional<std::chrono::steady_clock::time_point>& deadline = std::nullopt)
	/// Writes with ssh_channel_write(). With a stop check or a deadline, the write takes steps of at most
	/// SSHSession::STOP_CHECK_INTERVAL until libssh has passed all bytes to the socket, calls the stop check
	/// before each step and ends at the deadline; each wait of a step is limited to the time left until then.
	/// Returns length when libssh has passed all bytes to the socket. If the stop check returns true or the
	/// deadline passes first, returns the number of bytes written before it, or SSH_ERROR when all were written
	/// but libssh may still hold some. Returns SSH_ERROR on an error.
{
	if (stopRequested == nullptr && !deadline.has_value())
		return ssh_channel_write(channel, data, length);

	// One ssh_channel_write() waits with the session timeout for a key re-exchange to end, for the window and
	// for the socket, so that each wait may take a third of the interval
	constexpr auto WRITE_WAIT = SSHSession::STOP_CHECK_INTERVAL / 3;
	// Returns the given wait, shortened to the time left until the deadline, or zero when the write must end
	const auto nextWait = [&stopRequested, &deadline](std::chrono::milliseconds wait)
	{
		if (stopRequested != nullptr && stopRequested())
			return std::chrono::milliseconds::zero();
		return deadline.has_value() ? std::min(wait, remainingWait(*deadline)) : wait;
	};
	ssh_session session = ssh_channel_get_session(channel);
	uint32_t written = 0;
	while (written < length)
	{
		const auto wait = nextWait(WRITE_WAIT);
		if (wait == std::chrono::milliseconds::zero())
			break;
		// While the window takes the step, libssh does not wait for the window; with a larger step, it waits
		// again each time the client grows the window
		const uint32_t step = std::min(length - written, std::max(ssh_channel_window_size(channel), uint32_t{1}));
		if (!setSessionTimeout(session, wait))
			return SSH_ERROR;
		// A wait for the window that times out returns the bytes written before it
		const int count = ssh_channel_write(channel, data + written, step);
		if (!setSessionTimeout(session, std::chrono::milliseconds::zero()) || count < 0)
			return SSH_ERROR;
		written += static_cast<uint32_t>(count);
	}
	// ssh_channel_write() also counts the bytes that libssh still holds when its wait for the socket times out
	int flushed = SSH_AGAIN;
	while (written == length && flushed == SSH_AGAIN)
	{
		const auto wait = nextWait(SSHSession::STOP_CHECK_INTERVAL);
		if (wait == std::chrono::milliseconds::zero())
			break;
		flushed = ssh_blocking_flush(session, static_cast<int>(wait.count()));
	}
	if (flushed == SSH_AGAIN)
	{
		// The stop check returned true or the deadline passed. A stop or the deadline during the flush fails the
		// write, so that the caller does not take bytes that libssh holds for sent.
		if (stopRequested != nullptr && stopRequested())
			limitWaitsAfterStop(session);
		return written < length ? static_cast<int>(written) : SSH_ERROR;
	}
	return flushed == SSH_OK ? static_cast<int>(written) : SSH_ERROR;
}


[[nodiscard]] bool echo(ssh_channel channel, const char* data, uint32_t length,
	const std::function<bool()>& stopRequested, std::chrono::steady_clock::time_point deadline)
	/// Echoes input to the client until the deadline. Returns false if a stop ended the write. A failed echo is
	/// otherwise ignored: a following read reports a channel that has failed, and the loop of sshReadLine()
	/// throws its timeout unless the echo ended the line.
{
	return writeChannel(channel, data, length, stopRequested, deadline) == static_cast<int>(length) ||
		stopRequested == nullptr || !stopRequested();
}


} // namespace


SSHChannelStreamBuf::SSHChannelStreamBuf(ssh_channel channel, std::function<bool()> stopRequested):
	_channel(channel),
	_stopRequested(std::move(stopRequested))
{
}


SSHChannelStreamBuf::~SSHChannelStreamBuf() = default;


int SSHChannelStreamBuf::overflow(int c)
{
	if (c != EOF)
	{
		char ch = static_cast<char>(c);
		const int written = writeChannel(_channel, &ch, 1, _stopRequested);
		// Without a stop check, only an error fails the write, as before
		if (written < 0 || (written == 0 && _stopRequested != nullptr))
			return EOF;
	}
	return c;
}


std::streamsize SSHChannelStreamBuf::xsputn(const char* s, std::streamsize n)
{
	if (n <= 0) return 0;

	static constexpr uint32_t MAX_CHUNK = 0x7FFFFFFF; // max safe size for ssh_channel_write
	std::streamsize totalWritten = 0;
	while (totalWritten < n)
	{
		std::streamsize remaining = n - totalWritten;
		uint32_t chunk = (remaining > MAX_CHUNK) ? MAX_CHUNK : static_cast<uint32_t>(remaining);
		int written = writeChannel(_channel, s + totalWritten, chunk, _stopRequested);
		if (written <= 0)
			break;
		totalWritten += written;
	}
	return totalWritten;
}


SSHChannelStream::SSHChannelStream(ssh_channel channel, std::function<bool()> stopRequested):
	std::ostream(&_buf),
	_buf(channel, std::move(stopRequested))
{
}


SSHChannelStream::~SSHChannelStream() = default;


bool sshReadLine(ssh_channel channel, std::string& line, std::size_t maxLength, std::chrono::milliseconds timeout,
	const std::function<bool()>& stopRequested)
{
	line.clear();
	const auto deadline = deadlineAfter(timeout);
	char ch = 0;
	while (true)
	{
		auto waitTime = remainingWait(deadline);
		if (waitTime == std::chrono::milliseconds::zero())
			throw Poco::TimeoutException(Poco::format("No complete SSH input line within %?d ms", timeout.count()));
		if (stopRequested != nullptr)
			waitTime = std::min(waitTime, SSHSession::STOP_CHECK_INTERVAL);

		// The wait of ssh_channel_read_timeout() is an int; a longer remaining time takes several reads.
		const int nbytes = ssh_channel_read_timeout(channel, &ch, 1, 0, static_cast<int>(waitTime.count()));
		if (stopRequested != nullptr && stopRequested())
		{
			limitWaitsAfterStop(ssh_channel_get_session(channel));
			return false;
		}
		if (nbytes == 0 || nbytes == SSH_AGAIN)
		{
			// Nothing was read: EOF, or the wait timed out, which libssh reports as 0 or SSH_AGAIN by version
			if (ssh_channel_is_eof(channel))
				return false;
			continue;
		}
		if (nbytes < 0)
			return false; // error

		if (ch == '\r' || ch == '\n')
		{
			// Echo newline back to client
			if (!echo(channel, "\r\n", 2, stopRequested, deadline))
				return false;
			// Strip trailing CR if present
			if (!line.empty() && line.back() == '\r')
				line.pop_back();
			return true;
		}
		if (ch == '\x7f' || ch == '\b')
		{
			// Backspace: erase last character
			if (!line.empty())
			{
				// A UTF-8 sequence is its lead byte followed by continuation bytes (10xxxxxx)
				while (line.size() > 1 && (static_cast<unsigned char>(line.back()) & 0xC0) == 0x80)
					line.pop_back();
				line.pop_back();
				if (!echo(channel, "\b \b", 3, stopRequested, deadline))
					return false;
			}
		}
		else if (ch == '\x03')
		{
			// Ctrl+C: discard line
			if (!echo(channel, "^C\r\n", 4, stopRequested, deadline))
				return false;
			line.clear();
			return true;
		}
		else if (static_cast<unsigned char>(ch) >= ' ')
		{
			if (line.size() >= maxLength)
				throw SSHChannelException(Poco::format("SSH input line longer than %z bytes", maxLength));
			// Echo printable character back
			line += ch;
			if (!echo(channel, &ch, 1, stopRequested, deadline))
				return false;
		}
	}
}


} } // namespace Poco::SSH
