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
#include "Poco/UTF8String.h"
#include <ostream>

namespace Poco {

void toJSON(const std::string& value, std::ostream& out, bool wrap)
{
	if (wrap) out << '"';
	out << UTF8::escape(value.begin(), value.end());
	if (wrap) out << '"';
}


std::string toJSON(const std::string& value, bool wrap)
{
	std::string ret;
	if (wrap) ret.append(1, '"');
	ret.append(UTF8::escape(value.begin(), value.end()));
	if (wrap) ret.append(1, '"');
	return ret;
}


} // namespace Poco
