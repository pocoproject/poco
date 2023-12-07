//
// AbstractConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  AbstractConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/ConfigurationView.h"
#include "Poco/Util/LocalConfigurationView.h"
#include "Poco/Exception.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"


using Poco::Mutex;
using Poco::NotFoundException;
using Poco::SyntaxException;
using Poco::CircularReferenceException;
using Poco::NumberParser;
using Poco::NumberFormatter;
using Poco::icompare;


namespace Poco {
namespace Util {


AbstractConfiguration::AbstractConfiguration():
	_depth(0),
	_eventsEnabled(true)
{
}


AbstractConfiguration::~AbstractConfiguration()
{
}


bool AbstractConfiguration::hasProperty(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	return getRaw(key, value);
}


bool AbstractConfiguration::hasOption(const std::string& key) const
{
	return hasProperty(key);
}


bool AbstractConfiguration::has(const std::string& key) const
{
	return hasProperty(key);
}


std::string AbstractConfiguration::getString(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		throw NotFoundException(key);
}


std::string AbstractConfiguration::getString(const std::string& key, const std::string& defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		return defaultValue;
}


std::string AbstractConfiguration::getRawString(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		throw NotFoundException(key);
}


std::string AbstractConfiguration::getRawString(const std::string& key, const std::string& defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return value;
	else
		return defaultValue;
}


int AbstractConfiguration::getInt(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		throw NotFoundException(key);
}


int AbstractConfiguration::getInt(const std::string& key, int defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		return defaultValue;
}


unsigned AbstractConfiguration::getUInt(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		throw NotFoundException(key);
}


unsigned AbstractConfiguration::getUInt(const std::string& key, unsigned defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		return defaultValue;
}


Poco::Int16 AbstractConfiguration::getInt16(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt16(internalExpand(value));
	else
		throw NotFoundException(key);
}

	
Poco::Int16 AbstractConfiguration::getInt16(const std::string& key, Poco::Int16 defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt16(internalExpand(value));
	else
		return defaultValue;
}


Poco::UInt16 AbstractConfiguration::getUInt16(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt16(internalExpand(value));
	else
		throw NotFoundException(key);
}


Poco::UInt16 AbstractConfiguration::getUInt16(const std::string& key, Poco::UInt16 defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt16(internalExpand(value));
	else
		return defaultValue;
}


#if defined(POCO_HAVE_INT64)


Int64 AbstractConfiguration::getInt64(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt64(internalExpand(value));
	else
		throw NotFoundException(key);
}


Int64 AbstractConfiguration::getInt64(const std::string& key, Int64 defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt64(internalExpand(value));
	else
		return defaultValue;
}


UInt64 AbstractConfiguration::getUInt64(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt64(internalExpand(value));
	else
		throw NotFoundException(key);
}


UInt64 AbstractConfiguration::getUInt64(const std::string& key, UInt64 defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt64(internalExpand(value));
	else
		return defaultValue;
}


#endif // defined(POCO_HAVE_INT64)


double AbstractConfiguration::getDouble(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(internalExpand(value));
	else
		throw NotFoundException(key);
}


double AbstractConfiguration::getDouble(const std::string& key, double defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return NumberParser::parseFloat(internalExpand(value));
	else
		return defaultValue;
}


bool AbstractConfiguration::getBool(const std::string& key) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(internalExpand(value));
	else
		throw NotFoundException(key);
}


bool AbstractConfiguration::getBool(const std::string& key, bool defaultValue) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(internalExpand(value));
	else
		return defaultValue;
}


void AbstractConfiguration::setString(const std::string& key, const std::string& value)
{
	setRawWithEvent(key, value);
}


void AbstractConfiguration::setInt(const std::string& key, int value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void AbstractConfiguration::setUInt(const std::string& key, unsigned int value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void AbstractConfiguration::setInt16(const std::string& key, Poco::Int16 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}

	
void AbstractConfiguration::setUInt16(const std::string& key, Poco::UInt16 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void AbstractConfiguration::setInt32(const std::string& key, Poco::Int32 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}

	
void AbstractConfiguration::setUInt32(const std::string& key, Poco::UInt32 value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


#if defined(POCO_HAVE_INT64)


void AbstractConfiguration::setInt64(const std::string& key, Int64 value)
{
	Mutex::ScopedLock lock(_mutex);

	setRawWithEvent(key, NumberFormatter::format(value));
}


void AbstractConfiguration::setUInt64(const std::string& key, UInt64 value)
{
	Mutex::ScopedLock lock(_mutex);

	setRawWithEvent(key, NumberFormatter::format(value));
}


#endif // defined(POCO_HAVE_INT64)


void AbstractConfiguration::setDouble(const std::string& key, double value)
{
	setRawWithEvent(key, NumberFormatter::format(value));
}


void AbstractConfiguration::setBool(const std::string& key, bool value)
{
	setRawWithEvent(key, value ? "true" : "false");
}


void AbstractConfiguration::keys(Keys& range) const
{
	Mutex::ScopedLock lock(_mutex);

	std::string key;
	range.clear();
	enumerate(key, range);
}


void AbstractConfiguration::keys(const std::string& key, Keys& range) const
{
	Mutex::ScopedLock lock(_mutex);

	range.clear();
	enumerate(key, range);
}


const AbstractConfiguration::Ptr AbstractConfiguration::createView(const std::string& prefix) const
{
	return new ConfigurationView(prefix, AbstractConfiguration::Ptr(const_cast<AbstractConfiguration*>(this), true));
}


AbstractConfiguration::Ptr AbstractConfiguration::createView(const std::string& prefix)
{
	return new ConfigurationView(prefix, AbstractConfiguration::Ptr(this, true));
}


const AbstractConfiguration::Ptr AbstractConfiguration::createLocalView(const std::string& prefix) const
{
	return new LocalConfigurationView(prefix, AbstractConfiguration::Ptr(const_cast<AbstractConfiguration*>(this), true));
}


AbstractConfiguration::Ptr AbstractConfiguration::createLocalView(const std::string& prefix)
{
	return new LocalConfigurationView(prefix, AbstractConfiguration::Ptr(this, true));
}


namespace
{
	class AutoCounter
	{
	public:
		AutoCounter(int& count): _count(count)
		{
			++_count;
		}

		~AutoCounter()
		{
			--_count;
		}

	private:
		int& _count;
	};
}


std::string AbstractConfiguration::expand(const std::string& value) const
{
	Mutex::ScopedLock lock(_mutex);

	return internalExpand(value);
}


void AbstractConfiguration::remove(const std::string& key)
{
	if (_eventsEnabled)
	{
		propertyRemoving(this, key);
	}
	{

		Mutex::ScopedLock lock(_mutex);
		removeRaw(key);
	}
	if (_eventsEnabled)
	{
		propertyRemoved(this, key);
	}
}


void AbstractConfiguration::enableEvents(bool enable)
{
	_eventsEnabled = enable;
}


bool AbstractConfiguration::eventsEnabled() const
{
	return _eventsEnabled;
}


void AbstractConfiguration::removeRaw(const std::string& key)
{
	throw Poco::NotImplementedException("removeRaw()");
}


std::string AbstractConfiguration::internalExpand(const std::string& value) const
{
	AutoCounter counter(_depth);
	if (_depth > 10) throw CircularReferenceException("Too many property references encountered");
	return uncheckedExpand(value);
}


std::string AbstractConfiguration::uncheckedExpand(const std::string& value) const
{
	std::string result;
	std::string::const_iterator it  = value.begin();
	std::string::const_iterator end = value.end();
	while (it != end)
	{
		if (*it == '$')
		{
			++it;
			if (it != end && *it == '{')
			{
				++it;
				std::string prop;
				std::string deflt;
				bool haveDefault = false;
				while (it != end && *it != '}')
				{
					if (*it == ':')
					{
						++it;
						if (it != end && *it == '-')
						{
							haveDefault = true;
							++it;
							while (it != end && *it != '}') deflt += *it++;
						}
						else prop += ':';
					}
					else prop += *it++;
				}
				if (it != end) ++it;
				std::string value;
				if (getRaw(prop, value))
				{
					result.append(internalExpand(value));
				}
				else if (haveDefault)
				{
					result.append(deflt);
				}
				else
				{
					result.append("${");
					result.append(prop);
					result.append("}");
				}
			}
			else result += '$';
		}
		else result += *it++;
	}
	return result;
}


int AbstractConfiguration::parseInt(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return static_cast<int>(NumberParser::parseHex(value));
	else
		return NumberParser::parse(value);
}


unsigned AbstractConfiguration::parseUInt(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return NumberParser::parseHex(value);
	else
		return NumberParser::parseUnsigned(value);
}


Poco::Int16 AbstractConfiguration::parseInt16(const std::string& value)
{
	int intValue = 0;
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		intValue = static_cast<int>(NumberParser::parseHex(value));
	else
		intValue = NumberParser::parse(value);
	if (intValue >= -32768 && intValue <= 32767)
		return static_cast<Poco::Int16>(intValue);
	else
		throw Poco::RangeException("Not a valid 16-bit integer value", value);
}


Poco::UInt16 AbstractConfiguration::parseUInt16(const std::string& value)
{
	unsigned uintValue;
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		uintValue = NumberParser::parseHex(value);
	else
		uintValue = NumberParser::parseUnsigned(value);
	if (uintValue <= 65535)
		return static_cast<Poco::UInt16>(uintValue);
	else
		throw Poco::RangeException("Not a valid unsigned 16-bit integer value", value);
}


Int64 AbstractConfiguration::parseInt64(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return static_cast<Int64>(NumberParser::parseHex64(value));
	else
		return NumberParser::parse64(value);
}


UInt64 AbstractConfiguration::parseUInt64(const std::string& value)
{
	if ((value.compare(0, 2, "0x") == 0) || (value.compare(0, 2, "0X") == 0))
		return NumberParser::parseHex64(value);
	else
		return NumberParser::parseUnsigned64(value);
}


bool AbstractConfiguration::parseBool(const std::string& value)
{
	int n;
	if (NumberParser::tryParse(value, n))
		return n != 0;
	else if (icompare(value, "true") == 0)
		return true;
	else if (icompare(value, "yes") == 0)
		return true;
	else if (icompare(value, "on") == 0)
		return true;
	else if (icompare(value, "false") == 0)
		return false;
	else if (icompare(value, "no") == 0)
		return false;
	else if (icompare(value, "off") == 0)
		return false;
	else
		throw SyntaxException("Cannot convert to boolean", value);
}


void AbstractConfiguration::setRawWithEvent(const std::string& key, std::string value)
{
	KeyValue kv(key, value);
	if (_eventsEnabled)
	{
		propertyChanging(this, kv);
	}
	{
		Mutex::ScopedLock lock(_mutex);
		setRaw(key, value);
	}
	if (_eventsEnabled)
	{
		propertyChanged(this, kv);
	}
}


} } // namespace Poco::Util
