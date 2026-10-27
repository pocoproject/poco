//
// LayeredConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  LayeredConfiguration
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <set>
#include <utility>


using Poco::AutoPtr;
using Poco::InvalidArgumentException;
using Poco::NullPointerException;
using Poco::RuntimeException;


namespace Poco::Util {


LayeredConfiguration::LayeredConfiguration() = default;


LayeredConfiguration::~LayeredConfiguration() = default;


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig)
{
	add(pConfig, highest(), false);
}


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig, const std::string& label)
{
	add(pConfig, label, highest(), false);
}


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig, int priority)
{
	add(pConfig, priority, false);
}


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig, const std::string& label, int priority)
{
	add(pConfig, label, priority, false);
}


void LayeredConfiguration::addWriteable(AbstractConfiguration::Ptr pConfig, int priority)
{
	add(pConfig, priority, true);
}


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig, int priority, bool writeable)
{
	add(pConfig, std::string(), priority, writeable);
}


void LayeredConfiguration::add(AbstractConfiguration::Ptr pConfig, const std::string& label, int priority, bool writeable)
{
	AbstractConfiguration::ScopedLock lock(*this);

	const ConfigItem item{std::move(pConfig), priority, writeable, label};

	insert(item);
}


void LayeredConfiguration::replace(AbstractConfiguration::Ptr pConfig, const std::string& label, int priority, bool writeable)
{
	if (label.empty()) throw InvalidArgumentException("LayeredConfiguration::replace() requires a label");
	if (pConfig == nullptr) throw NullPointerException("LayeredConfiguration::replace() requires a configuration");

	const ConfigItem item{std::move(pConfig), priority, writeable, label};

	// Declared before the lock, so that the old configuration is released after the lock.
	AbstractConfiguration::Ptr pOld;
	AbstractConfiguration::ScopedLock lock(*this);

	auto itOld = std::find_if(_configs.begin(), _configs.end(),
		[&label](const ConfigItem& config) { return config.label == label; });
	// Insert before erasing: if the insertion throws, the configuration is unchanged.
	insert(item);
	if (itOld != _configs.end())
	{
		pOld = std::move(itOld->pConfig);
		_configs.erase(itOld);
	}
}


void LayeredConfiguration::removeConfiguration(AbstractConfiguration::Ptr pConfig)
{
	AbstractConfiguration::ScopedLock lock(*this);

	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->pConfig == pConfig)
		{
			_configs.erase(it);
			break;
		}
	}
}


AbstractConfiguration::Ptr LayeredConfiguration::find(const std::string& label) const
{
	AbstractConfiguration::ScopedLock lock(*this);

	for (const auto& conf: _configs)
	{
		if (conf.label == label) return conf.pConfig;
	}
	return nullptr;
}


bool LayeredConfiguration::getRaw(const std::string& key, std::string& value) const
{
	for (const auto& conf: _configs)
	{
		if (conf.pConfig->getRaw(key, value)) return true;
	}
	return false;
}


void LayeredConfiguration::setRaw(const std::string& key, const std::string& value)
{
	for (auto& conf: _configs)
	{
		if (conf.writeable)
		{
			conf.pConfig->setRaw(key, value);
			return;
		}
	}
	throw RuntimeException("No writeable configuration object to store the property", key);
}


void LayeredConfiguration::enumerate(const std::string& key, Keys& range) const
{
	std::set<std::string> keys;
	for (const auto& conf: _configs)
	{
		Keys partRange;
		conf.pConfig->enumerate(key, partRange);
		for (const auto& k: partRange)
		{
			if (keys.find(k) == keys.end())
			{
				range.push_back(k);
				keys.insert(k);
			}
		}
	}
}


void LayeredConfiguration::removeRaw(const std::string& key)
{
	for (auto& conf: _configs)
	{
		if (conf.writeable)
		{
			conf.pConfig->remove(key);
			return;
		}
	}
}


int LayeredConfiguration::lowest() const
{
	if (_configs.empty())
		return 0;
	else
		return _configs.front().priority - 1;
}


int LayeredConfiguration::highest() const
{
	if (_configs.empty())
		return 0;
	else
		return _configs.back().priority + 1;
}


void LayeredConfiguration::insert(const ConfigItem& item)
{
	ConfigList::iterator it = _configs.begin();
	while (it != _configs.end() && it->priority < item.priority) ++it;
	_configs.insert(it, item);
}


} // namespace Poco::Util
