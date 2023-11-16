//
// Process_UNIX.cpp
//
// Library: Foundation
// Package: Processes
// Module:  Process
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ProcessOptions.h"
#include "Poco/Process_UNIX.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Pipe.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>


#if defined(__QNX__)
#include <process.h>
#include <spawn.h>
#include <cstring>
#endif


namespace Poco {


//
// ProcessHandleImpl
//
ProcessHandleImpl::ProcessHandleImpl(pid_t pid):
	_pid(pid)
{
}


ProcessHandleImpl::~ProcessHandleImpl()
{
}


pid_t ProcessHandleImpl::id() const
{
	return _pid;
}


int ProcessHandleImpl::wait() const
{
	int status;
	int rc;
	do
	{
		rc = waitpid(_pid, &status, 0);
	}
	while (rc < 0 && errno == EINTR);
	if (rc != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));

	if (WIFEXITED(status)) // normal termination
		return WEXITSTATUS(status);
	else // termination by a signal
		return 256 + WTERMSIG(status);
}


int ProcessHandleImpl::tryWait() const
{
	int status;
	int rc;
	do
	{
		rc = waitpid(_pid, &status, WNOHANG);
	}
	while (rc < 0 && errno == EINTR);
	if (rc == 0)
		return -1;
	if (rc != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));
	if (WIFEXITED(status)) // normal termination
		return WEXITSTATUS(status);
	else // termination by a signal
		return 256 + WTERMSIG(status);
}


//
// ProcessImpl
//
ProcessImpl::PIDImpl ProcessImpl::idImpl()
{
	return getpid();
}


void ProcessImpl::timesImpl(long& userTime, long& kernelTime)
{
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	userTime   = usage.ru_utime.tv_sec;
	kernelTime = usage.ru_stime.tv_sec;
}


ProcessHandleImpl* ProcessImpl::launchImpl(const std::string& command, const ArgsImpl& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const EnvImpl& env, int options)
{
#if defined(__QNX__)
	if (initialDirectory.empty())
	{
		/// use QNX's spawn system call which is more efficient than fork/exec.
		char** argv = new char*[args.size() + 2];
		int i = 0;
		argv[i++] = const_cast<char*>(command.c_str());
		for (const auto& a: args)
			argv[i++] = const_cast<char*>(a.c_str());
		argv[i] = NULL;
		struct inheritance inherit;
		std::memset(&inherit, 0, sizeof(inherit));
		inherit.flags = SPAWN_ALIGN_DEFAULT | SPAWN_CHECK_SCRIPT | SPAWN_SEARCH_PATH;
		int fdmap[3];
		fdmap[0] = inPipe  ? inPipe->readHandle()   : 0;
		fdmap[1] = outPipe ? outPipe->writeHandle() : 1;
		fdmap[2] = errPipe ? errPipe->writeHandle() : 2;

		char** envPtr = 0;
		std::vector<char> envChars;
		std::vector<char*> envPtrs;
		if (!env.empty())
		{
			envChars = getEnvironmentVariablesBuffer(env);
			envPtrs.reserve(env.size() + 1);
			char* p = &envChars[0];
			while (*p)
			{
				envPtrs.push_back(p);
				while (*p) ++p;
				++p;
			}
			envPtrs.push_back(0);
			envPtr = &envPtrs[0];
		}

		int pid = spawn(command.c_str(), 3, fdmap, &inherit, argv, envPtr);
		delete [] argv;
		if (pid == -1)
			throw SystemException("cannot spawn", command);

		if (inPipe)  inPipe->close(Pipe::CLOSE_READ);
		if (options & PROCESS_CLOSE_STDIN) close(STDIN_FILENO);
		if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
		if (options & PROCESS_CLOSE_STDOUT) close(STDOUT_FILENO);
		if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
		if (options & PROCESS_CLOSE_STDERR) close(STDERR_FILENO);
		return new ProcessHandleImpl(pid);
	}
	else
	{
		return launchByForkExecImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options);
	}
#else
	return launchByForkExecImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options);
#endif
}


ProcessHandleImpl* ProcessImpl::launchByForkExecImpl(const std::string& command, const ArgsImpl& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const EnvImpl& env, int options)
{
#if !defined(POCO_NO_FORK_EXEC)
	// On some systems, sysconf(_SC_OPEN_MAX) returns a ridiculously high number,
	// which would closing all file descriptors up to that number extremely slow.
	// We therefore limit the maximum number of file descriptors we close.
	const long CLOSE_FD_MAX = 100000;

	do
	{
		// We must not allocate memory after fork(),
		// therefore allocate all required buffers first.

		std::vector<char> envChars = getEnvironmentVariablesBuffer(env);
		std::vector<char*> argv(args.size() + 2);
		int i = 0;
		argv[i++] = const_cast<char*>(command.c_str());
		for (const auto& a: args)
		{
			argv[i++] = const_cast<char*>(a.c_str());
		}
		argv[i] = NULL;

		const char* pInitialDirectory = initialDirectory.empty() ? 0 : initialDirectory.c_str();

		int pid = fork();
		if (pid < 0)
		{
			throw SystemException("Cannot fork process for", command);
		}
		else if (pid == 0)
		{
			if (pInitialDirectory)
			{
				if (chdir(pInitialDirectory) != 0)
				{
					break;
				}
			}

			// set environment variables
			char* p = &envChars[0];
			while (*p)
			{
				putenv(p);
				while (*p) ++p;
				++p;
			}

			// setup redirection
			if (inPipe)
			{
				dup2(inPipe->readHandle(), STDIN_FILENO);
				inPipe->close(Pipe::CLOSE_BOTH);
			}
			if (options & PROCESS_CLOSE_STDIN) close(STDIN_FILENO);

			// outPipe and errPipe may be the same, so we dup first and close later
			if (outPipe) dup2(outPipe->writeHandle(), STDOUT_FILENO);
			if (errPipe) dup2(errPipe->writeHandle(), STDERR_FILENO);
			if (outPipe) outPipe->close(Pipe::CLOSE_BOTH);
			if (options & PROCESS_CLOSE_STDOUT) close(STDOUT_FILENO);
			if (errPipe) errPipe->close(Pipe::CLOSE_BOTH);
			if (options & PROCESS_CLOSE_STDERR) close(STDERR_FILENO);
			// close all open file descriptors other than stdin, stdout, stderr
			long fdMax = sysconf(_SC_OPEN_MAX);
			// on some systems, sysconf(_SC_OPEN_MAX) returns a ridiculously high number
			if (fdMax > CLOSE_FD_MAX) fdMax = CLOSE_FD_MAX;
			for (long j = 3; j < fdMax; ++j)
			{
				close(j);
			}

			execvp(argv[0], &argv[0]);
			break;
		}

		if (inPipe)  inPipe->close(Pipe::CLOSE_READ);
		if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
		if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
		return new ProcessHandleImpl(pid);
	}
	while (false);

	_exit(72);
#else
	throw Poco::NotImplementedException("platform does not allow fork/exec");
#endif
}


void ProcessImpl::killImpl(ProcessHandleImpl& handle)
{
	killImpl(handle.id());
}


void ProcessImpl::killImpl(PIDImpl pid)
{
	if (kill(pid, SIGKILL) != 0)
	{
		switch (errno)
		{
		case ESRCH:
			throw NotFoundException("cannot kill process");
		case EPERM:
			throw NoPermissionException("cannot kill process");
		default:
			throw SystemException("cannot kill process");
		}
	}
}


bool ProcessImpl::isRunningImpl(const ProcessHandleImpl& handle)
{
	return isRunningImpl(handle.id());
}


bool ProcessImpl::isRunningImpl(PIDImpl pid)
{
	if (kill(pid, 0) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ProcessImpl::requestTerminationImpl(PIDImpl pid)
{
	if (kill(pid, SIGINT) != 0)
	{
		switch (errno)
		{
		case ESRCH:
			throw NotFoundException("cannot terminate process");
		case EPERM:
			throw NoPermissionException("cannot terminate process");
		default:
			throw SystemException("cannot terminate process");
		}
	}
}


} // namespace Poco
