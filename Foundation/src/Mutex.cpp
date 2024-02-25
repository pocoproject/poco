//
// Mutex.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex.h"

#if defined(POCO_ENABLE_STD_MUTEX)
#include "Mutex_STD.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Mutex_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "Mutex_VX.cpp"
#else
#include "Mutex_POSIX.cpp"
#endif


namespace Poco {


Mutex::Mutex()
{
}


Mutex::~Mutex()
{
}


FastMutex::FastMutex()
{
}


FastMutex::~FastMutex()
{
}


SpinlockMutex::SpinlockMutex()
{
}


SpinlockMutex::~SpinlockMutex()
{
}


} // namespace Poco
