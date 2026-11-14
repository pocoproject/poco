//
// REST.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  REST
//
// Basic definitions for the Poco RemotingNG REST library.
// This file must be the first file included by every other REST
// header file.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_REST_INCLUDED
#define RemotingNG_REST_REST_INCLUDED


#ifndef Foundation_Foundation_INCLUDED
#include "Poco/Foundation.h"
#endif


//
// RemotingNGREST_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNGREST_API POCO_API(RemotingNGREST_EXPORTS)


//
// Automatically link JSON library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGREST_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGREST" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_REST_REST_INCLUDED
