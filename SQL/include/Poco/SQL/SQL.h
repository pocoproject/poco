//
// SQL.h
//
// Library: SQL
// Package: SQLCore
// Module:  SQL
//
// Basic definitions for the Poco SQL library.
// This file must be the first file included by every other SQL
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_SQL_INCLUDED
#define SQL_SQL_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SQL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// Poco_SQL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//

#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(SQL_EXPORTS)
		#define Poco_SQL_API __declspec(dllexport)
	#else
		#define Poco_SQL_API __declspec(dllimport)
	#endif
#endif


#if !defined(Poco_SQL_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Poco_SQL_API __attribute__ ((visibility ("default")))
	#else
		#define Poco_SQL_API
	#endif
#endif

//
// Automatically link SQL library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SQL_EXPORTS)
		#pragma comment(lib, "PocoSQL" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SQL_SQL_INCLUDED
