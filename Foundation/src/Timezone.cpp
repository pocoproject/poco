//
// Timezone.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Timezone
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Poco/Timezone.h"
#include <ctime>


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Timezone_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "Timezone_VX.cpp"
#else
#include "Timezone_UNIX.cpp"
#endif


namespace Poco {


int Timezone::tzd()
{
	return utcOffset() + dst();
}


} // namespace Poco
