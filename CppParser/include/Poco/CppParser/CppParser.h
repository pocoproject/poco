//
// CppParser.h
//
// Library: CppParser
// Package: CppParser
// Module:  CppParser
//
// Basic definitions for the Poco CppParser library.
// This file must be the first file included by every other CppParser
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParser_CppParser_INCLUDED
#define CppParser_CppParser_INCLUDED


#include "Poco/Foundation.h"


//
// CppParser_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define CppParser_API POCO_API(CppParser_EXPORTS)


//
// Automatically link CppParser library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(CppParser_EXPORTS)
		#pragma comment(lib, "PocoCppParser" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // CppParser_CppParser_INCLUDED
