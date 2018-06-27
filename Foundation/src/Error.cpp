//
// Error.cpp
//
// Library: Foundation
// Package: Core
// Module:  Error
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Error.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include "Poco/UnWindows.h"
#endif
#include <string>
#include <string.h>
#include <errno.h>


namespace Poco {


#ifdef POCO_OS_FAMILY_WINDOWS


	Poco::UInt32 Error::last()
	{
		return GetLastError();
	}


	std::string Error::getMessage(Poco::UInt32 errorCode)
	{
		std::string errMsg;
		DWORD dwFlg = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	#if !defined(POCO_NO_WSTRING)
		LPWSTR lpMsgBuf = 0;
		if (FormatMessageW(dwFlg, 0, errorCode, 0, (LPWSTR) & lpMsgBuf, 0, NULL))
			UnicodeConverter::toUTF8(lpMsgBuf, errMsg);
		LocalFree(lpMsgBuf);
	#endif

		return errMsg;
	}


#else

	
	int Error::last()
	{
		return errno;
	}


	std::string Error::getMessage(int errorCode)
	{
#if defined _GNU_SOURCE || (_XOPEN_SOURCE >= 600) || POCO_OS == POCO_OS_ANDROID || __APPLE__
		char errmsg[256] = "";
		return std::string(strerror_result(strerror_r(errorCode, errmsg, sizeof(errmsg)), errmsg));
#else
		return std::string(strerror(errorCode));
#endif
	}


#endif


} // namespace Poco
