//
// HTTP.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  HTTP
//
// Basic definitions for the AI MCP HTTP(S) transport library (PocoAIMCPHTTP):
// the Streamable HTTP client and server transports. This file must be the
// first file included by every other AI MCP HTTP header.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_HTTP_INCLUDED
#define AIMCPHTTP_HTTP_INCLUDED


#include "Poco/AI/MCP/MCP.h"


//
// AIMCPHTTP_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define AIMCPHTTP_API POCO_API(AIMCPHTTP_EXPORTS)


//
// Automatically link AIMCPHTTP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AIMCPHTTP_EXPORTS)
		#pragma comment(lib, "PocoAIMCPHTTP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // AIMCPHTTP_HTTP_INCLUDED
