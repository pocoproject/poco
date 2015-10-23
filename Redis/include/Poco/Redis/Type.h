//
// Type.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Type
//
// Definition of the Type class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_Type_INCLUDED
#define Redis_Type_INCLUDED

#include "Poco/LineEndingConverter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/SharedPtr.h"
#include "Poco/Optional.h"

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/RedisSocket.h"

namespace Poco {
namespace Redis {


class Redis_API RedisType
{
public:

	typedef SharedPtr<RedisType> Ptr;

	RedisType();
	virtual ~RedisType();

	virtual int type() const = 0;

	virtual void read(RedisSocket& socket) = 0;

	virtual std::string toString() const = 0;

	enum Types {
		REDIS_INTEGER,
		REDIS_SIMPLE_STRING,
		REDIS_BULK_STRING,
		REDIS_ARRAY,
		REDIS_ERROR
	};

private:

};

template<typename T>
struct ElementTraits
{
};


template<>
struct ElementTraits<Poco::Int64>
{
	enum { TypeId = RedisType::REDIS_INTEGER };

	static const char marker = ':';

	static std::string toString(const Poco::Int64& value)
	{
		return marker + NumberFormatter::format(value) + "\r\n";
	}
};


template<>
struct ElementTraits<std::string>
{
	enum { TypeId = RedisType::REDIS_SIMPLE_STRING };

	static const char marker = '+';

	static std::string toString(const std::string& value)
	{
		return marker + value + LineEnding::NEWLINE_CRLF;
	}
};


typedef Optional<std::string> BulkString;


template<>
struct ElementTraits<BulkString>
{
	enum { TypeId = RedisType::REDIS_BULK_STRING };

	static const char marker = '$';

	static std::string toString(const BulkString& value)
	{
		if ( value.isSpecified() )
		{
			std::string s = value.value();
			return marker + NumberFormatter::format(s.length()) + LineEnding::NEWLINE_CRLF + s + LineEnding::NEWLINE_CRLF;
		}
		return marker + std::string("-1") + LineEnding::NEWLINE_CRLF;
	}
};


template<typename T>
class Redis_API Type : public RedisType
{
public:

	Type()
	{
	}

	Type(const T& t) : _value(t)
	{
	}

	Type(const Type& copy) : _value(copy._value)
	{
	}

	virtual ~Type()
	{
	}

	int  type() const
	{
		return ElementTraits<T>::TypeId;
	}

	virtual void read(RedisSocket& socket);

	virtual std::string toString() const
	{
		return ElementTraits<T>::toString(_value);
	}

	T value() const
	{
		return _value;
	}

private:

	T _value;
};

template<> inline
void Type<Int64>::read(RedisSocket& socket)
{
	std::string number;
	socket.readLine(number);
	_value = NumberParser::parse64(number);
}

template<> inline
void Type<std::string>::read(RedisSocket& socket)
{
	_value.clear();
	socket.readLine(_value);
}

template<> inline
void Type<BulkString>::read(RedisSocket& socket)
{
	_value.clear();

	std::string line;
	socket.readLine(line);

	int length = NumberParser::parse64(line);

	if ( length >= 0 )
	{
		std::string s;
		socket.read(length, s);
		_value.assign(s);

		socket.readLine(line);
	}
}


}}

#endif // Redis_Type_INCLUDED
