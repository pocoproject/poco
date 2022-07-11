//
// Platform_WIN32.h
//
// Library: Foundation
// Package: Core
// Module:  Platform
//
// Platform and architecture identification macros
// and platform-specific definitions for Windows.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Platform_WIN32_INCLUDED
#define Foundation_Platform_WIN32_INCLUDED


#include "Poco/UnWindows.h"



// Verify that we're built with the multithreaded
// versions of the runtime libraries
#if defined(_MSC_VER) && !defined(_MT)
	#error Must compile with /MD, /MDd, /MT or /MTd
#endif


// https://en.wikipedia.org/wiki/Microsoft_Visual_C++
#if (_MSC_VER >= 1300) && (_MSC_VER < 1400)   // Visual Studio 2003, MSVC++ 7.1
	#define POCO_MSVS_VERSION 2003
	#define POCO_MSVC_VERSION 71
#elif (_MSC_VER >= 1400) && (_MSC_VER < 1500) // Visual Studio 2005, MSVC++ 8.0
	#define POCO_MSVS_VERSION 2005
	#define POCO_MSVC_VERSION 80
#elif (_MSC_VER >= 1500) && (_MSC_VER < 1600) // Visual Studio 2008, MSVC++ 9.0
	#define POCO_MSVS_VERSION 2008
	#define POCO_MSVC_VERSION 90
#elif (_MSC_VER >= 1600) && (_MSC_VER < 1700) // Visual Studio 2010, MSVC++ 10.0
	#define POCO_MSVS_VERSION 2010
	#define POCO_MSVC_VERSION 100
#elif (_MSC_VER >= 1700) && (_MSC_VER < 1800) // Visual Studio 2012, MSVC++ 11.0
	#define POCO_MSVS_VERSION 2012
	#define POCO_MSVC_VERSION 110
#elif (_MSC_VER >= 1800) && (_MSC_VER < 1900) // Visual Studio 2013, MSVC++ 12.0
	#define POCO_MSVS_VERSION 2013
	#define POCO_MSVC_VERSION 120
#elif (_MSC_VER >= 1900) && (_MSC_VER < 1910) // Visual Studio 2015, MSVC++ 14.0
	#define POCO_MSVS_VERSION 2015
	#define POCO_MSVC_VERSION 140
#elif (_MSC_VER >= 1910) && (_MSC_VER < 1920) // Visual Studio 2017, MSVC++ 14.1
	#define POCO_MSVS_VERSION 2017
	#define POCO_MSVC_VERSION 141
#elif (_MSC_VER >= 1920) && (_MSC_VER < 1930) // Visual Studio 2019, MSVC++ 14.2
	#define POCO_MSVS_VERSION 2019
	#define POCO_MSVC_VERSION 142
#elif (_MSC_VER >= 1930) // Visual Studio 2022, MSVC++ 14.3
	#define POCO_MSVS_VERSION 2022
	#define POCO_MSVC_VERSION 143
#endif


// Turn off some annoying warnings
#if defined(_MSC_VER)
	#pragma warning(disable:4018)	// signed/unsigned comparison
	#pragma warning(disable:4250)	// VC++ 11.0: inheriting from std stream classes produces C4250 warning;
									// see <http://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning>
	#pragma warning(disable:4251)	// ... needs to have dll-interface warning
	#pragma warning(disable:4275)	// non dll-interface class 'std::exception' used as base for dll-interface class 'Poco::Exception'
	#pragma warning(disable:4344)	// behavior change: use of explicit template arguments results in call to '...' but '...' is a better match
	#pragma warning(disable:4351)	// new behavior: elements of array '...' will be default initialized
	#pragma warning(disable:4355)	// 'this' : used in base member initializer list
	#pragma warning(disable:4675)	// resolved overload was found by argument-dependent lookup
#endif


#if defined(__INTEL_COMPILER)
	#pragma warning(disable:1738) // base class dllexport/dllimport specification differs from that of the derived class
	#pragma warning(disable:1478) // function ... was declared "deprecated"
	#pragma warning(disable:1744) // field of class type without a DLL interface used in a class with a DLL interface
#endif


#endif // Foundation_Platform_WIN32_INCLUDED
