//
// Thread_STD_POSIX.cpp
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
#include <pthread.h>
#include <signal.h>
#if defined(__sun) && defined(__SVR4)
	#if !defined(__EXTENSIONS__)
		#define __EXTENSIONS__
	#endif
#endif
#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_MAC_OS_X || POCO_OS == POCO_OS_QNX
	#include <time.h>
	#include <unistd.h>
#endif
#if POCO_OS == POCO_OS_MAC_OS_X
	#include <mach/mach.h>
	#include <mach/task.h>
	#include <mach/thread_policy.h>
#endif
#if POCO_OS == POCO_OS_LINUX
	#include <sys/syscall.h>
#endif
#include <cstring>


namespace Poco {


int mapPrio(int prio, int policy)
{
	int pmin = ThreadImpl::getMinOSPriorityImpl(policy);
	int pmax = ThreadImpl::getMaxOSPriorityImpl(policy);

	switch (prio)
	{
	case ThreadImpl::PRIO_LOWEST_IMPL:
		return pmin;
	case ThreadImpl::PRIO_LOW_IMPL:
		return pmin + (pmax - pmin) / 4;
	case ThreadImpl::PRIO_NORMAL_IMPL:
		return pmin + (pmax - pmin) / 2;
	case ThreadImpl::PRIO_HIGH_IMPL:
		return pmin + 3 * (pmax - pmin) / 4;
	case ThreadImpl::PRIO_HIGHEST_IMPL:
		return pmax;
	default:
		poco_bugcheck_msg("invalid thread priority");
	}
	return -1; // just to satisfy compiler - we'll never get here anyway
}


int reverseMapPrio(int prio, int policy)
{
	if (policy == SCHED_OTHER)
	{
		int pmin = ThreadImpl::getMinOSPriorityImpl(policy);
		int pmax = ThreadImpl::getMaxOSPriorityImpl(policy);
		int normal = pmin + (pmax - pmin) / 2;
		if (prio == pmax)
			return ThreadImpl::PRIO_HIGHEST_IMPL;
		if (prio > normal)
			return ThreadImpl::PRIO_HIGH_IMPL;
		else if (prio == normal)
			return ThreadImpl::PRIO_NORMAL_IMPL;
		else if (prio > pmin)
			return ThreadImpl::PRIO_LOW_IMPL;
		else
			return ThreadImpl::PRIO_LOWEST_IMPL;
	}
	else return ThreadImpl::PRIO_HIGHEST_IMPL;
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
			struct MyStruct
			{

			};
			par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
			if (pthread_setschedparam(_pData->thread->native_handle(), SCHED_OTHER, &par))
				throw SystemException("cannot set thread priority");
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
			if (pthread_setschedparam(_pData->thread->native_handle(), policy, &par))
				throw SystemException("cannot set thread priority");
		}
		_pData->prio = reverseMapPrio(prio, policy);
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
	_pData->stackSize = size;
	// not supported
}


void ThreadImpl::setAffinityImpl(int cpu)
{
#if defined (POCO_OS_FAMILY_UNIX) && POCO_OS != POCO_OS_MAC_OS_X
#ifdef HAVE_PTHREAD_SETAFFINITY_NP
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
#ifdef HAVE_THREE_PARAM_SCHED_SETAFFINITY
	if (pthread_setaffinity_np(_pData->thread->native_handle(), sizeof(cpuset), &cpuset) != 0)
		throw SystemException("Failed to set affinity");
#else
	if (pthread_setaffinity_np(_pData->thread->native_handle(), &cpuset) != 0)
		throw SystemException("Failed to set affinity");
#endif
#endif
#endif // defined unix & !defined mac os x

#if POCO_OS == POCO_OS_MAC_OS_X
	kern_return_t ret;
	thread_affinity_policy policy;
	policy.affinity_tag = cpu;

	ret = thread_policy_set(pthread_mach_thread_np(_pData->thread->native_handle()),
		THREAD_AFFINITY_POLICY,
		(thread_policy_t)&policy,
		THREAD_AFFINITY_POLICY_COUNT);
	if (ret != KERN_SUCCESS)
	{
		throw SystemException("Failed to set affinity");
	}
#endif
	yieldImpl();
	_pData->cpu = cpu;
}


int ThreadImpl::getAffinityImpl() const
{
	int cpuSet = -1;
#if defined (POCO_OS_FAMILY_UNIX) && POCO_OS != POCO_OS_MAC_OS_X
#ifdef HAVE_PTHREAD_SETAFFINITY_NP
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
#ifdef HAVE_THREE_PARAM_SCHED_SETAFFINITY
	if (pthread_getaffinity_np(_pData->thread->native_handle(), sizeof(cpuset), &cpuset) != 0)
		throw SystemException("Failed to get affinity", errno);
#else
	if (pthread_getaffinity_np(_pData->thread->native_handle(), &cpuset) != 0)
		throw SystemException("Failed to get affinity", errno);
#endif
	int cpuCount = Environment::processorCount();
	for (int i = 0; i < cpuCount; i++)
	{
		if (CPU_ISSET(i, &cpuset))
		{
			cpuSet = i;
			break;
		}
	}
#endif
#endif // defined unix & !defined mac os x

#if POCO_OS == POCO_OS_MAC_OS_X
	kern_return_t ret;
	thread_affinity_policy policy;
	mach_msg_type_number_t count = THREAD_AFFINITY_POLICY_COUNT;
	boolean_t get_default = false;
	ret = thread_policy_get(pthread_mach_thread_np(_pData->thread->native_handle()),
		THREAD_AFFINITY_POLICY,
		(thread_policy_t)&policy,
		&count,
		&get_default);
	if (ret != KERN_SUCCESS)
	{
		throw SystemException("Failed to get affinity", errno);
	}
	cpuSet = policy.affinity_tag;
	int cpuCount = Environment::processorCount();
	if (cpuSet >= cpuCount)
		cpuSet = -1;

#endif
	return cpuSet;
}


} // namespace Poco
