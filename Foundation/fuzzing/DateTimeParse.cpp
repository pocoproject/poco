#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"

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
	DateTime dt;

	for (const auto& format : formats)
	{
		DateTimeParser::tryParse(format, input, dt, tzd);
		DateTimeFormatter::format(dt.timestamp(), format, tzd);
	}

	dt.makeLocal(tzd);
	dt.makeUTC(tzd);

	try
	{
		dt = DateTimeParser::parse(input, tzd);
	}
	catch (const std::exception&)
	{
	}

	return 0;
}
