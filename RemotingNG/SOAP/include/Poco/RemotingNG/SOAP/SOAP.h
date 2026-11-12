//
// SOAP.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAP
//
// Basic definitions for the RemotingNG SOAP library.
// This file must be the first file included by every other SOAP
// header file.
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_SOAP_INCLUDED
#define RemotingNG_SOAP_SOAP_INCLUDED


#include "Poco/Foundation.h"


//
// RemotingNGSOAP_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNGSOAP_API POCO_API(RemotingNGSOAP_EXPORTS)


//
// Automatically link RemotingNGSOAP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGSOAP_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGSOAP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_SOAP_SOAP_INCLUDED
