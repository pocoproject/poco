//
// LayeredConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  LayeredConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Exception.h"
#include <set>


using Poco::AutoPtr;
using Poco::RuntimeException;


namespace Poco {
namespace Util {


LayeredConfiguration::LayeredConfiguration()
{
}


LayeredConfiguration::~LayeredConfiguration()
{
}


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
	ConfigItem item;
	item.pConfig   = pConfig;
	item.priority  = priority;
	item.writeable = writeable;
	item.label     = label;

	ConfigList::iterator it = _configs.begin();
	while (it != _configs.end() && it->priority < priority) ++it;
	_configs.insert(it, item);
}


void LayeredConfiguration::removeConfiguration(AbstractConfiguration::Ptr pConfig)
{
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
	for (ConfigList::const_iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->label == label) return it->pConfig;
	}
	return 0;
}


bool LayeredConfiguration::getRaw(const std::string& key, std::string& value) const
{
	for (ConfigList::const_iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->pConfig->getRaw(key, value)) return true;
	}
	return false;
}


void LayeredConfiguration::setRaw(const std::string& key, const std::string& value)
{
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->writeable)
		{
			it->pConfig->setRaw(key, value); return;
		}
	}
	throw RuntimeException("No writeable configuration object to store the property", key);
}


void LayeredConfiguration::enumerate(const std::string& key, Keys& range) const
{
	std::set<std::string> keySet;
	for (ConfigList::const_iterator itc = _configs.begin(); itc != _configs.end(); ++itc)
	{
		Keys partRange;
		itc->pConfig->enumerate(key, partRange);
		for (Keys::const_iterator itr = partRange.begin(); itr != partRange.end(); ++itr)
		{
			if (keySet.find(*itr) == keySet.end())
			{
				range.push_back(*itr);
				keySet.insert(*itr);
			}
		}
	}
}


void LayeredConfiguration::removeRaw(const std::string& key)
{
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->writeable)
		{
			it->pConfig->remove(key);
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


} } // namespace Poco::Util
