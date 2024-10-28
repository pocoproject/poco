#include "Poco/JSON/Parser.h"

using namespace Poco;
using namespace Poco::JSON;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	std::string json(reinterpret_cast<const char*>(data), size);
	Parser parser;
	Dynamic::Var result;

	try
	{
		result = parser.parse(json);
	}
	catch (Exception& e)
	{
		return 0;
	}
	catch (const std::exception& e)
	{
		return 0;
	}

	return 0;
}
