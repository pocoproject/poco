#ifndef SQLPARSER_SQLPARSER_WIN_H
#define SQLPARSER_SQLPARSER_WIN_H


#if (__cplusplus >= 201703L)
	#if __has_include("Poco/Data/Data.h")
		#include "Poco/Data/Data.h"
	#endif
#endif


#ifdef Data_API
	#define SQLParser_API Data_API
	#if defined(Data_EXPORTS) && !defined(SQLParser_EXPORTS)
		#define SQLParser_EXPORTS
	#endif
#else
	#if defined(_DLL) || defined(_USRDLL)
		#if defined(SQLParser_EXPORTS)
			#define SQLParser_API __declspec(dllexport)
		#else
			#define SQLParser_API __declspec(dllimport)
		#endif
	#endif
#endif


#if defined(_WIN32)
	#define strncasecmp _strnicmp
	#define strcasecmp _stricmp
#endif


#if !defined(SQLParser_API)
	#if defined (__GNUC__) && (__GNUC__ >= 4)
		#define SQLParser_API __attribute__ ((visibility ("default")))
	#else
		#define SQLParser_API
	#endif
#endif


#endif  // SQLPARSER_SQLPARSER_WIN_H
