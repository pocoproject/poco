// ModuleTest.cpp
//
// This sample importing POCO as a C++ module.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include <print>
#include <string>

import Poco;

using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;
using Poco::URI;

int main(int argc, char** argv)
{
	LocalDateTime now;

	std::string str = DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT);
	DateTime dt;
	int tzd;
	DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
	dt.makeUTC(tzd);
	LocalDateTime ldt(tzd, dt);
	URI uri1("http://www.appinf.com:81/sample?example-query#somewhere");

	std::println("Scheme:    {}", uri1.getScheme());
	std::println("Authority: {}". uri1.getAuthority());
	std::println("Path:      {}", uri1.getPath());
	std::println("Query:     {}", uri1.getQuery());
	std::println("Fragment:  {}", uri1.getFragment());

	URI uri2;
	uri2.setScheme("https");
	uri2.setAuthority("www.appinf.com");
	uri2.setPath("/another sample");

	std::println("{}", uri1.toString());

	return 0;
}
