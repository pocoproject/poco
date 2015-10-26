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
#include "Poco/Redis/Exception.h"

namespace Poco {
namespace Redis {

class Redis_API Array
	/// Represents a Redis Array. An Array can contain Integers, Strings,
	/// Bulk Strings, Errors and other arrays. It can also contain a Null
	/// value.
{
public:
	Array();
		/// Default constructor. As long as there are no elements added,
		/// the array will contain a Null value.

	Array(const Array& copy);
		/// Copy constructor.

	virtual ~Array();
		/// Destructor.

	void add(Poco::Int64 value);
		/// Adds an integer element.

	void add(const std::string& value);
		/// Adds a simple string element (can't contain a newline!).

	void add(const BulkString& value);
		/// Adds a bulk string element.

	void add();
		/// Adds a Null bulk string element.

	void add(RedisType::Ptr value);
		/// Adds a Redis element.

	std::vector<RedisType::Ptr>::const_iterator begin() const;
		/// Returns an iterator to the start of the array. Note:
		/// this can throw a NullValueException when this is a Null array.

	void clear();

	std::vector<RedisType::Ptr>::const_iterator end() const;
		/// Returns an iterator to the end of the array. Note:
		/// this can throw a NullValueException when this is a Null array.

	bool isNull() const;
		/// Returns true when this is a Null array.
	
	std::string toString() const;
		/// Returns the String representation as specified in the
		/// Redis Protocol specification.

	size_t size() const;
		/// Returns the size of the array. Note:
		/// this can throw a NullValueException when this is a Null array.

private:

	Nullable<std::vector<RedisType::Ptr> > _elements;

	void checkNull();
};

inline std::vector<RedisType::Ptr>::const_iterator Array::begin() const
{
	return _elements.value().begin();
}

inline void Array::checkNull()
{
	std::vector<RedisType::Ptr> v;
	if ( _elements.isNull() ) _elements.assign(v);
}

inline void Array::clear()
{
	if ( !_elements.isNull() )
	{
		_elements.value().clear();
	}
}

inline std::vector<RedisType::Ptr>::const_iterator Array::end() const
{
	return _elements.value().end();
}

inline bool Array::isNull() const
{
	return _elements.isNull();
}

inline size_t Array::size() const
{
	return _elements.value().size();
}

inline void Array::add(RedisType::Ptr value)
{
	_elements.value().push_back(value);
}

template<>
struct ElementTraits<Array>
{
	enum { TypeId = RedisType::REDIS_ARRAY };

	static const char marker = '*';

	static std::string toString(const Array& value)
	{
		std::stringstream result;
		result <<  marker;
		if ( value.isNull() )
		{
			result << "-1" << LineEnding::NEWLINE_CRLF;
		}
		else
		{
			result << value.size() << LineEnding::NEWLINE_CRLF;
			for(std::vector<RedisType::Ptr>::const_iterator it = value.begin();
				it != value.end(); ++it)
			{
				result << (*it)->toString();
			}
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

	if ( length != -1 )
	{
		for(int i = 0; i < length; ++i)
		{
			char marker = socket.get();
			RedisType::Ptr element = Type::createRedisType(marker);

			if ( element.isNull() )
				throw RedisException("Wrong answer received from Redis server");

			element->read(socket);
			_value.add(element);
		}
	}
}


}}

#endif // Redis_Array_INCLUDED
