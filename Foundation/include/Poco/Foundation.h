//
// Foundation.h
//
// Library: Foundation
// Package: Core
// Module:  Foundation
//
// Basic definitions for the POCO Foundation library.
// This file must be the first file included by every other Foundation
// header file.
//
// Copyright (c) 2004-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Foundation_INCLUDED
#define Foundation_Foundation_INCLUDED


//
// Include library configuration
//
#include "Poco/Config.h"


//
// Ensure that POCO_DLL is default unless POCO_STATIC is defined
//
#if defined(_WIN32) && defined(_DLL)
	#if !defined(POCO_DLL) && !defined(POCO_STATIC)
		#define POCO_DLL
	#endif
#endif


//
// POCO_API(exports) is the attribute of the classes and functions that a shared
// library exports. Its argument is a symbol that is defined (empty or as 1) on the
// command line only while the library itself is compiled, and in no project that
// uses the library: the library then exports the symbols, and all other code
// imports them. Every library defines its own macro with it:
//
//     #define Net_API POCO_API(Net_EXPORTS)
//
#if defined(_WIN32) && defined(POCO_DLL)
	// A macro definition cannot contain #if, so POCO_API decides by token pasting.
	// POCO_API_EXPORTING_ is pasted with the expanded symbol and put in front of the list
	// "__declspec(dllexport), __declspec(dllimport)"; POCO_API_SECOND returns the
	// second element:
	//
	//   symbol defined as 1:  POCO_API_EXPORTING_1 expands to "~," (~ is a placeholder),
	//                         so __declspec(dllexport) is second
	//   symbol defined empty: the same with POCO_API_EXPORTING_
	//   symbol not defined:   POCO_API_EXPORTING_<symbol> is not a macro and stays in the
	//                         first element, so __declspec(dllimport) is second
	#define POCO_API(exports) POCO_API_PASTE(POCO_API_EXPORTING, exports)

	// ## does not expand its operands, so POCO_API expands the symbol and this macro
	// pastes it. The prefix is a parameter of its own, so that an empty symbol does not
	// result in a call with one empty argument (warning C4003 of Visual C++).
	#define POCO_API_PASTE(prefix, value) \
		POCO_API_EXPAND(POCO_API_SECOND, (prefix##_##value __declspec(dllexport), __declspec(dllimport)))

	// Arguments are separated before they are expanded, so the comma from
	// POCO_API_EXPORTING_1 counts only if the list is expanded before the call:
	// POCO_API_EXPAND expands it as its own argument and then forms the call. Passing
	// the list through one more macro parameter would expand it too, but the
	// traditional preprocessor of Visual C++ then keeps it as one argument.
	#define POCO_API_EXPAND(macro, args) macro args

	#define POCO_API_EXPORTING_ ~,
	#define POCO_API_EXPORTING_1 ~,
	#define POCO_API_SECOND(first, second, ...) second
#elif !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__)
	#define POCO_API(exports) __attribute__ ((visibility ("default")))
#else
	#define POCO_API(exports)
#endif


#define Foundation_API POCO_API(Foundation_EXPORTS)


//
// Automatically link Foundation library.
//
#if defined(_MSC_VER)
	#if defined(POCO_DLL)
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "d.lib"
		#else
			#define POCO_LIB_SUFFIX ".lib"
		#endif
	#elif defined(_DLL)
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "mdd.lib"
		#else
			#define POCO_LIB_SUFFIX "md.lib"
		#endif
	#else
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "mtd.lib"
		#else
			#define POCO_LIB_SUFFIX "mt.lib"
		#endif
	#endif

	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Foundation_EXPORTS)
		#pragma comment(lib, "PocoFoundation" POCO_LIB_SUFFIX)
	#endif
#endif

#include <string>

namespace Poco {

using namespace std::literals;

} // namespace Poco


//
// Include platform-specific definitions
//
#include "Poco/Platform.h"
#if defined(_WIN32)
	#include "Poco/Platform_WIN32.h"
#elif defined(POCO_VXWORKS)
	#include "Poco/Platform_VX.h"
#elif defined(POCO_OS_FAMILY_UNIX)
	#include "Poco/Platform_POSIX.h"
#endif


//
// Guard for headers that use std::numeric_limits<>::min()/max().
//
// A function-like min() or max() macro breaks those headers, and the compiler
// diagnostic does not say why. Most often the macro comes from <windows.h>
// included without NOMINMAX -- Poco/UnWindows.h sets it, but a consumer that
// reaches <windows.h> before any POCO header never goes through it. Any other
// header, on any platform, can define the same macros. Affected headers invoke
// POCO_CHECK_MINMAX_MACROS once, below their includes, to report the cause.
//
// The condition is evaluated here, when this header is first processed, so the
// check reports a macro that is already in scope by then -- the case above. A
// macro defined after the first POCO include is not reported, but neither is it
// reachable via Poco/UnWindows.h, which defines NOMINMAX before <windows.h>.
//
#if defined(min) || defined(max)
	#define POCO_CHECK_MINMAX_MACROS \
		static_assert(false, \
			"A min() or max() macro is defined, which breaks this header. " \
			"Undefine it; on Windows, define NOMINMAX or include a POCO " \
			"header before <windows.h>.");
#else
	#define POCO_CHECK_MINMAX_MACROS
#endif


//
// Include alignment settings early
//
#include "Poco/Alignment.h"

//
// Cleanup inconsistencies
//
#ifdef POCO_OS_FAMILY_WINDOWS
	#if defined(POCO_NO_WSTRING)
		#error POCO_NO_WSTRING is not supported on Windows.
	#endif
#endif


//
// POCO_JOIN
//
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in POCO_DO_JOIN2 but does in POCO_DO_JOIN.
//
#define POCO_JOIN(X, Y) POCO_DO_JOIN(X, Y)
#define POCO_DO_JOIN(X, Y) POCO_DO_JOIN2(X, Y)
#define POCO_DO_JOIN2(X, Y) X##Y

//
// MS Visual Studio can use type long for __LINE__ macro
// when /ZI compilation flag is used - https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170#zi-1
// This breaks some poco interfaces, for ex. logger
// We should fix type for line number
namespace Poco {

using LineNumber = decltype(__LINE__);

} // namespace Poco

//
// Pull in basic definitions
//
#include "Poco/Bugcheck.h"
#include "Poco/Types.h"


#endif // Foundation_Foundation_INCLUDED
