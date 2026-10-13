//
// Serial.h
//
// Library: Serial
// Package: Serial
// Module:  Serial
//
// Basic definitions for the Poco Serial library.
// This file must be the first file included by every other Serial
// header file.
//
// Copyright (c) 2013-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Poco_Serial_Serial_INCLUDED
#define Poco_Serial_Serial_INCLUDED


#include "Poco/Poco.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Serial_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// Serial_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Serial_EXPORTS)
		#define Serial_API __declspec(dllexport)
	#else
		#define Serial_API __declspec(dllimport)
	#endif
#endif


#if !defined(Serial_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Serial_API __attribute__ ((visibility ("default")))
	#else
		#define Serial_API
	#endif
#endif


//
// Automatically link PocoSerial library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Serial_EXPORTS)
		#pragma comment(lib, "PocoSerial" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Poco_Serial_Serial_INCLUDED
