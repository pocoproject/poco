//
// Zip.h
//
// Library: Zip
// Package: Zip
// Module:  Zip
//
// Basic definitions for the Poco Zip library.
// This file must be the first file included by every other Zip
// header file.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Zip_Zip_INCLUDED
#define Zip_Zip_INCLUDED


#include "Poco/Foundation.h"


//
// Zip_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Zip_API POCO_API(Zip_EXPORTS)


//
// Automatically link Zip library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Zip_EXPORTS)
		#pragma comment(lib, "PocoZip" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Zip_Zip_INCLUDED
