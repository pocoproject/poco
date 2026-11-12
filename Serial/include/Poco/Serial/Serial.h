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
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Poco_Serial_Serial_INCLUDED
#define Poco_Serial_Serial_INCLUDED


#include "Poco/Poco.h"


//
// Serial_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Serial_API POCO_API(Serial_EXPORTS)


//
// Automatically link PocoSerial library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Serial_EXPORTS)
		#pragma comment(lib, "PocoSerial" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Poco_Serial_Serial_INCLUDED
