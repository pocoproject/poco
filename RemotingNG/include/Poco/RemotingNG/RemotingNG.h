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
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_RemotingNG_INCLUDED
#define RemotingNG_RemotingNG_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the RemotingNG_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// RemotingNG_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(RemotingNG_EXPORTS)
		#define RemotingNG_API __declspec(dllexport)
	#else
		#define RemotingNG_API __declspec(dllimport)
	#endif
#endif


#if !defined(RemotingNG_API)
	#define RemotingNG_API
#endif


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
