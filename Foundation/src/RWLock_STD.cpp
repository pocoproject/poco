//
// RWLock_WIN32.cpp
//
// $Id: //poco/1.4/Foundation/src/RWLock_STD.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  RWLock
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RWLock_STD.h"


namespace Poco {


RWLockImpl::RWLockImpl(): _mutex()
{
}


void RWLockImpl::readLockImpl()
{
	_mutex.lock_shared();
}


bool RWLockImpl::tryReadLockImpl()
{
	return _mutex.try_lock_shared();
}


void RWLockImpl::writeLockImpl()
{
	_mutex.lock();
}


bool RWLockImpl::tryWriteLockImpl()
{
	return _mutex.try_lock();
}


void RWLockImpl::unlockImpl()
{
	// TODO: unlock_shared()?
	_mutex.unlock();
}


} // namespace Poco
