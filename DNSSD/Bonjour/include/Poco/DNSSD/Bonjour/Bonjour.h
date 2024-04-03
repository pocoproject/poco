//
// Bonjour.h
//
// $Id: //poco/1.7/DNSSD/Bonjour/include/Poco/DNSSD/Bonjour/Bonjour.h#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  Bonjour
//
// Basic definitions for the Poco DNSSD Bonjour library.
// This file must be the first file included by every other DNSSD Bonjour
// header file.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Bonjour_Bonjour_INCLUDED
#define DNSSD_Bonjour_Bonjour_INCLUDED


#include "Poco/DNSSD/DNSSD.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Bonjour_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DNSSD_Bonjour_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Bonjour_EXPORTS)
		#define DNSSD_Bonjour_API __declspec(dllexport)
	#else
		#define DNSSD_Bonjour_API __declspec(dllimport)
	#endif
#endif


#if !defined(DNSSD_Bonjour_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define DNSSD_Bonjour_API __attribute__ ((visibility ("default")))
	#else
		#define DNSSD_Bonjour_API
	#endif
#endif


#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Bonjour_EXPORTS)
		#pragma comment(lib, "PocoDNSSDBonjour" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {
namespace DNSSD {


void DNSSD_Bonjour_API initializeDNSSD();
	/// Initialize the DNSSD subsystem.


void DNSSD_Bonjour_API uninitializeDNSSD();
	/// Uninitialize the DNSSD subsystem.


} } // namespace Poco::DNSSD


#endif // DNSSD_Bonjour_Bonjour_INCLUDED
