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


#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Types.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


using namespace Poco::Util;


namespace
{
	const std::size_t maxInputSize = 64 * 1024;
	const std::size_t maxKeys = 64;


	void queryOne(AbstractConfiguration* cfg, const std::string& key)
		/// Drives the typed getters for key. DataException covers conversion
		/// failures and circular ${} expansion; anything else is a finding.
	{
		try
		{
			(void) cfg->has(key);
			(void) cfg->getString(key, "dflt");
			(void) cfg->getInt(key, 0);
			(void) cfg->getDouble(key, 0.0);
			(void) cfg->getBool(key, false);
			(void) cfg->getInt64(key, 0);
		}
		catch (const Poco::DataException&)
		{
		}
	}


	void collectKeys(AbstractConfiguration* cfg, const std::string& prefix, std::vector<std::string>& fullKeys)
	{
		if (fullKeys.size() >= maxKeys) return;

		AbstractConfiguration::Keys children;
		if (prefix.empty())
			cfg->keys(children);
		else
			cfg->keys(prefix, children);

		for (const auto& child : children)
		{
			if (fullKeys.size() >= maxKeys) return;
			const std::string full = prefix.empty() ? child : prefix + "." + child;
			fullKeys.push_back(full);
			collectKeys(cfg, full, fullKeys);
		}
	}


	void queryConfig(AbstractConfiguration* cfg)
	{
		std::vector<std::string> fullKeys;
		collectKeys(cfg, std::string(), fullKeys);

		for (const auto& key : fullKeys)
		{
			queryOne(cfg, key);

			const std::size_t dot = key.rfind('.');
			if (dot != std::string::npos)
			{
				const std::string parent = key.substr(0, dot);
				const std::string last = key.substr(dot + 1);
				AbstractConfiguration::Ptr view = cfg->createView(parent);
				queryOne(view, last);
			}
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > maxInputSize) return 0;

	const std::string document(reinterpret_cast<const char*>(data), size);
	// Block !include / ?include before any parse so the harness never opens
	// paths from the input.
	if (document.find("include") != std::string::npos) return 0;

	std::istringstream in(document);
	Poco::AutoPtr<PropertyFileConfiguration> cfg = new PropertyFileConfiguration(in);
	queryConfig(cfg);

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
	queryConfig(again);

	return 0;
}
