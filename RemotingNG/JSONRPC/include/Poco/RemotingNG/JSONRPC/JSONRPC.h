//
// JSONRPC.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  JSONRPC
//
// Basic definitions for the Poco RemotingNG JSONRPC library.
// This file must be the first file included by every other JSONRPC
// header file.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_JSONRPC_INCLUDED
#define RemotingNG_JSONRPC_JSONRPC_INCLUDED


#ifndef Foundation_Foundation_INCLUDED
#include "Poco/Foundation.h"
#endif


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the RemotingNGJSONRPC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file sees
// RemotingNGJSONRPC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(RemotingNGJSONRPC_EXPORTS)
		#define RemotingNGJSONRPC_API __declspec(dllexport)
	#else
		#define RemotingNGJSONRPC_API __declspec(dllimport)
	#endif
#endif


#if !defined(RemotingNGJSONRPC_API)
	#define RemotingNGJSONRPC_API
#endif


//
// Automatically link JSON library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGJSONRPC_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGJSONRPC" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_JSONRPC_JSONRPC_INCLUDED
