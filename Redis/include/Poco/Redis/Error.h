//
// Error.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Error
//
// Definition of the Error class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Redis_Error_INCLUDED
#define Redis_Error_INCLUDED

#include "Poco/Redis/Type.h"

namespace Poco {
namespace Redis {

class Redis_API Error
{
public:

	Error();
	Error(const std::string& message);
	virtual ~Error();

	std::string  getMessage() const;

	void setMessage(const std::string& message);

private:

	std::string _message;
};

inline std::string Error::getMessage() const
{
	return _message;
}

inline void Error::setMessage(const std::string& message)
{
	_message = message;
}

template<>
struct ElementTraits<Error>
{
	enum { TypeId = RedisType::REDIS_ERROR };

	static const char marker = '-';

	static std::string toString(const Error& value)
	{
		return marker + value.getMessage()  + "\r\n";
	}
};


template<> inline
void Type<Error>::read(RedisSocket& socket)
{
	std::string s;
	socket.readLine(s);
	_value = s;
}

}} // Namespace Poco::Redis

#endif // Redis_Error_INCLUDED