//
// JSONFormatter.cpp
//
// Library: Foundation
// Package: Logging
// Module:  JSONFormatter
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JSONFormatter.h"
#include "Poco/JSONString.h"
#include "Poco/Message.h"
#include "Poco/String.h"
#include "Poco/JSONString.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Timezone.h"


namespace Poco {


const std::string JSONFormatter::PROP_TIMES("times");
const std::string JSONFormatter::PROP_THREAD("thread");


void JSONFormatter::format(const Message& msg, std::string& text)
{
	Timestamp timestamp = msg.getTime();
	int tzd = DateTimeFormatter::UTC;
	if (_localTime)
	{
		tzd = Timezone::utcOffset();
		tzd += Timezone::dst();
		timestamp += tzd*Timestamp::resolution();
	}

	text += '{';
	text += "\"timestamp\":\"";
	text += Poco::DateTimeFormatter::format(timestamp, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, tzd);
	text += "\",\"source\":";
	text += toJSON(msg.getSource());
	text += ",\"level\":\"";
	text += getPriorityName(msg.getPriority());
	text += "\",\"message\":";
	text += toJSON(msg.getText());
	if (_threadFormat != JSONF_THREAD_NONE)
	{
		text += ",\"thread\":";
		text += getThread(msg);
	}
	if (msg.getSourceFile())
	{
		text += ",\"file\":";
		text += toJSON(msg.getSourceFile());
	}
	if (msg.getSourceLine())
	{
		text += ",\"line\":\"";
		text += Poco::NumberFormatter::format(msg.getSourceLine());
		text += "\"";
	}
	if (!msg.getAll().empty())
	{
		text += ",\"params\":{";
		const auto& props = msg.getAll();
		bool first = true;
		for (const auto& p: props)
		{
			if (!first) 
				text += ',';
			else
				first = false;
			text += toJSON(p.first);
			text += ':';
			text += toJSON(p.second);
		}
		text += '}';
	}
	text += '}';
}


void JSONFormatter::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_TIMES)
	{
		if (Poco::icompare(value, "local"s) == 0)
			_localTime = true;
		else if (Poco::icompare(value, "utc"s) == 0)
			_localTime = false;
		else
			throw Poco::InvalidArgumentException("Invalid times value (must be local or UTC)"s, value);
	}
	else if (name == PROP_THREAD)
	{
		if (Poco::icompare(value, "none"s) == 0)
			_threadFormat = JSONF_THREAD_NONE;
		else if (Poco::icompare(value, "name"s) == 0)
			_threadFormat = JSONF_THREAD_NAME;
		else if (Poco::icompare(value, "id"s) == 0)
			_threadFormat = JSONF_THREAD_ID;
		else if (Poco::icompare(value, "osid"s) == 0)
			_threadFormat = JSONF_THREAD_OS_ID;
		else
			throw Poco::InvalidArgumentException("Invalid thread value (must be name, id or osID)"s, value);
	}
	else throw Poco::PropertyNotSupportedException(name);
}


std::string JSONFormatter::getProperty(const std::string& name) const
{
	if (name == PROP_TIMES)
	{
		return _localTime ? "local"s : "UTC"s;
	}
	else if (name == PROP_THREAD)
	{
		switch (_threadFormat)
		{
		case JSONF_THREAD_NONE:
			return "none"s;
		case JSONF_THREAD_NAME:
			return "name"s;
		case JSONF_THREAD_ID:
			return "id"s;
		case JSONF_THREAD_OS_ID:
			return "osID"s;
		default:
			return "invalid"s;
		}
	}
	else throw Poco::PropertyNotSupportedException(name);
}


std::string JSONFormatter::getThread(const Message& message) const
{
	switch (_threadFormat)
	{
		case JSONF_THREAD_NONE:
			return ""s;
		case JSONF_THREAD_NAME:
			return toJSON(message.getThread());
		case JSONF_THREAD_ID:
			return Poco::NumberFormatter::format(message.getTid());
		case JSONF_THREAD_OS_ID:
			return Poco::NumberFormatter::format(message.getOsTid());
		default:
			return ""s;
	}
}


const std::string& JSONFormatter::getPriorityName(int prio)
{
	static const std::string PRIORITY_NAMES[] = {
		"none"s,
		"fatal"s,
		"critical"s,
		"error"s,
		"warning"s,
		"notice"s,
		"information"s,
		"debug"s,
		"trace"
	};

	poco_assert (prio >= Message::PRIO_FATAL && prio <= Message::PRIO_TRACE);
	
	return PRIORITY_NAMES[prio];
}


} // namespace Poco
