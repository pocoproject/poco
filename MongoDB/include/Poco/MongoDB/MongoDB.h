//
// MongoDB.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  MongoDB
//
// Basic definitions for the Poco MongoDB library.
// This file must be the first file included by every other MongoDB
// header file.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDBMongoDB_INCLUDED
#define MongoDBMongoDB_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MongoDB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MongoDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//


#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(MongoDB_EXPORTS)
		#define MongoDB_API __declspec(dllexport)
	#else
		#define MongoDB_API __declspec(dllimport)
	#endif
#endif


#if !defined(MongoDB_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define MongoDB_API __attribute__ ((visibility ("default")))
	#else
		#define MongoDB_API
	#endif
#endif


// MongoDB wire protocol and BSON specification limits.
// These constants are defined centrally so that all validation
// code in the library uses the same values.

namespace Poco {
namespace MongoDB {

/// Maximum BSON document size (16 MB) per MongoDB specification.
/// Applies to documents, strings, binary data, and cstrings.
static constexpr Poco::Int32 BSON_MAX_DOCUMENT_SIZE = 16 * 1024 * 1024;

/// Minimum BSON document size (5 bytes): 4-byte size field + 1-byte null terminator.
static constexpr Poco::Int32 BSON_MIN_DOCUMENT_SIZE = 5;

/// Minimum BSON string size (1 byte for the null terminator).
static constexpr Poco::Int32 BSON_MIN_STRING_SIZE = 1;

/// Maximum OP_MSG message size (48 MB) per MongoDB specification.
static constexpr Poco::Int32 OP_MSG_MAX_SIZE = 48 * 1024 * 1024;

/// Default local threshold for "nearest" read preference (15 ms = 15000 µs).
/// Servers within this threshold of the minimum RTT are eligible for selection.
static constexpr Poco::Int64 DEFAULT_LOCAL_THRESHOLD_US = 15000;

} } // namespace Poco::MongoDB


//
// Automatically link MongoDB library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(MongoDB_EXPORTS)
		#pragma comment(lib, "PocoMongoDB" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // MongoDBMongoDB_INCLUDED
