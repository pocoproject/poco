//
// ASN.h
//
// $Id$
//
// Library: ASN
// Package: ASN
// Module:  ASN
//
// Basic definitions for the Poco ASN library.
// This file must be the first file included by every other ASN
// header file.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ASN_ASN_INCLUDED
#define ASN_ASN_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ASN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ASN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(ASN_EXPORTS)
		#define ASN_API __declspec(dllexport)
	#else
		#define ASN_API __declspec(dllimport)
	#endif
#endif


#if !defined(ASN_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define ASN_API __attribute__ ((visibility ("default")))
	#else
		#define ASN_API
	#endif
#endif


//
// Automatically link ASN library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(ASN_EXPORTS)
		#pragma comment(lib, "PocoASN" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // ASN_ASN_INCLUDED
