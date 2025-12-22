//
// JSONString.h
//
// Library: Foundation
// Package: Core
// Module:  String
//
// JSONString utility functions.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_JSONString_INCLUDED
#define Foundation_JSONString_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


enum JSONOptions
{
	JSON_PRESERVE_KEY_ORDER = 1,
		/// Applies to JSON::Object. If specified, the Object will
		/// preserve the items insertion order. Otherwise, items
		/// will be sorted by keys.
		///
		/// Has no effect on toJSON() function.

	JSON_ESCAPE_UNICODE = 2,
		/// If specified, when the object is stringified, all
		/// unicode characters will be escaped in the resulting
		/// string.

	JSON_WRAP_STRINGS = 4,
		/// If specified, the object will preserve the items
		/// insertion order. Otherwise, items will be sorted
		/// by keys.

	JSON_LOWERCASE_HEX = 8
		/// If specified, all encoding happens with lowercase
		/// HEX characters instead of capitals
};


void Foundation_API toJSON(const std::string& value, std::ostream& out, int options = Poco::JSON_WRAP_STRINGS);
	/// Formats string value into the supplied output stream by
	/// escaping control characters.
	/// If JSON_WRAP_STRINGS is in options, the resulting strings is enclosed in double quotes
	/// If JSON_ESCAPE_UNICODE is in options, all unicode characters will be escaped, otherwise
	/// only the compulsory ones.


std::string Foundation_API toJSON(const std::string& value, int options = Poco::JSON_WRAP_STRINGS);
	/// Formats string value by escaping control characters.
	/// If JSON_WRAP_STRINGS is in options, the resulting string is enclosed in double quotes
	/// If JSON_ESCAPE_UNICODE is in options, all unicode characters will be escaped, otherwise
	/// only the compulsory ones.
	///
	/// Returns formatted string.
	/// If escapeAllUnicode is true, all unicode characters will be escaped, otherwise only the compulsory ones.



} // namespace Poco


#endif // Foundation_JSONString_INCLUDED
