//
// Domain.cpp
//
// $Id: //poco/1.7/DNSSD/src/Domain.cpp#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Domain
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Domain.h"


namespace Poco {
namespace DNSSD {


Domain::Domain():
	_networkInterface(0),
	_isDefault(false)
{
}


Domain::Domain(Poco::Int32 networkInterface, const std::string& name, bool isDefault):
	_networkInterface(networkInterface),
	_name(name),
	_isDefault(isDefault)
{
}


Domain::~Domain()
{
}


} } // namespace Poco::DNSSD
