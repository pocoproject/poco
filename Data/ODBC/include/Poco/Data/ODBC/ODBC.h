//
// ODBC.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  ODBC
//
// Basic definitions for the Poco ODBC library.
// This file must be the first file included by every other ODBC
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_ODBC_INCLUDED
#define Data_ODBC_ODBC_INCLUDED


#include "Poco/Foundation.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ODBC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ODBC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(ODBC_EXPORTS)
		#define ODBC_API __declspec(dllexport)
	#else
		#define ODBC_API __declspec(dllimport)
	#endif
#endif


#if !defined(ODBC_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define ODBC_API __attribute__ ((visibility ("default")))
	#else
		#define ODBC_API
	#endif
#endif


#include "Poco/Data/ODBC/Unicode.h"

#if (__cplusplus >= 201703L)
	#if __has_include(<msodbcsql.h>)
		#include <msodbcsql.h>
		#define POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT

		// To enable varchar(max) > 8000 bytes, set to 1.
		//
		// Notes:
		//
		// - this setting works in conjunction with
		//   the session "maxFieldSize" property, which
		//   ultimately determines the max string length.
		//
		// - increasing the "maxFieldSize" property may
		//   affect performance (more memory preallocated
		//   for prepared statements in order to safely
		//   accommodate data returned at execution)
		#if !defined(POCO_DATA_SQL_SERVER_BIG_STRINGS)
			#define POCO_DATA_SQL_SERVER_BIG_STRINGS 1
		#endif
	#endif
#endif


//
// Automatically link Data library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ODBC_EXPORTS)
		#pragma comment(lib, "PocoDataODBC" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ODBC_ODBC_INCLUDED
