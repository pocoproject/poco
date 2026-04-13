//
// SSHChannelStream.h
//
// Library: SSH
// Package: SSH
// Module:  SSHChannelStream
//
// I/O bridge between libssh channels and C++ streams.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHChannelStream_INCLUDED
#define SSH_SSHChannelStream_INCLUDED


#include "Poco/SSH/SSH.h"
#include <libssh/libssh.h>
#include <streambuf>
#include <ostream>
#include <string>


namespace Poco {
namespace SSH {


class SSH_API SSHChannelStreamBuf: public std::streambuf
	/// A streambuf that writes to an SSH channel via ssh_channel_write().
{
public:
	explicit SSHChannelStreamBuf(ssh_channel channel);
	~SSHChannelStreamBuf();

protected:
	int overflow(int c) override;
	std::streamsize xsputn(const char* s, std::streamsize n) override;

private:
	ssh_channel _channel;
};


class SSH_API SSHChannelStream: public std::ostream
	/// An ostream that writes to an SSH channel.
{
public:
	explicit SSHChannelStream(ssh_channel channel);
	~SSHChannelStream();

private:
	SSHChannelStreamBuf _buf;
};


SSH_API bool sshReadLine(ssh_channel channel, std::string& line);
	/// Reads one line from the SSH channel with character echo.
	/// Handles backspace, Ctrl+C, and CR/LF line termination.
	/// Returns true if a line was read, false on EOF or error.


} } // namespace Poco::SSH


#endif // SSH_SSHChannelStream_INCLUDED
