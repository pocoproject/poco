//
// NativeThreadInfo_VX.cpp
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


#include "Poco/NativeThreadInfo_VX.h"

namespace Poco {

NativeThreadInfoImpl::NativeThreadInfoImpl(NativeThreadInfoImpl::HandleImpl handle):
	_thread(handle)
{
}

std::string NativeThreadInfoImpl::nameImpl() const
{
	return taskName(_thread);
}

pid_t NativeThreadInfoImpl::idImpl() const
{
	return _thread;
}

NativeThreadInfoImpl::HandleImpl NativeThreadInfoImpl::currentThreadHandleImpl()
{
	return taskIdSelf();
}

} // namespace Poco
