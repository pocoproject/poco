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
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_AI_INCLUDED
#define AI_AI_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the AI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// AI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(AI_EXPORTS)
		#define AI_API __declspec(dllexport)
	#else
		#define AI_API __declspec(dllimport)
	#endif
#endif


#if !defined(AI_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define AI_API __attribute__ ((visibility ("default")))
	#else
		#define AI_API
	#endif
#endif


//
// Automatically link AI library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(AI_EXPORTS)
		#pragma comment(lib, "PocoAI" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // AI_AI_INCLUDED
