//
// estring.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/estring.h#1 $
//


#ifndef CppUnit_estring_INCLUDED
#define CppUnit_estring_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
#define STD_STRING_INCLUDED
#endif
#ifndef STD_STDIO_INCLUDED
#include <stdio.h>
#define STD_STDIO_INCLUDED
#endif


CppUnit_BEGIN


// Create a std::string from a const char pointer
inline std::string estring(const char *cstring)
{
	return std::string(cstring);
}


// Create a std::string from a std::string (for uniformities' sake)
inline std::string estring(std::string& expandedString)
{
	return expandedString;
}


// Create a std::string from an int
inline std::string estring(int number)
{
	char buffer[50]; 
	sprintf(buffer, "%d", number); 
	return std::string (buffer); 
}


// Create a string from a long
inline std::string estring(long number)
{
	char buffer[50]; 
	sprintf(buffer, "%ld", number); 
	return std::string (buffer); 
}


// Create a std::string from a double
inline std::string estring(double number)
{
	char buffer[50]; 
	sprintf(buffer, "%lf", number); 
	return std::string(buffer);
}


// Create a std::string from a double
inline std::string estring(const void* ptr)
{
	char buffer[50]; 
	sprintf(buffer, "%p", ptr); 
	return std::string(buffer);
}


CppUnit_END


#endif // CppUnit_estring_INCLUDED
