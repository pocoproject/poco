//
// ZeroconfException.cpp
//
// $Id: //poco/1.7/DNSSD/src/DNSSDException.cpp#1 $
//
// Library: Zeroconf
// Package: Core
// Module:  ZeroconfException
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/DNSSDException.h"
#include <typeinfo>


namespace Poco {
namespace DNSSD {


POCO_IMPLEMENT_EXCEPTION(DNSSDException, Poco::RuntimeException, "DNSSD Exception")


} } // namespace Poco::DNSSD
