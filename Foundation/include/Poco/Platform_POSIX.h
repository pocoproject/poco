//
// Platform_POSIX.h
//
// Library: Foundation
// Package: Core
// Module:  Platform
//
// Platform and architecture identification macros
// and platform-specific definitions for various POSIX platforms
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Platform_POSIX_INCLUDED
#define Foundation_Platform_POSIX_INCLUDED


//
// PA-RISC based HP-UX platforms have some issues...
//
#if defined(hpux) || defined(_hpux)
	#if defined(__hppa) || defined(__hppa__)
		#define POCO_NO_SYS_SELECT_H 1
		#if defined(__HP_aCC)
			#define POCO_NO_TEMPLATE_ICOMPARE 1
		#endif
	#endif
#endif


//
// Thread-safety of local static initialization
//
#if __cplusplus >= 201103L || __GNUC__ >= 4 || defined(__clang__)
	#ifndef POCO_LOCAL_STATIC_INIT_IS_THREADSAFE
		#define POCO_LOCAL_STATIC_INIT_IS_THREADSAFE 1
	#endif
#endif


//
// No syslog.h on QNX/BB10
//
#if defined(__QNXNTO__)
	#define POCO_NO_SYSLOGCHANNEL
#endif


//
// C++14 support
//

// Enable C++14 support for AppleClang 503.x (Clang 3.4)
#if defined(__clang__) && defined(__apple_build_version__) && (__apple_build_version__ >= 5030038) && !defined(POCO_ENABLE_CPP14) && !defined(POCO_DISABLE_CPP14)
	#define POCO_ENABLE_CPP14
#endif

// Enable C++14 support for Clang 3.4
#if defined(__clang__) && !defined(__apple_build_version__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 4)) && !defined(POCO_ENABLE_CPP14) && !defined(POCO_DISABLE_CPP14)
	#define POCO_ENABLE_CPP14
#endif

// Enable C++14 support for GCC 4.9.2
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 9 || (__GNUC_MINOR__ == 9 && __GNUC_PATCHLEVEL__ >= 2)))) && !defined(POCO_ENABLE_CPP14) && !defined(POCO_DISABLE_CPP14)
	#define POCO_ENABLE_CPP14
#endif


#endif // Foundation_Platform_POSIX_INCLUDED
