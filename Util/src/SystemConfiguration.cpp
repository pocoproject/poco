//
// SystemConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  SystemConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


const std::string SystemConfiguration::OSNAME         = "system.osName";
const std::string SystemConfiguration::OSVERSION      = "system.osVersion";
const std::string SystemConfiguration::OSARCHITECTURE = "system.osArchitecture";
const std::string SystemConfiguration::NODENAME       = "system.nodeName";
const std::string SystemConfiguration::NODEID         = "system.nodeId";
const std::string SystemConfiguration::CURRENTDIR     = "system.currentDir";
const std::string SystemConfiguration::HOMEDIR        = "system.homeDir";
const std::string SystemConfiguration::CONFIGHOMEDIR  = "system.configHomeDir";
const std::string SystemConfiguration::CACHEHOMEDIR   = "system.cacheHomeDir";
const std::string SystemConfiguration::DATAHOMEDIR    = "system.dataHomeDir";
const std::string SystemConfiguration::TEMPHOMEDIR    = "system.tempHomeDir";
const std::string SystemConfiguration::TEMPDIR        = "system.tempDir";
const std::string SystemConfiguration::CONFIGDIR      = "system.configDir";
const std::string SystemConfiguration::DATETIME       = "system.dateTime";
#if !defined(POCO_VXWORKS)
const std::string SystemConfiguration::PID            = "system.pid";
#endif
const std::string SystemConfiguration::ENV            = "system.env.";


SystemConfiguration::SystemConfiguration()
{
}


SystemConfiguration::~SystemConfiguration()
{
}


bool SystemConfiguration::getRaw(const std::string& key, std::string& value) const
{
	if (key == OSNAME)
	{
		value = Environment::osName();
	}
	else if (key == OSVERSION)
	{
		value = Environment::osVersion();
	}
	else if (key == OSARCHITECTURE)
	{
		value = Environment::osArchitecture();
	}
	else if (key == NODENAME)
	{
		value = Environment::nodeName();
	}
	else if (key == NODEID)
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
			value = result;
		}
		catch (...)
		{
			value = "000000000000";
		}
	}
	else if (key == CURRENTDIR)
	{
		value = Path::current();
	}
	else if (key == HOMEDIR)
	{
		value = Path::home();
	}
	else if (key == CONFIGHOMEDIR)
	{
		value = Path::configHome();
	}
	else if (key == CACHEHOMEDIR)
	{
		value = Path::cacheHome();
	}
	else if (key == DATAHOMEDIR)
	{
		value = Path::dataHome();
	}
	else if (key == TEMPHOMEDIR)
	{
		value = Path::tempHome();
	}
	else if (key == TEMPDIR)
	{
		value = Path::temp();
	}
	else if (key == CONFIGDIR)
	{
		value = Path::config();
	}
	else if (key == DATETIME)
	{
		value = Poco::DateTimeFormatter::format(Poco::DateTime(), Poco::DateTimeFormat::ISO8601_FORMAT);
	}
#if !defined(POCO_VXWORKS)
	else if (key == PID)
	{
		value = "0";
		value = Poco::NumberFormatter::format(Poco::Process::id());
	}
#endif
	else if (key.compare(0, ENV.size(), ENV) == 0)
	{
		return getEnv(key.substr(ENV.size()), value);
	}
	else return false;
	return true;
}


void SystemConfiguration::setRaw(const std::string& key, const std::string& value)
{
	throw Poco::InvalidAccessException("Attempt to modify a system property", key);
}


void SystemConfiguration::enumerate(const std::string& key, Keys& range) const
{
	if (key.empty())
	{
		range.push_back("system"s);
	}
	else if (key == "system")
	{
		range.push_back("osName"s);
		range.push_back("osVersion"s);
		range.push_back("osArchitecture"s);
		range.push_back("nodeName"s);
		range.push_back("nodeId"s);
		range.push_back("currentDir"s);
		range.push_back("homeDir"s);
		range.push_back("configHomeDir"s);
		range.push_back("cacheHomeDir"s);
		range.push_back("dataHomeDir"s);
		range.push_back("tempHomeDir"s);
		range.push_back("tempDir"s);
		range.push_back("configDir"s);
		range.push_back("dateTime"s);
#if !defined(POCO_VXWORKS)
		range.push_back("pid"s);
#endif
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


} } // namespace Poco::Util
