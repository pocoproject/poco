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
#include <fstream>
#include <cstdlib>
#include <vector>
#include <signal.h>

#include "Poco/FileStreamRWLock.h"
#include "Poco/Event.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/ProcessOptions.h"
#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"

#if defined(POCO_OS_FAMILY_UNIX)
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Thread.h"
#include "Poco/Process.h"
#include "Poco/Event.h"
#include "Poco/NamedEvent.h"
#endif

using namespace Poco;

#if defined(POCO_OS_FAMILY_UNIX)
class MyRunnable: public Runnable
{
public:
	MyRunnable(): _ran(false)
	{
	}

	void run()
	{
		Thread* pThread = Thread::current();
		if (pThread)
		{
			_threadName = pThread->name();
		}
		_ran = true;
	}

	bool ran() const
	{
		return _ran;
	}

private:
	bool _ran;
	std::string _threadName;
};
#endif

#if POCO_OS != POCO_OS_ANDROID
class MyApp
{
public:
	MyApp()
	{
#if defined(POCO_OS_FAMILY_UNIX)
		// The termination signals are blocked before a PID file tells the parent
		// that this process runs, so that a request to end that comes at once
		// waits for waitForTerminationRequest() instead of ending the process.
		const sigset_t sset = terminationSignals();
		sigprocmask(SIG_BLOCK, &sset, nullptr);
#endif
	}

	~MyApp() {}

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

	int runWithChild(std::string pidPath, const std::string& childPidPath, const std::string& exePath)
	{
		_pPIDFile.reset(new PIDFile(pidPath, true));
		// Launch a child TestApp with its own PID file.
		// The child auto-joins the Job Object created by ProcessRunner
		// (PROCESS_KILL_TREE), so closing the Job handle kills both.
		Process::Args childArgs;
		childArgs.push_back(std::string("--pidfile=").append(childPidPath));
		ProcessHandle ph = Process::launch(exePath, childArgs);
		waitForTerminationRequest();
		// Child may already be killed by Job Object; ignore errors
		try
		{
			if (Process::isRunning(ph))
				Process::requestTermination(ph.id());
		}
		catch (...) {}
		return 0;
	}

#elif defined(POCO_OS_FAMILY_UNIX)
	static sigset_t terminationSignals()
	{
		sigset_t sset;
		sigemptyset(&sset);
		if (!std::getenv("POCO_ENABLE_DEBUGGER"))
		{
			sigaddset(&sset, SIGINT);
		}
		sigaddset(&sset, SIGQUIT);
		sigaddset(&sset, SIGTERM);
		return sset;
	}

	void waitForTerminationRequest()
	{
		const sigset_t sset = terminationSignals();
		int sig;
		sigwait(&sset, &sig);
	}

	int runThreads(std::string pidPath)
	{
		_pPIDFile.reset(new PIDFile(pidPath, true));

		uint32_t sigMask = 4; // Block SIGINT

		Thread thread1(sigMask);
		Thread thread2(sigMask);
		Thread thread3(sigMask);
		Thread thread4(sigMask);

		MyRunnable r1;
		MyRunnable r2;
		MyRunnable r3;
		MyRunnable r4;

		thread1.start(r1);
		thread2.start(r2);
		thread3.start(r3);
		thread4.start(r4);

		waitForTerminationRequest();

		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		return 0;
	}

	int runWithChild(std::string pidPath, const std::string& childPidPath)
	{
		// Fork a child that writes its own PID file and sleeps.
		// Both parent and child are in the same process group
		// (set by PROCESS_KILL_TREE via setpgid), so sending a signal
		// to the process group should terminate both.
		pid_t child = fork();
		if (child == 0)
		{
			// Child process: write PID file and wait
			PIDFile childPID(childPidPath, true);
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGINT);
			sigaddset(&sset, SIGQUIT);
			sigaddset(&sset, SIGTERM);
			sigprocmask(SIG_BLOCK, &sset, nullptr);
			int sig;
			sigwait(&sset, &sig);
			_exit(0);
		}
		else if (child > 0)
		{
			// Parent process
			_pPIDFile.reset(new PIDFile(pidPath, true));
			waitForTerminationRequest();
			// Reap child
			int status = 0;
			waitpid(child, &status, WNOHANG);
			return 0;
		}
		else
		{
			return 1; // fork failed
		}
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
Poco::Event MyApp::_terminated;
Poco::NamedEvent      MyApp::_terminate(Poco::ProcessImpl::terminationEventName(Poco::Process::id()));

#include "Poco/File.h"
#include "Poco/File_WIN32U.h"

HANDLE openFileWithRWAccess(const std::string& path)
{
	DWORD access = GENERIC_READ | GENERIC_WRITE;
	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

	HANDLE handle = CreateFileA(path.c_str(), access, shareMode, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (handle == INVALID_HANDLE_VALUE)
		Poco::File::handleLastError(path);

	return handle;
}
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
Poco::Event breakArrived;


BOOL WINAPI breakHandler(DWORD ctrlType)
{
	if (ctrlType != CTRL_BREAK_EVENT && ctrlType != CTRL_C_EVENT) return FALSE;
	breakArrived.set();
	return TRUE;
}
#endif


int waitForBreak(const std::string& readyPath)
	/// Exits with 42 when a CTRL_BREAK_EVENT (Windows) or SIGINT (POSIX)
	/// arrives, the request to end a console program understands. The ready
	/// file is written once that request is waited for.
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	SetConsoleCtrlHandler(breakHandler, TRUE);
	{
		std::ofstream ready(readyPath);
		ready << "ready";
	}
	return breakArrived.tryWait(60000) ? 42 : 1;
#elif defined(POCO_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGINT);
	sigprocmask(SIG_BLOCK, &sset, nullptr);
	{
		std::ofstream ready(readyPath);
		ready << "ready";
	}
	int sig = 0;
	sigwait(&sset, &sig);
	return sig == SIGINT ? 42 : 1;
#else
	return 1;
#endif
}


int breakOwnChild(const std::string& self, const std::string& readyPath)
	/// Without a console of its own, starts a child that waits for the
	/// request to end, asks it to end, and exits with its exit code, or with
	/// 3 if its own standard handles were not the same afterwards (Windows).
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	FreeConsole();
	// Standard handles of its own, as a program without a console sets them.
	HANDLE readEnd = nullptr;
	HANDLE writeEnd = nullptr;
	if (!CreatePipe(&readEnd, &writeEnd, nullptr, 0)) return 4;
	SetStdHandle(STD_INPUT_HANDLE, readEnd);
	SetStdHandle(STD_OUTPUT_HANDLE, writeEnd);
	SetStdHandle(STD_ERROR_HANDLE, writeEnd);
#endif
	Poco::Process::Args args;
	args.push_back("-wait-break");
	args.push_back(readyPath);
	Poco::ProcessHandle child = Poco::Process::launch(self, args, Poco::PROCESS_NEW_GROUP);
	Poco::Stopwatch sw;
	sw.start();
	while (!Poco::File(readyPath).exists())
	{
		if (sw.elapsedSeconds() > 30)
		{
			Poco::Process::kill(child);
			return 2;
		}
		Poco::Thread::sleep(10);
	}
	Poco::Process::requestTermination(child.id(), Poco::PROCESS_NEW_GROUP);
	const int rc = child.wait();
#if defined(POCO_OS_FAMILY_WINDOWS)
	const bool kept = GetStdHandle(STD_INPUT_HANDLE) == readEnd
		&& GetStdHandle(STD_OUTPUT_HANDLE) == writeEnd
		&& GetStdHandle(STD_ERROR_HANDLE) == writeEnd;
	CloseHandle(readEnd);
	CloseHandle(writeEnd);
	if (!kept) return 3;
#endif
	return rc;
}


int signalInvalidPids()
	/// Exits with 42 if Process::kill() and Process::requestTermination()
	/// refuse a PID that names no single process, and Process::isRunning()
	/// takes it as not running. Meant to run in a process group of its own,
	/// which a signal sent to that group anyway would reach alone.
{
	std::vector<Poco::Process::PID> pids;
	pids.push_back(0);
#if defined(POCO_OS_FAMILY_UNIX)
	pids.push_back(-Poco::Process::id()); // its own process group
#endif
	for (const auto pid: pids)
	{
		if (Poco::Process::isRunning(pid)) return 1;
		try
		{
			Poco::Process::requestTermination(pid);
			return 2;
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
		try
		{
			Poco::Process::kill(pid);
			return 3;
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}
	return 42;
}


int breakOwnChildWithoutStdHandles(const std::string& self, const std::string& readyPath)
	/// Runs breakOwnChild() in a child that inherits no standard handles, and
	/// exits with its exit code. On Windows, that child starts without
	/// STARTF_USESTDHANDLES, whose standard handles attaching to a console
	/// replaces.
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	SetStdHandle(STD_INPUT_HANDLE, nullptr);
	SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
	SetStdHandle(STD_ERROR_HANDLE, nullptr);
#endif
	Poco::Process::Args args;
	args.push_back("-break-own-child");
	args.push_back(readyPath);
	Poco::ProcessHandle child = Poco::Process::launch(self, args);
	return child.wait();
}


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
		else if (arg == "-wait-break" && argc > 2)
		{
			return waitForBreak(argv[2]);
		}
		else if (arg == "-break-own-child" && argc > 2)
		{
			return breakOwnChild(argv[0], argv[2]);
		}
		else if (arg == "-break-own-child-without-std-handles" && argc > 2)
		{
			return breakOwnChildWithoutStdHandles(argv[0], argv[2]);
		}
		else if (arg == "-signal-invalid-pids")
		{
			return signalInvalidPids();
		}
		else if (arg == "-echo-args")
		{
			for (int i = 2; i < argc; ++i)
			{
				std::cout << argv[i] << std::endl;
			}
		}
#if defined(POCO_OS_FAMILY_UNIX)
		else if (arg == "-std-fds")
		{
			// Which of the standard descriptors this process has: 1 for
			// standard input, 2 for standard output and 4 for standard
			// error, added up.
			int taken = 0;
			for (int fd = 0; fd < 3; ++fd)
			{
				if (::fcntl(fd, F_GETFD) != -1) taken |= 1 << fd;
			}
			return taken;
		}
		else if (argc > 2 && arg.find("--pidfile") != std::string::npos && std::string(argv[2]) == "--launch-thread")
		{
			size_t equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = arg.substr(equals_pos + 1);
				MyApp myApp;
				int result = myApp.runThreads(pidPath);
				return result;
			}
		}
#endif
#if POCO_OS != POCO_OS_ANDROID
		else if (argc > 2 && arg.find("--pidfile") != std::string::npos && std::string(argv[2]).find("--spawn-child=") == 0)
		{
			size_t equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = arg.substr(equals_pos + 1);
				std::string childPidPath = std::string(argv[2]).substr(std::string("--spawn-child=").length());
				MyApp myApp;
#if defined(POCO_OS_FAMILY_UNIX)
				return myApp.runWithChild(pidPath, childPidPath);
#else
				return myApp.runWithChild(pidPath, childPidPath, argv[0]);
#endif
			}
		}
		else if (arg.find("--pidfile") != std::string::npos || arg.find("-p") != std::string::npos)
		{
			size_t equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = arg.substr(equals_pos + 1);
				MyApp myApp;
				int result = myApp.run(pidPath);
				return result;
			}
		}
#endif
		else if (argc > 2 && arg.find("--lock-file") != std::string::npos && std::string(argv[2]).find("--pidfile") != std::string::npos) 
		{
			std::string pidfArg = std::string(argv[2]);
			std::unique_ptr<PIDFile> pidF;
			size_t equals_pos = pidfArg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = pidfArg.substr(equals_pos + 1);
				pidF = std::make_unique<PIDFile>(pidPath, true);
			}
			if (pidF == nullptr)
			{
				return -1;
			}
			equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string fl = arg.substr(equals_pos + 1);
#if POCO_OS != POCO_OS_WINDOWS_NT
				FileStream fs(fl, std::ios::in | std::ios::out | std::ios::binary);
#else
				FileStream fs;
				fs.openHandle(openFileWithRWAccess(fl), std::ios::in | std::ios::out | std::ios::binary);
#endif // POCO_OS != POCO_OS_WINDOWS_NT
				Poco::Int32 ok = 1;
				Poco::Int32 lastCount = 0;
				Poco::Int32 counter = 0;
				FileStreamRWLock lock(fs, 0, sizeof(counter));
				for (int i = 0; i < 100; ++i)
				{
					lock.readLock();
					fs.seekg(0, std::ios::beg);
					fs.read((char *)&counter, sizeof(counter));
					lastCount = counter;
					for (int k = 0; k < 100; ++k)
					{
						if (counter != lastCount) ok = -1;
					}
					lock.unlock();
					lock.writeLock();
					for (int k = 0; k < 100; ++k)
					{
						counter = 0;
						fs.seekg(0, std::ios::beg);
						fs.read((char *)&counter, sizeof(counter));
						--counter;
						fs.seekp(0, std::ios::beg);
						fs.write((char *)&counter, sizeof(counter));
						fs.flushToDisk();
					}
					for (int k = 0; k < 100; ++k)
					{
						fs.seekg(0, std::ios::beg);
						fs.read((char *)&counter, sizeof(counter));
						++counter;
						fs.seekp(0, std::ios::beg);
						fs.write((char *)&counter, sizeof(counter));
						fs.flushToDisk();
					}
					fs.seekg(0, std::ios::beg);
					fs.read((char *)&counter, sizeof(counter));
					++counter;
					fs.seekp(0, std::ios::beg);
					fs.write((char *)&counter, sizeof(counter));
					fs.flushToDisk();
					if (counter <= lastCount) ok = -1;
					lock.unlock();
				}
				return ok * counter;
			}
			return -1;
		}
		else if (argc > 2 && arg.find("--trylock-file") != std::string::npos && std::string(argv[2]).find("--pidfile") != std::string::npos) 
		{
			std::string pidfArg = std::string(argv[2]);
			std::unique_ptr<PIDFile> pidF;
			size_t equals_pos = pidfArg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string pidPath = pidfArg.substr(equals_pos + 1);
				pidF = std::make_unique<PIDFile>(pidPath, true);
			}
			if (pidF == nullptr)
			{
				return -1;
			}
			equals_pos = arg.find('=');
			if (equals_pos != std::string::npos)
			{
				std::string fl = arg.substr(equals_pos + 1);
#if POCO_OS != POCO_OS_WINDOWS_NT
				FileStream fs(fl, std::ios::in | std::ios::out | std::ios::binary);
#else
				FileStream fs;
				fs.openHandle(openFileWithRWAccess(fl), std::ios::in | std::ios::out | std::ios::binary);
#endif // POCO_OS != POCO_OS_WINDOWS_NT
				Poco::Int32 ok = 1;
				Poco::Int32 lastCount = 0;
				Poco::Int32 counter = 0;
				FileStreamRWLock lock(fs, 0, sizeof(counter));
				for (int i = 0; i < 100; ++i)
				{
					while (!lock.tryReadLock()) Thread::yield();
					fs.seekg(0, std::ios::beg);
					fs.read((char *)&counter, sizeof(counter));
					lastCount = counter;
					for (int k = 0; k < 100; ++k)
					{
						if (counter != lastCount) ok = -1;
						Thread::yield();
					}
					lock.unlock();
					while (!lock.tryWriteLock()) Thread::yield();
					for (int k = 0; k < 100; ++k)
					{
						counter = 0;
						fs.seekg(0, std::ios::beg);
						fs.read((char *)&counter, sizeof(counter));
						--counter;
						fs.seekp(0, std::ios::beg);
						fs.write((char *)&counter, sizeof(counter));
						fs.flushToDisk();
						Thread::yield();
					}
					for (int k = 0; k < 100; ++k)
					{
						fs.seekg(0, std::ios::beg);
						fs.read((char *)&counter, sizeof(counter));
						++counter;
						fs.seekp(0, std::ios::beg);
						fs.write((char *)&counter, sizeof(counter));
						fs.flushToDisk();
						Thread::yield();
					}
					fs.seekg(0, std::ios::beg);
					fs.read((char *)&counter, sizeof(counter));
					++counter;
					fs.seekp(0, std::ios::beg);
					fs.write((char *)&counter, sizeof(counter));
					fs.flushToDisk();
					if (counter <= lastCount) ok = -1;
					lock.unlock();
				}
				return ok * counter;
			}
			return -1;
		}
	}
	return argc - 1;
}
