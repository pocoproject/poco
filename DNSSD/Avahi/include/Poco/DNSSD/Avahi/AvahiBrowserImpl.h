//
// AvahiBrowserImpl.h
//
// $Id: //poco/1.7/DNSSD/Avahi/include/Poco/DNSSD/Avahi/AvahiBrowserImpl.h#1 $
//
// Library: DNSSD/Avahi
// Package: Implementation
// Module:  AvahiBrowserImpl
//
// Definition of the AvahiBrowserImpl class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Avahi_AvahiBrowserImpl_INCLUDED
#define DNSSD_Avahi_AvahiBrowserImpl_INCLUDED


#include "Poco/DNSSD/Avahi/Avahi.h"
#include "Poco/DNSSD/DNSSDBrowser.h"
#include <avahi-client/lookup.h>
#include <map>


namespace Poco {
namespace DNSSD {
namespace Avahi {


class AvahiResponderImpl;


class DNSSD_Avahi_API AvahiBrowserImpl: public DNSSDBrowser
	/// The DNSSDBrowser implementation for Avahi.
{
public:
	AvahiBrowserImpl(AvahiResponderImpl& responder);
		/// Creates the AvahiBrowserImpl.

	~AvahiBrowserImpl();
		/// Destroys the AvahiBrowserImpl.

	// DNSSDBrowser
	BrowseHandle browse(const std::string& regType, const std::string& domain, int options, Poco::Int32 networkInterface);
	BrowseHandle resolve(const Service& service, int options);
	BrowseHandle enumerateBrowseDomains(Poco::Int32 networkInterface);
	BrowseHandle enumerateRegistrationDomains(Poco::Int32 networkInterface);
	BrowseHandle queryRecord(const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz, int options, Poco::Int32 networkInterface);
	BrowseHandle resolveHost(const std::string& host, int options, Poco::Int32 networkInterface);
	void cancel(BrowseHandle& browseHandle);

	// Implementation
	void onBrowseReply(AvahiServiceBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* name, const char* type, const char* domain, AvahiLookupResultFlags flags);
	void onResolveReply(AvahiServiceResolver* resolver, AvahiIfIndex interface, AvahiProtocol protocol, AvahiResolverEvent event, const char* name, const char* type, const char* domain, const char* host, const AvahiAddress* a, uint16_t port, AvahiStringList* txt, AvahiLookupResultFlags flags);
	void onEnumerateBrowseDomainsReply(AvahiDomainBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* domain, AvahiLookupResultFlags flags, bool isDefault);
	void onEnumerateRegistrationDomainsReply(AvahiDomainBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* domain, AvahiLookupResultFlags flags, bool isDefault);
    void onQueryRecordReply(AvahiRecordBrowser* browser, AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event, const char* name, uint16_t clazz, uint16_t type, const void* data, std::size_t size, AvahiLookupResultFlags flags);

protected:
	enum HandleTypes
	{
		DOMAIN_BROWSER_HANDLE,
		SERVICE_BROWSER_HANDLE,
		SERVICE_RESOLVER_HANDLE,
		RECORD_BROWSER_HANDLE
	};

	static void parseTXTRecord(AvahiStringList* strList, Service::Properties& properties);
	static void escape(const char* str, std::string& escaped);

private:
	typedef std::map<std::string, Poco::Net::IPAddress> AddressMap;

	AvahiResponderImpl& _responder;
	AddressMap _addressMap;
};


} } } // namespace Poco::DNSSD::Avahi


#endif // DNSSD_Avahi_AvahiBrowserImpl_INCLUDED
