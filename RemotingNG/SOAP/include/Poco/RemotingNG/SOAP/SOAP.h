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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SOAP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// SOAP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(RemotingNGSOAP_EXPORTS)
		#define RemotingNGSOAP_API __declspec(dllexport)
	#else
		#define RemotingNGSOAP_API __declspec(dllimport)
	#endif
#endif


#if !defined(RemotingNGSOAP_API)
	#define RemotingNGSOAP_API
#endif


//
// Automatically link RemotingNGSOAP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGSOAP_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGSOAP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_SOAP_SOAP_INCLUDED
