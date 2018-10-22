//
// CppUnit.h
//


#ifndef Poco_CppUnit__CppUnitINCLUDED
#define Poco_CppUnit__CppUnitINCLUDED


#include "Poco/Platform.h"


//
// Ensure that POCO_DLL is default unless POCO_STATIC is defined
//
#if defined(_WIN32) && defined(_DLL)
	#if !defined(POCO_DLL) && !defined(POCO_STATIC)
		#define POCO_DLL
	#endif
#endif


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Poco_CppUnitEXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CppUnit_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(POCO_COMPILER_MSVC) && defined(POCO_DLL)
	#if defined(CppUnit_EXPORTS)
		#define CppUnit_API __declspec(dllexport)
	#else
		#define CppUnit_API __declspec(dllimport)
	#endif
#endif


#if !defined(CppUnit_API)
	#if defined (__GNUC__) && (__GNUC__ >= 4)
		#define CppUnit_API __attribute__ ((visibility ("default")))
	#else
		#define CppUnit_API
	#endif
#endif

//
// Automatically link CppUnit library.
//
#if defined(POCO_COMPILER_MSVC)
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

	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(CppUnit_EXPORTS)
		#pragma comment(lib, "PocoCppUnit" POCO_LIB_SUFFIX)
	#endif
#endif


// Turn off some annoying warnings
#ifdef POCO_COMPILER_MSVC
	#pragma warning(disable:4786)  // identifier truncation warning
	#pragma warning(disable:4503)  // decorated name length exceeded - mainly a problem with STLPort
	#pragma warning(disable:4018)  // signed/unsigned comparison
	#pragma warning(disable:4284)  // return type for operator -> is not UDT
	#pragma warning(disable:4251)  // ... needs to have dll-interface warning
	#pragma warning(disable:4273)
	#pragma warning(disable:4275)  // ... non dll-interface class used as base for dll-interface class
#endif


#endif // Poco_CppUnit__CppUnitINCLUDED
