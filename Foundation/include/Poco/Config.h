//
// Config.h
//
// Library: Foundation
// Package: Core
// Module:  Foundation
//
// Feature configuration for the POCO libraries.
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Config_INCLUDED
#define Foundation_Config_INCLUDED


// Define to disable implicit linking
// #define POCO_NO_AUTOMATIC_LIBS


// Define to disable automatic initialization
// Defining this will disable ALL automatic
// initialization framework-wide (e.g. Net
// on Windows, all Data back-ends, etc).
//
// #define POCO_NO_AUTOMATIC_LIB_INIT


// Define to disable FPEnvironment support
// #define POCO_NO_FPENVIRONMENT


// Define if std::wstring is not available
// #define POCO_NO_WSTRING


// Define to disable shared memory
// #define POCO_NO_SHAREDMEMORY


// Define if no <locale> header is available
// #define POCO_NO_LOCALE


// Define to desired default thread stack size
// Zero means OS default
#ifndef POCO_THREAD_STACK_SIZE
	#define POCO_THREAD_STACK_SIZE 0
#endif

// Defined to desired max thread name length
#ifndef POCO_MAX_THREAD_NAME_LEN
#define POCO_MAX_THREAD_NAME_LEN 15
#endif


// Define to override system-provided
// minimum thread priority value on POSIX
// platforms (returned by Poco::Thread::getMinOSPriority()).
// #define POCO_THREAD_PRIORITY_MIN 0


// Define to override system-provided
// maximum thread priority value on POSIX
// platforms (returned by Poco::Thread::getMaxOSPriority()).
// #define POCO_THREAD_PRIORITY_MAX 31


// Define to disable small object optimization. If not
// defined, Any and Dynamic::Var (and similar optimization
// candidates) will be auto-allocated on the stack in
// cases when value holder fits into POCO_SMALL_OBJECT_SIZE
// (see below).
//
// #define POCO_NO_SOO


// Small object size in bytes. When assigned to Any or Var,
// objects larger than this value will be alocated on the heap,
// while those smaller will be placement new-ed into an
// internal stack-auto-allocated buffer.
#if !defined(POCO_SMALL_OBJECT_SIZE)
	#define POCO_SMALL_OBJECT_SIZE 64
#endif


// Define to disable compilation of DirectoryWatcher
// on platforms with no inotify.
// #define POCO_NO_INOTIFY

// Define to force the use of PollingDirectoryWatcher
// #define POCO_DW_FORCE_POLLING


// Following are options to remove certain features
// to reduce library/executable size for smaller
// embedded platforms. By enabling these options,
// the size of a statically executable can be
// reduced by a few 100 Kbytes.


// No automatic registration of FileChannel in
// LoggingFactory - avoids FileChannel and friends
// being linked to executable.
// #define POCO_NO_FILECHANNEL


// No automatic registration of SplitterChannel in
// LoggingFactory - avoids SplitterChannel being
// linked to executable.
// #define POCO_NO_SPLITTERCHANNEL


// No automatic registration of SyslogChannel in
// LoggingFactory - avoids SyslogChannel being
// linked to executable on Unix/Linux systems.
// #define POCO_NO_SYSLOGCHANNEL


// Define to enable MSVC secure warnings
// #define POCO_MSVC_SECURE_WARNINGS


// No support for INI file configurations in
// Poco::Util::Application.
// #define POCO_UTIL_NO_INIFILECONFIGURATION


// No support for JSON configuration in
// Poco::Util::Application. Avoids linking of JSON
// library and saves a few 100 Kbytes.
// #define POCO_UTIL_NO_JSONCONFIGURATION


// No support for XML configuration in
// Poco::Util::Application. Avoids linking of XML
// library and saves a few 100 Kbytes.
// #define POCO_UTIL_NO_XMLCONFIGURATION


// No IPv6 support
// Define to disable IPv6
// #define POCO_NET_NO_IPv6


// No UNIX socket support
// Define to disable unix sockets
// UNIX local sockets are default-enabled on
// all UNIX systems, on Windows if available
// See Net/SocketDefs.h
// See https://devblogs.microsoft.com/commandline/af_unix-comes-to-windows/
// #define POCO_NET_NO_UNIX_SOCKET


// Define to nonzero to enable move semantics
// on classes where it introduces a new state.
// For explanation, see:
// https://github.com/pocoproject/poco/wiki/Move-Semantics-in-POCO
#ifndef POCO_NEW_STATE_ON_MOVE
// #define POCO_NEW_STATE_ON_MOVE 1
#endif


// Enable the poco_debug_* and poco_trace_* macros
// even if the _DEBUG variable is not set.
// This allows the use of these macros in a release version.
// #define POCO_LOG_DEBUG


// Define to prevent changing the suffix for shared libraries
// to "d.so", "d.dll", etc. for _DEBUG builds in Poco::SharedLibrary.
// #define POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX

// Enable usage of Poco::Mutex and Poco::FastMutex
// as wrappers for std::recursive_mutex and std::mutex
#ifndef POCO_ENABLE_STD_MUTEX
//	#define POCO_ENABLE_STD_MUTEX
#endif

#ifndef POCO_HAVE_SENDFILE
//	#define POCO_HAVE_SENDFILE
#endif

#define POCO_HAVE_CPP17_COMPILER (__cplusplus >= 201703L)
#define POCO_HAVE_CPP20_COMPILER (__cplusplus >= 202002L)
#define POCO_HAVE_CPP23_COMPILER (__cplusplus >= 202302L)

#if (POCO_HAVE_CPP20_COMPILER)
#include <version>
#if defined(__cpp_lib_jthread)
	#define POCO_HAVE_JTHREAD true
#else
	#define POCO_HAVE_JTHREAD false
#endif
#endif

// Option to silence deprecation warnings.
#ifndef POCO_SILENCE_DEPRECATED
	#define POCO_DEPRECATED(reason) [[deprecated(reason)]]
#else
	#define POCO_DEPRECATED(reason)
#endif

// Uncomment to explicitly disable SQLParser
// #define POCO_DATA_NO_SQL_PARSER

// Uncomment to enable stack trace autogeneration in Exception
//#define POCO_ENABLE_TRACE 1

#endif // Foundation_Config_INCLUDED
