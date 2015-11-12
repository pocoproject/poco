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

	typedef std::vector<RedisType::Ptr>::const_iterator const_iterator;

	Array();
		/// Default constructor. As long as there are no elements added,
		/// the array will contain a Null value.

	Array(const Array& copy);
		/// Copy constructor.

	virtual ~Array();
		/// Destructor.

	Array& add(Poco::Int64 value);
		/// Adds an integer element.

	Array& add(const std::string& value);
		/// Adds a bulk string element. A bulk string is a string that
		/// is binary safe (it can contain newlines). If you want a simple
		/// string use addSimpleString (can't contain a newline!).

	Array& add(const BulkString& value);
		/// Adds a bulk string element.

	Array& add();
		/// Adds a Null bulk string element.

	Array& add(RedisType::Ptr value);
		/// Adds a Redis element.

	Array& add(const Array& array);
		/// Adds an array.

	Array& addSimpleString(const std::string& value);
		/// Adds a simple string (can't contain newline characters!)

	const_iterator begin() const;
		/// Returns an iterator to the start of the array. Note:
		/// this can throw a NullValueException when this is a Null array.

	void clear();
		/// Removes all elements from the array.

	const_iterator end() const;
		/// Returns an iterator to the end of the array. Note:
		/// this can throw a NullValueException when this is a Null array.

	template<typename T>
	T get(size_t pos) const
		/// Returns the element on the given position and tries to convert
		/// to the template type. A BadCastException will be thrown when the
		/// the conversion fails. An InvalidArgumentException will be thrown
		/// when the index is out of range. When the array is a Null array
		/// a NullValueException is thrown.
	{
		if ( _elements.isNull() ) throw NullValueException();

		if ( pos >= _elements.value().size() ) throw InvalidArgumentException();

		RedisType::Ptr element = _elements.value().at(pos);
		if ( RedisTypeTraits<T>::TypeId == element->type() )
		{
			Type<T>* concrete = dynamic_cast<Type<T>* >(element.get());
			if ( concrete != NULL ) return concrete->value();
		}
		throw BadCastException();
	}

	bool isNull() const;
		/// Returns true when this is a Null array.

	void makeNull();
		/// Turns the array into a Null array. When the array already has some
		/// elements, the array will be cleared.

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

inline Array& Array::add()
{
	BulkString value;
	return add(new Type<BulkString>(value));
}

inline Array& Array::add(Int64 value)
{
	return add(new Type<Int64>(value));
}

inline Array& Array::add(const std::string& value)
{
	return add(new Type<BulkString>(value));
}

inline Array& Array::add(const BulkString& value)
{
	return add(new Type<BulkString>(value));
}

inline Array& Array::add(const Array& value)
{
	return add(new Type<Array>(value));
}

inline Array& Array::addSimpleString(const std::string& value)
{
	return add(new Type<std::string>(value));
}

inline Array::const_iterator Array::begin() const
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

inline Array::const_iterator Array::end() const
{
	return _elements.value().end();
}

inline bool Array::isNull() const
{
	return _elements.isNull();
}

inline void Array::makeNull()
{
	if ( !_elements.isNull() ) _elements.value().clear();

	_elements.clear();
}

inline size_t Array::size() const
{
	return _elements.value().size();
}


template<>
struct RedisTypeTraits<Array>
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

	static void read(RedisInputStream& input, Array& value)
	{
		value.clear();

		Int64 length = NumberParser::parse64(input.getline());

		if ( length != -1 )
		{
			for(int i = 0; i < length; ++i)
			{
				char marker = input.get();
				RedisType::Ptr element = RedisType::createRedisType(marker);

				if ( element.isNull() )
					throw RedisException("Wrong answer received from Redis server");

				element->read(input);
				value.add(element);
			}
		}
	}
};


}}

#endif // Redis_Array_INCLUDED
