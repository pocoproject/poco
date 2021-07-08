//
// ActiveRecordLib.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  ActiveRecordLib
//
// Basic definitions for the ActiveRecord library.
// This file must be the first file included by every other ActiveRecordLib
// header file.
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_ActiveRecordLib_INCLUDED
#define ActiveRecord_ActiveRecordLib_INCLUDED


#include "Poco/Poco.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ActiveRecordLib_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ActiveRecordLib_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(ActiveRecordLib_EXPORTS)
		#define ActiveRecordLib_API __declspec(dllexport)
	#else
		#define ActiveRecordLib_API __declspec(dllimport)
	#endif
#endif


#if !defined(ActiveRecordLib_API)
	#define ActiveRecordLib_API
#endif


//
// Automatically link ActiveRecordLib library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ActiveRecordLib_EXPORTS)
		#pragma comment(lib, "PocoActiveRecord" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ActiveRecord_ActiveRecordLib_INCLUDED
