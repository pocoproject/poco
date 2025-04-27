//
// NativeThreadInfo_POSIX.h
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfo
//
// Definition of the NativeThreadInfoImpl class for POSIX NativeThreadInfos.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NativeThreadInfo_POSIX_INCLUDED
#define Foundation_NativeThreadInfo_POSIX_INCLUDED


#include "Poco/Foundation.h"
#include <string>
#include <pthread.h>
#include <sys/types.h>

namespace Poco {

class Foundation_API NativeThreadInfoImpl
{
public:
#if (POCO_OS == POCO_OS_MAC_OS_X)
	typedef uint64_t ThreadIdImpl;
#else
	typedef pid_t ThreadIdImpl;
#endif
	typedef pthread_t HandleImpl;

	NativeThreadInfoImpl(HandleImpl handle);

	std::string nameImpl() const;
	ThreadIdImpl idImpl() const;

	static HandleImpl currentThreadHandleImpl();

private:
	HandleImpl _thread;
};

} // namespace Poco

#endif // Foundation_NativeThreadInfo_POSIX_INCLUDED
