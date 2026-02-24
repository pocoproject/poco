//
// Types.h
//
// Library: Foundation
// Package: Core
// Module:  Types
//
// Definitions of fixed-size integer types for various platforms
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Types_INCLUDED
#define Foundation_Types_INCLUDED


#include "Poco/Foundation.h"
#include <cstdint>
#include <type_traits>
#include <typeinfo>
#include <cstdlib>

#if defined(POCO_HAVE_CXXABI_H)
	#include <cxxabi.h>
#endif

namespace Poco {


using Int8    = std::conditional_t<std::is_same_v<signed char, std::int8_t>, std::int8_t, signed char>;
using UInt8   = std::uint8_t;
using Int16   = std::int16_t;
using UInt16  = std::uint16_t;
using Int32   = std::int32_t;
using UInt32  = std::uint32_t;
using Int64   = std::int64_t;
using UInt64  = std::uint64_t;
using IntPtr  = std::intptr_t;
using UIntPtr = std::uintptr_t;

#if defined(_MSC_VER)
	#if defined(_WIN64)
		#define POCO_PTR_IS_64_BIT 1
	#endif
	#define POCO_HAVE_INT64 1
#elif defined(__GNUC__) || defined(__clang__) || defined(__QNX__)
	#if defined(_WIN64)
		#define POCO_PTR_IS_64_BIT 1
	#else
		#if defined(__LP64__)
			#define POCO_PTR_IS_64_BIT 1
			#define POCO_LONG_IS_64_BIT 1
			#if POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_FREE_BSD || POCO_OS == POCO_OS_ANDROID || POCO_OS == POCO_OS_AIX || POCO_OS == POCO_OS_QNX || POCO_OS == POCO_OS_SOLARIS
				#define POCO_INT64_IS_LONG 1
			#endif
		#endif
	#endif
	#define POCO_HAVE_INT64 1
#elif defined(__SUNPRO_CC)
	#if defined(__sparcv9)
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
	#endif
	#define POCO_HAVE_INT64 1
#elif defined(__IBMCPP__)
	#if defined(__64BIT__)
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
	#endif
	#define POCO_HAVE_INT64 1
#elif defined(_DIAB_TOOL)
	#define POCO_HAVE_INT64 1
#endif


inline std::string Foundation_API demangle(const char* typeName)
	/// Returns a human-readable type name from a mangled type name.
	///
	/// Takes the output of typeid(T).name() and returns a demangled,
	/// human-readable type name.
	///
	/// On GCC/Clang (platforms with cxxabi.h), uses abi::__cxa_demangle().
	/// On MSVC, strips the type category prefix ("class ", "struct ",
	/// "enum ", "union ") that MSVC prepends to type names.
	/// Function types on MSVC are not prefixed; they use calling convention
	/// syntax instead (e.g., "void __cdecl(void)").
	///
	/// Returns the demangled name, or an error token on failure:
	///   - [ERRMEM]  - memory allocation failed
	///   - [ERRNAME] - invalid mangled name
	///   - [ERRARG]  - invalid argument
	///   - [ERRUNK]  - unknown error
{
	std::string result(typeName);
#ifdef POCO_HAVE_CXXABI_H
	int status;
	char* demangled = abi::__cxa_demangle(typeName, nullptr, nullptr, &status);
	if (demangled != nullptr)
	{
		if (status == 0) result = demangled;
		else
		{
			switch (status)
			{
				case -1: result = "[ERRMEM]";  break;
				case -2: result = "[ERRNAME]"; break;
				case -3: result = "[ERRARG]";  break;
				default: result = "[ERRUNK]";
			}
		}

		std::free(demangled);
	}
#elif defined(_MSC_VER)
	// MSVC prefixes types with "class ", "struct ", "enum ", or "union "
	static const char* prefixes[] = {"class ", "struct ", "enum ", "union "};
	for (const char* prefix : prefixes)
	{
		std::size_t len = std::strlen(prefix);
		if (result.compare(0, len, prefix) == 0)
		{
			result.erase(0, len);
			break;
		}
	}
#endif
	return result;
}


template <typename T>
std::string demangle()
	/// Returns a human-readable name for type T.
{
	return demangle(typeid(T).name());
}


template <typename T>
std::string demangle(const T& t)
	/// Returns a human-readable name for the type of instance t.
{
	return demangle(typeid(std::remove_const_t<std::remove_reference_t<decltype(t)>>).name());
}


inline std::string Foundation_API demangleDot(const std::string& name)
	/// Replaces :: with . in a demangled type name.
{
	std::string result;
	result.reserve(name.size());
	std::size_t i = 0;
	while (i < name.size())
	{
		if (name[i] == ':' && i + 1 < name.size() && name[i + 1] == ':')
		{
			result += '.';
			i += 2; // skip both ':' characters
		}
		else
		{
			result += name[i];
			++i;
		}
	}
	return result;
}


template <typename T>
std::string demangleDot()
	/// Returns demangled type name with :: replaced by .
{
	return demangleDot(demangle<T>());
}


template <typename T>
std::string demangleDot(const T& t)
	/// Returns demangled type name with :: replaced by .
{
	return demangleDot(demangle(t));
}


} // namespace Poco


#endif // Foundation_Types_INCLUDED
