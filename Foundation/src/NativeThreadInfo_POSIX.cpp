//
// NativeThreadInfo_POSIX.cpp
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfoImpl
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NativeThreadInfo_POSIX.h"
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

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

#if POCO_OS == POCO_OS_QNX
#	include <sys/neutrino.h>
#endif

#ifndef POCO_NO_THREADNAME
	#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
	#	include <sys/prctl.h>
	#endif
#endif

#if POCO_OS == POCO_OS_LINUX
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE         /* See feature_test_macros(7) */
	#endif
	#include <unistd.h>
	#include <sys/syscall.h>   /* For SYS_xxx definitions */
#endif


namespace Poco {


NativeThreadInfoImpl::NativeThreadInfoImpl(NativeThreadInfoImpl::HandleImpl)
{
}


std::string NativeThreadInfoImpl::nameImpl() const
{
#ifndef POCO_NO_THREADNAME
	constexpr size_t nameSize =
#	if (POCO_OS == POCO_OS_QNX)
			_NTO_THREAD_NAME_MAX;
#	else
			POCO_MAX_THREAD_NAME_LEN;
#	endif
		char name[nameSize + 1]{'\0'};
#	if (POCO_OS == POCO_OS_FREE_BSD)
		pthread_getname_np(pthread_self(), name, nameSize + 1);
#	elif (POCO_OS == POCO_OS_MAC_OS_X)
#		ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#			if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
				pthread_getname_np(pthread_self(), name, nameSize + 1);
#			endif
#	endif // POCO_OS == POCO_OS_FREE_BSD
#elif (POCO_OS == POCO_OS_QNX)
		pthread_getname_np(pthread_self(), name, nameSize);
#else
		prctl(PR_GET_NAME, name);
#endif
		return name;
#endif // POCO_NO_THREADNAME
}


NativeThreadInfoImpl::ThreadIdImpl NativeThreadInfoImpl::osTidImpl() const
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


NativeThreadInfoImpl::HandleImpl NativeThreadInfoImpl::currentThreadHandleImpl()
{
	return pthread_self();
}

} // namespace Poco
