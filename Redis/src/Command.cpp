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

Command Command::set(const std::string& key, const std::string& value, bool overwrite, const Poco::Timespan& expireTime, bool create)
{
	Command cmd("SET");
	cmd.add(key);
	cmd.add(value);
	if ( ! overwrite ) cmd.add("NX");
	if ( ! create ) cmd.add("XX");

	if ( expireTime.totalMicroseconds() > 0 )
	{
		cmd.add("PX");
		cmd.add(expireTime.totalMilliseconds());
	}

	return cmd;
}


}} // Poco::Redis