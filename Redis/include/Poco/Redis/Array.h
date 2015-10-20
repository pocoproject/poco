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
#include "Poco/Redis/Redis.h"
#include "Poco/Redis/Type.h"
#include "Poco/Redis/Exception.h"

namespace Poco {
namespace Redis {

class Redis_API Array
{
public:
	Array();

	Array(const Array& copy);

	virtual ~Array();

	void add(Poco::Int64 value);

	void add(const std::string& value);

	void add(const BulkString& value);

	void add();

	void add(AbstractType::Ptr value);

	void clear();
	
	std::string toString() const;

	size_t size() const;

private:

	std::vector<AbstractType::Ptr> _elements;

	friend class Connection;
};

inline void Array::clear()
{
	_elements.clear();
}

inline size_t Array::size() const
{
	return _elements.size();
}

inline void Array::add(AbstractType::Ptr value)
{
	_elements.push_back(value);
}

template<>
struct ElementTraits<Array>
{
	enum { TypeId = AbstractType::REDIS_ARRAY };

	static std::string typeName()
	{
		return "Array";
	}

	static std::string toString(const Array& value)
	{
		return value.toString();
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
		AbstractType::Ptr t;

		switch(elementType)
		{
			case ':': // Integer
				t =  new Type<Int64>();
				break;
			case '+' : // Simple String
				t = new Type<std::string>();
				break;
			case '$' : // Bulk String
				t  = new Type<BulkString>();
				break;
		}

		if ( t.isNull() ) throw RedisException("Wrong answer received from Redis server");

		t->read(socket);
		_value.add(t);
	}
}


}}

#endif // Redis_Array_INCLUDED
