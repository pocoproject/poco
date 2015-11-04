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
#include "Poco/Nullable.h"

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/RedisStream.h"

namespace Poco {
namespace Redis {


class Redis_API RedisType
{
public:

	typedef SharedPtr<RedisType> Ptr;

	RedisType();
	virtual ~RedisType();

	bool isArray() const;

	bool isBulkString() const;

	bool isError() const;

	bool isInteger() const;

	bool isSimpleString() const;

	virtual int type() const = 0;

	virtual void read(RedisInputStream& input) = 0;

	virtual std::string toString() const = 0;

	enum Types {
		REDIS_INTEGER,
		REDIS_SIMPLE_STRING,
		REDIS_BULK_STRING,
		REDIS_ARRAY,
		REDIS_ERROR
	};

	static RedisType::Ptr createRedisType(char marker);
		/// Create a Redis type based on the marker :
		/// + : a simple string (std::string)
		/// - : an error (Error)
		/// $ : a bulk string (BulkString)
		/// * : an array (Array)
		/// : : a signed 64 bit integer (Int64)

private:

};

inline bool RedisType::isArray() const
{
	return type() == REDIS_ARRAY;
}

inline bool RedisType::isBulkString() const
{
	return type() == REDIS_BULK_STRING;
}

inline bool RedisType::isError() const
{
	return type() == REDIS_ERROR;
}

inline bool RedisType::isInteger() const
{
	return type() == REDIS_INTEGER;
}

inline bool RedisType::isSimpleString() const
{
	return type() == REDIS_SIMPLE_STRING;
}

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


typedef Nullable<std::string> BulkString;
	/// A bulk string is a string that can contain a NULL value.
	/// So, BulkString is a typedef for Nullable<std::string>.


template<>
struct ElementTraits<BulkString>
{
	enum { TypeId = RedisType::REDIS_BULK_STRING };

	static const char marker = '$';

	static std::string toString(const BulkString& value)
	{
		if ( value.isNull() )
		{
			return marker + std::string("-1") + LineEnding::NEWLINE_CRLF;
		}
		else
		{
			std::string s = value.value();
			return marker
				+ NumberFormatter::format(s.length())
				+ LineEnding::NEWLINE_CRLF
				+ s
				+ LineEnding::NEWLINE_CRLF;
		}
	}
};


template<typename T>
class Type : public RedisType
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

	virtual void read(RedisInputStream& socket);

	virtual std::string toString() const
	{
		return ElementTraits<T>::toString(_value);
	}

	T& value()
	{
		return _value;
	}

	const T& value() const
	{
		return _value;
	}

private:

	T _value;
};

template<> inline
void Type<Int64>::read(RedisInputStream& input)
{
	std::string number = input.getline();
	_value = NumberParser::parse64(number);
}

template<> inline
void Type<std::string>::read(RedisInputStream& input)
{
	_value.clear();
	_value = input.getline();
}

template<> inline
void Type<BulkString>::read(RedisInputStream& input)
{
	_value.clear();

	std::string line = input.getline();
	int length = NumberParser::parse64(line);

	if ( length >= 0 )
	{
		std::string s;
		s.resize(length, ' ');
		input.read(&*s.begin(), length);
		_value.assign(s);

		line = input.getline();
	}

}


}}

#endif // Redis_Type_INCLUDED
