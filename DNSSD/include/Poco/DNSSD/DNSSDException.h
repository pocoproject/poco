//
// DNSSDException.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/DNSSDException.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSDException
//
// Definition of the DNSSDException class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_DNSSDException_INCLUDED
#define DNSSD_DNSSDException_INCLUDED


#include "Poco/DNSSD/DNSSD.h"
#include "Poco/Exception.h"


namespace Poco {
namespace DNSSD {


POCO_DECLARE_EXCEPTION(DNSSD_API, DNSSDException, Poco::RuntimeException)


} } // namespace Poco::DNSSD


#endif // DNSSD_DNSSDException_INCLUDED
