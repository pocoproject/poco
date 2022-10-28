//
// UnWindows.h
//
// Library: Foundation
// Package: Core
// Module:  UnWindows
//
// Simple wrapper around the <windows.h> header file.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_UnWindows_INCLUDED
#define Foundation_UnWindows_INCLUDED


// Reduce bloat
#if defined(_WIN32)
	#if !defined(WIN32_LEAN_AND_MEAN) && !defined(POCO_BLOATED_WIN32)
		#define WIN32_LEAN_AND_MEAN
	#endif
#endif

// disable min/max macros
#define NOMINMAX

#if !defined(POCO_NO_WINDOWS_H)
    #include <windows.h>
    #ifdef __MINGW32__
        #include <winsock2.h>
        #include <iphlpapi.h>
        #include <ws2tcpip.h>
    #endif // __MINGW32__
#endif


#endif // Foundation_UnWindows_INCLUDED
