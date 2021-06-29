//
// NativeThreadInfo_WINCE.h
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfo
//
// Definition of the NativeThreadInfoImpl class for WINCE NativeThreadInfos.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NativeThreadInfo_WINCE_INCLUDED
#define Foundation_NativeThreadInfo_WINCE_INCLUDED


#include "Poco/Foundation.h"
#include <string>
#include "Poco/UnWindows.h"

namespace Poco {

class Foundation_API NativeThreadInfoImpl
{
public:
	typedef DWORD ThreadIdImpl;
	typedef DWORD HandleImpl;

	NativeThreadInfoImpl(HandleImpl handle);

	std::string nameImpl() const;
	ThreadIdImpl idImpl() const;

	static HandleImpl currentThreadHandleImpl();

private:
	HandleImpl _thread;
};

} // namespace Poco

#endif // Foundation_NativeThreadInfo_WINCE_INCLUDED
