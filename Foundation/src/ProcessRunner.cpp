//
// ProcessRunner.cpp
//
// Library: Foundation
// Package: Processes
// Module:  ProcessRunner
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:    BSL-1.0
//


#include "Poco/ProcessRunner.h"
#include "Poco/PIDFile.h"
#include "Poco/FileStream.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/Error.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/NamedEvent.h"
#include "Poco/UnWindows.h"
#else
#include <signal.h>
#include <cerrno>
#endif
#include <fstream>


using Poco::Thread;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::FileInputStream;
using Poco::AutoPtr;
using Poco::File;
using Poco::Path;
using Poco::Stopwatch;


namespace Poco {


ProcessRunner::ProcessRunner(const std::string& cmd,
		const Args& args,
		const std::string& pidFile,
		int options,
		int timeout,
		bool startProcess,
		const Args& pidArgFmt): _cmd(cmd),
			_args(args),
			_pid(INVALID_PID),
			_pidFile(pidFile),
			_options(options),
			_timeout(timeout),
			_pPH(nullptr),
			_started(false),
			_rc(RESULT_UNKNOWN),
			_runCount(0)
{
	if (_pidFile.empty() && !_args.empty() && !pidArgFmt.empty())
	{
		for (const auto& fmt : pidArgFmt)
		{
			for (const auto& arg : _args)
			{
				std::string a = Poco::trim(arg);
				std::size_t pos = a.find(fmt);
				if (pos == 0)
				{
					_pidFile = a.substr(fmt.length());
					PIDFile::getFileName(_pidFile);
					break;
				}
			}
		}
	}
	if (startProcess) start();
}


ProcessRunner::~ProcessRunner()
{
	try
	{
		stop();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


std::string ProcessRunner::cmdLine() const
{
	std::string cmdL = _cmd + ' ';
	auto it = _args.begin();
	auto end = _args.end();
	for (; it != end;)
	{
		cmdL.append(*it);
		if (++it == end) break;
		cmdL.append(1, ' ');
	}
	return cmdL;
}


void ProcessRunner::run()
{
	int errHandle = 0;
	int errPID = 0;
	int errRC = 0;

	{
		Poco::FastMutex::ScopedLock l(_mutex);
		_error.clear();
	}

	_pid = INVALID_PID;
	_pPH = nullptr;

	ProcessHandle* pPH = nullptr;
	try
	{
		pPH = new ProcessHandle(Process::launch(_cmd, _args, _options));
		errHandle = Error::last();

		_pid = pPH->id();
		errPID = Error::last();

#if defined(POCO_OS_FAMILY_WINDOWS)
		if (_options & PROCESS_KILL_TREE)
		{
			_hJob = CreateJobObjectW(nullptr, nullptr);
			if (_hJob)
			{
				JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {};
				jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
				SetInformationJobObject(static_cast<HANDLE>(_hJob),
					JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
				HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE,
					FALSE, static_cast<DWORD>(pPH->id()));
				if (hProc)
				{
					AssignProcessToJobObject(static_cast<HANDLE>(_hJob), hProc);
					CloseHandle(hProc);
				}
			}
		}
#endif
		// Set _pPH after _pid to ensure pid() returns valid value when running() is true
		_pPH = pPH;

		_rc = pPH->wait();
		errRC = Error::last();

		if (errHandle || errPID || errRC || _rc != 0)
		{
			Poco::FastMutex::ScopedLock l(_mutex);

			Poco::format(_error, "ProcessRunner::run() error; "
				"handle=%d (%d:%s); pid=%d (%d:%s); return=%d (%d:%s)",
				(pPH ? pPH->id() : 0), errHandle, Error::getMessage(errHandle),
				_pid.load(), errPID, Error::getMessage(errPID),
				_rc.load(), errRC, Error::getMessage(errRC));
		}
	}
	catch (Poco::Exception& ex)
	{
		setError(ex.displayText());
	}
	catch (std::exception& ex)
	{
		setError(ex.what());
	}
	catch (...)
	{
		setError("Unknown exception"s);
	}

	_pid = INVALID_PID;
	_pPH = nullptr;
	++_runCount;
	delete pPH;
}


void ProcessRunner::stop()
{
	if (_started)
	{
		PID pid;
		_sw.restart();
		if (_pPH.exchange(nullptr) && ((pid = _pid.exchange(INVALID_PID))) != INVALID_PID)
		{
			if (pid <= 0)
				throw Poco::IllegalStateException("Invalid PID, can't terminate process");

#if defined(POCO_OS_FAMILY_WINDOWS)
			// On Windows, Process::requestTermination() creates a temporary NamedEvent,
			// signals it, and immediately destroys it. If the child process has not yet
			// created its own NamedEvent handle (during static initialization of its
			// ServerApplication::_terminate member), the kernel event object is destroyed
			// when our handle closes, and the termination signal is lost.
			// Keep the NamedEvent alive here so the kernel object persists until the child
			// opens its own handle and receives the signal.
			NamedEvent terminateEvent(Process::terminationEventName(pid));
			terminateEvent.set();
#else
			if (_options & PROCESS_KILL_TREE)
				::kill(-pid, SIGINT); // signal entire process group
			else
				Process::requestTermination(pid);
#endif

#if !defined(POCO_OS_FAMILY_WINDOWS)
			if (_options & PROCESS_KILL_TREE)
			{
				// Wait for the entire process group to exit.
				// setpgid(0, 0) was called in the child, so -pid
				// addresses the whole group.
				// kill(-pid, 0) returns 0 if any process in the group exists
				// and we have permission, or -1 with errno:
				//   ESRCH  - no such process group (done)
				//   EPERM  - group exists but no permission (still alive)
				while (::kill(-pid, 0) == 0 || errno == EPERM)
				{
					if (_sw.elapsedSeconds() > _timeout)
					{
						::kill(-pid, SIGKILL);
						Stopwatch killSw; killSw.start();
						while (::kill(-pid, 0) == 0 || errno == EPERM)
						{
							if (killSw.elapsedSeconds() > _timeout)
							{
								throw Poco::TimeoutException("Unable to terminate process tree");
							}
							Thread::sleep(10);
						}
						break;
					}
					Thread::sleep(10);
				}
			}
			else
#endif
			{
				while (Process::isRunning(pid))
				{
					if (_sw.elapsedSeconds() > _timeout)
					{
						Process::kill(pid);
						Stopwatch killSw; killSw.start();
						while (Process::isRunning(pid))
						{
							if (killSw.elapsedSeconds() > _timeout)
							{
								throw Poco::TimeoutException("Unable to terminate process");
							}
							Thread::sleep(10);
						}
						break;
					}
					Thread::sleep(10);
				}
			}
			_t.join();
		}

#if defined(POCO_OS_FAMILY_WINDOWS)
		if (_hJob)
		{
			CloseHandle(static_cast<HANDLE>(_hJob));
			_hJob = nullptr;
		}
#endif

		if (!_pidFile.empty())
		{
			File pidFile(_pidFile);
			if (pidFile.exists())
			{
				std::string msg;
				Poco::format(msg, "Waiting for PID file (pidFile: '%s')", _pidFile);
				_sw.restart();
				while (pidFile.exists())
				{
					if (_sw.elapsedSeconds() > _timeout)
					{
						try
						{
							pidFile.remove(false);
						}
						catch (...)
						{
							// give up trying to remove stale pid file
						}
						break;
					}
					Thread::sleep(10);
				}
			}
		}

		{
			Poco::FastMutex::ScopedLock l(_mutex);
			_error.clear();
		}
	}
	_started.store(false);
}


void ProcessRunner::checkError()
{
	Poco::FastMutex::ScopedLock l(_mutex);

	if (!_error.empty())
		throw Poco::RuntimeException(_error);
}


void ProcessRunner::checkTimeout(const std::string& msg)
{
	if (_sw.elapsedSeconds() > _timeout)
	{
		throw Poco::TimeoutException(
			Poco::format("ProcessRunner::checkTimeout(): %s", msg));
	}
	Thread::sleep(10);
}


void ProcessRunner::checkStatus(const std::string& msg, bool tOut)
{
	checkError();
	if (tOut) checkTimeout(msg);
}


void ProcessRunner::start()
{
	if (!_started.exchange(true))
	{
		std::string execPath = File(_cmd).getExecutablePath();
		if (execPath.empty())
		{
			throw Poco::FileNotFoundException(
				Poco::format("ProcessRunner::start(%s): command not found", _cmd));
		}
		else if (!File(execPath).canExecute())
		{
			throw Poco::ExecuteFileException(
				Poco::format("ProcessRunner::start(%s): cannot execute", _cmd));
		}

		int prevRunCnt = runCount();

		_t.start(*this);

		try
		{
			std::string msg;
			Poco::format(msg, "Waiting for process to start (pidFile: '%s')", _pidFile);
			_sw.restart();

			// wait for the process to be either running or completed by monitoring run counts.
			while (!running() && prevRunCnt >= runCount()) checkStatus(msg);

			// we could wait for the process handle != INVALID_PID,
			// but if pidFile name was given, we should wait for
			// the process to write it
			if (!_pidFile.empty())
			{
				_sw.restart();
				// wait until process is fully initialized
				File pidFile(_pidFile);
				while (!pidFile.exists())
					checkStatus(Poco::format("waiting for PID file '%s' creation.", _pidFile));

				// verify that the file content is actually the process PID
				FileInputStream fis(_pidFile);
				PID fPID = 0;
				if (fis.peek() != std::ifstream::traits_type::eof())
					fis >> fPID;
				while (fPID != pid())
				{
					fis.clear(); fis.seekg(0); fis >> fPID;
					checkStatus(Poco::format("waiting for new PID (%s)", _pidFile));
				}
			}
		}
		catch (...)
		{
			// Clean up the process to prevent orphan
			PID p = _pid.load();
			if (p != INVALID_PID)
			{
				Process::kill(p);
			}
			if (_t.isRunning())
				_t.join();
			_started.store(false);
			throw;
		}
	}
	else
		throw Poco::InvalidAccessException("start() called on started ProcessRunner");
}


} // namespace Poco
