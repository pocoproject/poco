//
// SystemConfiguration.h
//
// Library: Util
// Package: Configuration
// Module:  SystemConfiguration
//
// Definition of the SystemConfiguration class.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_SystemConfiguration_INCLUDED
#define Util_SystemConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"
#include <map>
#include <functional>

namespace Poco {
namespace Util {


class Util_API SystemConfiguration: public AbstractConfiguration
	/// This class implements a Configuration interface to
	/// various system properties and environment variables.
	///
	/// The following properties are supported:
	///   - system.osName: the operating system name
	///   - system.osVersion: the operating system version
	///   - system.osArchitecture: the operating system architecture
	///   - system.nodeName: the node (or host) name
	///   - system.nodeId: system ID, based on the Ethernet address (format "xxxxxxxxxxxx")
	///     of the first Ethernet adapter found on the system.
	///   - system.currentDir: the current working directory
	///   - system.homeDir: the user's home directory
	///   - system.configHomeDir: the base directory relative to which user specific configuration files should be stored
	///   - system.cacheHomeDir: the base directory relative to which user specific non-essential data files should be stored
	///   - system.dataHomeDir: the base directory relative to which user specific data files should be stored
	///   - system.tempHomeDir: the base directory relative to which user-specific temporary files and other file objects should be placed
	///   - system.tempDir: the system's temporary directory
	///   - system.configDir: the system's configuration directory
	///   - system.dateTime: the current UTC date and time, formatted in ISO 8601 format.
	///   - system.pid: the current process ID.
	///   - system.env.<NAME>: the environment variable with the given <NAME>.
	///
	/// An attempt to set a system variable will result in an
	/// InvalidAccessException being thrown.
	///
	/// Enumerating environment variables is not supported.
	/// An attempt to call keys("system.env") will return an empty range.
	///
	/// Removing key is not supported. An attempt to remove a key results
	/// in a NotImplementedException being thrown.
{
public:
	SystemConfiguration() = default;
		/// Creates the SystemConfiguration.

protected:
	bool getRaw(const std::string& key, std::string& value) const override;
	void setRaw(const std::string& key, const std::string& value) override;
	void enumerate(const std::string& key, Keys& range) const override;
	void removeRaw(const std::string& key) override;

	~SystemConfiguration() = default;

private:
	static bool getEnv(const std::string& name, std::string& value);
	static std::string getNodeId();

	using SystemProperty = std::function<std::string ()>;
	static std::map<std::string, SystemProperty> _functions;
};


} } // namespace Poco::Util


#endif // Util_SystemConfiguration_INCLUDED
