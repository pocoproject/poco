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

#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Process.h"
#include "Poco/Event.h"
#include "Poco/NamedEvent.h"
#endif

using namespace Poco;

#if POCO_OS != POCO_OS_ANDROID
class App
{
public:
	App() {}
	~App() {}

	std::unique_ptr<PIDFile> _pPIDFile;
#if defined(POCO_OS_FAMILY_WINDOWS)
	static Poco::Event _terminated;
	static Poco::NamedEvent _terminate;


	static void terminate()
	{
		_terminate.set();
	}


	static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType)
	{
		switch (ctrlType)
		{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:
			terminate();
			return _terminated.tryWait(10000) ? TRUE : FALSE;
		default:
			return FALSE;
		}
	}

	void waitForTerminationRequest()
	{
		SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
		_terminate.wait();
		_terminated.set();
	}
#elif defined(POCO_OS_FAMILY_UNIX)
	void waitForTerminationRequest()
	{
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
	}
#endif

	int run(std::string pidPath)
	{
		_pPIDFile.reset(new PIDFile(pidPath, true));
		waitForTerminationRequest();
		return 0;
	}
};
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
Poco::Event App::_terminated;
Poco::NamedEvent      App::_terminate(Poco::ProcessImpl::terminationEventName(Poco::Process::id()));
#endif

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
#if POCO_OS != POCO_OS_ANDROID
		else if (arg.find("--pidfile") != std::string::npos || arg.find("-p") != std::string::npos)
		{
			size_t equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = arg.substr(equals_pos + 1);
				App myApp;
				int result = myApp.run(pidPath);
				return result;
			}
		}
#endif
	}
	return argc - 1;
}
