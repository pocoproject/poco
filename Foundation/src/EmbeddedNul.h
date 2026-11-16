//
// EmbeddedNul.h
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Definition of the rejectEmbeddedNul() function.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_EmbeddedNul_INCLUDED
#define Foundation_EmbeddedNul_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include <string>
#include <string_view>


namespace Poco {


inline void rejectEmbeddedNul(std::string_view path)
	/// Throws a PathSyntaxException if path contains a NUL character.
	/// The operating system sees a path only up to the first NUL,
	/// so "a.txt\0.exe" would act on "a.txt". The message shows each NUL as "\0".
{
	using namespace std::string_literals;
	if (path.find('\0') != std::string_view::npos)
		throw PathSyntaxException("embedded NUL character", replace(std::string(path), "\0"s, "\\0"s));
}


} // namespace Poco


#endif // Foundation_EmbeddedNul_INCLUDED
