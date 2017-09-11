//
// Format.h
//
// Library: Foundation
// Package: Core
// Module:  Format
//
// Definition of the format freestanding function.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Format_INCLUDED
#define Foundation_Format_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Any.h"
#include <vector>
#include <type_traits>


namespace Poco {


std::string Foundation_API format(const std::string& fmt, const Any& value);


void Foundation_API format(std::string& result, const char *fmt, const std::vector<Any>& values);
	/// Supports a variable number of arguments and is used by
	/// all other variants of format().

void Foundation_API format(std::string& result, const std::string& fmt, const std::vector<Any>& values);
	/// Supports a variable number of arguments and is used by
	/// all other variants of format().


template <typename T, typename... Args>
void format(std::string &result, const std::string &fmt, T arg1, Args... args)
	/// Appends the formatted string to result.
{
	std::vector<Any> values;
	values.push_back(arg1);
	values.insert(values.end(), { args... });
	format(result, fmt, values);
}


template <typename FMT, typename T, typename... Args,
	typename std::enable_if< std::is_const< typename std::remove_reference<FMT>::type >::value, int >::type = 0>
std::string format(FMT &fmt, T arg1, Args... args)
	/// Returns the formatted string.
{
	std::vector<Any> values;
	values.push_back(arg1);
	values.insert(values.end(), { args... });
	std::string result;
	format(result, fmt, values);
	return result;
}


} // namespace Poco


#endif // Foundation_Format_INCLUDED
