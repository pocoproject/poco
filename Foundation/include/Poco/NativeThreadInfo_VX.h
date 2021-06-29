//
// NativeThreadInfo_VX.h
//
// Library: Foundation
// Package: Threading
// Module:  NativeThreadInfo
//
// Definition of the NativeThreadInfoImpl class for VX NativeThreadInfos.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NativeThreadInfo_VX_INCLUDED
#define Foundation_NativeThreadInfo_VX_INCLUDED


#include "Poco/Foundation.h"
#include <string>
#include <taskLib.h>

namespace Poco {

class Foundation_API NativeThreadInfoImpl
{
public:
	typedef int ThreadIdImpl;
	typedef int HandleImpl;

	NativeThreadInfoImpl(HandleImpl handle);

	std::string nameImpl() const;
	ThreadIdImpl idImpl() const;

	static HandleImpl currentThreadHandleImpl();

private:
	HandleImpl _thread;
};

} // namespace Poco

#endif // Foundation_NativeThreadInfo_VX_INCLUDED
