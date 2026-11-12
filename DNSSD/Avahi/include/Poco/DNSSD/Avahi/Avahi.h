//
// Avahi.h
//
// $Id: //poco/1.7/DNSSD/Avahi/include/Poco/DNSSD/Avahi/Avahi.h#1 $
//
// Library: DNSSD/Avahi
// Package: Implementation
// Module:  Avahi
//
// Basic definitions for the Poco DNSSD Avahi library.
// This file must be the first file included by every other DNSSD Avahi
// header file.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Avahi_Avahi_INCLUDED
#define DNSSD_Avahi_Avahi_INCLUDED


#include "Poco/DNSSD/DNSSD.h"


//
// Avahi_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define DNSSD_Avahi_API POCO_API(Avahi_EXPORTS)


#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Avahi_EXPORTS)
		#pragma comment(lib, "PocoDNSSDAvahi" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::DNSSD {


void DNSSD_Avahi_API initializeDNSSD();
	/// Initialize the DNSSD subsystem.


void DNSSD_Avahi_API uninitializeDNSSD();
	/// Uninitialize the DNSSD subsystem.


} // namespace Poco::DNSSD


#endif // DNSSD_Avahi_Avahi_INCLUDED
