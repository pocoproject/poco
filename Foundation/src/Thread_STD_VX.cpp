//
// Thread_STD_VX.cpp
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
#include <taskLib.h>
#include <taskVarLib.h>
#include <timers.h>


namespace Poco {


void ThreadImpl::setPriorityImpl(int prio)
{
	if (prio != _pData->prio)
	{
		_pData->prio = prio;
		_pData->osPrio = mapPrio(_pData->prio);
		if (isRunningImpl())
		{
			if (taskPrioritySet(_pData->task, _pData->osPrio) != OK)
				throw SystemException("cannot set task priority");
		}
	}
}


void ThreadImpl::setOSPriorityImpl(int prio, int policy)
{
	if (prio != _pData->osPrio)
	{
		_pData->prio = reverseMapPrio(prio);
		_pData->osPrio = prio;
		if (_pData->pRunnableTarget || _pData->pCallbackTarget)
		{
			if (taskPrioritySet(_pData->task, prio) != OK)
				throw SystemException("cannot set task priority");
		}
	}
}


int ThreadImpl::getMinOSPriorityImpl(int policy)
{
	return 255;
}


int ThreadImpl::getMaxOSPriorityImpl(int policy)
{
	return 0;
}


void ThreadImpl::setStackSizeImpl(int size)
{
	_pData->stackSize = size;
	// not supported
}


void ThreadImpl::setAffinityImpl(int cpu)
{
	// not supported
}


int ThreadImpl::getAffinityImpl() const
{
	return -1;
}


} // namespace Poco
