//
// PropertiesFuzz.cpp
//
// Feeds a properties document to PropertyFileConfiguration: parse, query
// enumerated keys, save and reparse.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ConfigFuzzShared.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/PropertyFileConfiguration.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>


using namespace Poco::Util;

	constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;
	constexpr std::size_t MAX_KEYS = 64;


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string document(reinterpret_cast<const char*>(data), size);
	// Block !include / ?include before any parse so the harness never opens
	// paths from the input.
	if (document.find("include") != std::string::npos) return 0;

	std::istringstream in(document);
	Poco::AutoPtr<PropertyFileConfiguration> cfg = new PropertyFileConfiguration(in);
	queryConfig(cfg.get(), MAX_KEYS,
	            [](Poco::Util::AbstractConfiguration* c, const std::string& key) { queryOne(c, key); });

	std::ostringstream savedStream;
	cfg->save(savedStream);
	const std::string saved = savedStream.str();
	if (saved.find("include") != std::string::npos) return 0;

	std::istringstream back(saved);
	Poco::AutoPtr<PropertyFileConfiguration> again = new PropertyFileConfiguration(back);
	std::ostringstream againStream;
	again->save(againStream);
	if (againStream.str() != saved)
	{
		std::cerr << "properties save round trip mismatch\n";
		std::abort();
	}

	return 0;
}
