//
// Record.cpp
//
// $Id: //poco/1.7/DNSSD/src/Record.cpp#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Record
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Record.h"


namespace Poco {
namespace DNSSD {


Record::Record():
	_networkInterface(0),
	_type(0),
	_clazz(0),
	_length(0),
	_data(0),
	_ttl(0)
{
}


Record::Record(Poco::Int32 networkInterface, const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz, Poco::UInt16 length, const void* data, Poco::UInt32 ttl):
	_networkInterface(networkInterface),
	_name(name),
	_type(type),
	_clazz(clazz),
	_length(length),
	_data(data),
	_ttl(ttl)
{
}


Record::Record(const std::string& name, Poco::UInt16 type, Poco::UInt16 length, const void* data, Poco::UInt32 ttl):
	_networkInterface(0),
	_name(name),
	_type(type),
	_clazz(RC_IN),
	_length(length),
	_data(data),
	_ttl(ttl)
{
}


Record::~Record()
{
}


} } // namespace Poco::DNSSD
