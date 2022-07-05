//
// Thread_WINCE.h
//
// Library: Foundation
// Package: Threading
// Module:  Thread
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Thread_WINCE.h"
#include "Poco/Exception.h"
#include "Poco/ErrorHandler.h"

namespace
{
	/// See <http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx>
	/// and <http://blogs.msdn.com/b/stevejs/archive/2005/12/19/505815.aspx> for
	/// more information on the code below.

	const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;     // Must be 0x1000.
		LPCSTR szName;    // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags;    // Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)

	void setThreadName(DWORD dwThreadID, const std::string& threadName)
	{
		THREADNAME_INFO info;
		info.dwType     = 0x1000;
		info.szName     = threadName.c_str();
		info.dwThreadID = dwThreadID;
		info.dwFlags    = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}

	std::string getThreadName()
	{
		/// TODO
	}
}

namespace Poco {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;


ThreadImpl::ThreadImpl():
	_pRunnableTarget(0),
	_thread(0),
	_threadId(0),
	_prio(PRIO_NORMAL_IMPL),
	_stackSize(POCO_THREAD_STACK_SIZE)
{
}

			
ThreadImpl::~ThreadImpl()
{
	if (_thread) CloseHandle(_thread);
}

void ThreadImpl::setNameImpl(const std::string& threadName)
{
	std::string realName = threadName;
	if (threadName.size() > POCO_MAX_THREAD_NAME_LEN) {
		int half = (POCO_MAX_THREAD_NAME_LEN - 1) / 2;
		std::string truncName(threadName, 0, half);
		truncName.append("~");
		truncName.append(threadName, threadName.size() - half);
		realName = truncName;
	}

	if (realName != _name) {
		_name = realName;
	}
}


std::string ThreadImpl::getNameImpl() const
{
	return _name;
}

std::string ThreadImpl::getOSThreadNameImpl()
{
	// TODO
	return isRunningImpl() ? _name : "";
}

void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _prio)
	{
		_prio = prio;
		if (_thread)
		{
			if (SetThreadPriority(_thread, _prio) == 0)
				throw SystemException("cannot set thread priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio, int /* policy */)
{
	setPriorityImpl(prio);
}


void ThreadImpl::startImpl(SharedPtr<Runnable> pTarget)
{
	if (isRunningImpl())
		throw SystemException("thread already running");

	_pRunnableTarget = pTarget;

	createImpl(runnableEntry, this);
}


void ThreadImpl::createImpl(Entry ent, void* pData)
{
	_thread = CreateThread(NULL, _stackSize, ent, pData, 0, &_threadId);

	if (!_thread)
		throw SystemException("cannot create thread");
	if (_prio != PRIO_NORMAL_IMPL && !SetThreadPriority(_thread, _prio))
		throw SystemException("cannot set thread priority");
}


void ThreadImpl::joinImpl()
{
	if (!_thread) return;

	switch (WaitForSingleObject(_thread, INFINITE))
	{
	case WAIT_OBJECT_0:
		threadCleanup();
		return;
	default:
		throw SystemException("cannot join thread");
	}
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (!_thread) return true;

	switch (WaitForSingleObject(_thread, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		threadCleanup();
		return true;
	default:
		throw SystemException("cannot join thread");
	}
}


bool ThreadImpl::isRunningImpl() const
{
	if (_thread)
	{
		DWORD ec = 0;
		return GetExitCodeThread(_thread, &ec) && ec == STILL_ACTIVE;
	}
	return false;
}


void ThreadImpl::threadCleanup()
{
	if (!_thread) return;
	if (CloseHandle(_thread)) _thread = 0;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	return _currentThreadHolder.get();
}


ThreadImpl::TIDImpl ThreadImpl::currentTidImpl()
{
	return GetCurrentThreadId();
}

long ThreadImpl::currentOsTidImpl()
{
	return GetCurrentThreadId();
}

DWORD WINAPI ThreadImpl::runnableEntry(LPVOID pThread)
{
	auto * pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	_currentThreadHolder.set(pThreadImpl);
	setThreadName(-1, pThreadImpl->_name);
	try
	{
		pThreadImpl->_pRunnableTarget->run();
	}
	catch (Exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (...)
	{
		ErrorHandler::handle();
	}
	return 0;
}


} // namespace Poco
