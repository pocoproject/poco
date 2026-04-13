//
// SSH.h
//
// Library: SSH
// Package: SSH
// Module:  SSH
//
// Basic definitions for the Poco SSH library.
// This file must be the first file included by every other SSH
// header file.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSH_INCLUDED
#define SSH_SSH_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SSH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SSH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(SSH_EXPORTS)
		#define SSH_API __declspec(dllexport)
	#else
		#define SSH_API __declspec(dllimport)
	#endif
#endif


#if !defined(SSH_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define SSH_API __attribute__ ((visibility ("default")))
	#else
		#define SSH_API
	#endif
#endif


//
// Automatically link SSH library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SSH_EXPORTS)
		#pragma comment(lib, "PocoSSH" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SSH_SSH_INCLUDED
