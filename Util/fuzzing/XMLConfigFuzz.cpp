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


#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Types.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/XMLConfiguration.h"
#include "Poco/XML/XMLException.h"

#include <cstdint>
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


	void queryWithXmlSuffixes(AbstractConfiguration* cfg, const std::string& key)
	{
		queryOne(cfg, key);
		queryOne(cfg, key + "[1]");
		queryOne(cfg, key + "[@id]");
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
			queryWithXmlSuffixes(cfg, key);

			const std::size_t dot = key.rfind('.');
			if (dot != std::string::npos)
			{
				const std::string parent = key.substr(0, dot);
				const std::string last = key.substr(dot + 1);
				AbstractConfiguration::Ptr view = cfg->createView(parent);
				queryWithXmlSuffixes(view, last);
			}
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > maxInputSize) return 0;

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

	queryConfig(cfg);

	return 0;
}
