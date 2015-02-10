//
// String.h
//
// $Id: //poco/1.4/Foundation/src/String.cpp#1 $
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


namespace Poco {


std::string toJSON(char c)
{
	switch (c)
	{
	case '\\': return "\\\\";
	case '"': return "\\\"";
	case '/': return "\\/";
	case '\b': return "\\b";
	case '\f': return "\\f";
	case '\n': return "\\n";
	case '\r': return "\\r";
	case '\t': return "\\t";
	default: return std::string(1, c);
	}
}


void toJSON(const std::string& value, std::ostream& out, bool wrap)
{
	if (wrap) out << std::string(1, '"');
	for (std::string::const_iterator it = value.begin(),
		end = value.end(); it != end; ++it)
	{
		out << toJSON(*it);
	}
	if (wrap) out << std::string(1, '"');
}


std::string toJSON(const std::string& value, bool wrap)
{
	std::string ret;
	if (wrap) ret.append(1, '"');
	for (std::string::const_iterator it = value.begin(),
		end = value.end(); it != end; ++it)
	{
		ret.append(toJSON(*it));
	}
	if (wrap) ret.append(1, '"');
	return ret;
}


} // namespace Poco
