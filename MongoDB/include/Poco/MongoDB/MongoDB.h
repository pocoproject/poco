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
// MongoDB_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define MongoDB_API POCO_API(MongoDB_EXPORTS)


// MongoDB wire protocol and BSON specification limits.
// These constants are defined centrally so that all validation
// code in the library uses the same values.

namespace Poco {
namespace MongoDB {

/// Maximum size of a document a client can insert or update (hello.maxBsonObjectSize).
/// Replies can contain larger documents, such as oplog entries.
inline constexpr Poco::Int32 BSON_MAX_DOCUMENT_SIZE = 16 * 1024 * 1024;

/// Minimum BSON document size (5 bytes): 4-byte size field + 1-byte null terminator.
inline constexpr Poco::Int32 BSON_MIN_DOCUMENT_SIZE = 5;

/// Minimum BSON string size (1 byte for the null terminator).
inline constexpr Poco::Int32 BSON_MIN_STRING_SIZE = 1;

/// No longer used; MAX_MESSAGE_SIZE_BYTES limits messages in both directions.
POCO_DEPRECATED("use MAX_MESSAGE_SIZE_BYTES")
inline constexpr Poco::Int32 OP_MSG_MAX_SIZE = 48 * 1024 * 1024;

/// Maximum size of a message in either direction, header included
/// (hello.maxMessageSizeBytes). The server closes the connection on a larger
/// request. It also bounds every document and value that is read.
inline constexpr Poco::Int32 MAX_MESSAGE_SIZE_BYTES = 48000000;

/// Default local threshold for "nearest" read preference (15 ms = 15000 us).
/// Servers within this threshold of the minimum RTT are eligible for selection.
inline constexpr Poco::Int64 DEFAULT_LOCAL_THRESHOLD_US = 15000;

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
