//
// SQLiteBDB.h
//
// Library: Data/SQLiteBDB
// Package: SQLiteBDB
// Module:  SQLiteBDB
//
// Basic definitions for the Poco SQLiteBDB library.
// This file must be the first file included by every other SQLiteBDB
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLiteBDB_SQLiteBDB_INCLUDED
#define SQLiteBDB_SQLiteBDB_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SQLiteBDB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SQLiteBDB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(SQLiteBDB_EXPORTS)
		#define SQLiteBDB_API __declspec(dllexport)
	#else
		#define SQLiteBDB_API __declspec(dllimport)
	#endif
#endif


#if !defined(SQLiteBDB_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define SQLiteBDB_API __attribute__ ((visibility ("default")))
	#else
		#define SQLiteBDB_API
	#endif
#endif


//
// Automatically link SQLiteBDB library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SQLiteBDB_EXPORTS)
		#pragma comment(lib, "PocoDataSQLiteBDB" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SQLiteBDB_SQLiteBDB_INCLUDED
