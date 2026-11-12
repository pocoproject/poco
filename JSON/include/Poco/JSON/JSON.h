//
// JSON.h
//
// Library: JSON
// Package: JSON
// Module:  JSON
//
// Basic definitions for the Poco JSON library.
// This file must be the first file included by every other JSON
// header file.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_JSON_INCLUDED
#define JSON_JSON_INCLUDED


#include "Poco/Foundation.h"


//
// JSON_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define JSON_API POCO_API(JSON_EXPORTS)


//
// Automatically link JSON library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(JSON_EXPORTS)
		#pragma comment(lib, "PocoJSON" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // JSON_JSON_INCLUDED
