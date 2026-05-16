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
#include <libssh/libssh.h>


namespace Poco {
namespace SSH {


SSHChannelStreamBuf::SSHChannelStreamBuf(ssh_channel channel):
	_channel(channel)
{
}


SSHChannelStreamBuf::~SSHChannelStreamBuf() = default;


int SSHChannelStreamBuf::overflow(int c)
{
	if (c != EOF)
	{
		char ch = static_cast<char>(c);
		if (ssh_channel_write(_channel, &ch, 1) < 0)
			return EOF;
	}
	return c;
}


std::streamsize SSHChannelStreamBuf::xsputn(const char* s, std::streamsize n)
{
	if (n <= 0) return 0;

	static const uint32_t MAX_CHUNK = 0x7FFFFFFF; // max safe size for ssh_channel_write
	std::streamsize totalWritten = 0;
	while (totalWritten < n)
	{
		std::streamsize remaining = n - totalWritten;
		uint32_t chunk = (remaining > MAX_CHUNK) ? MAX_CHUNK : static_cast<uint32_t>(remaining);
		int written = ssh_channel_write(_channel, s + totalWritten, chunk);
		if (written <= 0)
			break;
		totalWritten += written;
	}
	return totalWritten;
}


SSHChannelStream::SSHChannelStream(ssh_channel channel):
	std::ostream(&_buf),
	_buf(channel)
{
}


SSHChannelStream::~SSHChannelStream() = default;


bool sshReadLine(ssh_channel channel, std::string& line)
{
	line.clear();
	char ch = 0;
	while (true)
	{
		const int nbytes = ssh_channel_read(channel, &ch, 1, 0);
		if (nbytes <= 0)
			return false; // EOF or error

		if (ch == '\r' || ch == '\n')
		{
			// Echo newline back to client
			ssh_channel_write(channel, "\r\n", 2);
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
				line.pop_back();
				ssh_channel_write(channel, "\b \b", 3);
			}
		}
		else if (ch == '\x03')
		{
			// Ctrl+C: discard line
			ssh_channel_write(channel, "^C\r\n", 4);
			line.clear();
			return true;
		}
		else if (ch >= ' ')
		{
			// Echo printable character back
			line += ch;
			ssh_channel_write(channel, &ch, 1);
		}
	}
}


} } // namespace Poco::SSH
