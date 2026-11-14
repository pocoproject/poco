//
// SevenZip.h
//
// Library: SevenZip
// Package: SevenZip
// Module:  SevenZip
//
// Basic definitions for the Poco SevenZip library.
// This file must be the first file included by every other SevenZip
// header file.
//
// Copyright (c) 2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SevenZip_SevenZip_INCLUDED
#define SevenZip_SevenZip_INCLUDED


#include "Poco/Foundation.h"


//
// SevenZip_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define SevenZip_API POCO_API(SevenZip_EXPORTS)


//
// Automatically link SevenZip library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SevenZip_EXPORTS)
		#pragma comment(lib, "PocoSevenZip" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SevenZip_SevenZip_INCLUDED
