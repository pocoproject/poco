//
// XMLConfigFuzz.cpp
//
// Feeds an XML document to XMLConfiguration and queries the keys it
// enumerates, including fixed [1] and [@id] suffixes.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConfigFuzzShared.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/XMLConfiguration.h"
#include "Poco/XML/XMLException.h"

#include <cstddef>
#include <sstream>
#include <string>


using namespace Poco::Util;

	constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;
	constexpr std::size_t MAX_KEYS = 64;


namespace
{

	void queryWithXmlSuffixes(Poco::Util::AbstractConfiguration* cfg, const std::string& key)
	{
		queryOne(cfg, key);
		queryOne(cfg, key + "[1]");
		queryOne(cfg, key + "[@id]");
	}

}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string document(reinterpret_cast<const char*>(data), size);

	Poco::AutoPtr<XMLConfiguration> cfg;
	try
	{
		std::istringstream in(document);
		cfg = new XMLConfiguration(in);
	}
	catch (const Poco::XML::XMLException&)
	{
		return 0;
	}
	catch (const Poco::PoolOverflowException&)
	{
		return 0;
	}

	queryConfig(cfg.get(), MAX_KEYS,
	            [](Poco::Util::AbstractConfiguration* c, const std::string& key) { queryWithXmlSuffixes(c, key); });

	return 0;
}
