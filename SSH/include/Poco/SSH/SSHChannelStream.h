//
// SSHChannelStream.h
//
// Library: SSH
// Package: SSH
// Module:  SSHChannelStream
//
// I/O bridge between libssh channels and C++ streams.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHChannelStream_INCLUDED
#define SSH_SSHChannelStream_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/SSH/SSHTypes.h"
#include <chrono>
#include <cstddef>
#include <functional>
#include <streambuf>
#include <ostream>
#include <string>


namespace Poco {
namespace SSH {


class SSH_API SSHChannelStreamBuf: public std::streambuf
	/// A streambuf that writes to an SSH channel via ssh_channel_write().
{
public:
	explicit SSHChannelStreamBuf(ssh_channel channel, std::function<bool()> stopRequested = {});
		/// Creates the streambuf for the channel.
		///
		/// With a stop check, such as SSHSession::stopRequested(), a write waits
		/// for the channel window and for the connection in steps of at most
		/// SSHSession::STOP_CHECK_INTERVAL, writes the rest after each step and
		/// calls the stop check before each step, until libssh has passed all
		/// bytes to the socket. When the stop check returns true, the write ends
		/// and the stream sets badbit. For each step that writes to the channel,
		/// the streambuf sets the timeout of the session and then sets it back
		/// to zero, the timeout of a new session, so it must be used only in the
		/// thread of the session.
		///
		/// When the stop check has returned true, the streambuf leaves the
		/// timeout of the session at SSHSession::STOP_CHECK_INTERVAL, so that
		/// a later wait of libssh for the client ends after that time. Closing
		/// or freeing the channel is such a wait: without this limit, it would
		/// last until libssh has sent the data it holds, which does not happen
		/// while the client does not read.
		///
		/// The wait for the connection is limited this way on a non-blocking
		/// socket, such as the socket of a session that SSHServer accepted. On
		/// a blocking socket, libssh waits in send() without a time limit.

	~SSHChannelStreamBuf();

protected:
	int overflow(int c) override;
	std::streamsize xsputn(const char* s, std::streamsize n) override;

private:
	ssh_channel _channel;
	std::function<bool()> _stopRequested;
};


class SSH_API SSHChannelStream: public std::ostream
	/// An ostream that writes to an SSH channel.
{
public:
	explicit SSHChannelStream(ssh_channel channel, std::function<bool()> stopRequested = {});
		/// Creates the stream for the channel. The stop check is
		/// described at SSHChannelStreamBuf.

	~SSHChannelStream();

private:
	SSHChannelStreamBuf _buf;
};


inline constexpr std::size_t DEFAULT_READ_LINE_MAX_LENGTH = 4096;
	/// Default maximum length in bytes of a line read with sshReadLine().

inline constexpr std::chrono::milliseconds DEFAULT_READ_LINE_TIMEOUT = std::chrono::minutes(10);
	/// Default time limit for reading a line with sshReadLine().


[[nodiscard]] SSH_API bool sshReadLine(
	ssh_channel channel,
	std::string& line,
	std::size_t maxLength = DEFAULT_READ_LINE_MAX_LENGTH,
	std::chrono::milliseconds timeout = DEFAULT_READ_LINE_TIMEOUT,
	const std::function<bool()>& stopRequested = {});
	/// Reads one line from the SSH channel with character echo.
	/// Handles backspace, Ctrl+C, and CR/LF line termination.
	/// Bytes of 0x80 and above are kept, so UTF-8 input arrives
	/// unchanged, and backspace erases a whole UTF-8 sequence.
	///
	/// The line can be at most maxLength bytes long and must be
	/// complete within the timeout, which starts with the call.
	/// A timeout that is too long for std::chrono::steady_clock,
	/// such as std::chrono::milliseconds::max(), sets no time limit.
	/// A zero or negative timeout times out at once.
	///
	/// With a stop check, such as SSHSession::stopRequested(), the
	/// function waits for input in steps of at most
	/// SSHSession::STOP_CHECK_INTERVAL and calls the stop check after
	/// each step. It writes the echo as SSHChannelStreamBuf describes.
	/// When the stop check returns true, it sets the timeout of the
	/// session to SSHSession::STOP_CHECK_INTERVAL, as SSHChannelStreamBuf
	/// describes.
	///
	/// Returns true if a line was read, false on EOF, on error or when
	/// the stop check returns true.
	/// Throws an SSHChannelException if the line is longer than
	/// maxLength, or a Poco::TimeoutException if no complete line
	/// arrived within the timeout.


} } // namespace Poco::SSH


#endif // SSH_SSHChannelStream_INCLUDED
