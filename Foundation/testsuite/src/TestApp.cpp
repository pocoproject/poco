//
// TestApp.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "Poco/PIDFile.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <signal.h>

using namespace Poco;


int main(int argc, char** argv)
{
	if (argc > 1)
	{
		std::string arg(argv[1]);
		if (arg == "-hello")
		{
			std::cout << "Hello, world!";
		}
		else if (arg == "-count")
		{
			int n = 0;
			int c = std::cin.get();
			while (c != -1) { ++n; c = std::cin.get(); }
			return n;
		}
		else if (arg == "-env")
		{
			const char* s = std::getenv("TESTENV");
			if (s)
			{
				std::cout << s;
				return 0;
			}
			else return 1;
		}
		else if (arg == "-raise-int")
		{
			signal(SIGINT, SIG_DFL);
			raise(SIGINT);
		}
		else if (arg == "-echo-args")
		{
			for (int i = 2; i < argc; ++i)
			{
				std::cout << argv[i] << std::endl;
			}
		}
		else if (arg.find("--pidfile") != std::string::npos || arg.find("-p") != std::string::npos)
		{
			std::unique_ptr<PIDFile> _pPIDFile;
			size_t equals_pos = arg.find('=');

			if (equals_pos != std::string::npos)
			{
				std::string pidPath = arg.substr(equals_pos + 1);
				_pPIDFile.reset(new PIDFile(pidPath, true, false));
				_pPIDFile->create(false);

				sigset_t sset;
				sigemptyset(&sset);
				if (!std::getenv("POCO_ENABLE_DEBUGGER"))
				{
					sigaddset(&sset, SIGINT);
				}
				sigaddset(&sset, SIGQUIT);
				sigaddset(&sset, SIGTERM);
				sigprocmask(SIG_BLOCK, &sset, NULL);
				int sig;
				sigwait(&sset, &sig);

				return 0;
			}
		}
	}
	return argc - 1;
}
