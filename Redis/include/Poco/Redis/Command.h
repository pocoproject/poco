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

	static Command set(const std::string& key, const std::string& value, bool overwrite = true, const Poco::Timespan& expireTime = 0, bool create = true);
		/// Returns a SET command to set the key with a value
};

}} // namespace Poco::Redis

#endif // Redis_Command_INCLUDED
