// ModuleTest.cpp
//
// This sample importing POCO as a C++ module.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include <format>
#include <iostream>
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

	std::cout << std::format("Scheme:    {}\n", uri1.getScheme());
	std::cout << std::format("Authority: {}\n", uri1.getAuthority());
	std::cout << std::format("Path:      {}\n", uri1.getPath());
	std::cout << std::format("Query:     {}\n", uri1.getQuery());
	std::cout << std::format("Fragment:  {}\n", uri1.getFragment());

	URI uri2;
	uri2.setScheme("https");
	uri2.setAuthority("www.appinf.com");
	uri2.setPath("/another sample");

	std::cout << std::format("{}\n", uri1.toString());

	return 0;
}
