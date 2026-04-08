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
	_pid(pid),
	_event(Event::EVENT_MANUALRESET)
{
}


ProcessHandleImpl::~ProcessHandleImpl()
{
}


pid_t ProcessHandleImpl::id() const
{
	return _pid;
}


// Converts a raw waitpid status to a Poco exit code.
// Normal exit: WEXITSTATUS (0-255).
// Signal death: 256 + signal_number.
int ProcessHandleImpl::statusToExitCode(int status)
{
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else
		return 256 + WTERMSIG(status);
}


int ProcessHandleImpl::wait() const
{
	if (wait(0) != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));

	return statusToExitCode(_status.load());
}


int ProcessHandleImpl::wait(int options) const
{
	if (_hasStatus.load()) return _pid;
	int status;
	int rc;
	do
	{
		rc = ::waitpid(_pid, &status, options);
	}
	while (rc < 0 && errno == EINTR);
	if (rc == _pid)
	{
		_status.store(status);
		_hasStatus.store(true);
		// N.B. _hasStatus must be stored before _event.set()
		// because the ECHILD/WNOHANG path checks tryWait(0) && _hasStatus
		_event.set();
	}
	else if (rc < 0 && errno == ECHILD)
	{
		// Another thread reaped the process; synchronize with its status update.
		// Preserve the requested wait semantics:
		//  - blocking waits block until the status has been published
		//  - WNOHANG waits remain non-blocking
		if (options & WNOHANG)
		{
			if (_event.tryWait(0) && _hasStatus.load())
				rc = _pid;
			else
				rc = 0;
		}
		else
		{
			_event.wait();
			if (_hasStatus.load())
				rc = _pid;
			else
				throw SystemException("Lost process status (internal error)", NumberFormatter::format(_pid));
		}
	}

	return rc;
}


int ProcessHandleImpl::tryWait() const
{
	int rc = wait(WNOHANG);
	if (rc == 0)
		return -1;
	if (rc != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));
	return statusToExitCode(_status.load());
}


bool ProcessHandleImpl::isRunning() const
{
	if (_hasStatus.load())
		return false;
	// Note: concurrent calls may briefly return true after the process
	// exits (transient false-positive) until the reaping thread publishes
	// status. Self-correcting on the next call.
	return wait(WNOHANG) == 0;
}


//
// ProcessImpl
//
ProcessImpl::PIDImpl ProcessImpl::idImpl()
{
	return ::getpid();
}


void ProcessImpl::timesImpl(long& userTime, long& kernelTime)
{
	struct rusage usage;
	::getrusage(RUSAGE_SELF, &usage);
	userTime   = usage.ru_utime.tv_sec;
	kernelTime = usage.ru_stime.tv_sec;
}


void ProcessImpl::timesMicrosecondsImpl(Poco::Int64& userTime, Poco::Int64& kernelTime)
{
	struct rusage usage;
	::getrusage(RUSAGE_SELF, &usage);
	userTime   = static_cast<Poco::Int64>(usage.ru_utime.tv_sec)*1000000 + usage.ru_utime.tv_usec;
	kernelTime = static_cast<Poco::Int64>(usage.ru_stime.tv_sec)*1000000 + usage.ru_stime.tv_usec;
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
		argv[i] = nullptr;
		struct inheritance inherit;
		std::memset(&inherit, 0, sizeof(inherit));
		inherit.flags = SPAWN_ALIGN_DEFAULT | SPAWN_CHECK_SCRIPT | SPAWN_SEARCH_PATH;
		if (options & PROCESS_KILL_TREE)
		{
			inherit.flags |= SPAWN_SETGROUP;
			inherit.pgroup = SPAWN_NEWPGROUP;
		}
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

		int pid = ::spawn(command.c_str(), 3, fdmap, &inherit, argv, envPtr);
		delete [] argv;
		if (pid == -1)
			throw SystemException("cannot spawn", command);

		if (inPipe)  inPipe->close(Pipe::CLOSE_READ);
		if (options & PROCESS_CLOSE_STDIN) ::close(STDIN_FILENO);
		if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
		if (options & PROCESS_CLOSE_STDOUT) ::close(STDOUT_FILENO);
		if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
		if (options & PROCESS_CLOSE_STDERR) ::close(STDERR_FILENO);
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
		argv[i] = nullptr;

		const char* pInitialDirectory = initialDirectory.empty() ? nullptr : initialDirectory.c_str();

		int pid = ::fork();
		if (pid < 0)
		{
			throw SystemException("Cannot fork process for", command);
		}
		else if (pid == 0)
		{
			if (pInitialDirectory)
			{
				if (::chdir(pInitialDirectory) != 0)
				{
					break;
				}
			}

			// set environment variables
			char* p = &envChars[0];
			while (*p)
			{
				::putenv(p);
				while (*p) ++p;
				++p;
			}

			// setup redirection
			if (inPipe)
			{
				::dup2(inPipe->readHandle(), STDIN_FILENO);
				inPipe->close(Pipe::CLOSE_BOTH);
			}
			if (options & PROCESS_CLOSE_STDIN) ::close(STDIN_FILENO);

			// outPipe and errPipe may be the same, so we dup first and close later
			if (outPipe) ::dup2(outPipe->writeHandle(), STDOUT_FILENO);
			if (errPipe) ::dup2(errPipe->writeHandle(), STDERR_FILENO);
			if (outPipe) outPipe->close(Pipe::CLOSE_BOTH);
			if (options & PROCESS_CLOSE_STDOUT) ::close(STDOUT_FILENO);
			if (errPipe) errPipe->close(Pipe::CLOSE_BOTH);
			if (options & PROCESS_CLOSE_STDERR) ::close(STDERR_FILENO);
			// close all open file descriptors other than stdin, stdout, stderr
			long fdMax = ::sysconf(_SC_OPEN_MAX);
			// on some systems, sysconf(_SC_OPEN_MAX) returns a ridiculously high number
			if (fdMax > CLOSE_FD_MAX) fdMax = CLOSE_FD_MAX;
			for (long j = 3; j < fdMax; ++j)
			{
				::close(j);
			}

			// Create a new process group so the entire tree can be signaled.
			if (options & PROCESS_KILL_TREE)
			{
				if (setpgid(0, 0) != 0)
					_exit(PROCESS_EXIT_SETPGID_FAILED);
			}

			::execvp(argv[0], &argv[0]);
			break;
		}

		// Close the race window where stop() could be called before the
		// child reaches setpgid(0, 0). setpgid(pid, pid) is idempotent
		// with the child's call; if the child already called it, this
		// is a harmless no-op (EACCES after exec is also benign).
		if (options & PROCESS_KILL_TREE)
			setpgid(pid, pid);

		if (inPipe)  inPipe->close(Pipe::CLOSE_READ);
		if (outPipe) outPipe->close(Pipe::CLOSE_WRITE);
		if (errPipe) errPipe->close(Pipe::CLOSE_WRITE);
		return new ProcessHandleImpl(pid);
	}
	while (false);

	_exit(PROCESS_EXIT_EXEC_FAILED);
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
	if (::kill(pid, SIGKILL) != 0)
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
	return handle.isRunning();
}


bool ProcessImpl::isRunningImpl(PIDImpl pid)
{
	int status;
	int rc;
	do
	{
		rc = ::waitpid(pid, &status, WNOHANG);
	}
	while (rc < 0 && errno == EINTR);
	if (rc == pid)
		return false;
	if (rc == 0)
		return true;
	// Not our child or error; fall back to kill check
	rc = ::kill(pid, 0);
	if (rc == 0) return true;
	return errno == EPERM;
}


void ProcessImpl::requestTerminationImpl(PIDImpl pid)
{
	if (::kill(pid, SIGINT) != 0)
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
