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
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_XSDTypes_INCLUDED
#define XSDTypes_XSDTypes_INCLUDED


#include "Poco/Foundation.h"


//
// XSDTypes_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define XSDTypes_API POCO_API(XSDTypes_EXPORTS)


//
// Automatically link XSDTypes library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XSDTypes_EXPORTS)
		#pragma comment(lib, "PocoXSDTypes" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // XSDTypes_XSDTypes_INCLUDED
