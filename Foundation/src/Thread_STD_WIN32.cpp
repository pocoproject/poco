//
// Thread_STD_WIN32.cpp
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
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/UnWindows.h"
#endif

namespace Poco {


void ThreadImpl::setPriorityImpl(int prio)
{
	HANDLE handle = reinterpret_cast<HANDLE>(_pData->thread->native_handle());
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		_pData->policy = 0;
		if (_pData->started && !_pData->joined && _pData->thread)
		{
			if (SetThreadPriority(handle, _pData->prio) == 0)
				throw SystemException("cannot set thread priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio, int /*policy*/)
{
	setPriorityImpl(prio);
}


int ThreadImpl::getMinOSPriorityImpl(int /*policy*/)
{
	return PRIO_LOWEST_IMPL;
}


int ThreadImpl::getMaxOSPriorityImpl(int /*policy*/)
{
	return PRIO_HIGHEST_IMPL;
}


void ThreadImpl::setStackSizeImpl(int size)
{
	_pData->stackSize = size;
	// not supported
}


void ThreadImpl::setAffinityImpl(int cpu)
{
	HANDLE handle = reinterpret_cast<HANDLE>(_pData->thread->native_handle());
	DWORD mask = 1;
	mask <<= cpu;
	if (_pData->started && !_pData->joined && _pData->thread)
	{
		if (SetThreadAffinityMask(handle, mask) == 0)
		{
			throw SystemException("Failed to set affinity");
		}
	}
	_pData->cpu = cpu;
}


int ThreadImpl::getAffinityImpl() const
{
	return _pData->cpu;
}


long ThreadImpl::currentOsTidImpl()
{
	return GetCurrentThreadId();
}


} // namespace Poco
