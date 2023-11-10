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


// Define if no <locale> header is available (such as on WinCE)
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


// Windows CE has no locale support
#if defined(_WIN32_WCE)
	#define POCO_NO_LOCALE
#endif


// Enable the poco_debug_* and poco_trace_* macros
// even if the _DEBUG variable is not set.
// This allows the use of these macros in a release version.
// #define POCO_LOG_DEBUG


// OpenSSL on Windows
//
// Poco has its own OpenSSL build system.
// See <https://github.com/pocoproject/openssl/blob/master/README.md>
// for details.
//
// These options are Windows only.
//
// To disable the use of Poco-provided OpenSSL binaries,
// define POCO_EXTERNAL_OPENSSL.
//
// Possible values:
//   POCO_EXTERNAL_OPENSSL_SLPRO:
//     Automatically link OpenSSL libraries from OpenSSL Windows installer provided
//     by Shining Light Productions <http://slproweb.com/products/Win32OpenSSL.html>
//     The (global) library search path must be set accordingly.
//   POCO_EXTERNAL_OPENSSL_DEFAULT:
//     Automatically link OpenSSL libraries from standard OpenSSL Windows build.
//     The (global) library search path must be set accordingly.
//   empty or other value:
//     Do not link any OpenSSL libraries automatically. You will have to edit the
//     Visual C++ project files for Crypto and NetSSL_OpenSSL.
#if !defined(POCO_EXTERNAL_OPENSSL) && defined(POCO_EXTERNAL_OPENSSL_SLPRO)
	#define POCO_EXTERNAL_OPENSSL POCO_EXTERNAL_OPENSSL_SLPRO
#endif


// Define to prevent changing the suffix for shared libraries
// to "d.so", "d.dll", etc. for _DEBUG builds in Poco::SharedLibrary.
// #define POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX


// Disarm POCO_DEPRECATED macro.
// #define POCO_NO_DEPRECATED

// Enable usage of Poco::Mutex and Poco::FastMutex
// as wrappers for std::recursive_mutex and std::mutex
#ifndef POCO_ENABLE_STD_MUTEX
//	#define POCO_ENABLE_STD_MUTEX
#endif

#define POCO_HAVE_CPP17_COMPILER (__cplusplus >= 201703L)

// Uncomment to explicitly disable SQLParser
// #define POCO_DATA_NO_SQL_PARSER

// Automatically disable SQL parser for < c++17 compile
#if !defined(POCO_DATA_NO_SQL_PARSER)
	#ifndef POCO_HAVE_CPP17_COMPILER
		#define POCO_DATA_NO_SQL_PARSER
	#endif
#endif

#endif // Foundation_Config_INCLUDED
