//
// Redis.h
//
// Library: Redis
// Package: Redis
// Module:  Redis
//
// Basic definitions for the Poco Redis library.
// This file must be the first file included by every other Redis
// header file.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RedisRedis_INCLUDED
#define RedisRedis_INCLUDED


#include "Poco/Foundation.h"


//
// Redis_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Redis_API POCO_API(Redis_EXPORTS)


//
// Automatically link Redis library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Redis_EXPORTS)
		#pragma comment(lib, "PocoRedis" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RedisRedis_INCLUDED
