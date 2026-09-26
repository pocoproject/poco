//
// XSDTypes.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  XSDTypes
//
// Basic definitions for the Poco XSDTypes library.
// This file must be the first file included by every other XSDTypes
// header file.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_XSDTypes_INCLUDED
#define XSDTypes_XSDTypes_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the XSDTypes_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// XSDTypes_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(XSDTypes_EXPORTS)
		#define XSDTypes_API __declspec(dllexport)
	#else
		#define XSDTypes_API __declspec(dllimport)
	#endif
#endif


#if !defined(XSDTypes_API)
	#define XSDTypes_API
#endif


//
// Automatically link XSDTypes library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XSDTypes_EXPORTS)
		#pragma comment(lib, "PocoXSDTypes" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // XSDTypes_XSDTypes_INCLUDED
