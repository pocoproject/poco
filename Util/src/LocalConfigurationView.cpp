//
// LocalConfigurationView.cpp
//
// Library: Util
// Package: Configuration
// Module:  LocalConfigurationView
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Util/LocalConfigurationView.h"


namespace Poco {
namespace Util {


LocalConfigurationView::LocalConfigurationView(const std::string& prefix, AbstractConfiguration::Ptr pConfig) :
    AbstractConfigurationView(prefix, pConfig)
{
}


bool LocalConfigurationView::getRaw(const std::string& key, std::string& value) const
{
	std::string translatedKey = translateKey(key);
	return pConfig()->getRaw(translatedKey, value);
}


}}  // namespace Poco::Util
