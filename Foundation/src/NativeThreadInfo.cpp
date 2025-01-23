//
// NativeThreadInfo.cpp
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfo
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NativeThreadInfo.h"

#if defined(POCO_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "NativeThreadInfo_WINCE.cpp"
#else
#include "NativeThreadInfo_WIN32.cpp"
#endif
#elif defined(POCO_VXWORKS)
#include "NativeThreadInfo_VX.cpp"
#else
#include "NativeThreadInfo_POSIX.cpp"
#endif


namespace Poco {

NativeThreadInfo::NativeThreadInfo(Handle handle):
	NativeThreadInfoImpl{handle}
{ }

NativeThreadInfo::NativeThreadInfo():
	NativeThreadInfo{ NativeThreadInfoImpl::currentThreadHandleImpl() }
{ }

} // namespace Poco
