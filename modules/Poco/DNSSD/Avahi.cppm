//
// Avahi.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_DNSSD_AVAHI
#include "Poco/DNSSD/Avahi/Avahi.h"
#include "Poco/DNSSD/Avahi/AvahiBrowserImpl.h"
#include "Poco/DNSSD/Avahi/AvahiResponderImpl.h"
#endif

export module Poco.DNSSD:Avahi;

export namespace Poco::DNSSD {
	#ifdef ENABLE_DNSSD
	using Poco::DNSSD::initializeDNSSD;
	using Poco::DNSSD::uninitializeDNSSD;
	#endif

	namespace Avahi {
		#ifdef ENABLE_DNSSD_AVAHI
		using Poco::DNSSD::Avahi::AvahiBrowserImpl;
		using Poco::DNSSD::Avahi::AvahiResponderImpl;
		using Poco::DNSSD::Avahi::AvahiResponderImplFactory;
		#endif
	}
}
