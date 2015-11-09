//
// Command.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Command
//
// Implementation of the Command class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Command.h"
#include "Poco/NumberFormatter.h"

namespace Poco {
namespace Redis {

Command::Command(const std::string& command) : Array()
{
	add(command);
}

Command::Command(const Command& copy) : Array(copy)
{
}

Command::~Command()
{
}

Command Command::append(const std::string& key, const std::string& value)
{
	Command cmd("APPEND");
	cmd.add(key).add(value);
	return cmd;
}

Command Command::del(const std::string& key)
{
	Command cmd("DEL");
	cmd.add(key);
	return cmd;
}

Command Command::del(const std::vector<std::string>& keys)
{
	Command cmd("DEL");
	for(std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		cmd.add(*it);
	}
	return cmd;
}

Command Command::get(const std::string& key)
{
	Command cmd("GET");
	cmd.add(key);
	return cmd;
}

Command Command::incr(const std::string& key, Int64 by)
{
	Command cmd(by == 0 ? "INCR" : "INCRBY");
	cmd.add(key);
	if ( by > 0 ) cmd.add(NumberFormatter::format(by));
	return cmd;
}

Command Command::lindex(const std::string& list, Int64 index)
{
	Command cmd("LINDEX");
	cmd.add(list).add(NumberFormatter::format(index));
	return cmd;
}

Command Command::linsert(const std::string& list, bool before, const std::string& reference, const std::string& value)
{
	Command cmd("LINSERT");
	cmd.add(list).add(before ? "BEFORE" : "AFTER").add(reference).add(value);
	return cmd;
}

Command Command::llen(const std::string& list)
{
	Command cmd("LLEN");
	cmd.add(list);
	return cmd;
}

Command Command::lpush(const std::string& list, const std::string& value, bool create)
{
	Command cmd(create ? "LPUSH" : "LPUSHX");
	cmd.add(list).add(value);

	return cmd;
}

Command Command::lpush(const std::string& list, const std::vector<std::string>& values, bool create)
{
	Command cmd(create ? "LPUSH" : "LPUSHX");
	cmd.add(list);

	for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		cmd.add(*it);
	}

	return cmd;
}

Command Command::lrange(const std::string& list, Int64 start, Int64 stop)
{
	Command cmd("LRANGE");
	cmd.add(list).add(NumberFormatter::format(start)).add(NumberFormatter::format(stop));
	return cmd;
}


Command Command::set(const std::string& key, const std::string& value, bool overwrite, const Poco::Timespan& expireTime, bool create)
{
	Command cmd("SET");
	cmd.add(key).add(value);
	if ( ! overwrite ) cmd.add("NX");
	if ( ! create ) cmd.add("XX");

	if ( expireTime.totalMicroseconds() > 0 )
	{
		cmd.add("PX").add(expireTime.totalMilliseconds());
	}

	return cmd;
}

Command Command::set(const std::string& key, Int64 value, bool overwrite, const Poco::Timespan& expireTime, bool create)
{
	return set(key, NumberFormatter::format(value), overwrite, expireTime, create);
}

Command Command::rpush(const std::string& list, const std::string& value, bool create)
{
	Command cmd(create ? "RPUSH" : "RPUSHX");
	cmd.add(list).add(value);

	return cmd;
}

Command Command::rpush(const std::string& list, const std::vector<std::string>& values, bool create)
{
	Command cmd(create ? "RPUSH" : "RPUSHX");
	cmd.add(list);

	for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		cmd.add(*it);
	}

	return cmd;
}


}} // Poco::Redis