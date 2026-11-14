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
// XSDParser_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define XSDParser_API POCO_API(XSDParser_EXPORTS)


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
