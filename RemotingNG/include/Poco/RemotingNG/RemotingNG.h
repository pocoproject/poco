//
// RemotingNG.h
//
// Library: RemotingNG
// Package: RemotingNG
// Module:  RemotingNG
//
// Basic definitions for the Applied Informatics RemotingNG library.
// This file must be the first file included by every other RemotingNG
// header file.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_RemotingNG_INCLUDED
#define RemotingNG_RemotingNG_INCLUDED


#include "Poco/Foundation.h"


//
// RemotingNG_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNG_API POCO_API(RemotingNG_EXPORTS)


#if defined(_WIN32)
#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated.
#endif


//
// Automatically link RemotingNG library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNG_EXPORTS)
		#pragma comment(lib, "PocoRemotingNG" POCO_LIB_SUFFIX)
	#endif
#endif


//
// For backwards compatibility, make static initialization workaround required on some platforms prior to C++11 a no-op.
//
#define remoting__staticInitBegin(var)
#define remoting__staticInitEnd(var)


#endif // RemotingNG_RemotingNG_INCLUDED
