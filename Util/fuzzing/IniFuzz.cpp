//
// IniFuzz.cpp
//
// Feeds an INI document to IniFileConfiguration and queries the keys it
// enumerates.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConfigFuzzShared.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"

#include <cstddef>
#include <sstream>
#include <string>


using namespace Poco::Util;

	constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;
	constexpr std::size_t MAX_KEYS = 64;


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string document(reinterpret_cast<const char*>(data), size);

	std::istringstream in(document);
	Poco::AutoPtr<IniFileConfiguration> cfg = new IniFileConfiguration(in);
	queryConfig(cfg.get(), MAX_KEYS,
	            [](Poco::Util::AbstractConfiguration* c, const std::string& key) { queryOne(c, key); });

	return 0;
}
