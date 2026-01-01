//
// ConfigurationView.cpp
//
// Library: Util
// Package: Configuration
// Module:  ConfigurationView
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/ConfigurationView.h"


namespace Poco {
namespace Util {


AbstractConfigurationView::AbstractConfigurationView(const std::string& prefix, AbstractConfiguration::Ptr pConfig):
	_prefix(prefix),
	_pConfig(pConfig)
{
	poco_check_ptr (pConfig);
}


void AbstractConfigurationView::setRaw(const std::string& key, const std::string& value)
{
	std::string translatedKey = translateKey(key);
	_pConfig->setRaw(translatedKey, value);
}


void AbstractConfigurationView::enumerate(const std::string& key, Keys& range) const
{
	std::string translatedKey = translateKey(key);
	_pConfig->enumerate(translatedKey, range);
}


void AbstractConfigurationView::removeRaw(const std::string& key)
{
	std::string translatedKey = translateKey(key);
	_pConfig->remove(translatedKey);
}


std::string AbstractConfigurationView::translateKey(const std::string& key) const
{
	std::string result = _prefix;
	if (!result.empty() && !key.empty() && key[0] != '[') result += '.';
	result += key;
	return result;
}


ConfigurationView::ConfigurationView(const std::string& prefix, AbstractConfiguration::Ptr pConfig)
	: AbstractConfigurationView(prefix, pConfig)
{
}


bool ConfigurationView::getRaw(const std::string& key, std::string& value) const
{
	std::string translatedKey = translateKey(key);
	return pConfig()->getRaw(translatedKey, value) || pConfig()->getRaw(key, value);
}


} } // namespace Poco::Util
