//
// RWLock_WIN32.h
//
// Library: Foundation
// Package: Threading
// Module:  RWLock
//
// Definition of the RWLockImpl class for STD.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_RWLock_STD_INCLUDED
#define Foundation_RWLock_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <shared_mutex>


namespace Poco {


class Foundation_API RWLockImpl
{
protected:
	RWLockImpl();
	void readLockImpl();
	bool tryReadLockImpl();
	void writeLockImpl();
	bool tryWriteLockImpl();
	void unlockImpl();
	
private:
	std::shared_timed_mutex _mutex;
};


} // namespace Poco


#endif // Foundation_RWLock_STD_INCLUDED
