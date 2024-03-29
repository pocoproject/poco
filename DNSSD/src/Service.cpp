//
// Service.cpp
//
// $Id: //poco/1.7/DNSSD/src/Service.cpp#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Service
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Service.h"


namespace Poco {
namespace DNSSD {


Service::Service():
	_networkInterface(0),
	_port(0)
{
}


Service::Service(Poco::Int32 networkInterface, const std::string& name, const std::string& type, const std::string& domain):
	_networkInterface(networkInterface),
	_name(name),
	_type(type),
	_domain(domain),
	_port(0)
{
}


Service::Service(const std::string& type, Poco::UInt16 port, const Properties& properties):
	_networkInterface(0),
	_type(type),
	_port(port),
	_properties(properties)
{
}


Service::Service(Poco::Int32 networkInterface, const std::string& name, const std::string& fullName,  const std::string& type, const std::string& domain, const std::string& host, Poco::UInt16 port):
	_networkInterface(networkInterface),
	_name(name),
	_fullName(fullName),
	_type(type),
	_domain(domain),
	_host(host),
	_port(port)
{
}


Service::Service(Poco::Int32 networkInterface, const std::string& name, const std::string& fullName, const std::string& type, const std::string& domain, const std::string& host, Poco::UInt16 port, const Properties& properties):
	_networkInterface(networkInterface),
	_name(name),
	_fullName(fullName),
	_type(type),
	_domain(domain),
	_host(host),
	_port(port),
	_properties(properties)
{
}


Service::~Service()
{
}


} } // namespace Poco::DNSSD
