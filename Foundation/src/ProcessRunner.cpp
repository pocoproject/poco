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
			_done(false)
{
	if (!File(_cmd).exists())
		throw Poco::NotFoundException(_cmd);

	if (_pidFile.empty() && !_args.empty() && !pidArgFmt.empty())
	{
		// try to detect PID file name from arguments;
		// we follow Poco convention
		// (-pfile or --pidfile=file)
		// TODO: windows
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
	stop();
	_t.join();
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
	_done = false;

	try
	{
		_pPH.store(new ProcessHandle(Process::launch(_cmd, _args, _options)));
		pPH = _pPH;
		_pid = pPH->id();
		_rc = pPH->wait();
	}
	catch (...)
	{
	}

	_pid = INVALID_PID;
	_pPH.store(nullptr);
	_done = true;

	delete pPH;
}


void ProcessRunner::stop()
{
	if (_started)
	{
		ProcessHandle* pPH = nullptr;
		if ((pPH = _pPH.exchange(nullptr)))
			Process::requestTermination(pPH->id());

		std::string msg = "Waiting for PID file";
		if (!_pidFile.empty())
		{
			Poco::format(msg, "%s (pidFile: '%s')", msg, _pidFile);

			if (!_pidFile.empty())
			{
				File pidFile(_pidFile);
				_pidFile.clear();
				Stopwatch sw; sw.start();
				while (pidFile.exists())
					checkTimeout(sw, msg);
			}
		}

		Stopwatch sw; sw.start();
		while (_pPH) checkTimeout(sw, "Waiting for process shutdown");
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
		_t.start(*this);

		std::string msg;
		Poco::format(msg, "Waiting for process to start (pidFile: '%s')", _pidFile);
		Stopwatch sw; sw.start();
		while (!running() || done()) checkTimeout(sw, msg);

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
