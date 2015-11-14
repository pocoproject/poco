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

	cmd << key << value;

	return cmd;
}

Command Command::blpop(const std::vector<std::string>& lists, Int64 timeout)
{
	Command cmd("BLPOP");

	for(std::vector<std::string>::const_iterator it = lists.begin(); it != lists.end(); ++it)
	{
		cmd << *it;
	}
	cmd << NumberFormatter::format(timeout);

	return cmd;
}

Command Command::brpop(const std::vector<std::string>& lists, Int64 timeout)
{
	Command cmd("BRPOP");

	for(std::vector<std::string>::const_iterator it = lists.begin(); it != lists.end(); ++it)
	{
		cmd << *it;
	}
	cmd << NumberFormatter::format(timeout);

	return cmd;
}

Command Command::brpoplpush(const std::string& sourceList, const std::string& destinationList, Int64 timeout)
{
	Command cmd("BRPOPLPUSH");

	cmd << sourceList << destinationList << NumberFormatter::format(timeout);

	return cmd;
}

Command Command::decr(const std::string& key, Int64 by)
{
	Command cmd(by == 0 ? "DECR" : "DECRBY");

	cmd << key;
	if ( by > 0 ) cmd << NumberFormatter::format(by);

	return cmd;
}

Command Command::del(const std::string& key)
{
	Command cmd("DEL");

	cmd << key;

	return cmd;
}

Command Command::del(const std::vector<std::string>& keys)
{
	Command cmd("DEL");

	for(std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		cmd << *it;
	}

	return cmd;
}

Command Command::get(const std::string& key)
{
	Command cmd("GET");

	cmd << key;

	return cmd;
}

Command Command::incr(const std::string& key, Int64 by)
{
	Command cmd(by == 0 ? "INCR" : "INCRBY");

	cmd << key;
	if ( by > 0 ) cmd << NumberFormatter::format(by);

	return cmd;
}

Command Command::lindex(const std::string& list, Int64 index)
{
	Command cmd("LINDEX");

	cmd << list << NumberFormatter::format(index);

	return cmd;
}

Command Command::linsert(const std::string& list, bool before, const std::string& reference, const std::string& value)
{
	Command cmd("LINSERT");

	cmd << list << (before ? "BEFORE" : "AFTER") << reference << value;
	return cmd;
}

Command Command::llen(const std::string& list)
{
	Command cmd("LLEN");

	cmd << list;

	return cmd;
}

Command Command::lpop(const std::string& list)
{
	Command cmd("LPOP");

	cmd << list;

	return cmd;
}

Command Command::lpush(const std::string& list, const std::string& value, bool create)
{
	Command cmd(create ? "LPUSH" : "LPUSHX");

	cmd << list << value;

	return cmd;
}

Command Command::lpush(const std::string& list, const std::vector<std::string>& values, bool create)
{
	Command cmd(create ? "LPUSH" : "LPUSHX");

	cmd << list;
	for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		cmd << *it;
	}

	return cmd;
}

Command Command::lrange(const std::string& list, Int64 start, Int64 stop)
{
	Command cmd("LRANGE");

	cmd << list << NumberFormatter::format(start) << NumberFormatter::format(stop);

	return cmd;
}

Command Command::lrem(const std::string& list, Int64 count, const std::string& value)
{
	Command cmd("LREM");

	cmd << list << NumberFormatter::format(count) << value;

	return cmd;
}

Command Command::lset(const std::string& list, Int64 index, const std::string& value)
{
	Command cmd("LSET");

	cmd << list << NumberFormatter::format(index) << value;

	return cmd;
}

Command Command::ltrim(const std::__cxx11::string& list, Int64 start, Int64 stop)
{
	Command cmd("LTRIM");

	cmd << list << NumberFormatter::format(start) << NumberFormatter::format(stop);

	return cmd;
}

Command Command::mget(const std::vector<std::string>& keys)
{
	Command cmd("MGET");

	for(std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		cmd << *it;
	}

	return cmd;
}

Command Command::mset(const std::map<std::string, std::string>& keyvalues, bool create)
{
	Command cmd(create ? "MSET" : "MSETNX");

	for(std::map<std::string, std::string>::const_iterator it = keyvalues.begin(); it != keyvalues.end(); ++it)
	{
		cmd << it->first << it->second;
	}

	return cmd;
}

Command Command::sadd(const std::string& key, const std::string& value)
{
	Command cmd("SADD");

	cmd << key << value;

	return cmd;
}

Command Command::sadd(const std::string& key, const std::vector<std::string>& values)
{
	Command cmd("SADD");

	cmd << key;
	for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		cmd << *it;
	}

	return cmd;
}

Command Command::scard(const std::string& key)
{
	Command cmd("SCARD");

	cmd << key;

	return cmd;
}

Command Command::smembers(const std::string& key)
{
	Command cmd("SMEMBERS");

	cmd << key;

	return cmd;
}

Command Command::set(const std::string& key, const std::string& value, bool overwrite, const Poco::Timespan& expireTime, bool create)
{
	Command cmd("SET");

	cmd << key << value;
	if (! overwrite) cmd << "NX";
	if (! create) cmd << "XX";
	if (expireTime.totalMicroseconds() > 0) cmd << "PX" << expireTime.totalMilliseconds();

	return cmd;
}

Command Command::set(const std::string& key, Int64 value, bool overwrite, const Poco::Timespan& expireTime, bool create)
{
	return set(key, NumberFormatter::format(value), overwrite, expireTime, create);
}

Command Command::rename(const std::string& key, const std::string& newName, bool overwrite)
{
	Command cmd(overwrite ? "RENAME" : "RENAMENX");

	cmd << key << newName;

	return cmd;
}


Command Command::rpop(const std::string& list)
{
	Command cmd("RPOP");

	cmd << list;

	return cmd;
}

Command Command::rpoplpush(const std::string& sourceList, const std::string& destinationList)
{
	Command cmd("RPOPLPUSH");

	cmd << sourceList << destinationList;

	return cmd;
}

Command Command::rpush(const std::string& list, const std::string& value, bool create)
{
	Command cmd(create ? "RPUSH" : "RPUSHX");

	cmd << list << value;

	return cmd;
}

Command Command::rpush(const std::string& list, const std::vector<std::string>& values, bool create)
{
	Command cmd(create ? "RPUSH" : "RPUSHX");

	cmd << list;
	for(std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
	{
		cmd << *it;
	}

	return cmd;
}


}} // Poco::Redis