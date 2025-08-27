module;

#ifdef ENABLE_DNSSD_BONJOUR
#include "Poco/DNSSD/Bonjour/Bonjour.h"
#include "Poco/DNSSD/Bonjour/BonjourBrowserImpl.h"
#include "Poco/DNSSD/Bonjour/BonjourResponderImpl.h"
#include "Poco/DNSSD/Bonjour/EventLoop.h"
#endif

export module Poco.DNSSD:Bonjour;

export namespace Poco::DNSSD {
	#ifdef ENABLE_DNSSD
	using Poco::DNSSD::initializeDNSSD;
	using Poco::DNSSD::uninitializeDNSSD;
	#endif

	namespace Bonjour {
		#ifdef ENABLE_DNSSD_BONJOUR
		using Poco::DNSSD::Bonjour::BonjourBrowserImpl;
		using Poco::DNSSD::Bonjour::BonjourResponderImpl;
		using Poco::DNSSD::Bonjour::BonjourResponderImplFactory;
		using Poco::DNSSD::Bonjour::EventLoop;
		#endif
	}
}
