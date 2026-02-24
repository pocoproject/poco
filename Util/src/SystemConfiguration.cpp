//
// SystemConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  SystemConfiguration
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "Poco/Util/SystemConfiguration.h"
#include "Poco/Environment.h"
#include "Poco/Path.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/NumberFormatter.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#endif
#include "Poco/Exception.h"
#include <cstdio>


using Poco::Environment;
using Poco::Path;
using namespace std::string_literals;


namespace Poco {
namespace Util {


std::map<std::string, std::function<std::string ()>> SystemConfiguration::_functions = {
	  { "system.osName", Environment::osName }
	, { "system.osVersion", Environment::osVersion }
	, { "system.osArchitecture", Environment::osArchitecture }
	, { "system.nodeName", Environment::nodeName }
	, { "system.nodeId", getNodeId }
	, { "system.currentDir", Path::current }
	, { "system.homeDir", Path::home }
	, { "system.configHomeDir", Path::configHome }
	, { "system.cacheHomeDir", Path::cacheHome }
	, { "system.dataHomeDir", Path::dataHome }
	, { "system.tempHomeDir", Path::tempHome }
	, { "system.tempDir", Path::temp }
	, { "system.configDir", Path::config }
	, { "system.dateTime", []() { return Poco::DateTimeFormatter::format(Poco::DateTime(), Poco::DateTimeFormat::ISO8601_FORMAT);} }
#if !defined(POCO_VXWORKS)
	, { "system.pid", []() { return Poco::NumberFormatter::format(Poco::Process::id()); } }
#endif
};


namespace {
	const std::string ENV = "system.env.";
}


bool SystemConfiguration::getRaw(const std::string& key, std::string& value) const
{
	auto it = _functions.find(key);
	if (it != _functions.end())
	{
		value = it->second();
		return true;
	}
	else if (key.compare(0, ENV.size(), ENV) == 0)
	{
		return getEnv(key.substr(ENV.size()), value);
	}
	else return false;
}


void SystemConfiguration::setRaw(const std::string& key, const std::string& value)
{
	throw Poco::InvalidAccessException("Attempt to modify a system property", key);
}


void SystemConfiguration::enumerate(const std::string& key, Keys& range) const
{
	range.clear();
	if (key.empty())
	{
		range.push_back("system"s);
	}
	else if (key == "system")
	{
		std::size_t sz = key.size() + 1;
		for (auto& it: _functions)
		{
			range.push_back(it.first.substr(sz));
		}
		range.push_back("env"s);
	}
}


void SystemConfiguration::removeRaw(const std::string& key)
{
	throw Poco::NotImplementedException("Removing a key in a SystemConfiguration");
}


bool SystemConfiguration::getEnv(const std::string& name, std::string& value)
{
	if (Environment::has(name))
	{
		value = Environment::get(name);
		return true;
	}
	return false;
}


std::string SystemConfiguration::getNodeId()
{
    try
    {
		Poco::Environment::NodeId id;
		Poco::Environment::nodeId(id);
		char result[13];
		std::snprintf(result, sizeof(result), "%02x%02x%02x%02x%02x%02x",
			id[0],
			id[1],
			id[2],
			id[3],
			id[4],
			id[5]);
		return result;
	}
	catch (...)
	{
		return "000000000000";
	}
}

} } // namespace Poco::Util
