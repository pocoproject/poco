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
	HRESULT hr = GetThreadDescription(_thread, &data);
	if (SUCCEEDED(hr))
	{   
		std::string result;
		const UINT codePage = CP_THREAD_ACP;
		const DWORD flags = 0;
		int convSize = WideCharToMultiByte(
				codePage,
				flags,
				data,
				-1,
				NULL /* LPSTR lpMultiByteStr*/,
				0 /* int cbMultiByte */,
				NULL /* LPCCH lpDefaultChar */,
				NULL /* LPBOOL lpUsedDefaultChar */
			);
		if (convSize >= 0) {
			result.resize(convSize);
			WideCharToMultiByte(
				codePage,
				flags,
				data,
				-1,
				&result[0],
				convSize,
				NULL /* LPCCH lpDefaultChar */,
				NULL /* LPBOOL lpUsedDefaultChar */
			);
		}		
		LocalFree(data);
		return result;
	}

	return {};
}

NativeThreadInfoImpl::ThreadIdImpl NativeThreadInfoImpl::idImpl() const
{
	return GetThreadId(_thread);
}

NativeThreadInfoImpl::HandleImpl NativeThreadInfoImpl::currentThreadHandleImpl()
{
	return GetCurrentThread();
}

} // namespace Poco
