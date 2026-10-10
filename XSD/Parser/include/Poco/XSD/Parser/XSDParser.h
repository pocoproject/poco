//
// XSDParser.h
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  XSDParser
//
// Basic definitions for the Poco XSDParser library.
// This file must be the first file included by every other XSDParser
// header file.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDParser_XSDParser_INCLUDED
#define XSDParser_XSDParser_INCLUDED


#include "Poco/Foundation.h"
#include <map>


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the XSDParser_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// XSDParser_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(XSDParser_EXPORTS)
		#define XSDParser_API __declspec(dllexport)
	#else
		#define XSDParser_API __declspec(dllimport)
	#endif
#endif


#if !defined(XSDParser_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define XSDParser_API __attribute__ ((visibility ("default")))
	#else
		#define XSDParser_API
	#endif
#endif


//
// Automatically link XSDParser library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XSDParser_EXPORTS)
		#pragma comment(lib, "PocoXSDParser" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::XSD::Parser {


using CompactAttributes = std::map<std::string, std::string>; // maps attrname to value


} // namespace Poco::XSD::Parser


#endif // XSDParser_XSDParser_INCLUDED
