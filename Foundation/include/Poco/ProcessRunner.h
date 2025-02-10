//
// ProcessRunner.h
//
// Library: Foundation
// Package: Processes
// Module:  ProcessRunner
//
// Definition of the ProcessRunner class.
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:    BSL-1.0
//


#ifndef Foundation_ProcessRunner_INCLUDED
#define Foundation_ProcessRunner_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Process.h"
#include "Poco/ProcessOptions.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Format.h"
#include "Poco/Stopwatch.h"
#include <atomic>
#include <vector>


namespace Poco {


class Foundation_API ProcessRunner: public Poco::Runnable
	/// ProcessRunner is a wrapper class for `Poco::ProcessHandle.
	/// It starts and terminates a process with enabled or disabled (default)
	/// `stdio` pipes, and optionally waits for process PID to be created before
	/// returning control to the caller. The process is spawned from an
	/// internal thread. Starting/stopping the process may block up to
	/// a certain (configurable) period of time.
	///
	/// ProcessRunner can hold and control only one process at a time, which
	/// can be started/stopped multiple times during the ProcessRunner lifetime.
{
public:
	using Args = Poco::Process::Args;
	using PID = Poco::ProcessHandle::PID;

	static const int NO_OUT = Poco::PROCESS_CLOSE_STDOUT|Poco::PROCESS_CLOSE_STDERR;
		/// Constant to prevent std out and err from being received from the process.

	ProcessRunner(const std::string& cmd,
		const Args& args,
		const std::string& pidFile = "",
		int options = NO_OUT,
		int timeout = 10, /*seconds*/
		bool startProcess = true,
		const Args& pidArgFmt = pidArgFormat());
		/// Creates the ProcessRunner.
		///
		/// If `pidFile` is not empty, the starting of the process waits
		/// until the pid file has been updated with the new pid, and
		/// the stopping of the process waits until the pid file is gone.
		/// Waiting is terminated after timeout seconds.
		///
		/// If `pidFile` is empty and `pidArgFmt` is not empty, autodetect
		/// of PID file from `args` is attempted; the default PID file
		/// argument format corresponds to the one used by
		/// `Poco::Util::Application`
		///
		/// The `options` are passed to the process, defaulting to
		/// closed stdio output pipes.
		///
		/// The `timeout` in seconds determines how long the ProcessRunner
		/// waits for the process to start; if PID file name is provided or
		/// autodetected from arguments, ProcessRunner will wait until the file
		/// exists and contains the process PID or timeout expires (in which
		/// case a TimeoutException is thrown).
		///
		/// If `startProcess` is true, the process is started on object creation.

	~ProcessRunner() override;
		/// Destroys the ProcessRunner.

	PID pid() const;
		/// Returns the process PID.

	const std::string& pidFile() const;
		/// Returns the process PID filename.
		/// Returns empty string when pid filename
		/// is not specified at construction, either
		/// explicitly, or implicitly through
		/// command line argument.

	bool running() const;
		/// Returns true if process is running.

	void start();
		/// Starts the process and waits for it to be fully initialized.
		/// Process initialization completion is indicated by a new pid in
		/// the pid file (if specified at construction, otherwise there
		/// is no wating for pid).
		/// If pid file is not specified, there is no waiting.
		///
		/// Attempting to start a started process results in
		/// Poco::InvalidAccessException being thrown.

	void stop();
		/// Stops the process.
		///
		/// Calling stop() on a stopped process is a no-op.

	std::string cmdLine() const;
		/// Returns process full command line.

	int result() const;
		/// Returns process return code.

	int runCount() const;
		/// Returns the number of times the process has been executed.

	const std::string& error() const;
		/// Returns the error message.

private:
	static const Poco::ProcessHandle::PID INVALID_PID = -1;
	static const int RESULT_UNKNOWN = -1;

	static Args pidArgFormat()
	{
#if defined(POCO_OS_FAMILY_WINDOWS)
		return Args{"-p", "--pidfile=", "/p", "/pidfile="};
#else
		return Args{"-p", "--pidfile="};
#endif
	}

	void run() override;
		/// Starts the process and waits for it to be fully initialized.
		/// Process initialization completion is indicated by new pid in
		/// the pid file. If pid file is not specified, there is no waiting.

	void checkError();
		/// If timeout is exceeded, throws TimeoutException with `msg`
		/// message.

	void checkTimeout(const std::string& msg);
		/// If timeout is exceeded, throws TimeoutException with `msg`
		/// message.

	void checkStatus(const std::string& msg, bool tOut = true);
		/// If there were andy errors during process start/stop,
		/// throws RuntimeException with the error message;
		/// otherwise, if tOut is true and timeout is exceeded, throws
		/// TimeoutException with `msg` message.

	void setError(const std::string& msg);
		/// Sets the error message.

	Poco::Thread _t;
	std::string _cmd;
	Args _args;
	std::atomic<PID> _pid;
	std::string _pidFile;
	int _options;
	int _timeout;
	std::atomic<Poco::ProcessHandle*> _pPH;
	std::atomic<bool> _started;
	std::atomic<int> _rc;
	std::atomic<int> _runCount;
	Stopwatch _sw;
	std::string _error;
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//

inline const std::string& ProcessRunner::pidFile() const
{
	return _pidFile;
}


inline bool ProcessRunner::running() const
{
	return _pid != INVALID_PID;
}


inline ProcessRunner::PID ProcessRunner::pid() const
{
	return _pid;
}


inline int ProcessRunner::result() const
{
	return _rc;
}


inline int ProcessRunner::runCount() const
{
	return _runCount;
}


inline void ProcessRunner::setError(const std::string& msg)
{
	_error = Poco::format("ProcessRunner(%s): %s", cmdLine(), msg);
}


inline const std::string& ProcessRunner::error() const
{
	Poco::FastMutex::ScopedLock l(_mutex);

	return _error;
}


} // namespace Poco


#endif // Foundation_ProcessRunner_INCLUDED
