#include "Poco/URI.h"
#include "Poco/Path.h"
#include "Poco/UUID.h"
#include "Poco/UTF8String.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>


using namespace Poco;


static void fuzzURI(FuzzedDataProvider& dataProvider)
{
	try
	{
		URI uri1(dataProvider.ConsumeRandomLengthString());
		URI uri2(uri1, dataProvider.ConsumeRandomLengthString());

		URI uri3(uri2);
		uri3.normalize();
		uri3.setQueryParameters(uri1.getQueryParameters());
		uri2 = uri3.toString();
	}
	catch (const std::exception&)
	{
	}
}


static void fuzzPath(FuzzedDataProvider& dataProvider)
{
	try
	{
		Path path1;
		(void) path1.tryParse(dataProvider.ConsumeRandomLengthString());

		Path path2(path1, Path(dataProvider.ConsumeRandomLengthString()));
		// The result must not depend on the working directory of the process.
		Path path3 = path1.absolute(Path("/fuzz/base/dir/"));
		Path path4 = Path::expand(dataProvider.ConsumeRandomLengthString());

		const auto style1 = dataProvider.ConsumeIntegralInRange<char>(Path::PATH_UNIX, Path::PATH_GUESS);
		const auto style2 = dataProvider.ConsumeIntegralInRange<char>(Path::PATH_UNIX, Path::PATH_GUESS);
		path3.assign(path4.toString(static_cast<Path::Style>(style1)), static_cast<Path::Style>(style2));
	}
	catch (const std::exception&)
	{
	}
}


static void fuzzUUID(FuzzedDataProvider& dataProvider)
{
	try
	{
		UUID uuid1(dataProvider.ConsumeRandomLengthString());
		UUID uuid2(uuid1.toString());
		// The text form of an accepted UUID parses back to the same UUID.
		if (uuid2 != uuid1)
		{
			std::cerr << "The text form of a UUID does not parse back to the same UUID\n";
			std::abort();
		}
		(void) uuid2.tryParse(dataProvider.ConsumeRandomLengthString());
	}
	catch (const std::exception&)
	{
	}
}


static void fuzzUTF8String(FuzzedDataProvider& dataProvider)
{
	try
	{
		auto str1 = UTF8::unescape(dataProvider.ConsumeRandomLengthString());
		auto str2 = UTF8::escape(str1);

		UTF8::toUpperInPlace(str1);
		UTF8::toLowerInPlace(str2);
	}
	catch (const std::exception&)
	{
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider dataProvider(data, size);

	// Every value of the selector runs one of the four functions.
	switch (dataProvider.ConsumeIntegralInRange<int>(0, 3))
	{
	case 0:
		fuzzURI(dataProvider);
		break;
	case 1:
		fuzzPath(dataProvider);
		break;
	case 2:
		fuzzUUID(dataProvider);
		break;
	case 3:
		fuzzUTF8String(dataProvider);
		break;
	default:
		break;
	}

	return 0;
}
