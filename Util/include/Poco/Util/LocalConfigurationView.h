//
// LocalConfigurationView.h
//
// Library: Util
// Package: Configuration
// Module:  LocalConfigurationView
//
// Definition of the ConfigurationView class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Util_LocalConfigurationView_INCLUDED
#define Util_LocalConfigurationView_INCLUDED

#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/Util.h"

namespace Poco {
namespace Util {


class Util_API LocalConfigurationView : public AbstractConfiguration
	/// This configuration implements a "view" into a sub-hierarchy
	/// of another configuration.
	///
	/// For example, given a configuration with the following properties:
	///	 config.value1
	///	 config.value2
	///	 config.sub.value1
	///	 config.sub.value2
	/// and a LocalConfigurationView with the prefix "config", then
	/// the above properties will be available via the view as
	///	 value1
	///	 value2
	///	 sub.value1
	///	 sub.value2
	///
	/// A LocalConfigurationView is most useful in combination with a
	/// LayeredConfiguration.
	///
	/// The LocalConfigurationView only searches for the properties in the viewed Space.
{
public:
	LocalConfigurationView(const std::string& prefix, AbstractConfiguration::Ptr pConfig);
		/// Creates the LocalConfigurationView. The LocalConfigurationView
		/// retains (shared) ownership of the passed configuration.

protected:
	bool getRaw(const std::string& key, std::string& value) const;
	void setRaw(const std::string& key, const std::string& value);
	void enumerate(const std::string& key, Keys& range) const;
	void removeRaw(const std::string& key);
	std::string translateKey(const std::string& key) const;
	~LocalConfigurationView();

private:
	LocalConfigurationView(const LocalConfigurationView&);
	LocalConfigurationView& operator=(const LocalConfigurationView&);
	std::string _prefix;
	AbstractConfiguration::Ptr _pConfig;
};


}} // namespace Poco::Util


#endif  // Util_LocalConfigurationView_INCLUDED
