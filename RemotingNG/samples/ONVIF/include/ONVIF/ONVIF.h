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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef ONVIF_INCLUDED
#define ONVIF_INCLUDED


#include "Poco/Poco.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ONVIF_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// ONVIF_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(ONVIF_EXPORTS)
		#define ONVIF_API __declspec(dllexport)
	#else
		#define ONVIF_API __declspec(dllimport)
	#endif
#endif


#if !defined(ONVIF_API)
	#define ONVIF_API
#endif


//
// Automatically link ONVIF library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ONVIF_EXPORTS)
		#pragma comment(lib, "ONVIF" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ONVIF_INCLUDED
