//
// DateTimeParserTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DateTimeParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"


using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeParser;
using Poco::Timestamp;
using Poco::SyntaxException;


DateTimeParserTest::DateTimeParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


DateTimeParserTest::~DateTimeParserTest()
{
}


void DateTimeParserTest::testISO8601()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00+01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00-01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
}


void DateTimeParserTest::testISO8601Frac()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00+01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00-01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.1Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 100);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.123+01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.12345-01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 450);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2010-09-23T16:17:01.2817002+02:00", tzd);
	assertTrue (dt.year() == 2010);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 23);
	assertTrue (dt.hour() == 16);
	assertTrue (dt.minute() == 17);
	assertTrue (dt.second() == 1);
	assertTrue (dt.millisecond() == 281);
	assertTrue (dt.microsecond() == 700);
	assertTrue (tzd == 7200);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.123456", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 456);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (dt.millisecond() == 0);
	assertTrue (dt.microsecond() == 0);
	assertTrue (tzd == 0);
}


void DateTimeParserTest::testRFC822()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 GMT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Tue, 18 Jan 05 12:30:00 CET", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 18);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Wed, 12 Sep 73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "12 Sep 73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);
}


void DateTimeParserTest::testRFC1123()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 GMT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sun, 20 Jul 1969 16:17:30 EDT", tzd);
	assertTrue (dt.year() == 1969);
	assertTrue (dt.month() == 7);
	assertTrue (dt.day() == 20);
	assertTrue (dt.hour() == 16);
	assertTrue (dt.minute() == 17);
	assertTrue (dt.second() == 30);
	assertTrue (tzd == -14400);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sun, 20 Jul 1969 16:17:30 GMT+01:00", tzd);
	assertTrue (dt.year() == 1969);
	assertTrue (dt.month() == 7);
	assertTrue (dt.day() == 20);
	assertTrue (dt.hour() == 16);
	assertTrue (dt.minute() == 17);
	assertTrue (dt.second() == 30);
	assertTrue (tzd == 3600);
}


void DateTimeParserTest::testHTTP()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 GMT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
}


void DateTimeParserTest::testRFC850()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan-05 12:30:00 GMT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan-05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan-05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Wed, 12-Sep-73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);
}


void DateTimeParserTest::testRFC1036()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 GMT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
}


void DateTimeParserTest::testASCTIME()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::ASCTIME_FORMAT, "Sat Jan  8 12:30:00 2005", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
}


void DateTimeParserTest::testSORTABLE()
{
	int tzd;
	DateTime dt = DateTimeParser::parse(DateTimeFormat::SORTABLE_FORMAT, "2005-01-08 12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse(DateTimeFormat::SORTABLE_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
}


void DateTimeParserTest::testCustom()
{
	int tzd;
	DateTime dt = DateTimeParser::parse("%d-%b-%Y", "18-Jan-2005", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 18);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("%m/%d/%y", "01/18/05", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 18);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("%h:%M %a", "12:30 am", tzd);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);

	dt = DateTimeParser::parse("%h:%M %a", "12:30 PM", tzd);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);

	assertTrue (!DateTimeParser::tryParse("%h:%M %a", "", dt, tzd));
	assertTrue (!DateTimeParser::tryParse("", "12:30 PM", dt, tzd));
	assertTrue (!DateTimeParser::tryParse("", "", dt, tzd));

	try
	{
		DateTimeParser::parse("%h:%M %a", "", tzd);
		fail ("must fail");
	}
	catch (SyntaxException&)
	{
	}

	try
	{
		DateTimeParser::parse("", "12:30 PM", tzd);
		fail ("must fail");
	}
	catch (SyntaxException&)
	{
	}

	try
	{
		DateTimeParser::parse("", "", tzd);
		fail ("must fail");
	}
	catch (SyntaxException&)
	{
	}
}


void DateTimeParserTest::testGuess()
{
	int tzd;
	DateTime dt = DateTimeParser::parse("2005-01-08T12:30:00Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("20050108T123000Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("2005-01-08T12:30:00+01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse("2005-01-08T12:30:00.123456Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 456);

	dt = DateTimeParser::parse("2005-01-08T12:30:00,123456Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 456);

	dt = DateTimeParser::parse("20050108T123000,123456Z", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 456);

	dt = DateTimeParser::parse("20050108T123000.123+0200", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 7200);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 0);


	dt = DateTimeParser::parse("2005-01-08T12:30:00.123456-02:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -7200);
	assertTrue (dt.millisecond() == 123);
	assertTrue (dt.microsecond() == 456);

	dt = DateTimeParser::parse("Sat, 8 Jan 05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse("Sat, 8 Jan 2005 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);

	dt = DateTimeParser::parse("Sat Jan  8 12:30:00 2005", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("2005-01-08 12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);

	dt = DateTimeParser::parse("2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
}


void DateTimeParserTest::testParseMonth()
{
	std::string str = "January";
	std::string::const_iterator it = str.begin();
	int month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 1);
	str = "February";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 2);
	str = "December";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 12);
	str = "Jan";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 1);
	str = "Feb";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 2);
	str = "jan";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 1);
	str = "feb";
	it = str.begin();
	month = DateTimeParser::parseMonth(it, str.end());
	assertTrue (month == 2);

	try
	{
		str = "ja";
		it = str.begin();
		month = DateTimeParser::parseMonth(it, str.end());
		fail("Not a valid month name - must throw");
	}
	catch (SyntaxException&)
	{
	}
}


void DateTimeParserTest::testParseDayOfWeek()
{
	std::string str = "Sunday";
	std::string::const_iterator it = str.begin();
	int dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 0);
	str = "Monday";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 1);
	str = "Saturday";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 6);
	str = "Sun";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 0);
	str = "Mon";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 1);
	str = "sun";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 0);
	str = "mon";
	it = str.begin();
	dow = DateTimeParser::parseDayOfWeek(it, str.end());
	assertTrue (dow == 1);

	try
	{
		str = "su";
		it = str.begin();
		dow = DateTimeParser::parseDayOfWeek(it, str.end());
		fail("Not a valid weekday name - must throw");
	}
	catch (SyntaxException&)
	{
	}
}


void DateTimeParserTest::setUp()
{
}


void DateTimeParserTest::tearDown()
{
}


CppUnit::Test* DateTimeParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DateTimeParserTest");

	CppUnit_addTest(pSuite, DateTimeParserTest, testISO8601);
	CppUnit_addTest(pSuite, DateTimeParserTest, testISO8601Frac);
	CppUnit_addTest(pSuite, DateTimeParserTest, testRFC822);
	CppUnit_addTest(pSuite, DateTimeParserTest, testRFC1123);
	CppUnit_addTest(pSuite, DateTimeParserTest, testHTTP);
	CppUnit_addTest(pSuite, DateTimeParserTest, testRFC850);
	CppUnit_addTest(pSuite, DateTimeParserTest, testRFC1036);
	CppUnit_addTest(pSuite, DateTimeParserTest, testASCTIME);
	CppUnit_addTest(pSuite, DateTimeParserTest, testSORTABLE);
	CppUnit_addTest(pSuite, DateTimeParserTest, testCustom);
	CppUnit_addTest(pSuite, DateTimeParserTest, testGuess);
	CppUnit_addTest(pSuite, DateTimeParserTest, testParseMonth);
	CppUnit_addTest(pSuite, DateTimeParserTest, testParseDayOfWeek);

	return pSuite;
}
