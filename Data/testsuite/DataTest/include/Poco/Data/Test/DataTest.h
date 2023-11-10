//
// DataTest.h
//
// Library: DataTest
// Package: DataTestCore
// Module:  DataTest
//
// Basic definitions for the Poco DataTest library.
// This file must be the first file included by every other DataTest
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering d.o.o., and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataTest_DataTest_INCLUDED
#define DataTest_DataTest_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DataTest_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DataTest_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(DataTest_EXPORTS)
		#define DataTest_API __declspec(dllexport)
	#else
		#define DataTest_API __declspec(dllimport)
	#endif
#endif


#if !defined(DataTest_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define DataTest_API __attribute__ ((visibility ("default")))
	#else
		#define DataTest_API
	#endif
#endif


//
// Automatically link DataTest library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(DataTest_EXPORTS)
		#pragma comment(lib, "PocoDataTest" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // DataTest_DataTest_INCLUDED
