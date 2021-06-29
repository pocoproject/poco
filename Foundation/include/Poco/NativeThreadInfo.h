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
	/// This class implements a platform independent API to query information about a native thread.
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

	ThreadId id() const;
};

//
// inlines
//
inline std::string NativeThreadInfo::name() const
{
	return nameImpl();
}

inline NativeThreadInfo::ThreadId NativeThreadInfo::id() const
{
	return idImpl();
}

} // namespace Poco

#endif // Foundation_NativeThreadInfo_INCLUDED
