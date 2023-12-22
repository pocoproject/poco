#include "../src/JSONTest.h"


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
        std::string json(reinterpret_cast<const char*>(data), size);
        Poco::JSON::Parser parser;

        Poco::Dynamic::Var result;
        try
        {
                result = parser.parse(json);
        }
        catch(const std::exception& e)
        {
                return 0;
        }
        return 0;
}
