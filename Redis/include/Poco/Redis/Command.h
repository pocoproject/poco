//
// Command.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Command
//
// Definition of the Command class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_Command_INCLUDED
#define Redis_Command_INCLUDED

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Array.h"

#include <vector>

namespace Poco {
namespace Redis {

class Redis_API Command : public Array
	/// Helper class for creating commands. This class contains
	/// factory methods for common used Redis commands.
{
public:
	Command(const std::string& command);
		/// Constructor

	Command(const Command& copy);
		/// Copy constructor

	virtual ~Command();
		/// Destructor

	static Command append(const std::string& key, const std::string& value);
		/// Returns an APPEND command

	static Command del(const std::string& key);
		/// Returns an DEL command

	static Command del(const std::vector<std::string>& keys);
		/// Returns an DEL command

	static Command get(const std::string& key);
		/// Returns an GET command

	static Command incr(const std::string& key, Int64 by = 0);
		/// Returns an INCR or INCRBY command. Calls INCR when by is omitted or zero.

	static Command lindex(const std::string& list, Int64 index = 0);
		/// Returns a LINDEX command

	static Command linsert(const std::string& list, bool before, const std::string& reference, const std::string& value);
		/// Returns a LINSERT command

	static Command llen(const std::string& list);
		/// Returns a LINDEX command

	static Command lpush(const std::string& list, const std::string& value, bool create = true);
		/// Returns a LPUSH or LPUSHX (when create is false) command

	static Command lpush(const std::string& list, const std::vector<std::string>& value, bool create = true);
		/// Returns a LPUSH or LPUSHX (when create is false) command

	static Command lrange(const std::string& list, Int64 start = 0, Int64 stop = 0);
		/// Returns a LRANGE command

	static Command set(const std::string& key, const std::string& value, bool overwrite = true, const Poco::Timespan& expireTime = 0, bool create = true);
		/// Returns a SET command to set the key with a value

	static Command set(const std::string& key, Int64 value, bool overwrite = true, const Poco::Timespan& expireTime = 0, bool create = true);
		/// Returns a SET command to set the key with a value

	static Command rpush(const std::string& list, const std::string& value, bool create = true);
		/// Returns a RPUSH or RPUSHX (when create is false) command

	static Command rpush(const std::string& list, const std::vector<std::string>& value, bool create = true);
		/// Returns a RPUSH or RPUSHX (when create is false) command
};

}} // namespace Poco::Redis

#endif // Redis_Command_INCLUDED
