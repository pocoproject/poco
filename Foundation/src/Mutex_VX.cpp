//
// Mutex_VX.cpp
//
// $Id: //poco/1.4/Foundation/src/Mutex_VX.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex_VX.h"
#include <sysLib.h>


namespace Poco {


MutexImpl::MutexImpl(MutexTypeImpl type)
{
	switch (type)
	{
	case MUTEX_RECURSIVE_IMPL:
		_sem = semMCreate(SEM_INVERSION_SAFE | SEM_Q_PRIORITY);
		break;
	case MUTEX_NONRECURSIVE_IMPL:
		_sem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
		break;
	}
	if (_sem == 0)
		throw Poco::SystemException("cannot create mutex");
}


MutexImpl::~MutexImpl()
{
	semDelete(_sem);
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	int ticks = milliseconds*sysClkRateGet()/1000;
	return semTake(_sem, ticks) == OK;
}


FastMutexImpl::FastMutexImpl(): MutexImpl(MUTEX_NONRECURSIVE_IMPL)
{
}


FastMutexImpl::~FastMutexImpl()
{
}


} // namespace Poco
