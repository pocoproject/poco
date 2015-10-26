//
// Type.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Type
//
// Implementation of the Type class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Type.h"
#include "Poco/Redis/Error.h"
#include "Poco/Redis/Array.h"

namespace Poco {
namespace Redis {


RedisType::RedisType()
{
}

RedisType::~RedisType()
{
}


RedisType::Ptr RedisType::createRedisType(char marker)
{
	RedisType::Ptr result;

	switch(marker)
	{
		case ElementTraits<Int64>::marker :
			result = new Type<Int64>();
			break;
		case ElementTraits<std::string>::marker :
			result = new Type<std::string>();
			break;
		case ElementTraits<BulkString>::marker :
			result = new Type<BulkString>();
			break;
		case ElementTraits<Array>::marker :
			result = new Type<Array>();
			break;
		case ElementTraits<Error>::marker :
			result = new Type<Error>();
			break;
	}
	return result;
}


}}