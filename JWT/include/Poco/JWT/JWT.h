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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the JWT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// JWT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(JWT_EXPORTS)
		#define JWT_API __declspec(dllexport)
	#else
		#define JWT_API __declspec(dllimport)
	#endif
#endif


#if !defined(JWT_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define JWT_API __attribute__ ((visibility ("default")))
	#else
		#define JWT_API
	#endif
#endif


//
// Automatically link JWT library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(JWT_EXPORTS)
		#pragma comment(lib, "PocoJWT" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // JWT_JWT_INCLUDED
