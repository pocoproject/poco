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
// Bonjour_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define DNSSD_Bonjour_API POCO_API(Bonjour_EXPORTS)


#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Bonjour_EXPORTS)
		#pragma comment(lib, "PocoDNSSDBonjour" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::DNSSD {


void DNSSD_Bonjour_API initializeDNSSD();
	/// Initialize the DNSSD subsystem.


void DNSSD_Bonjour_API uninitializeDNSSD();
	/// Uninitialize the DNSSD subsystem.


} // namespace Poco::DNSSD


#endif // DNSSD_Bonjour_Bonjour_INCLUDED
