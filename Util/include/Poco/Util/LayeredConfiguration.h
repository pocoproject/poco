//
// LayeredConfiguration.h
//
// Library: Util
// Package: Configuration
// Module:  LayeredConfiguration
//
// Definition of the LayeredConfiguration class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_LayeredConfiguration_INCLUDED
#define Util_LayeredConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include <list>


namespace Poco {
namespace Util {


class Util_API LayeredConfiguration: public AbstractConfiguration
	/// A LayeredConfiguration consists of a number of AbstractConfigurations.
	///
	/// When reading a configuration property in a LayeredConfiguration,
	/// all added configurations are searched, in order of their priority.
	/// Configurations with lower priority values have precedence.
	///
	/// When setting a property, the property is always written to the first writeable
	/// configuration (see addWriteable()).
	/// If no writeable configuration has been added to the LayeredConfiguration, and an
	/// attempt is made to set a property, a RuntimeException is thrown.
	///
	/// Every configuration added to the LayeredConfiguration has a priority value.
	/// The priority determines the position where the configuration is inserted,
	/// with lower priority values coming before higher priority values.
	///
	/// If no priority is specified, a priority of 0 is assumed.
{
public:
	typedef Poco::AutoPtr<LayeredConfiguration> Ptr;

	LayeredConfiguration();
		/// Creates the LayeredConfiguration.

	void add(AbstractConfiguration::Ptr pConfig);
		/// Adds a read-only configuration to the back of the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void add(AbstractConfiguration::Ptr pConfig, const std::string& label);
		/// Adds a read-only configuration with the given label to the back of the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void add(AbstractConfiguration::Ptr pConfig, int priority);
		/// Adds a read-only configuration to the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void add(AbstractConfiguration::Ptr pConfig, const std::string& label, int priority);
		/// Adds a read-only configuration with the given label to the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void add(AbstractConfiguration::Ptr pConfig, int priority, bool writeable);
		/// Adds a configuration to the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void add(AbstractConfiguration::Ptr pConfig, const std::string& label, int priority, bool writeable);
		/// Adds a configuration with the given label to the LayeredConfiguration.
		/// The LayeredConfiguration takes shared ownership of the given configuration.

	void addWriteable(AbstractConfiguration::Ptr pConfig, int priority);
		/// Adds a writeable configuration to the LayeredConfiguration.
		/// The LayeredConfiguration does not take ownership of the given
		/// configuration. In other words, the configuration's reference
		/// count is incremented.

	AbstractConfiguration::Ptr find(const std::string& label) const;
		/// Finds and returns the configuration with the given label.
		///
		/// Returns null if no such configuration can be found.

	void removeConfiguration(AbstractConfiguration::Ptr pConfig);
		/// Removes the given configuration from the LayeredConfiguration.
		///
		/// Does nothing if the given configuration is not part of the
		/// LayeredConfiguration.

protected:
	struct ConfigItem
	{
		typedef AbstractConfiguration::Ptr ACPtr;
		ACPtr       pConfig;
		int         priority;
		bool        writeable;
		std::string label;
	};

	bool getRaw(const std::string& key, std::string& value) const;
	void setRaw(const std::string& key, const std::string& value);
	void enumerate(const std::string& key, Keys& range) const;
	void removeRaw(const std::string& key);
	
	int lowest() const;
	int highest() const;
	void insert(const ConfigItem& item);
	
	~LayeredConfiguration();

private:
	LayeredConfiguration(const LayeredConfiguration&);
	LayeredConfiguration& operator = (const LayeredConfiguration&);

	typedef std::list<ConfigItem> ConfigList;
	
	ConfigList _configs;
};


} } // namespace Poco::Util


#endif // Util_LayeredConfiguration_INCLUDED
