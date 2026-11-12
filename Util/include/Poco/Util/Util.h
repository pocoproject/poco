//
// Util.h
//
// Library: Util
// Package: Util
// Module:  Util
//
// Basic definitions for the Poco Util library.
// This file must be the first file included by every other Util
// header file.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_Util_INCLUDED
#define Util_Util_INCLUDED


#include "Poco/Foundation.h"


//
// Util_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Util_API POCO_API(Util_EXPORTS)


//
// Automatically link Util library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Util_EXPORTS)
		#pragma comment(lib, "PocoUtil" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Util_Util_INCLUDED
