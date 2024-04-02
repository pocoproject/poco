//
// BonjourBrowserImpl.cpp
//
// $Id: //poco/1.7/DNSSD/Bonjour/src/BonjourBrowserImpl.cpp#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  BonjourBrowserImpl
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Bonjour/BonjourBrowserImpl.h"
#include "Poco/DNSSD/Bonjour/BonjourResponderImpl.h"
#include "Poco/DNSSD/Bonjour/EventLoop.h"
#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/ByteOrder.h"
#include <dns_sd.h>


namespace Poco {
namespace DNSSD {
namespace Bonjour {


extern "C" void DNSSD_API onBrowseReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* serviceName,
	const char* regtype,
	const char* replyDomain,
	void* context)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onBrowseReply(sdRef, flags, interfaceIndex, errorCode, serviceName, regtype, replyDomain);
	}
	catch (...)
	{
	}
}


extern "C" void DNSSD_API onResolveReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* fullName,
	const char* host,
	uint16_t port,
	uint16_t txtLen,
#if _DNS_SD_H+0 >= 1070600
	const unsigned char* txtRecord,
#else
	const char* txtRecord,
#endif
	void* context
	)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onResolveReply(sdRef, flags, interfaceIndex, errorCode, fullName, host, port, txtLen, (const unsigned char*) txtRecord);
	}
	catch (...)
	{
	}
}


extern "C" void DNSSD_API onEnumerateBrowseDomainsReply(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    uint32_t interfaceIndex,
    DNSServiceErrorType errorCode,
    const char* replyDomain,
    void* context)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onEnumerateBrowseDomainsReply(sdRef, flags, interfaceIndex, errorCode, replyDomain);
	}
	catch (...)
	{
	}
}


extern "C" void DNSSD_API onEnumerateRegistrationDomainsReply(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    uint32_t interfaceIndex,
    DNSServiceErrorType errorCode,
    const char* replyDomain,
    void* context)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onEnumerateRegistrationDomainsReply(sdRef, flags, interfaceIndex, errorCode, replyDomain);
	}
	catch (...)
	{
	}
}


extern "C" void DNSSD_API onQueryRecordReply(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    uint32_t interfaceIndex,
    DNSServiceErrorType errorCode,
    const char* fullName,
    uint16_t rrtype,
    uint16_t rrclass,
    uint16_t rdlen,
    const void* rdata,
    uint32_t ttl,
    void* context)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onQueryRecordReply(sdRef, flags, interfaceIndex, errorCode, fullName, rrtype, rrclass, rdlen, rdata, ttl);
	}
	catch (...)
	{
	}
}


extern "C" void DNSSD_API onResolveHostReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* hostname,
	const struct sockaddr* address,
	uint32_t ttl,
	void* context)
{
	try
	{
		BonjourBrowserImpl* pBrowser = reinterpret_cast<BonjourBrowserImpl*>(context);
		pBrowser->onResolveHostReply(sdRef, flags, interfaceIndex, errorCode, hostname, address, ttl);
	}
	catch (...)
	{
	}
}


BonjourBrowserImpl::BonjourBrowserImpl(EventLoop& eventLoop):
	_eventLoop(eventLoop)
{
}


BonjourBrowserImpl::~BonjourBrowserImpl()
{
}


BrowseHandle BonjourBrowserImpl::browse(const std::string& regType, const std::string& domain, int options, Poco::Int32 networkInterface)
{
	DNSServiceRef sdRef(nullptr);
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceBrowse(&sdRef, 0, networkInterface, regType.c_str(), domain.empty() ? 0 : domain.c_str(), Poco::DNSSD::Bonjour::onBrowseReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to browse for " + regType, BonjourResponderImpl::describeError(err), err);
}


BrowseHandle BonjourBrowserImpl::resolve(const Service& service, int options)
{
	DNSServiceRef sdRef(0);
	Poco::Int32 ifIndex = (options & RESOLVE_ON_ALL_INTERFACES) ? 0 : service.networkInterface();
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceResolve(&sdRef, 0, ifIndex, service.name().c_str(), service.type().c_str(), service.domain().c_str(), Poco::DNSSD::Bonjour::onResolveReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_serviceMap[sdRef] = service;
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to resolve " + service.name(), BonjourResponderImpl::describeError(err), err);
}


BrowseHandle BonjourBrowserImpl::enumerateBrowseDomains(Poco::Int32 networkInterface)
{
	DNSServiceRef sdRef(0);
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceEnumerateDomains(&sdRef, kDNSServiceFlagsBrowseDomains, networkInterface, Poco::DNSSD::Bonjour::onEnumerateBrowseDomainsReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to enumerate browse domains", BonjourResponderImpl::describeError(err), err);
}


BrowseHandle BonjourBrowserImpl::enumerateRegistrationDomains(Poco::Int32 networkInterface)
{
	DNSServiceRef sdRef(0);
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceEnumerateDomains(&sdRef, kDNSServiceFlagsRegistrationDomains, networkInterface, Poco::DNSSD::Bonjour::onEnumerateRegistrationDomainsReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to enumerate registration domains", BonjourResponderImpl::describeError(err), err);
}


BrowseHandle BonjourBrowserImpl::queryRecord(const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz, int options, Poco::Int32 networkInterface)
{
	DNSServiceRef sdRef(nullptr);
	DNSServiceFlags flags(0);
	if (options & BROWSE_FORCE_MULTICAST) flags |= kDNSServiceFlagsForceMulticast;
	if (options & BROWSE_LONG_LIVED_QUERY) flags |= kDNSServiceFlagsLongLivedQuery;
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceQueryRecord(&sdRef, flags, networkInterface, name.c_str(), type, clazz, Poco::DNSSD::Bonjour::onQueryRecordReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to query record " + name, BonjourResponderImpl::describeError(err), err);
}


BrowseHandle BonjourBrowserImpl::resolveHost(const std::string& host, int options, Poco::Int32 networkInterface)
{
	DNSServiceRef sdRef(0);
	DNSServiceFlags flags(0);
	if (options & BROWSE_FORCE_MULTICAST) flags |= kDNSServiceFlagsForceMulticast;
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceGetAddrInfo(&sdRef, flags, networkInterface, 0, host.c_str(), Poco::DNSSD::Bonjour::onResolveHostReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return BrowseHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to resolve host " + host, BonjourResponderImpl::describeError(err), err);
}


void BonjourBrowserImpl::cancel(BrowseHandle& browseHandle)
{
	DNSServiceRef sdRef = browseHandle.cast<DNSServiceRef>();
	ServiceMap::iterator it = _serviceMap.find(sdRef);
	if (it != _serviceMap.end()) _serviceMap.erase(it);
	_eventLoop.remove(sdRef);
	browseHandle.reset();
}


void BonjourBrowserImpl::onBrowseReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* serviceName,
	const char* regtype,
	const char* replyDomain)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
		Service service(interfaceIndex, serviceName, regtype, replyDomain);
		ServiceEventArgs args(BrowseHandle(sdRef), eventFlags, service);
		if (flags & kDNSServiceFlagsAdd)
		{
			serviceFound(this, args);
		}
		else
		{
			serviceRemoved(this, args);
		}
	}
	else
	{
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		browseError(this, args);
	}
}


void BonjourBrowserImpl::onResolveReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* fullName,
	const char* host,
	uint16_t port,
	uint16_t txtLen,
	const unsigned char* txtRecord)
{
	ServiceMap::iterator it = _serviceMap.find(sdRef);
	_eventLoop.remove(sdRef);
	if (errorCode == kDNSServiceErr_NoError)
	{
		if (it != _serviceMap.end())
		{
			Service service(interfaceIndex, it->second.name(), fullName, it->second.type(), it->second.domain(), host, Poco::ByteOrder::fromNetwork(port));
			_serviceMap.erase(it);
			parseTXTRecord(txtLen, txtRecord, service.properties());
			int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
			ServiceEventArgs args(BrowseHandle(sdRef), eventFlags, service);
			serviceResolved(this, args);
		}
	}
	else
	{
		if (it != _serviceMap.end()) _serviceMap.erase(it);
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		resolveError(this, args);
	}
}


void BonjourBrowserImpl::onEnumerateBrowseDomainsReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* replyDomain)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
		Domain domain(interfaceIndex, replyDomain, (flags & kDNSServiceFlagsDefault) != 0);
		DomainEventArgs args(BrowseHandle(sdRef), eventFlags, domain);
		if (flags & kDNSServiceFlagsAdd)
		{
			browseDomainFound(this, args);
		}
		else
		{
			browseDomainRemoved(this, args);
		}
	}
	else
	{
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		browseDomainError(this, args);
	}
}


void BonjourBrowserImpl::onEnumerateRegistrationDomainsReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* replyDomain)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
		Domain domain(interfaceIndex, replyDomain, (flags & kDNSServiceFlagsDefault) != 0);
		DomainEventArgs args(BrowseHandle(sdRef), eventFlags, domain);
		if (flags & kDNSServiceFlagsAdd)
		{
			registrationDomainFound(this, args);
		}
		else
		{
			registrationDomainRemoved(this, args);
		}
	}
	else
	{
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		registrationDomainError(this, args);
	}
}


void BonjourBrowserImpl::onQueryRecordReply(
	DNSServiceRef sdRef,
	DNSServiceFlags flags,
	uint32_t interfaceIndex,
	DNSServiceErrorType errorCode,
	const char* fullName,
	uint16_t rrtype,
	uint16_t rrclass,
	uint16_t rdlen,
	const void* rdata,
	uint32_t ttl)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
		Record record(interfaceIndex, fullName, rrtype, rrclass, rdlen, rdata, ttl);
		RecordEventArgs args(BrowseHandle(sdRef), eventFlags, record);
		if (flags & kDNSServiceFlagsAdd)
		{
			recordFound(this, args);
		}
		else
		{
			recordRemoved(this, args);
		}
	}
	else
	{
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		recordError(this, args);
	}
}


void BonjourBrowserImpl::onResolveHostReply(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char* hostname, const struct sockaddr* address, uint32_t ttl)
{
	_eventLoop.remove(sdRef);
	if (errorCode == kDNSServiceErr_NoError)
	{
		Poco::Net::IPAddress addr;
		switch (address->sa_family)
		{
		case AF_INET:
			addr = Poco::Net::IPAddress(&reinterpret_cast<const struct sockaddr_in*>(address)->sin_addr, sizeof(in_addr));
			break;
#if defined(POCO_HAVE_IPv6)
		case AF_INET6:
			addr = Poco::Net::IPAddress(&reinterpret_cast<const struct sockaddr_in6*>(address)->sin6_addr, sizeof(in6_addr));
			break;
#endif
		}
		int eventFlags((flags & kDNSServiceFlagsMoreComing) ? BROWSE_MORE_COMING : 0);
		std::string host(hostname);
		ResolveHostEventArgs args(BrowseHandle(sdRef), eventFlags, host, addr, interfaceIndex, ttl);
		hostResolved(this, args);
	}
	else
	{
		Error error(interfaceIndex, errorCode, BonjourResponderImpl::describeError(errorCode));
		ErrorEventArgs args(BrowseHandle(sdRef), 0, error);
		hostResolveError(this, args);
	}
}


void BonjourBrowserImpl::parseTXTRecord(Poco::UInt16 length, const void* data, Service::Properties& properties)
{
	Poco::UInt16 n = TXTRecordGetCount(length, data);
	char key[256];
	std::string strKey;
	for (Poco::UInt16 i = 0; i < n; ++i)
	{
		Poco::UInt8 valueLength = 0;
		const void* value;
		TXTRecordGetItemAtIndex(length, data, i, sizeof(key), key, &valueLength, &value);
		strKey.assign(key);
		if (!strKey.empty()) // An empty TXT record will contain one key with an empty name.
		{
			properties.add(strKey, std::string(value ? reinterpret_cast<const char*>(value) : "", valueLength));
		}
	}
}


} } } // namespace Poco::DNSSD::Bonjour
