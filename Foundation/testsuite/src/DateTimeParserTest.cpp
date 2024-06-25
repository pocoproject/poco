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

using namespace std::string_literals;

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12.30:00Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-00-08T12:30:00Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-00T12:30:00Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-00T33:30:00Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-00T12:80:00Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-00T12:30:90Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-0012:30:90Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-00X12:30:90Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "200501-00T12:30:90Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-0100T12:30:90Z", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005_01+00T12:30:90Z", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00+01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-8T12:30:00+01:00", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00+41:00", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00+01:70", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00-01:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-0812:30:00-01:00", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08T12:30:00", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:0", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-8", tzd);
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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-1-08T12:30:00Z", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08Z12:30:00+01:00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00-01.00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12;30:00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01+08", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.1J", tzd);
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.Z", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.123*01:00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01+08T12:30:00.12345-01:00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "201009-23T16:17:01.2817002+02:00", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:0", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "005-01-08T12:30:00.123456", tzd);

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
	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-0", tzd);

	testBad(DateTimeFormat::ISO8601_FRAC_FORMAT, "jnghjgnbcfjb", tzd);

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
	testBad(DateTimeFormat::RFC822_FORMAT, "at, 8 Jan 05 12:30:00 GMT", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::RFC822_FORMAT, "Sat, x Jan 05 12:30:00 +0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::RFC822_FORMAT, "Sat, 8 Jan 05 12:30:00 *0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Tue, 18 Jan 05 12:30:00 CET", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 18);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::RFC822_FORMAT, "Tue, 18 Jan 05 12:30:00 abc", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "Wed, 12 Sep 73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);
	testBad(DateTimeFormat::RFC822_FORMAT, "Wed, 12 Sep 73 02:01:2 CST", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC822_FORMAT, "12 Sep 73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);

	testBad(DateTimeFormat::RFC822_FORMAT, "12 Sep 73 02.01:12 EST", tzd);
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
	testBad(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 GPX", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 xy:30:00 +0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 2005 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Sat, 8 Jan 205 12:30:00 -0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Sun, 20 Jul 1969 16:17:30 EDT", tzd);
	assertTrue (dt.year() == 1969);
	assertTrue (dt.month() == 7);
	assertTrue (dt.day() == 20);
	assertTrue (dt.hour() == 16);
	assertTrue (dt.minute() == 17);
	assertTrue (dt.second() == 30);
	assertTrue (tzd == -14400);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Hue, 18 Jan 2005 12:30:00 EDT", tzd);

	testBad(DateTimeFormat::RFC1123_FORMAT, "Sun, 20 Jul 1969 16:17:30 GMT+01:00", tzd);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Sun, 20 Jul 1969 16:17:30 GMT+01?00", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Tue, 18 Jan 2005 12:30:00 CDT", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 18);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -18000);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Hue, 18 Jan 2005 12:30:00 CDT", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "Wed, 12 Sep 1973 02:01:12 CST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == -21600);
	testBad(DateTimeFormat::RFC1123_FORMAT, "Wed, 12 Sp 1973 02:01:12 CST", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1123_FORMAT, "12 Sep 1973 02:01:12 EST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == -18000);
	testBad(DateTimeFormat::RFC1123_FORMAT, "12 Sep 193 02:01:12 EST", tzd);
	testBad(DateTimeFormat::RFC1123_FORMAT, "12 Sep 1973 02:01:12 ABC", tzd);
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
	testBad(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jn 2005 12:30:00 GMT", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 j0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::HTTP_FORMAT, "Sat, 08 Jan 2005 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::HTTP_FORMAT, "Sat 08 Jan 2005 12:30:00 -0100", tzd);
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
	testBad(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan 05 12:30:00 GMT", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan-05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::RFC850_FORMAT, "Saturday, 8+Jan-05 12:30:00 +0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Saturday, 8-Jan-05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::RFC850_FORMAT, "Saturday 8-Jan-05 12:30:00 -0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC850_FORMAT, "Wed, 12-Sep-73 02:01:12 CEST", tzd);
	assertTrue (dt.year() == 1973);
	assertTrue (dt.month() == 9);
	assertTrue (dt.day() == 12);
	assertTrue (dt.hour() == 2);
	assertTrue (dt.minute() == 1);
	assertTrue (dt.second() == 12);
	assertTrue (tzd == 7200);
	testBad(DateTimeFormat::RFC850_FORMAT, "Wed, 12-pep-73 02:01:12 CST", tzd);
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
	testBad(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 0512:30:00 GMT", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 +0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 3600);
	testBad(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 051 12:30:00 +0100", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 -0100", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 12);
	assertTrue (dt.minute() == 30);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == -3600);
	testBad(DateTimeFormat::RFC1036_FORMAT, "Saturday, 8 Jan 05 12:30:00 -0100x", tzd);
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
	testBad(DateTimeFormat::ASCTIME_FORMAT, "Bat Jan  8 12:30:00 2005", tzd);
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
	testBad(DateTimeFormat::SORTABLE_FORMAT, "2005-01-08 12:30;00", tzd);

	dt = DateTimeParser::parse(DateTimeFormat::SORTABLE_FORMAT, "2005-01-08", tzd);
	assertTrue (dt.year() == 2005);
	assertTrue (dt.month() == 1);
	assertTrue (dt.day() == 8);
	assertTrue (dt.hour() == 0);
	assertTrue (dt.minute() == 0);
	assertTrue (dt.second() == 0);
	assertTrue (tzd == 0);
	testBad(DateTimeFormat::SORTABLE_FORMAT, "2005+01-08", tzd);
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

	// bad year (not a number)
	testBad("%y", "YY", tzd);

	// bad year (number too big)
	testBad("%r", "123456789101112131415", tzd);

	// check that an invalid millisecond is detected with a custom format
	testBad("T%H:%M:%s %z", "T12:30:00.Z", tzd);
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

void DateTimeParserTest::testCleanup()
{
	int tzd;

	DateTime dt = DateTimeParser::parse("2005-01-08T12:30:00Z", tzd);
	DateTime dt2 = DateTimeParser::parse(" 	2005-01-08T12:30:00Z ", tzd);

	assertTrue (dt == dt2);

	assertTrue(DateTimeParser::tryParse("		2005-01-08T12:30:00Z  ", dt, tzd));

	dt = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.1Z", tzd);
	dt2 = DateTimeParser::parse(DateTimeFormat::ISO8601_FRAC_FORMAT, "2005-01-08T12:30:00.1Z	", tzd);

	assertTrue (dt == dt2);

	assertTrue(DateTimeParser::tryParse(DateTimeFormat::ISO8601_FRAC_FORMAT, "		2005-01-08T12:30:00Z  ", dt, tzd));

	assertFalse(DateTimeParser::tryParse(DateTimeFormat::ISO8601_FRAC_FORMAT, "  		", dt, tzd));

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
	catch (const SyntaxException&)
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
	catch (const SyntaxException&)
	{
	}
}


void DateTimeParserTest::testBad(const std::string& fmt, const std::string& dateStr, int tzd)
{
	try
	{
		DateTime dt;
		DateTimeParser::parse(fmt, dateStr, dt, tzd);
		fail ("must fail: "s + fmt + ", " + dateStr);
	}
	catch(const Poco::Exception&) { }
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
	CppUnit_addTest(pSuite, DateTimeParserTest, testCleanup);
	CppUnit_addTest(pSuite, DateTimeParserTest, testParseMonth);
	CppUnit_addTest(pSuite, DateTimeParserTest, testParseDayOfWeek);

	return pSuite;
}
