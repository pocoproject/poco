//
// Thread_STD.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Thread
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Thread_STD.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Format.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Thread_STD_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "Thread_STD_VX.cpp"
#else
#include "Thread_STD_POSIX.cpp"
#endif


namespace Poco {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;
thread_local ThreadImpl* ThreadImpl::CurrentThreadHolder::_pThread;


ThreadImpl::ThreadImpl(): _pData(new ThreadData)
{
}


ThreadImpl::~ThreadImpl()
{
	if (_pData->started && !_pData->joined && _pData->thread)
	{
		_pData->thread->detach();
	}
}


void ThreadImpl::startImpl(SharedPtr<Runnable> pTarget)
{
	if (_pData->pRunnableTarget)
		throw SystemException("thread already running");

	_pData->pRunnableTarget = pTarget;
	try
	{
		_pData->thread = std::unique_ptr<std::thread>(new std::thread(runnableEntry, this));
	}
	catch (std::system_error &e) {
		_pData->pRunnableTarget = 0;
		throw SystemException(Poco::format("cannot start thread: %s", std::string(e.what())));
	}

	_pData->tid = _pData->thread->native_handle();

	_pData->started = true;
}


void ThreadImpl::joinImpl()
{
	if (!_pData->started) return;
	_pData->done.wait();
	try
	{
		_pData->thread->join();
	}
	catch (std::system_error &e) {
		throw SystemException(Poco::format("cannot join thread: %s", std::string(e.what())));
	}

	_pData->joined = true;
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (_pData->started && _pData->done.tryWait(milliseconds))
	{
		try
		{
			_pData->thread->join();
		}
		catch (std::system_error &e) {
			throw SystemException(Poco::format("cannot join thread: %s", std::string(e.what())));
		}
		_pData->joined = true;
		return true;
	}
	else if (_pData->started) return false;
	else return true;
}


ThreadImpl* ThreadImpl::currentImpl()
{
	return _currentThreadHolder.get();
}


ThreadImpl::TIDImpl ThreadImpl::currentTidImpl()
{
	ThreadImpl* pThreadImpl = currentImpl();
	if (!pThreadImpl)
		return 0;

	return pThreadImpl->tidImpl();
	//return std::this_thread::get_id();
}


void ThreadImpl::sleepImpl(long milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


void* ThreadImpl::runnableEntry(void* pThread)
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);

	AutoPtr<ThreadData> pData = pThreadImpl->_pData;

	try
	{
		pData->pRunnableTarget->run();
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

	pData->pRunnableTarget = 0;
	pData->done.set();
	return 0;
}


} // namespace Poco
