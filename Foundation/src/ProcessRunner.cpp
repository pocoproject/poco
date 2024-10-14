//
// ProcessRunner.cpp
//
// Library: Foundation
// Package: Processes
// Module:  ProcessRunner
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
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
	ProcessHandle* pPH = nullptr;
	try
	{
		_pPH = pPH = new ProcessHandle(Process::launch(_cmd, _args, _options));
		_pid = pPH->id();
		_rc = pPH->wait();
	}
	catch (...)
	{
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
		Stopwatch sw; sw.start();
		if (_pPH.exchange(nullptr) && ((pid = _pid.exchange(INVALID_PID))) != INVALID_PID)
		{
			while (Process::isRunning(pid))
			{
				if (pid > 0)
				{
					Process::requestTermination(pid);
					checkTimeout(sw, "Waiting for process termination");
				}
				else throw Poco::IllegalStateException("Invalid PID, can;t terminate process");
			}
			_t.join();
		}

		if (!_pidFile.empty())
		{
			if (!_pidFile.empty())
			{
				File pidFile(_pidFile);
				_pidFile.clear();
				std::string msg;
				Poco::format(msg, "Waiting for PID file (pidFile: '%s')", _pidFile);
				sw.restart();
				while (pidFile.exists())
					checkTimeout(sw, msg);
			}
		}
	}
	_started.store(false);
}


void ProcessRunner::checkTimeout(const Stopwatch& sw, const std::string& msg)
{
	if (sw.elapsedSeconds() > _timeout)
	{
		throw Poco::TimeoutException(
			Poco::format("ProcessRunner::checkTimeout(): %s", msg));
	}
	Thread::sleep(10);
}


void ProcessRunner::start()
{
	if (!_started.exchange(true))
	{
		int prevRunCnt = runCount();

		_t.start(*this);

		std::string msg;
		Poco::format(msg, "Waiting for process to start (pidFile: '%s')", _pidFile);
		Stopwatch sw; sw.start();

		// wait for the process to be either running or completed by monitoring run counts.
		while (!running() && prevRunCnt >= runCount()) checkTimeout(sw, msg);

		// we could wait for the process handle != INVALID_PID,
		// but if pidFile name was given, we should wait for
		// the process to write it
		if (!_pidFile.empty())
		{
			sw.restart();
			// wait until process is fully initialized
			File pidFile(_pidFile);
			while (!pidFile.exists())
				checkTimeout(sw, "waiting for PID file");

			// verify that the file content is actually the process PID
			FileInputStream fis(_pidFile);
			int fPID = 0;
			if (fis.peek() != std::ifstream::traits_type::eof())
				fis >> fPID;
			while (fPID != pid())
			{
				fis.clear(); fis.seekg(0); fis >> fPID;
				checkTimeout(sw, Poco::format("waiting for new PID (%s)", _pidFile));
			}
		}
	}
	else
		throw Poco::InvalidAccessException("start() called on started ProcessRunner");
}


} // namespace Poco
