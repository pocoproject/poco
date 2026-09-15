//
// Deserializer.cpp
//
// Library: RemotingNG
// Package: Serialization
// Module:  Deserializer
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"


namespace Poco {
namespace RemotingNG {


Deserializer::Deserializer()
{
}


Deserializer::~Deserializer()
{
}


bool Deserializer::deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified)
{
	bool isNull;
	bool ret = deserializeNullableBegin(name, isMandatory, isNull);
	isSpecified = !isNull;
	return ret;
}


void Deserializer::deserializeOptionalEnd(const std::string& name)
{
	deserializeNullableEnd(name);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::DateTime& value)
{
	std::string str;
	bool found = deserialize(name, isMandatory, str);
	if (found)
	{
		int tzd = 0;
		Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, str, value, tzd);
		value.makeUTC(tzd);
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::LocalDateTime& value)
{
	std::string str;
	bool found = deserialize(name, isMandatory, str);
	if (found)
	{
		int tzd = 0;
		Poco::DateTime dt;
		Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, str, dt, tzd);
		value = Poco::LocalDateTime(tzd, dt, false);
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Timestamp& value)
{
	Poco::DateTime dateTime;
	bool found = deserialize(name, isMandatory, dateTime);
	if (found)
	{
		value = dateTime.timestamp();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Timespan& value)
{
	Poco::Timespan::TimeDiff diff;
	bool found = deserialize(name, isMandatory, diff);
	if (found)
	{
		value = Poco::Timespan(diff);
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::URI& value)
{
	std::string str;
	bool found = deserialize(name, isMandatory, str);
	if (found)
		value = str;
	else
		value.clear();
	return found;	
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UUID& value)
{
	std::string str;
	bool found = deserialize(name, isMandatory, str);
	if (found)
		value.parse(str);
	else
		value = Poco::UUID();
	return found;
}


void Deserializer::pushAttribute(const std::string& /*attrNamespace*/, const std::string& /*attrName*/, bool /*isMandatory*/)
{
}


} } // namespace Poco::RemotingNG
