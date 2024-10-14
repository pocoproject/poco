//
// AvahiBrowserImpl.cpp
//
// $Id: //poco/1.7/DNSSD/Avahi/src/AvahiBrowserImpl.cpp#1 $
//
// Library: DNSSD/Avahi
// Package: Implementation
// Module:  AvahiBrowserImpl
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Avahi/AvahiBrowserImpl.h"
#include "Poco/DNSSD/Avahi/AvahiResponderImpl.h"
#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/NumberFormatter.h"
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/address.h>
#include <iostream>


namespace Poco {
namespace DNSSD {
namespace Avahi {


extern "C" void onBrowseReply(
	AvahiServiceBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* name,
	const char* type,
	const char* domain,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onBrowseReply(browser, interface, protocol, event, name, type, domain, flags);
	}
	catch (...)
	{
	}
}


extern "C" void onResolveReply(
	AvahiServiceResolver* resolver,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiResolverEvent event,
	const char* name,
	const char* type,
	const char* domain,
	const char* host,
	const AvahiAddress* a,
	uint16_t port,
	AvahiStringList* txt,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onResolveReply(resolver, interface, protocol, event, name, type, domain, host, a, port, txt, flags);
	}
	catch (...)
	{
	}
}


extern "C" void onEnumerateBrowseDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onEnumerateBrowseDomainsReply(browser, interface, protocol, event, domain, flags, false);
	}
	catch (...)
	{
	}
}


extern "C" void onEnumerateDefaultBrowseDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onEnumerateBrowseDomainsReply(browser, interface, protocol, event, domain, flags, true);
	}
	catch (...)
	{
	}
	if (event == AVAHI_BROWSER_ALL_FOR_NOW || event == AVAHI_BROWSER_FAILURE)
	{
		avahi_domain_browser_free(browser);
	}
}


extern "C" void onEnumerateRegistrationDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onEnumerateRegistrationDomainsReply(browser, interface, protocol, event, domain, flags, false);
	}
	catch (...)
	{
	}
}


extern "C" void onEnumerateDefaultRegistrationDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onEnumerateRegistrationDomainsReply(browser, interface, protocol, event, domain, flags, true);
	}
	catch (...)
	{
	}
	if (event == AVAHI_BROWSER_ALL_FOR_NOW || event == AVAHI_BROWSER_FAILURE)
	{
		avahi_domain_browser_free(browser);
	}
}


extern "C" void onQueryRecordReply(
	AvahiRecordBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* name,
	uint16_t clazz,
	uint16_t type,
	const void* data,
	std::size_t size,
	AvahiLookupResultFlags flags,
	void* context)
{
	try
	{
		AvahiBrowserImpl* pBrowser = reinterpret_cast<AvahiBrowserImpl*>(context);
		pBrowser->onQueryRecordReply(browser, interface, protocol, event, name, clazz, type, data, size, flags);
	}
	catch (...)
	{
	}
}


AvahiBrowserImpl::AvahiBrowserImpl(AvahiResponderImpl& responder):
	_responder(responder)
{
}


AvahiBrowserImpl::~AvahiBrowserImpl()
{
}


BrowseHandle AvahiBrowserImpl::browse(const std::string& regType, const std::string& domain, int options, Poco::Int32 networkInterface)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AvahiIfIndex ifIndex = networkInterface == 0 ? AVAHI_IF_UNSPEC : networkInterface;
	AvahiServiceBrowser* browser = avahi_service_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, regType.c_str(), domain.empty() ? NULL : domain.c_str(), (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onBrowseReply, this);
	if (!browser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		throw Poco::DNSSD::DNSSDException("Failed to browse for " + regType, AvahiResponderImpl::describeError(error), error);
	}
	return BrowseHandle(browser, SERVICE_BROWSER_HANDLE);
}


BrowseHandle AvahiBrowserImpl::resolve(const Service& service, int options)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AvahiIfIndex ifIndex = service.networkInterface();
	AvahiServiceResolver* resolver = avahi_service_resolver_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, service.name().c_str(), service.type().c_str(), service.domain().c_str(), AVAHI_PROTO_UNSPEC, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onResolveReply, this);
	if (!resolver)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		throw Poco::DNSSD::DNSSDException("Failed to resolve service " + service.name(), AvahiResponderImpl::describeError(error), error);
	}
	return BrowseHandle(resolver, SERVICE_RESOLVER_HANDLE);
}


BrowseHandle AvahiBrowserImpl::enumerateBrowseDomains(Poco::Int32 networkInterface)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AvahiIfIndex ifIndex = networkInterface == 0 ? AVAHI_IF_UNSPEC : networkInterface;
	AvahiDomainBrowser* browser = avahi_domain_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, NULL, AVAHI_DOMAIN_BROWSER_BROWSE, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onEnumerateBrowseDomainsReply, this);
	if (!browser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		throw Poco::DNSSD::DNSSDException("Failed to enumerate browse domains", AvahiResponderImpl::describeError(error), error);
	}
	AvahiDomainBrowser* defaultBrowser = avahi_domain_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, NULL, AVAHI_DOMAIN_BROWSER_BROWSE_DEFAULT, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onEnumerateDefaultBrowseDomainsReply, this);
	if (!defaultBrowser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		avahi_domain_browser_free(browser);
		throw Poco::DNSSD::DNSSDException("Failed to enumerate default browse domains", AvahiResponderImpl::describeError(error), error);
	}
	return BrowseHandle(browser, DOMAIN_BROWSER_HANDLE);
}


BrowseHandle AvahiBrowserImpl::enumerateRegistrationDomains(Poco::Int32 networkInterface)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AvahiIfIndex ifIndex = networkInterface == 0 ? AVAHI_IF_UNSPEC : networkInterface;
	AvahiDomainBrowser* browser = avahi_domain_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, NULL, AVAHI_DOMAIN_BROWSER_REGISTER, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onEnumerateRegistrationDomainsReply, this);
	if (!browser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		throw Poco::DNSSD::DNSSDException("Failed to enumerate registration domains", AvahiResponderImpl::describeError(error), error);
	}
	AvahiDomainBrowser* defaultBrowser = avahi_domain_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, NULL, AVAHI_DOMAIN_BROWSER_REGISTER_DEFAULT, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onEnumerateDefaultRegistrationDomainsReply, this);
	if (!defaultBrowser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		avahi_domain_browser_free(browser);
		throw Poco::DNSSD::DNSSDException("Failed to enumerate default registration domains", AvahiResponderImpl::describeError(error), error);
	}
	return BrowseHandle(browser, DOMAIN_BROWSER_HANDLE);
}


BrowseHandle AvahiBrowserImpl::queryRecord(const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz, int options, Poco::Int32 networkInterface)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AvahiIfIndex ifIndex = networkInterface == 0 ? AVAHI_IF_UNSPEC : networkInterface;
	AvahiRecordBrowser* browser = avahi_record_browser_new(_responder._avahiClient, ifIndex, AVAHI_PROTO_UNSPEC, name.c_str(), clazz, type, (AvahiLookupFlags) 0, Poco::DNSSD::Avahi::onQueryRecordReply, this);
	if (!browser)
	{
		int error = avahi_client_errno(_responder._avahiClient);
		throw Poco::DNSSD::DNSSDException("Failed to query for record", AvahiResponderImpl::describeError(error), error);
	}
	return BrowseHandle(browser, RECORD_BROWSER_HANDLE);
}


BrowseHandle AvahiBrowserImpl::resolveHost(const std::string& host, int options, Poco::Int32 networkInterface)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	AddressMap::iterator it = _addressMap.find(host);
	if (it != _addressMap.end())
	{
		ResolveHostEventArgs args(BrowseHandle(), 0, host, it->second, networkInterface, 0);
		hostResolved(this, args);
	}
	else
	{
		int err = AVAHI_ERR_DNS_NXDOMAIN;
		Error error(networkInterface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(), 0, error);
		hostResolveError(this, args);
	}
	return BrowseHandle();
}


void AvahiBrowserImpl::cancel(BrowseHandle& browseHandle)
{
	if (!browseHandle.isValid()) return;

	AvahiResponderImpl::ScopedLock lock(_responder);

	switch (browseHandle.subtype())
	{
	case SERVICE_BROWSER_HANDLE:
		avahi_service_browser_free(browseHandle.cast<AvahiServiceBrowser*>());
		break;
	case SERVICE_RESOLVER_HANDLE:
		avahi_service_resolver_free(browseHandle.cast<AvahiServiceResolver*>());
		break;
	case DOMAIN_BROWSER_HANDLE:
		avahi_domain_browser_free(browseHandle.cast<AvahiDomainBrowser*>());
		break;
	case RECORD_BROWSER_HANDLE:
		avahi_record_browser_free(browseHandle.cast<AvahiRecordBrowser*>());
		break;
	}
	browseHandle.reset();
}


void AvahiBrowserImpl::onBrowseReply(
	AvahiServiceBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* name,
	const char* type,
	const char* domain,
	AvahiLookupResultFlags flags)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	if (event == AVAHI_BROWSER_NEW || event == AVAHI_BROWSER_REMOVE)
	{
		int eventFlags(0);
		Service service(interface, name, type, domain);
		ServiceEventArgs args(BrowseHandle(browser), eventFlags, service);
		if (event == AVAHI_BROWSER_NEW)
		{
			serviceFound(this, args);
		}
		else
		{
			serviceRemoved(this, args);
		}
	}
	else if (event == AVAHI_BROWSER_FAILURE)
	{
		int err = avahi_client_errno(_responder._avahiClient);
		Error error(interface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(browser), 0, error);
		browseError(this, args);
	}
}


void AvahiBrowserImpl::onResolveReply(
	AvahiServiceResolver* resolver,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiResolverEvent event,
	const char* name,
	const char* type,
	const char* domain,
	const char* host,
	const AvahiAddress* a,
	uint16_t port,
	AvahiStringList* txt,
	AvahiLookupResultFlags flags)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	if (event == AVAHI_RESOLVER_FOUND)
	{
		Poco::Net::IPAddress addr;
		switch (a->proto)
		{
		case AVAHI_PROTO_INET:
			addr = Poco::Net::IPAddress(&a->data.ipv4.address, sizeof(a->data.ipv4.address));
			break;
#if defined(POCO_HAVE_IPv6)
		case AVAHI_PROTO_INET6:
			addr = Poco::Net::IPAddress(&a->data.ipv6.address, sizeof(a->data.ipv6.address));
			break;
#endif
		}
		int eventFlags(0);
		std::string fullName;
		escape(name, fullName);
		fullName += '.';
		fullName += type;
		fullName += '.';
		fullName += domain;
		fullName += '.';
		Service service(interface, name, fullName, type, domain, host, port);
		_addressMap[service.host()] = addr;
		parseTXTRecord(txt, service.properties());
		ServiceEventArgs args(BrowseHandle(resolver), eventFlags, service);
		serviceResolved(this, args);
	}
	else if (event == AVAHI_RESOLVER_FAILURE)
	{
		int err = avahi_client_errno(_responder._avahiClient);
		Error error(interface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(resolver), 0, error);
		resolveError(this, args);
	}
}


void AvahiBrowserImpl::onEnumerateBrowseDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	bool isDefault)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	if (event == AVAHI_BROWSER_NEW || event == AVAHI_BROWSER_REMOVE)
	{
		int eventFlags(0);
		Poco::Int32 ifIndex = interface == AVAHI_IF_UNSPEC ? 0 : interface;
		Domain dom(ifIndex, domain, isDefault);
		DomainEventArgs args(BrowseHandle(browser), eventFlags, dom);
		if (event == AVAHI_BROWSER_NEW)
		{
			browseDomainFound(this, args);
		}
		else
		{
			browseDomainRemoved(this, args);
		}
	}
	else if (event == AVAHI_BROWSER_FAILURE)
	{
		int err = avahi_client_errno(_responder._avahiClient);
		Error error(interface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(browser), 0, error);
		browseDomainError(this, args);
	}
}


void AvahiBrowserImpl::onEnumerateRegistrationDomainsReply(
	AvahiDomainBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* domain,
	AvahiLookupResultFlags flags,
	bool isDefault)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	if (event == AVAHI_BROWSER_NEW || event == AVAHI_BROWSER_REMOVE)
	{
		int eventFlags(0);
		Poco::Int32 ifIndex = interface == AVAHI_IF_UNSPEC ? 0 : interface;
		Domain dom(ifIndex, domain, isDefault);
		DomainEventArgs args(BrowseHandle(browser), eventFlags, dom);
		if (event == AVAHI_BROWSER_NEW)
		{
			registrationDomainFound(this, args);
		}
		else
		{
			registrationDomainRemoved(this, args);
		}
	}
	else if (event == AVAHI_BROWSER_FAILURE)
	{
		int err = avahi_client_errno(_responder._avahiClient);
		Error error(interface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(browser), 0, error);
		registrationDomainError(this, args);
	}
}


void AvahiBrowserImpl::onQueryRecordReply(
	AvahiRecordBrowser* browser,
	AvahiIfIndex interface,
	AvahiProtocol protocol,
	AvahiBrowserEvent event,
	const char* name,
	uint16_t clazz,
	uint16_t type,
	const void* data,
	std::size_t size,
	AvahiLookupResultFlags flags)
{
	AvahiResponderImpl::ScopedLock lock(_responder);

	if (event == AVAHI_BROWSER_NEW || event == AVAHI_BROWSER_REMOVE)
	{
		int eventFlags(0);
		Poco::Int32 ifIndex = interface == AVAHI_IF_UNSPEC ? 0 : interface;
		Record record(ifIndex, name, type, clazz, size, data, 0);
		RecordEventArgs args(BrowseHandle(browser), eventFlags, record);
		if (event == AVAHI_BROWSER_NEW)
		{
			recordFound(this, args);
		}
		else
		{
			recordRemoved(this, args);
		}
	}
	else if (event == AVAHI_BROWSER_FAILURE)
	{
		int err = avahi_client_errno(_responder._avahiClient);
		Error error(interface, err, AvahiResponderImpl::describeError(err));
		ErrorEventArgs args(BrowseHandle(browser), 0, error);
		recordError(this, args);
	}
}


void AvahiBrowserImpl::parseTXTRecord(AvahiStringList* strList, Service::Properties& properties)
{
	while (strList)
	{
		char* key;
		char* value;
		std::size_t size;
		avahi_string_list_get_pair(strList, &key, &value, &size);
		properties.set(key, std::string(value ? value : "", size));
		avahi_free(key);
		avahi_free(value);
		strList = avahi_string_list_get_next(strList);
	}
}


void AvahiBrowserImpl::escape(const char* str, std::string& escaped)
{
	while (*str)
	{
		if (*str <= ' ' || *str >= 127)
		{
			escaped += '\\';
			Poco::NumberFormatter::append0(escaped, static_cast<unsigned>(static_cast<unsigned char>(*str)), 3);
		}
		else if (*str == '\\')
		{
			escaped += "\\\\";
		}
		else if (*str == '.')
		{
			escaped += "\\.";
		}
		else
		{
			escaped += *str;
		}
		++str;
	}
}


} } } // namespace Poco::DNSSD::Avahi

