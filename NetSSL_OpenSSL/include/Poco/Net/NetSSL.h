//
// NetSSL.h
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  OpenSSL
//
// Basic definitions for the Poco OpenSSL library.
// This file must be the first file included by every other OpenSSL
// header file.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_NetSSL_INCLUDED
#define NetSSL_NetSSL_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Crypto/Crypto.h"

//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NetSSL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NetSSL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(POCO_COMPILER_MSVC) && defined(POCO_DLL)
	#if defined(NetSSL_EXPORTS)
		#define NetSSL_API __declspec(dllexport)
	#else
		#define NetSSL_API __declspec(dllimport)
	#endif
#endif


#if !defined(NetSSL_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define NetSSL_API __attribute__ ((visibility ("default")))
	#else
		#define NetSSL_API
	#endif
#endif


//
// Automatically link NetSSL
//
#if defined(POCO_COMPILER_MSVC)
	#if !defined(POCO_NO_AUTOMATIC_LIBS)
		#if defined(POCO_INTERNAL_OPENSSL_MSVC_VER)
			#if defined(POCO_EXTERNAL_OPENSSL)
				#pragma warning "External OpenSSL defined but internal headers used - possible mismatch!"
			#endif // POCO_EXTERNAL_OPENSSL
			#if !defined(_DEBUG)
				#define POCO_DEBUG_SUFFIX ""
				#if !defined (_DLL)
					#define POCO_STATIC_SUFFIX "mt"
				#else // _DLL
					#define POCO_STATIC_SUFFIX ""
				#endif
			#else // _DEBUG
				#define POCO_DEBUG_SUFFIX "d"
				#if !defined (_DLL)
					#define POCO_STATIC_SUFFIX "mt"
				#else // _DLL
					#define POCO_STATIC_SUFFIX ""
				#endif
			#endif
			#pragma comment(lib, "libcrypto" POCO_STATIC_SUFFIX POCO_DEBUG_SUFFIX ".lib")
			#pragma comment(lib, "libssl" POCO_STATIC_SUFFIX POCO_DEBUG_SUFFIX ".lib")
			#if !defined(_WIN64) && !defined (_DLL) && \
						(POCO_INTERNAL_OPENSSL_MSVC_VER == 120) && \
						(POCO_MSVC_VERSION < POCO_INTERNAL_OPENSSL_MSVC_VER)
				#pragma comment(lib, "libPreVS2013CRT" POCO_STATIC_SUFFIX POCO_DEBUG_SUFFIX ".lib")
			#endif
			#if !defined (_DLL) && (POCO_MSVS_VERSION >= 2015)
				#pragma comment(lib, "legacy_stdio_definitions.lib")
				#pragma comment(lib, "legacy_stdio_wide_specifiers.lib")
			#endif
		#endif // POCO_INTERNAL_OPENSSL_MSVC_VER
		#if !defined(NetSSL_EXPORTS)
			#pragma comment(lib, "PocoNetSSL" POCO_LIB_SUFFIX)
		#endif
	#endif // POCO_NO_AUTOMATIC_LIBS
#endif


namespace Poco {
namespace Net {


void NetSSL_API initializeSSL();
	/// Initialize the NetSSL library, as well as the underlying OpenSSL
	/// libraries, by calling Poco::Crypto::OpenSSLInitializer::initialize().
	///
	/// Should be called before using any class from the NetSSL library.
	/// The NetSSL will be initialized automatically, through
	/// Poco::Crypto::OpenSSLInitializer instances or similar mechanisms
	/// when creating Context or SSLManager instances.
	/// However, it is recommended to call initializeSSL()
	/// in any case at application startup.
	///
	/// Can be called multiple times; however, for every call to
	/// initializeSSL(), a matching call to uninitializeSSL()
	/// must be performed.
	

void NetSSL_API uninitializeSSL();
	/// Uninitializes the NetSSL library by calling
	/// Poco::Crypto::OpenSSLInitializer::uninitialize() and
	/// shutting down the SSLManager.


} } // namespace Poco::Net


#endif // NetSSL_NetSSL_INCLUDED
