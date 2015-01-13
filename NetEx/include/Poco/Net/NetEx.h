//
// NetEx.h
//
// $Id: //poco/1.4/NetEx/include/Poco/NetEx/NetEx.h#2 $
//
// Library: NetEx
// Package: NetCore
// Module:  NetEx
//
// Basic definitions for the Poco NetEx library.
// This file must be the first file included by every other NetEx
// header file.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetEx_NetEx_INCLUDED
#define NetEx_NetEx_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NetEx_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NetEx_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(NetEx_EXPORTS)
		#define NetEx_API __declspec(dllexport)
	#else
		#define NetEx_API __declspec(dllimport)
	#endif
#endif


#if !defined(NetEx_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define NetEx_API __attribute__ ((visibility ("default")))
	#else
		#define NetEx_API
	#endif
#endif


//
// Automatically link NetEx library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(NetEx_EXPORTS)
		#pragma comment(lib, "PocoNetEx" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Net_NetEx_INCLUDED
