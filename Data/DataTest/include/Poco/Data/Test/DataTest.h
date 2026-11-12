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
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataTest_DataTest_INCLUDED
#define DataTest_DataTest_INCLUDED


#include "Poco/Foundation.h"


//
// DataTest_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define DataTest_API POCO_API(DataTest_EXPORTS)


//
// Automatically link DataTest library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(DataTest_EXPORTS)
		#pragma comment(lib, "PocoDataTest" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // DataTest_DataTest_INCLUDED
