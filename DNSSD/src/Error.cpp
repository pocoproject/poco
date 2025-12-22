//
// Error.cpp
//
// $Id: //poco/1.7/DNSSD/src/Error.cpp#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Error
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Error.h"


namespace Poco {
namespace DNSSD {


Error::Error():
	_networkInterface(0),
	_code(0)
{
}


Error::Error(Poco::Int32 networkInterface, Poco::Int32 code, const std::string& message):
	_networkInterface(networkInterface),
	_code(code),
	_message(message)
{
}


Error::~Error()
{
}


} } // namespace Poco::DNSSD
