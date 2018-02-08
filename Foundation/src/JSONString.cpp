//
// String.h
//
// Library: Foundation
// Package: Core
// Module:  String
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/JSONString.h"
#include "Poco/UTF8String.h"
#include <ostream>

namespace Poco {

void toJSON(const std::string& value, std::ostream& out, bool wrap, bool escapeAllUnicode)
{
	if (wrap) out << '"';
	if (escapeAllUnicode)
	{
		out << UTF8::escape(value.begin(), value.end());
	}
	else
	{
		for (std::string::const_iterator it = value.begin(),
			end = value.end(); it != end; ++it)
		{
			// Forward slash isn't strictly required by JSON spec, but some parsers expect it
			if ((*it >= 0 && *it <= 31) ||  (*it == '"') || (*it == '\\') || (*it == '/'))
			{
				out << UTF8::escape(it, it+1);
			}
			else out << *it;
		}
	}
	if (wrap) out << '"';
}


std::string toJSON(const std::string& value, bool wrap, bool escapeAllUnicode)
{
	std::string ret;
	if (wrap) ret.append(1, '"');
	if (escapeAllUnicode)
	{
		ret.append(UTF8::escape(value.begin(), value.end()));
	}
	else
	{
		for (std::string::const_iterator it = value.begin(),
			end = value.end(); it != end; ++it)
		{
			// Forward slash isn't strictly required by JSON spec, but some parsers expect it
			if ((*it >= 0 && *it <= 31) ||  (*it == '"') || (*it == '\\') || (*it == '/'))
			{
				ret.append(UTF8::escape(it, it+1));
			}
			else ret.append(1, *it);
		}
	}
	if (wrap) ret.append(1, '"');
	return ret;
}


} // namespace Poco
