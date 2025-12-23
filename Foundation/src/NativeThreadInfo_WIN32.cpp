//
// NativeThreadInfo_WIN32.cpp
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


#include "Poco/NativeThreadInfo_WIN32.h"
#include <processthreadsapi.h>
#include <stringapiset.h>

namespace Poco {

NativeThreadInfoImpl::NativeThreadInfoImpl(NativeThreadInfoImpl::HandleImpl handle):
	_thread(handle)
{
}

std::string NativeThreadInfoImpl::nameImpl() const
{
	PWSTR data = nullptr;
	HRESULT hr = GetThreadDescription(_thread, &data);
	if (SUCCEEDED(hr) && data)
	{
		std::string result;
		int convSize = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
		if (convSize > 0)
		{
			result.resize(convSize);
			WideCharToMultiByte(CP_UTF8, 0, data, -1, &result[0], convSize, NULL, NULL);
			if (!result.empty() && result.back() == '\0')
				result.pop_back();
		}
		LocalFree(data);
		return result;
	}
	return std::string();
}

NativeThreadInfoImpl::ThreadIdImpl NativeThreadInfoImpl::osTidImpl() const
{
	return GetThreadId(_thread);
}

NativeThreadInfoImpl::HandleImpl NativeThreadInfoImpl::currentThreadHandleImpl()
{
	return GetCurrentThread();
}

} // namespace Poco
