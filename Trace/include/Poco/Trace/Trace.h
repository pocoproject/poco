//
// Trace.h
//
// Library: Trace
// Package: Trace
// Module:  Trace
//
// Basic definitions for the Poco Trace library.
// This file must be the first file included by every other Trace
// header file.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Trace_Trace_INCLUDED
#define Trace_Trace_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Trace_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// Trace_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Trace_EXPORTS)
		#define Trace_API __declspec(dllexport)
	#else
		#define Trace_API __declspec(dllimport)
	#endif
#endif


#if !defined(Trace_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Trace_API __attribute__ ((visibility ("default")))
	#else
		#define Trace_API
	#endif
#endif


//
// Automatically link Trace library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Trace_EXPORTS)
		#pragma comment(lib, "PocoTrace" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Trace_Trace_INCLUDED
