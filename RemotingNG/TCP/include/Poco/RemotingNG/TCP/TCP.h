//
// TCP.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  TCP
//
// Basic definitions for the RemotingNG TCP library.
// This file must be the first file included by every other TCP
// header file.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_TCP_INCLUDED
#define RemotingNG_TCP_TCP_INCLUDED


#include "Poco/Foundation.h"


//
// RemotingNGTCP_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNGTCP_API POCO_API(RemotingNGTCP_EXPORTS)


//
// Automatically link RemotingNGTCP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGTCP_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGTCP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_TCP_TCP_INCLUDED
