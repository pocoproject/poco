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
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSH_INCLUDED
#define SSH_SSH_INCLUDED


#include "Poco/Foundation.h"


//
// SSH_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define SSH_API POCO_API(SSH_EXPORTS)


//
// Automatically link SSH library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(SSH_EXPORTS)
		#pragma comment(lib, "PocoSSH" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // SSH_SSH_INCLUDED
