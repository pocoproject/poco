//
// Serializer.cpp
//
// Library: RemotingNG
// Package: Serialization
// Module:  Serializer
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/Serializer.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"


namespace Poco {
namespace RemotingNG {


Serializer::Serializer()
{
}


Serializer::~Serializer()
{
}


void Serializer::serializeOptionalBegin(const std::string& name, bool isSpecified)
{
	serializeNullableBegin(name, !isSpecified);
}


void Serializer::serializeOptionalEnd(const std::string& name)
{
	serializeNullableEnd(name);
}


void Serializer::serialize(const std::string& name, const Poco::DateTime& value)
{
	const std::string str(Poco::DateTimeFormatter::format(value, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
	serialize(name, str);
}


void Serializer::serialize(const std::string& name, const Poco::LocalDateTime& value)
{
	const std::string str(Poco::DateTimeFormatter::format(value, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
	serialize(name, str);
}


void Serializer::serialize(const std::string& name, const Poco::Timestamp& value)
{
	const std::string str(Poco::DateTimeFormatter::format(value, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
	serialize(name, str);
}


void Serializer::serialize(const std::string& name, const Poco::Timespan& value)
{
	Poco::Timespan::TimeDiff ms = value.totalMicroseconds();
	serialize(name, ms);
}


void Serializer::serialize(const std::string& name, const Poco::URI& value)
{
	const std::string str(value.toString());
	serialize(name, str);
}


void Serializer::serialize(const std::string& name, const Poco::UUID& value)
{
	const std::string str(value.toString());
	serialize(name, str);
}


void Serializer::pushAttribute(const std::string&, const std::string&)
{
}


void Serializer::registerNamespace(const std::string&)
{
}


} } // namespace Poco::RemotingNG
