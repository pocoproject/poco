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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the RemotingNGTCP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// RemotingNGTCP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(RemotingNGTCP_EXPORTS)
		#define RemotingNGTCP_API __declspec(dllexport)
	#else
		#define RemotingNGTCP_API __declspec(dllimport)
	#endif
#endif


#if !defined(RemotingNGTCP_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define RemotingNGTCP_API __attribute__ ((visibility ("default")))
	#else
		#define RemotingNGTCP_API
	#endif
#endif


//
// Automatically link RemotingNGTCP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGTCP_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGTCP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_TCP_TCP_INCLUDED
