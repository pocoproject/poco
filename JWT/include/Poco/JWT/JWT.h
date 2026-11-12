//
// JWT.h
//
// Library: JWT
// Package: JWT
// Module:  JWT
//
// Basic definitions for the Poco JWT library.
// This file must be the first file included by every other JWT
// header file.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JWT_JWT_INCLUDED
#define JWT_JWT_INCLUDED


#include "Poco/Foundation.h"


//
// JWT_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define JWT_API POCO_API(JWT_EXPORTS)


//
// Automatically link JWT library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(JWT_EXPORTS)
		#pragma comment(lib, "PocoJWT" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // JWT_JWT_INCLUDED
