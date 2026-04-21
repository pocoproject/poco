//
// pocossh.cpp
//
// A simple standalone SSH client for connecting to an OSP shell
// or any SSH server.
//
// Usage: pocossh [-H host] [-P port] [-u user] [-p password] [-k keyfile]
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHClient.h"
#include "Poco/SSH/SSHChannelStream.h"
#include "Poco/Exception.h"
#include <iostream>
#include <cstring>
#include <string>
#include <libssh/libssh.h>

#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <termios.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#endif


namespace
{
	struct TerminalRawMode
	{
#if !defined(POCO_OS_FAMILY_WINDOWS)
		struct termios _saved;
		bool _active = false;

		void enable()
		{
			if (tcgetattr(STDIN_FILENO, &_saved) == 0)
			{
				struct termios raw = _saved;
				raw.c_lflag &= ~(ECHO | ICANON | ISIG);
				raw.c_cc[VMIN] = 0;
				raw.c_cc[VTIME] = 1; // 100ms timeout
				tcsetattr(STDIN_FILENO, TCSANOW, &raw);
				_active = true;
			}
		}

		void disable()
		{
			if (_active)
			{
				tcsetattr(STDIN_FILENO, TCSANOW, &_saved);
				_active = false;
			}
		}

		~TerminalRawMode() { disable(); }
#else
		void enable() {}
		void disable() {}
#endif
	};


	void printUsage(const char* progName)
	{
		std::cerr << "Usage: " << progName << " [-H host] [-P port] [-u user] [-p password] [-k keyfile]\n"
		          << "  -H host      SSH server hostname (default: localhost)\n"
		          << "  -P port      SSH server port (default: 22023)\n"
		          << "  -u user      Username (default: admin)\n"
		          << "  -p password  Password (prompts if not given and no key)\n"
		          << "  -k keyfile   Path to private key file (default: auto from ~/.ssh/)\n"
		          << std::endl;
	}
}


int main(int argc, char* argv[])
{
	std::string host = "localhost";
	int port = 22023;
	std::string user = "admin";
	std::string password;
	std::string keyFile;
	bool hasPassword = false;

	// Parse arguments
	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-H") == 0 && i + 1 < argc)
			host = argv[++i];
		else if (std::strcmp(argv[i], "-P") == 0 && i + 1 < argc)
			port = std::stoi(argv[++i]);
		else if (std::strcmp(argv[i], "-u") == 0 && i + 1 < argc)
			user = argv[++i];
		else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc)
		{
			password = argv[++i];
			hasPassword = true;
		}
		else if (std::strcmp(argv[i], "-k") == 0 && i + 1 < argc)
			keyFile = argv[++i];
		else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0)
		{
			printUsage(argv[0]);
			return 0;
		}
		else
		{
			std::cerr << "Unknown option: " << argv[i] << std::endl;
			printUsage(argv[0]);
			return 1;
		}
	}

	try
	{
		Poco::SSH::SSHClient client;
		client.connect(host, port);

		// Try key auth first, then password
		bool authenticated = false;
		if (!keyFile.empty())
		{
			authenticated = client.authenticatePublicKey(user, keyFile);
		}
		else if (!hasPassword)
		{
			// Try auto key auth
			authenticated = client.authenticatePublicKey(user);
		}

		if (!authenticated)
		{
			if (!hasPassword)
			{
				std::cerr << "Password: " << std::flush;
#if !defined(POCO_OS_FAMILY_WINDOWS)
				struct termios oldt{};
				struct termios newt{};
				tcgetattr(STDIN_FILENO, &oldt);
				newt = oldt;
				newt.c_lflag &= ~ECHO;
				tcsetattr(STDIN_FILENO, TCSANOW, &newt);
				std::getline(std::cin, password);
				tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
				std::cerr << std::endl;
#else
				std::getline(std::cin, password);
#endif
			}

			authenticated = client.authenticatePassword(user, password);
		}

		if (!authenticated)
		{
			std::cerr << "Authentication failed." << std::endl;
			return 1;
		}

		ssh_channel channel = client.openShellChannel();
		if (!channel)
		{
			std::cerr << "Failed to open shell channel." << std::endl;
			return 1;
		}

		// Interactive loop: stdin -> channel, channel -> stdout
		TerminalRawMode rawMode;
		rawMode.enable();

#if !defined(POCO_OS_FAMILY_WINDOWS)
		struct pollfd fds[1]{};
		fds[0].fd = STDIN_FILENO;
		fds[0].events = POLLIN;

		while (ssh_channel_is_eof(channel) == 0 && ssh_channel_is_closed(channel) == 0)
		{
			// Read from channel (non-blocking)
			char buf[4096];
			int nbytes = ssh_channel_read_nonblocking(channel, buf, sizeof(buf), 0);
			if (nbytes > 0)
			{
				const ssize_t written = write(STDOUT_FILENO, buf, nbytes);
				(void)written; // best-effort echo; channel may disappear next loop
			}
			else if (nbytes < 0)
			{
				break;
			}

			// Read from stdin
			if (poll(fds, 1, 50) > 0 && (fds[0].revents & POLLIN) != 0)
			{
				const ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
				if (n > 0)
				{
					ssh_channel_write(channel, buf, static_cast<uint32_t>(n));
				}
				else if (n == 0)
				{
					break; // EOF on stdin
				}
			}
		}
#else
		std::cerr << "Interactive mode not supported on Windows in this sample." << std::endl;
#endif

		rawMode.disable();

		ssh_channel_send_eof(channel);
		ssh_channel_close(channel);
		ssh_channel_free(channel);

		return 0;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << "Error: " << exc.displayText() << std::endl;
		return 1;
	}
	catch (std::exception& exc)
	{
		std::cerr << "Error: " << exc.what() << std::endl;
		return 1;
	}
}
