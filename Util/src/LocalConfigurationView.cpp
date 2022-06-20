//
// LocalConfigurationView.cpp
//
// Library: Util
// Package: Configuration
// Module:  LocalConfigurationView
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Util/LocalConfigurationView.h"


namespace Poco {
namespace Util {


LocalConfigurationView::LocalConfigurationView(const std::string& prefix, AbstractConfiguration::Ptr pConfig) :
	_prefix(prefix), _pConfig(pConfig)
{
	poco_check_ptr(pConfig);
}


LocalConfigurationView::~LocalConfigurationView()
{
}


bool LocalConfigurationView::getRaw(const std::string& key, std::string& value) const
{
	std::string translatedKey = translateKey(key);
	return _pConfig->getRaw(translatedKey, value);
}


void LocalConfigurationView::setRaw(const std::string& key, const std::string& value)
{
	std::string translatedKey = translateKey(key);
	_pConfig->setRaw(translatedKey, value);
}


void LocalConfigurationView::enumerate(const std::string& key, Keys& range) const
{
	std::string translatedKey = translateKey(key);
	_pConfig->enumerate(translatedKey, range);
}


void LocalConfigurationView::removeRaw(const std::string& key)
{
	std::string translatedKey = translateKey(key);
	_pConfig->remove(translatedKey);
}


std::string LocalConfigurationView::translateKey(const std::string& key) const
{
	std::string result = _prefix;
	if (!result.empty() && !key.empty() && key[0] != '[') result += '.';
	result += key;
	return result;
}


}}  // namespace Poco::Util
