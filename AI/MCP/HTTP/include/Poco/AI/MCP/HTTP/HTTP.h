//
// HTTP.h
//
// Library: AIMCPHTTP
// Package: HTTP
// Module:  HTTP
//
// Basic definitions for the AI MCP HTTP(S) client transport library
// (PocoAIMCPHTTP). This file must be the first file included by every other
// AI MCP HTTP header.
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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the AIMCPHTTP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// AIMCPHTTP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(AIMCPHTTP_EXPORTS)
		#define AIMCPHTTP_API __declspec(dllexport)
	#else
		#define AIMCPHTTP_API __declspec(dllimport)
	#endif
#endif


#if !defined(AIMCPHTTP_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define AIMCPHTTP_API __attribute__ ((visibility ("default")))
	#else
		#define AIMCPHTTP_API
	#endif
#endif


//
// Automatically link AIMCPHTTP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AIMCPHTTP_EXPORTS)
		#pragma comment(lib, "PocoAIMCPHTTP" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // AIMCPHTTP_HTTP_INCLUDED
