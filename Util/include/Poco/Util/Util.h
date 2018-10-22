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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Util_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// Util_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(POCO_COMPILER_MSVC) && defined(POCO_DLL)
	#if defined(Util_EXPORTS)
		#define Util_API __declspec(dllexport)
	#else
		#define Util_API __declspec(dllimport)
	#endif
#endif


#if !defined(Util_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Util_API __attribute__ ((visibility ("default")))
	#else
		#define Util_API
	#endif
#endif


//
// Define wrapper if wmain() is disabled in MinGW.
// Use option "-municode" to enable wmain().
// Required by Application and ServerApplication.
//
#if defined(__MINGW32__) && !defined(POCO_NO_WMAIN_WRAPPER)
	#define POCO_WMAIN_WRAPPER()  \
	extern int _CRT_glob;         \
	extern "C" void __wgetmainargs(int*, wchar_t***, wchar_t***, int, int*); \
	int main() {			      \
		wchar_t **enpv, **argv;   \
		int argc, si = 0;	      \
		__wgetmainargs(&argc, &argv, &enpv, _CRT_glob, &si); \
		return wmain(argc, argv); \
	}
#else
	#define POCO_WMAIN_WRAPPER()
#endif


//
// Automatically link Util library.
//
#if defined(POCO_COMPILER_MSVC)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Util_EXPORTS)
		#pragma comment(lib, "PocoUtil" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Util_Util_INCLUDED
