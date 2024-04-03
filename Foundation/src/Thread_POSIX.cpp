//
// Thread_POSIX.cpp
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


#include "Poco/Thread_POSIX.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"
#include "Poco/Error.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/Format.h"
#include "Poco/Error.h"
#include <signal.h>
#include <limits.h>


#if POCO_OS == POCO_OS_FREE_BSD
#    include <sys/thr.h>
#    include <pthread_np.h>
#    include <osreldate.h>
#endif


#if defined(__sun) && defined(__SVR4)
#	if !defined(__EXTENSIONS__)
#		define __EXTENSIONS__
#	endif
#endif
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID || POCO_OS == POCO_OS_MAC_OS_X || POCO_OS == POCO_OS_QNX
#	include <time.h>
#endif


#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
#	include <sys/prctl.h>
#endif

#if POCO_OS == POCO_OS_LINUX
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE         /* See feature_test_macros(7) */
	#endif
	#include <unistd.h>
	#include <sys/syscall.h>   /* For SYS_xxx definitions */
#endif


//
// Block SIGPIPE in main thread.
//
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
namespace
{
	class SignalBlocker
	{
	public:
		SignalBlocker()
		{
			sigset_t sset;
			sigemptyset(&sset);
			sigaddset(&sset, SIGPIPE);
			pthread_sigmask(SIG_BLOCK, &sset, 0);
		}
		~SignalBlocker()
		{
		}
	};

	static SignalBlocker signalBlocker;
}
#endif


namespace
{
	std::string truncName(const std::string& name, int nameSize = POCO_MAX_THREAD_NAME_LEN)
	{
		if (name.size() > nameSize)
			return name.substr(0, nameSize).append("~");
		return name;
	}

	void setThreadName(const std::string& threadName)
		/// Sets thread name. Support for this feature varies
		/// on platforms. Any errors are ignored.
	{
#if (POCO_OS == POCO_OS_FREE_BSD)
		pthread_setname_np(pthread_self(), truncName(threadName).c_str());
#elif (POCO_OS == POCO_OS_MAC_OS_X)
	#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
		#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
			pthread_setname_np(truncName(threadName).c_str()); // __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_2)
		#endif
	#endif // __MAC_OS_X_VERSION_MIN_REQUIRED
#elif (POCO_OS == POCO_OS_QNX)
		pthread_setname_np(pthread_self(), truncName(threadName, _NTO_THREAD_NAME_MAX).c_str());
#else
		prctl(PR_SET_NAME, truncName(threadName).c_str());
#endif
	}

	std::string getThreadName()
	{
		char name[POCO_MAX_THREAD_NAME_LEN + 1]{'\0'};
#if (POCO_OS == POCO_OS_FREE_BSD)
		pthread_getname_np(pthread_self(), name, POCO_MAX_THREAD_NAME_LEN + 1);
#elif (POCO_OS == POCO_OS_MAC_OS_X)
	#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
		#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
			pthread_getname_np(pthread_self(), name, POCO_MAX_THREAD_NAME_LEN + 1);
		#endif
	#endif // __MAC_OS_X_VERSION_MIN_REQUIRED
#elif (POCO_OS == POCO_OS_QNX)
		tName[_NTO_THREAD_NAME_MAX] = {'\0'};
		pthread_getname_np(pthread_self(), tName, _NTO_THREAD_NAME_MAX);
#else
		prctl(PR_GET_NAME, name);
#endif
		return name;
	}
}


namespace Poco {


ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;


ThreadImpl::ThreadImpl():
	_pData(new ThreadData)
{
}


ThreadImpl::~ThreadImpl()
{
	if (_pData->started && !_pData->joined)
	{
		pthread_detach(_pData->thread);
	}
}


void ThreadImpl::setNameImpl(const std::string& threadName)
{
	std::string realName = threadName;
#if (POCO_OS == POCO_OS_MAC_OS_X)
	if (threadName.size() > POCO_MAX_THREAD_NAME_LEN)
	{
		int half = (POCO_MAX_THREAD_NAME_LEN - 1) / 2;
#else
	if (threadName.size() > std::min(POCO_MAX_THREAD_NAME_LEN, 15))
	{
		int half = (std::min(POCO_MAX_THREAD_NAME_LEN, 15) - 1) / 2;
#endif
		std::string truncName(threadName, 0, half);
		truncName.append("~");
		truncName.append(threadName, threadName.size() - half);
		realName = truncName;
	}

	ScopedLock<FastMutex> lock(_pData->mutex);
	if (realName != _pData->name)
	{
		_pData->name = realName;
	}
}


std::string ThreadImpl::getNameImpl() const
{
	ScopedLock<FastMutex> lock(_pData->mutex);
	return _pData->name;
}


std::string ThreadImpl::getOSThreadNameImpl()
{
	return isRunningImpl() ? getThreadName() : "";
}


void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		_pData->policy = SCHED_OTHER;
		if (isRunningImpl())
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			int errorCode;
			if ((errorCode = pthread_setschedparam(_pData->thread, SCHED_OTHER, &par)))
				throw SystemException(Poco::format("cannot set thread priority (%s)",
					Error::getMessage(errorCode)));
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio, int policy)
{
	if (prio != _pData->osPrio || policy != _pData->policy)
	{
		if (_pData->pRunnableTarget)
		{
			struct sched_param par;
			par.sched_priority = prio;
			int errorCode;
			if ((errorCode = pthread_setschedparam(_pData->thread, policy, &par)))
				throw SystemException(Poco::format("cannot set thread priority (%s)",
					Error::getMessage(errorCode)));
		}
		_pData->prio   = reverseMapPrio(prio, policy);
		_pData->osPrio = prio;
		_pData->policy = policy;
	}
}


int ThreadImpl::getMinOSPriorityImpl(int policy)
{
#if defined(POCO_THREAD_PRIORITY_MIN)
	return POCO_THREAD_PRIORITY_MIN;
#elif defined(__digital__)
	return PRI_OTHER_MIN;
#else
	return sched_get_priority_min(policy);
#endif
}


int ThreadImpl::getMaxOSPriorityImpl(int policy)
{
#if defined(POCO_THREAD_PRIORITY_MAX)
	return POCO_THREAD_PRIORITY_MAX;
#elif defined(__digital__)
	return PRI_OTHER_MAX;
#else
	return sched_get_priority_max(policy);
#endif
}


void ThreadImpl::setStackSizeImpl(int size)
{
#ifndef PTHREAD_STACK_MIN
	_pData->stackSize = 0;
#else
	if (size != 0)
	{
#if defined(POCO_OS_FAMILY_BSD)
		// we must round up to a multiple of the memory page size
		const int STACK_PAGE_SIZE = 4096;
		size = ((size + STACK_PAGE_SIZE - 1)/STACK_PAGE_SIZE)*STACK_PAGE_SIZE;
#endif
 		if (size < PTHREAD_STACK_MIN)
 			size = PTHREAD_STACK_MIN;
	}
 	_pData->stackSize = size;
#endif
}


void ThreadImpl::setSignalMaskImpl(uint32_t sigMask)
{
	sigset_t sset;
	sigemptyset(&sset);

	for (int sig = 0; sig < sizeof(uint32_t) * 8; ++sig)
	{
		if ((sigMask & (1 << sig)) != 0)
			sigaddset(&sset, sig);
	}

	pthread_sigmask(SIG_BLOCK, &sset, 0);
}


void ThreadImpl::startImpl(SharedPtr<Runnable> pTarget)
{
	{
		FastMutex::ScopedLock l(_pData->mutex);
		if (_pData->pRunnableTarget)
			throw SystemException("thread already running");
	}

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (_pData->stackSize != 0)
	{
		int errorCode;
		if ((errorCode = pthread_attr_setstacksize(&attributes, _pData->stackSize)))
		{
			pthread_attr_destroy(&attributes);
			throw SystemException(Poco::format("cannot set thread stack size: (%s)",
				Error::getMessage(errorCode)));
		}
	}

	{
		FastMutex::ScopedLock l(_pData->mutex);
		_pData->pRunnableTarget = pTarget;
		int errorCode;
		if ((errorCode = pthread_create(&_pData->thread, &attributes, runnableEntry, this)))
		{
			_pData->pRunnableTarget = 0;
			pthread_attr_destroy(&attributes);
			throw SystemException(Poco::format("cannot start thread (%s)",
				Error::getMessage(errorCode)));
		}
	}
	_pData->started = true;
	_pData->joined = false;
	pthread_attr_destroy(&attributes);

	if (_pData->policy == SCHED_OTHER)
	{
		if (_pData->prio != PRIO_NORMAL_IMPL)
		{
			struct sched_param par;
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			int errorCode;
			if ((errorCode = pthread_setschedparam(_pData->thread, SCHED_OTHER, &par)))
				throw SystemException(Poco::format("cannot set thread priority (%s)",
					Error::getMessage(errorCode)));
		}
	}
	else
	{
		struct sched_param par;
		par.sched_priority = _pData->osPrio;
		int errorCode;
		if ((errorCode = pthread_setschedparam(_pData->thread, _pData->policy, &par)))
			throw SystemException(Poco::format("cannot set thread priority (%s)",
				Error::getMessage(errorCode)));
	}
}


void ThreadImpl::joinImpl()
{
	if (!_pData->started) return;
	_pData->done.wait();
	if (!_pData->joined)
	{
		int errorCode;
		if ((errorCode = pthread_join(_pData->thread, nullptr)))
		{
			throw SystemException(Poco::format("cannot join thread (%s)",
				Error::getMessage(errorCode)));
		}
		_pData->joined = true;
	}
}


bool ThreadImpl::joinImpl(long milliseconds)
{
	if (_pData->started && _pData->done.tryWait(milliseconds) && !_pData->joined)
	{
		int errorCode;
		if ((errorCode = pthread_join(_pData->thread, nullptr)))
			throw SystemException(Poco::format("cannot join thread (%s)",
				Error::getMessage(errorCode)));
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
	return pthread_self();
}


long ThreadImpl::currentOsTidImpl()
{
	long id = 0;
#if (POCO_OS == POCO_OS_LINUX) && !defined(POCO_EMSCRIPTEN)
	id = ::syscall(SYS_gettid);
#elif POCO_OS == POCO_OS_MAC_OS_X
	id = ::pthread_mach_thread_np(::pthread_self());
#elif POCO_OS == POCO_OS_FREE_BSD
	if (0 != thr_self(&id)) id = 0;
#endif
	return id;
}


void* ThreadImpl::runnableEntry(void* pThread)
{
	_currentThreadHolder.set(reinterpret_cast<ThreadImpl*>(pThread));

#if defined(POCO_OS_FAMILY_UNIX)
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigaddset(&sset, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sset, 0);
#endif

	ThreadImpl* pThreadImpl = reinterpret_cast<ThreadImpl*>(pThread);
	setThreadName(reinterpret_cast<Thread*>(pThread)->getName());
	AutoPtr<ThreadData> pData = pThreadImpl->_pData;

	{
		FastMutex::ScopedLock lock(pData->mutex);
		setThreadName(pData->name);
	}

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

	FastMutex::ScopedLock l(pData->mutex);
	pData->pRunnableTarget = 0;
	pData->done.set();
	return 0;
}


int ThreadImpl::mapPrio(int prio, int policy)
{
	int pmin = getMinOSPriorityImpl(policy);
	int pmax = getMaxOSPriorityImpl(policy);

	switch (prio)
	{
	case PRIO_LOWEST_IMPL:
		return pmin;
	case PRIO_LOW_IMPL:
		return pmin + (pmax - pmin)/4;
	case PRIO_NORMAL_IMPL:
		return pmin + (pmax - pmin)/2;
	case PRIO_HIGH_IMPL:
		return pmin + 3*(pmax - pmin)/4;
	case PRIO_HIGHEST_IMPL:
		return pmax;
	default:
		poco_bugcheck_msg("invalid thread priority");
	}
	return -1; // just to satisfy compiler - we'll never get here anyway
}


int ThreadImpl::reverseMapPrio(int prio, int policy)
{
	if (policy == SCHED_OTHER)
	{
		int pmin = getMinOSPriorityImpl(policy);
		int pmax = getMaxOSPriorityImpl(policy);
		int normal = pmin + (pmax - pmin)/2;
		if (prio == pmax)
			return PRIO_HIGHEST_IMPL;
		if (prio > normal)
			return PRIO_HIGH_IMPL;
		else if (prio == normal)
			return PRIO_NORMAL_IMPL;
		else if (prio > pmin)
			return PRIO_LOW_IMPL;
		else
			return PRIO_LOWEST_IMPL;
	}
	else return PRIO_HIGHEST_IMPL;
}


bool ThreadImpl::setAffinityImpl(int coreID)
{
#if POCO_OS == POCO_OS_LINUX
	int numCores = sysconf(_SC_NPROCESSORS_ONLN);
	if (coreID < 0 || coreID >= numCores)
		return false;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(coreID, &cpuset);

	return 0 == pthread_setaffinity_np(_pData->thread, sizeof(cpu_set_t), &cpuset);
#else
	return false;
#endif
}


int ThreadImpl::getAffinityImpl() const
{
#if POCO_OS == POCO_OS_LINUX
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	if (0 == pthread_getaffinity_np(_pData->thread, sizeof(cpu_set_t), &cpuset))
	{
		for (int i = 0; i < CPU_SETSIZE; ++i)
		{
			if (CPU_ISSET(i, &cpuset)) return i;
		}
	}
#endif
	return -1;
}


} // namespace Poco
