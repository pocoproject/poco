//
// DNSSD.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_DNSSD
#include "Poco/DNSSD/DNSSD.h"
#include "Poco/DNSSD/DNSSDBrowser.h"
#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/DNSSD/DNSSDResponder.h"
#include "Poco/DNSSD/DNSSDResponderImpl.h"
#include "Poco/DNSSD/Domain.h"
#include "Poco/DNSSD/Error.h"
#include "Poco/DNSSD/Record.h"
#include "Poco/DNSSD/Service.h"
#endif

export module Poco.DNSSD;

#ifdef ENABLE_DNSSD_AVAHI
export import :Avahi;
#endif
#ifdef ENABLE_DNSSD_BONJOUR
export import :Bonjour;
#endif

export namespace Poco::DNSSD {
	#ifdef ENABLE_DNSSD
	using Poco::DNSSD::DNSSDBrowser;
	using Poco::DNSSD::DNSSDException;
	using Poco::DNSSD::DNSSDResponder;
	using Poco::DNSSD::DNSSDResponderImpl;
	using Poco::DNSSD::DNSSDResponderImplFactory;
	using Poco::DNSSD::Domain;
	using Poco::DNSSD::Error;
	using Poco::DNSSD::HandleType;
	using Poco::DNSSD::OpaqueHandle;
	using Poco::DNSSD::Record;
	using Poco::DNSSD::Service;

	using Poco::DNSSD::BrowserHandle;
	using Poco::DNSSD::RecordHandle;
	using Poco::DNSSD::ServiceHandle;
	#endif
}
