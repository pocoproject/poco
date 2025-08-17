module;

#include <Poco/Redis/Array.h>
#include <Poco/Redis/AsyncReader.h>
#include <Poco/Redis/Client.h>
#include <Poco/Redis/Command.h>
#include <Poco/Redis/Error.h>
#include <Poco/Redis/Exception.h>
#include <Poco/Redis/PoolableConnectionFactory.h>
#include <Poco/Redis/RedisEventArgs.h>
#include <Poco/Redis/Redis.h>
#include <Poco/Redis/RedisStream.h>
#include <Poco/Redis/Type.h>

export module Poco.Redis;

export namespace Poco::Redis {
	using Poco::Redis::Array;
	using Poco::Redis::AsyncReader;
	using Poco::Redis::Client;
	using Poco::Redis::Command;
	using Poco::Redis::Error;
	using Poco::Redis::PooledConnection;
	using Poco::Redis::RedisEventArgs;
	using Poco::Redis::RedisException;
	using Poco::Redis::RedisIOS;
	using Poco::Redis::RedisInputStream;
	using Poco::Redis::RedisOutputStream;
	using Poco::Redis::RedisStreamBuf;
	using Poco::Redis::RedisType;
	using Poco::Redis::RedisTypeTraits;
	using Poco::Redis::Type;

	using Poco::Redis::BulkString;
}
