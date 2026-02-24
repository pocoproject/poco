//
// MapConfiguration.h
//
// Library: Util
// Package: Configuration
// Module:  MapConfiguration
//
// Definition of the MapConfiguration class.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_MapConfiguration_INCLUDED
#define Util_MapConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/AbstractConfiguration.h"
#include <map>


namespace Poco {
namespace Util {


class Util_API MapConfiguration: public AbstractConfiguration
	/// An implementation of AbstractConfiguration that stores configuration data in a map.
{
public:
	MapConfiguration() = default;
		/// Creates an empty MapConfiguration.

	~MapConfiguration() = default;
		/// Destroyes the MapConfiguration.

	void copyTo(AbstractConfiguration& config);
		/// Copies all configuration properties to the given configuration.

	void clear();
		/// Clears the configuration.

protected:
	using StringMap = std::map<std::string, std::string>;
	using iterator = StringMap::const_iterator;

	bool getRaw(const std::string& key, std::string& value) const override;
	void setRaw(const std::string& key, const std::string& value) override;
	void enumerate(const std::string& key, Keys& range) const override;
	void removeRaw(const std::string& key) override;

	iterator begin() const
	{
		return _map.begin();
	}

	iterator end() const
	{
		return _map.end();
	}

private:
	StringMap _map;
};


} } // namespace Poco::Util


#endif // Util_MapConfiguration_INCLUDED
