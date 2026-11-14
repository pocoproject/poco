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
// RemotingNGJSONRPC_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define RemotingNGJSONRPC_API POCO_API(RemotingNGJSONRPC_EXPORTS)


//
// Automatically link JSON library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(RemotingNGJSONRPC_EXPORTS)
		#pragma comment(lib, "PocoRemotingNGJSONRPC" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // RemotingNG_JSONRPC_JSONRPC_INCLUDED
