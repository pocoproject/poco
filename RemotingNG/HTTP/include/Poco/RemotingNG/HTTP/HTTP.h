//
// HTTP.h
//
// Library: RemotingNG/HTTP
// Package: HTTP
// Module:  HTTP
//
// Basic definitions for the Poco RemotingNG HTTP library.
// This file must be the first file included by every other HTTP
// header file.
//
// Copyright (c) 2010-2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_HTTP_HTTP_INCLUDED
#define RemotingNG_HTTP_HTTP_INCLUDED


#ifndef Foundation_Foundation_INCLUDED
#include "Poco/Foundation.h"
#endif


//
// RemotingNGHTTP_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNGHTTP_API POCO_API(RemotingNGHTTP_EXPORTS)


//
// Automatically link JSON library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGHTTP_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGHTTP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_HTTP_HTTP_INCLUDED
