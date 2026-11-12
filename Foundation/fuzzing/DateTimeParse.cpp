#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include <cstddef>
#include <cstdint>
#include <string>


using namespace Poco;


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	const std::string input(reinterpret_cast<const char*>(data), size);

	const std::string formats[] = {
		DateTimeFormat::ISO8601_FORMAT,
		DateTimeFormat::ISO8601_FRAC_FORMAT,
		DateTimeFormat::RFC822_FORMAT,
		DateTimeFormat::RFC1123_FORMAT,
		DateTimeFormat::HTTP_FORMAT,
		DateTimeFormat::RFC850_FORMAT,
		DateTimeFormat::RFC1036_FORMAT,
		DateTimeFormat::ASCTIME_FORMAT,
		DateTimeFormat::SORTABLE_FORMAT,
		"%m/%d/%y %h:%M %a",
		"T%H:%M:%F",
	};

	int tzd = 0;
	// A fixed start value, so that a run does not depend on the clock.
	DateTime dt(Timestamp(0));

	for (const auto& format : formats)
	{
		(void) DateTimeParser::tryParse(format, input, dt, tzd);
		try
		{
			(void) DateTimeFormatter::format(dt.timestamp(), format, tzd);
		}
		catch (const InvalidArgumentException&)
		{
			// A parsed value whose UTC time is outside the range of DateTime.
		}
	}

	try { dt.makeLocal(tzd); } catch (const InvalidArgumentException&) {}
	try { dt.makeUTC(tzd); } catch (const InvalidArgumentException&) {}

	try
	{
		dt = DateTimeParser::parse(input, tzd);
	}
	catch (const SyntaxException&)
	{
	}

	return 0;
}
