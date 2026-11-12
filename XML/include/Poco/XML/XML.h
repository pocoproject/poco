//
// XML.h
//
// Library: XML
// Package: XML
// Module:  XML
//
// Basic definitions for the Poco XML library.
// This file must be the first file included by every other XML
// header file.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XML_XML_INCLUDED
#define XML_XML_INCLUDED


#include "Poco/Foundation.h"


//
// XML_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define XML_API POCO_API(XML_EXPORTS)


//
// Automatically link XML library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XML_EXPORTS)
		#pragma comment(lib, "PocoXML" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // XML_XML_INCLUDED
