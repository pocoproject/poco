//
// Process.cpp
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


#include "Poco/Process.h"
#include "Poco/Environment.h"


namespace
{
	std::vector<char> getEnvironmentVariablesBuffer(const Poco::Process::Env& env)
	{
		std::vector<char> envbuf;
		std::size_t pos = 0;

		for (const auto& p: env)
		{
			std::size_t envlen = p.first.length() + p.second.length() + 1;

			envbuf.resize(pos + envlen + 1);
			std::copy(p.first.begin(), p.first.end(), &envbuf[pos]);
			pos += p.first.length();
			envbuf[pos] = '=';
			++pos;
			std::copy(p.second.begin(), p.second.end(), &envbuf[pos]);
			pos += p.second.length();

			envbuf[pos] = '\0';
			++pos;
		}

		envbuf.resize(pos + 1);
		envbuf[pos] = '\0';

		return envbuf;
	}
}


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Process_WIN32U.cpp"
#elif defined(POCO_VXWORKS)
#include "Process_VX.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "Process_UNIX.cpp"
#endif


namespace Poco {


//
// ProcessHandle
//
ProcessHandle::ProcessHandle(const ProcessHandle& handle):
	_pImpl(handle._pImpl)
{
	_pImpl->duplicate();
}


ProcessHandle::~ProcessHandle()
{
	_pImpl->release();
}


ProcessHandle::ProcessHandle(ProcessHandleImpl* pImpl):
	_pImpl(pImpl)
{
	poco_check_ptr (_pImpl);
}


ProcessHandle& ProcessHandle::operator = (const ProcessHandle& handle)
{
	if (&handle != this)
	{
		_pImpl->release();
		_pImpl = handle._pImpl;
		_pImpl->duplicate();
	}
	return *this;
}


ProcessHandle::PID ProcessHandle::id() const
{
	return _pImpl->id();
}


int ProcessHandle::wait() const
{
	return _pImpl->wait();
}


int ProcessHandle::tryWait() const
{
	return _pImpl->tryWait();
}


//
// Process
//
ProcessHandle Process::launch(const std::string& command, const Args& args, int options)
{
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env, options));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, int options)
{
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, 0, 0, 0, env, options));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, int options)
{
	poco_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, int options)
{
	poco_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	Env env;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env, int options)
{
	poco_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	std::string initialDirectory;
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}


ProcessHandle Process::launch(const std::string& command, const Args& args, const std::string& initialDirectory, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Env& env, int options)
{
	poco_assert (inPipe == 0 || (inPipe != outPipe && inPipe != errPipe));
	return ProcessHandle(launchImpl(command, args, initialDirectory, inPipe, outPipe, errPipe, env, options));
}


int Process::wait(const ProcessHandle& handle)
{
	return handle.wait();
}


int Process::tryWait(const ProcessHandle& handle)
{
	return handle.tryWait();
}


void Process::kill(ProcessHandle& handle)
{
	killImpl(*handle._pImpl);
}


void Process::kill(PID pid)
{
	killImpl(pid);
}

bool Process::isRunning(const ProcessHandle& handle)
{
	return isRunningImpl(*handle._pImpl);
}


bool Process::isRunning(PID pid)
{
	return isRunningImpl(pid);
}


void Process::requestTermination(PID pid)
{
	requestTerminationImpl(pid);
}


} // namespace Poco
