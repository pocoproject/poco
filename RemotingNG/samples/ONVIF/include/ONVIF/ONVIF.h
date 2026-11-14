//
// ONVIF.h
//
// Basic definitions for the ONVIF library.
// This file must be the first file included by every other ONVIF
// header file.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ONVIF_INCLUDED
#define ONVIF_INCLUDED


#include "Poco/Poco.h"


//
// ONVIF_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define ONVIF_API POCO_API(ONVIF_EXPORTS)


//
// Automatically link ONVIF library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ONVIF_EXPORTS)
		#pragma comment(lib, "ONVIF" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ONVIF_INCLUDED
