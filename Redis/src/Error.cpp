//
// Error.cpp
//
// Library: Redis
// Package: Redis
// Module:  Error
//
// Implementation of the Error class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Redis/Error.h"


namespace Poco {
namespace Redis {


Error::Error()
= default;


Error::Error(std::string  message): _message(std::move(message))
{
}


Error::~Error()
= default;


} } // namespace Poco::Redis
