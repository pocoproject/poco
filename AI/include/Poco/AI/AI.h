//
// AI.h
//
// Library: AI
// Package: Core
// Module:  AI
//
// Basic definitions for the Poco AI library.
// This file must be the first file included by every other AI
// header file.
//
// The AI library is experimental: its interface may change within 2.x, as
// the vendor APIs it talks to change.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_AI_INCLUDED
#define AI_AI_INCLUDED


#include "Poco/Foundation.h"


//
// AI_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define AI_API POCO_API(AI_EXPORTS)


//
// Automatically link AI library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AI_EXPORTS)
		#pragma comment(lib, "PocoAI" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // AI_AI_INCLUDED
