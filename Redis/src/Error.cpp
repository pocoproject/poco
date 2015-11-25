//
// Error.cpp
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Error
//
// Implementation of the Error class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Error.h"

namespace Poco {
namespace Redis {

Error::Error()
{
}

Error::Error(const std::string& message) : _message(message)
{
}

Error::~Error()
{
}

} }