//
// NativeThreadInfo.h
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfo
//
// Definition of the NativeThreadInfo class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NativeThreadInfo_INCLUDED
#define Foundation_NativeThreadInfo_INCLUDED

#include "Poco/Foundation.h"

#if defined(POCO_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "Poco/NativeThreadInfo_WINCE.h"
#else
#include "Poco/NativeThreadInfo_WIN32.h"
#endif
#elif defined(POCO_VXWORKS)
#include "Poco/NativeThreadInfo_VX.h"
#else
#include "Poco/NativeThreadInfo_POSIX.h"
#endif

namespace Poco {

class Foundation_API NativeThreadInfo: private NativeThreadInfoImpl
	/// This class provides a platform-independent API to query information about native threads.
	///
	/// NativeThreadInfo is used to get information (name and OS thread ID) about threads that
	/// are not managed by Poco::Thread. This includes:
	///   - std::thread instances
	///   - The main thread
	///   - Threads created by third-party libraries
	///   - Any other native OS threads not created via Poco::Thread
	///
	/// The OS thread ID returned by osTid() is the same kernel-level thread identifier
	/// returned by Thread::currentOsTid() and visible in system tools (debuggers, profilers, etc.).
{
public:
	typedef NativeThreadInfoImpl::ThreadIdImpl ThreadId;
	typedef NativeThreadInfoImpl::HandleImpl Handle;

	NativeThreadInfo();
		/// Creates a thread info for the current thread.

	NativeThreadInfo(Handle handle);
		/// Creates a thread info for a specific thread id.

	std::string name() const;
		/// Returns the name of the thread.

	ThreadId osTid() const;
		/// Returns the operating system specific thread ID.
};

//
// inlines
//
inline std::string NativeThreadInfo::name() const
{
	return nameImpl();
}

inline NativeThreadInfo::ThreadId NativeThreadInfo::osTid() const
{
	return osTidImpl();
}

} // namespace Poco

#endif // Foundation_NativeThreadInfo_INCLUDED
