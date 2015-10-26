//
// Array.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Array
//
// Definition of the Array class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_Array_INCLUDED
#define Redis_Array_INCLUDED

#include <vector>
#include <sstream>

#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Type.h"
#include "Poco/Redis/Error.h"
#include "Poco/Redis/Exception.h"

namespace Poco {
namespace Redis {

class Redis_API Array
	/// Represents a Redis Array.
{
public:
	Array();

	Array(const Array& copy);

	virtual ~Array();

	void add(Poco::Int64 value);

	void add(const std::string& value);

	void add(const BulkString& value);

	void add();

	void add(RedisType::Ptr value);

	std::vector<RedisType::Ptr>::const_iterator begin() const;

	void clear();

	std::vector<RedisType::Ptr>::const_iterator end() const;
	
	std::string toString() const;

	size_t size() const;

private:

	std::vector<RedisType::Ptr> _elements;
};

inline std::vector<RedisType::Ptr>::const_iterator Array::begin() const
{
	return _elements.begin();
}

inline void Array::clear()
{
	_elements.clear();
}

inline std::vector<RedisType::Ptr>::const_iterator Array::end() const
{
	return _elements.end();
}

inline size_t Array::size() const
{
	return _elements.size();
}

inline void Array::add(RedisType::Ptr value)
{
	_elements.push_back(value);
}

template<>
struct ElementTraits<Array>
{
	enum { TypeId = RedisType::REDIS_ARRAY };

	static const char marker = '*';

	static std::string toString(const Array& value)
	{
		std::stringstream result;
		result <<  marker << value.size() << LineEnding::NEWLINE_CRLF;
		for(std::vector<RedisType::Ptr>::const_iterator it = value.begin(); it != value.end(); ++it)
		{
			result << (*it)->toString();
		}
		return result.str();
	}
};

template<> inline
void Type<Array>::read(RedisSocket& socket)
{
	std::string line;
	socket.readLine(line);
	Int64 length = NumberParser::parse64(line);

	for(int i = 0; i < length; ++i)
	{
		char elementType = socket.get();
		RedisType::Ptr element;

		switch(elementType)
		{
			case ElementTraits<Int64>::marker :
				element = new Type<Int64>();
				break;
			case ElementTraits<std::string>::marker :
				element = new Type<std::string>();
				break;
			case ElementTraits<BulkString>::marker :
				element = new Type<BulkString>();
				break;
			case ElementTraits<Array>::marker :
				element = new Type<Array>();
				break;
			case ElementTraits<Error>::marker :
				element = new Type<Error>();
				break;
		}

		if ( element.isNull() ) throw RedisException("Wrong answer received from Redis server");

		element->read(socket);
		_value.add(element);
	}
}


}}

#endif // Redis_Array_INCLUDED
