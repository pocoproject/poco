//
// TypeId.h
//
// Library: Foundation
// Package: Core
// Module:  poco_typeid_name
//
// Definition of poco_typeid_name function.
//
// Copyright (c) 2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_TypeId_INCLUDED
#define Foundation_TypeId_INCLUDED


#if defined(POCO_COMPILER_GCC) || defined(POCO_COMPILER_MINGW) || POCO_OS == POCO_OS_CYGWIN


#include <cxxabi.h>
#include <string>


template <typename T>
const char* poco_typeid_name(bool full = true)
	/// Returns type name for the provided type.
	/// Names are demangled for g++ only at this time.
	/// If full is false, the scope is trimmed off.
{
	std::string name(typeid(T).name());
	int status = 0;
#if (POCO_OS == POCO_OS_CYGWIN)
	char* pName = __cxxabiv1::__cxa_demangle(typeid(T).name(), 0, 0, &status);
#else
	char* pName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
#endif
	if (status == 0) name = std::string(pName);
	free(pName);
	if (!full) // strip scope, if any
	{
		std::size_t pos = name.rfind("::");
		if (pos != std::string::npos) name = name.substr(pos+2);
	}

	return name.c_str();
} // TODO: demangle other compilers


#else


template <typename T>
const char* poco_typeid_name(bool full)
	/// Returns type name for the provided type.
	/// Names are demangled for g++ only at this time.
	/// If full is false, the scope is trimmed off.
{
	return typeid(c).name();
}


#endif


#endif // Foundation_TypeId_INCLUDED
