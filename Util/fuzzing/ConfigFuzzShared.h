//
// ConfigFuzzShared.h
//
// Helpers shared by the Util configuration fuzz targets (properties, INI
// and XML): key enumeration and the typed getters per key.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_fuzzing_ConfigFuzzShared_INCLUDED
#define Util_fuzzing_ConfigFuzzShared_INCLUDED


#include "Poco/Util/AbstractConfiguration.h"

#include <cstddef>
#include <string>
#include <vector>


inline void queryOne(Poco::Util::AbstractConfiguration* cfg, const std::string& key)
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


inline void collectKeys(Poco::Util::AbstractConfiguration* cfg, const std::string& prefix,
                        std::size_t maxKeys, std::vector<std::string>& fullKeys)
	/// Enumerates full keys recursively, at most maxKeys of them.  A key
	/// that is empty or begins with '.' yields an empty full key at the
	/// root again; recursing into it would fill the budget with
	/// repetitions, so it is not followed.
{
	if (fullKeys.size() >= maxKeys) return;

	Poco::Util::AbstractConfiguration::Keys children;
	if (prefix.empty())
		cfg->keys(children);
	else
		cfg->keys(prefix, children);

	for (const auto& child : children)
	{
		if (fullKeys.size() >= maxKeys) return;
		const std::string full = prefix.empty() ? child : prefix + "." + child;
		fullKeys.push_back(full);
		if (!full.empty())
			collectKeys(cfg, full, maxKeys, fullKeys);
	}
}


template <typename QueryFn>
inline void queryConfig(Poco::Util::AbstractConfiguration* cfg, std::size_t maxKeys, QueryFn queryKey)
	/// Enumerates the keys and queries each full key, plus a view of the
	/// parent with the last component of the key.
{
	std::vector<std::string> fullKeys;
	collectKeys(cfg, std::string(), maxKeys, fullKeys);

	for (const auto& key : fullKeys)
	{
		queryKey(cfg, key);

		const std::size_t dot = key.rfind('.');
		if (dot != std::string::npos)
		{
			const std::string parent = key.substr(0, dot);
			const std::string last = key.substr(dot + 1);
			Poco::Util::AbstractConfiguration::Ptr view = cfg->createView(parent);
			queryKey(view, last);
		}
	}
}


#endif // Util_fuzzing_ConfigFuzzShared_INCLUDED