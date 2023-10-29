#ifndef SQLPARSER_SQLPARSER_WIN_H
#define SQLPARSER_SQLPARSER_WIN_H


#if defined(_WIN32) || defined(_WIN64)
	#if defined(_MSC_VER)
		#define strncasecmp _strnicmp
		#define strcasecmp _stricmp
	#endif
	#if defined(SQLParser_EXPORTS)
		#define SQLParser_API __declspec(dllexport)
	#else
		#define SQLParser_API __declspec(dllimport)
	#endif
#endif


#endif  // SQLPARSER_SQLPARSER_WIN_H
