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

#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/SharedPtr.h"
#include "Poco/Optional.h"

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/RedisSocket.h"

namespace Poco {
namespace Redis {


class Redis_API  AbstractType
{
public:

	typedef SharedPtr<AbstractType> Ptr;

	AbstractType();
	virtual ~AbstractType();

	virtual int getType() const = 0;

	virtual void read(RedisSocket& socket) = 0;

	virtual std::string toString() const = 0;

	enum Types {
		REDIS_INTEGER,
		REDIS_SIMPLE_STRING,
		REDIS_BULK_STRING,
		REDIS_ARRAY
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
	enum { TypeId = AbstractType::REDIS_INTEGER };

	static std::string typeName()
	{
		return "Integer";
	}

	static std::string toString(const Poco::Int64& value)
	{
		return ":" + NumberFormatter::format(value) + "\r\n";
	}
};


template<>
struct ElementTraits<std::string>
{
	enum { TypeId = AbstractType::REDIS_SIMPLE_STRING };

	static std::string typeName()
	{
		return "Simple String";
	}

	static std::string toString(const std::string& value)
	{
		return "+" + value + "\r\n";
	}
};


typedef Optional<std::string> BulkString;


template<>
struct ElementTraits<BulkString>
{
	enum { TypeId = AbstractType::REDIS_BULK_STRING };

	static std::string typeName()
	{
		return "Bulk String";
	}

	static std::string toString(const BulkString& value)
	{
		if ( value.isSpecified() ) {
			std::string s = value.value();
			return "$" + NumberFormatter::format(s.length()) + "\r\n" + s + "\r\n";
		}
		return "$-1\r\n";
	}
};


template<typename T>
class Redis_API Type : public AbstractType
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

	int  getType() const
	{
		return ElementTraits<T>::TypeId;
	}

	void read(RedisSocket& socket)
	{
	}

	virtual std::string toString() const {
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
	std::string s;
	socket.readLine(s);
	_value = s;
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
	else // -1
	{
		socket.readLine(line);
	}
}


}}

#endif // Redis_Type_INCLUDED
