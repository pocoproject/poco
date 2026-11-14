//
// Geo.h
//
// Library: Geo
// Package: Geo
// Module:  Geo
//
// Basic definitions for the Poco Geo library.
// This file must be the first file included by every other Geo
// header file.
//
// Copyright (c) 2010-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Geo_Geo_INCLUDED
#define Geo_Geo_INCLUDED


#include "Poco/Poco.h"


//
// Geo_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Geo_API POCO_API(Geo_EXPORTS)


//
// Automatically link Geo library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Geo_EXPORTS)
		#pragma comment(lib, "PocoGeo" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Geo_Geo_INCLUDED
