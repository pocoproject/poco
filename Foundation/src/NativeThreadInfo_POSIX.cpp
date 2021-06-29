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

namespace Poco {

NativeThreadInfoImpl::NativeThreadInfoImpl(NativeThreadInfoImpl::HandleImpl handle):
	_thread(handle)
{
}

std::string NativeThreadInfoImpl::nameImpl() const
{
	std::string result(128, '\0');
	int rc = pthread_getname_np(_thread, &result[0], result.size());
	if (rc < 0) {
		return {};
	}
	result.resize(std::strlen(result.data()));
	return result;
}

NativeThreadInfoImpl::ThreadIdImpl NativeThreadInfoImpl::idImpl() const
{
	ThreadIdImpl tid;
#if (POCO_OS == POCO_OS_MAC_OS_X)
	pthread_threadid_np(_thread, &tid);
#else
	tid = gettid();
#endif
	return tid;
}

NativeThreadInfoImpl::HandleImpl NativeThreadInfoImpl::currentThreadHandleImpl()
{
	return pthread_self();
}

} // namespace Poco
