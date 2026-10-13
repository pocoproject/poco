//
// MCP.h
//
// Library: AIMCP
// Package: Core
// Module:  MCP
//
// Basic definitions for the AI MCP (Model Context Protocol) library.
// This file must be the first file included by every other AI MCP header.
//
// The core library is the protocol implementation, a generic ingress that
// accepts a stream, and the stdio transport; it depends only on Poco::JSON
// (no Poco::Net). The HTTP(S) client transport lives in the separate
// PocoAIMCPHTTP library.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_MCP_INCLUDED
#define AIMCP_MCP_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make
// exporting from a DLL simpler. All files within this DLL are compiled with the
// AIMCP_EXPORTS symbol defined on the command line. This symbol should not be
// defined on any project that uses this DLL. This way any other project whose
// source files include this file see AIMCP_API functions as being imported from a
// DLL, whereas this DLL sees symbols defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(AIMCP_EXPORTS)
		#define AIMCP_API __declspec(dllexport)
	#else
		#define AIMCP_API __declspec(dllimport)
	#endif
#endif


#if !defined(AIMCP_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define AIMCP_API __attribute__ ((visibility ("default")))
	#else
		#define AIMCP_API
	#endif
#endif


//
// Automatically link MCP library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AIMCP_EXPORTS)
		#pragma comment(lib, "PocoAIMCP" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {
namespace AI {
namespace MCP {


//
// Protocol constants. Compile-time (inline constexpr): a const std::string here
// would be dynamically initialized once per translation unit including this
// header. Character arrays rather than std::string_view so every existing use
// (Dynamic::Var construction, CppUnit assertEqual, string comparisons) keeps
// working unchanged.
//
inline constexpr char JSONRPC_VERSION[] = "2.0";
	/// The JSON-RPC version every MCP message carries.

inline constexpr char PROTOCOL_VERSION[] = "2025-06-18";
	/// The MCP protocol revision this library implements and advertises.


} } } // namespace Poco::AI::MCP


#endif // AIMCP_MCP_INCLUDED
