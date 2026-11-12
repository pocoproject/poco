//
// NetSSL.h
//
// Library: NetSSL_Win
// Package: SSLCore
// Module:  SSLCore
//
// Basic definitions for the Poco NetSSL library.
// This file must be the first file included by every other NetSSL
// header file.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_NetSSL_INCLUDED
#define NetSSL_NetSSL_INCLUDED


#include "Poco/Net/Net.h"


//
// NetSSL_Win_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define NetSSL_Win_API POCO_API(NetSSL_Win_EXPORTS)


//
// Allow detection of NetSSL_Win at compile time
//
#define POCO_NETSSL_WIN 1


//
// Automatically link NetSSL library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(NetSSL_Win_EXPORTS)
		#pragma comment(lib, "PocoNetSSLWin" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::Net {


void NetSSL_Win_API initializeSSL();
	/// Initialize the NetSSL library, as well as the underlying Windows
	/// libraries.
	///
	/// Can be called multiple times; however, for every call to
	/// initializeSSL(), a matching call to uninitializeSSL()
	/// must be performed.


void NetSSL_Win_API uninitializeSSL();
	/// Uninitializes the NetSSL library and
	/// shutting down the SSLManager.


} // namespace Poco::Net


#endif // NetSSL_NetSSL_INCLUDED
