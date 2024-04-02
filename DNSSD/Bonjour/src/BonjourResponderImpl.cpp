//
// BonjourResponderImpl.cpp
//
// $Id: //poco/1.7/DNSSD/Bonjour/src/BonjourResponderImpl.cpp#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  BonjourResponderImpl
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Bonjour/BonjourResponderImpl.h"
#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/ByteOrder.h"
#include "Poco/Buffer.h"
#include <dns_sd.h>


namespace Poco {
namespace DNSSD {
namespace Bonjour {


extern "C" void DNSSD_API onRegisterServiceReply(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    DNSServiceErrorType errorCode,
    const char* name,
    const char* regtype,
    const char* domain,
    void *context)
{
	try
	{
		BonjourResponderImpl* pResponder = reinterpret_cast<BonjourResponderImpl*>(context);
		pResponder->onRegisterServiceReply(sdRef, flags, errorCode, name, regtype, domain);
	}
	catch (...)
	{
	}
}


BonjourResponderImpl::BonjourResponderImpl(Poco::DNSSD::DNSSDResponder& owner):
	_owner(owner),
	_browser(_eventLoop),
	_running(false)
{
}


BonjourResponderImpl::~BonjourResponderImpl()
{
	try
	{
		stop();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


DNSSDBrowser& BonjourResponderImpl::browser()
{
	return _browser;
}


ServiceHandle BonjourResponderImpl::registerService(const Service& service, int options)
{
	DNSServiceRef sdRef(nullptr);
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceFlags flags(0);
	uint32_t intf = (options & DNSSDResponder::REG_LOCAL_ONLY) ? kDNSServiceInterfaceIndexLocalOnly : service.networkInterface();
	if (options & DNSSDResponder::REG_NO_AUTORENAME) flags |= kDNSServiceFlagsNoAutoRename;
#if (_DNS_SD_H  <10960002)
	// kDNSServiceFlagsNonBrowsable was removed in 1096.0.2
	if (options & DNSSDResponder::REG_NON_BROWSABLE) flags |= kDNSServiceFlagsNonBrowsable;
#endif
	std::string txtRecord = createTXTRecord(service.properties());
	DNSServiceErrorType err = DNSServiceRegister(&sdRef, flags, intf, service.name().empty() ? 0 : service.name().c_str(), service.type().c_str(), service.domain().empty() ? 0 : service.domain().c_str(), service.host().empty() ? 0 : service.host().c_str(), Poco::ByteOrder::toNetwork(service.port()), txtRecord.size(), txtRecord.empty() ? 0 : txtRecord.data(), Poco::DNSSD::Bonjour::onRegisterServiceReply, this);
	if (err == kDNSServiceErr_NoError)
	{
		_eventLoop.add(sdRef);
		return ServiceHandle(sdRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to register " + service.type(), describeError(err), err);
}


void BonjourResponderImpl::unregisterService(ServiceHandle& serviceHandle)
{
	DNSServiceRef sdRef = serviceHandle.cast<DNSServiceRef>();
	_eventLoop.remove(sdRef);
	serviceHandle.reset();
}


RecordHandle BonjourResponderImpl::addRecord(ServiceHandle serviceHandle, const Record& record)
{
	DNSRecordRef recRef(0);
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceAddRecord(serviceHandle.cast<DNSServiceRef>(), &recRef, 0, record.type(), record.length(), record.data(), record.ttl());
	if (err == kDNSServiceErr_NoError)
	{
		return RecordHandle(recRef);
	}
	else throw Poco::DNSSD::DNSSDException("Failed to add record " + record.name(), describeError(err), err);
}


void BonjourResponderImpl::updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record)
{
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceUpdateRecord(serviceHandle.cast<DNSServiceRef>(), recordHandle.cast<DNSRecordRef>(), 0, record.length(), record.data(), record.ttl());
	if (err != kDNSServiceErr_NoError)
	{
		throw Poco::DNSSD::DNSSDException("Failed to update record " + record.name(), describeError(err), err);
	}
}


void BonjourResponderImpl::removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle)
{
	EventLoop::ScopedLock lock(_eventLoop);
	DNSServiceErrorType err = DNSServiceRemoveRecord(serviceHandle.cast<DNSServiceRef>(), recordHandle.cast<DNSRecordRef>(), 0);
	if (err == kDNSServiceErr_NoError)
	{
		recordHandle.reset();
	}
	else throw Poco::DNSSD::DNSSDException("Failed to remove record", describeError(err), err);
}


void BonjourResponderImpl::start()
{
	poco_assert (!_running);

	_eventLoopThread.start(_eventLoop);
	_running = true;
}


void BonjourResponderImpl::stop()
{
	if (_running)
	{
		_eventLoop.stop();
		_eventLoopThread.join();
		_eventLoop.shutdown();
		_running = false;
	}
}


const char* BonjourResponderImpl::describeError(int code)
{
	switch (code)
	{
    case kDNSServiceErr_Unknown:
    	return "Unknown";
    case kDNSServiceErr_NoSuchName:
    	return "No such name";
    case kDNSServiceErr_NoMemory:
    	return "No memory";
    case kDNSServiceErr_BadParam:
    	return "Bad parameter";
    case kDNSServiceErr_BadReference:
    	return "Bad reference";
    case kDNSServiceErr_BadState:
    	return "Bad state";
    case kDNSServiceErr_BadFlags:
    	return "Bad flags";
    case kDNSServiceErr_Unsupported:
    	return "Unsupported";
    case kDNSServiceErr_NotInitialized:
    	return "Not initialized";
    case kDNSServiceErr_AlreadyRegistered:
    	return "Already registered";
    case kDNSServiceErr_NameConflict:
    	return "Name conflict";
    case kDNSServiceErr_Invalid:
    	return "Invalid";
    case kDNSServiceErr_Firewall:
    	return "Firewall";
    case kDNSServiceErr_Incompatible:
    	return "Client library incompatible with daemon";
    case kDNSServiceErr_BadInterfaceIndex:
    	return "Bad interface index";
    case kDNSServiceErr_Refused:
    	return "Refused";
    case kDNSServiceErr_NoSuchRecord:
    	return "No such record";
    case kDNSServiceErr_NoAuth:
    	return "No auth";
    case kDNSServiceErr_NoSuchKey:
    	return "No such key";
    case kDNSServiceErr_NATTraversal:
    	return "NAT traversal";
    case kDNSServiceErr_DoubleNAT:
    	return "Double NAT";
    case kDNSServiceErr_BadTime:
    	return "Bad time";
    case kDNSServiceErr_BadSig:
    	return "Bad signature";
    case kDNSServiceErr_BadKey:
    	return "Bad key";
    case kDNSServiceErr_Transient:
    	return "Transient error";
    case kDNSServiceErr_ServiceNotRunning:
    	return "Service not running";
    case kDNSServiceErr_NATPortMappingUnsupported:
    	return "NAT port mapping not supported";
    case kDNSServiceErr_NATPortMappingDisabled:
    	return "NAT port mapping disabled";
#if _DNS_SD_H+0 >= 2580000
    case kDNSServiceErr_NoRouter:
    	return "No router";
    case kDNSServiceErr_PollingMode:
    	return "Polling mode";
#endif
	default:
		return "Error";
	}
}


std::string BonjourResponderImpl::createTXTRecord(const Service::Properties& properties)
{
	if (properties.empty()) return std::string(1, '\0');

	Service::Properties::ConstIterator it = properties.begin();
	Service::Properties::ConstIterator itEnd = properties.end();
	std::size_t requiredSize = 0;
	for (; it != itEnd; ++it)
	{
		if (it->first.size() == 0)
			throw Poco::InvalidArgumentException("Empty property name is not allowed");

		std::size_t prevSize = requiredSize;
		requiredSize += it->first.size() + 1; // add length byte
		if (!it->second.empty())
		{
			requiredSize += it->second.size() + 1; // add '=' character
		}
		if (requiredSize - prevSize > 256)
			throw Poco::InvalidArgumentException("Property too large: size of key and value together must not exceed 254 bytes");
	}

	if (requiredSize > 65535)
		throw Poco::InvalidArgumentException("Too many properties: maximum TXT record size of 65535 bytes exceeded");

	Poco::UInt16 size = static_cast<Poco::UInt16>(requiredSize);
	Poco::Buffer<char> buffer(requiredSize);

	TXTRecordRef ref;
	TXTRecordCreate(&ref, size, buffer.begin());

	// if present, txtvers must be first key in TXT record
	Service::Properties::ConstIterator itVers = properties.find("txtvers");
	if (itVers != itEnd)
	{
		Poco::UInt8 valueSize = static_cast<Poco::UInt8>(itVers->second.size());
		TXTRecordSetValue(&ref, itVers->first.c_str(), valueSize, valueSize == 0 ? 0 : itVers->second.c_str());
	}

	it = properties.begin();
	for (; it != itEnd; ++it)
	{
		if (it != itVers)
		{
			Poco::UInt8 valueSize = static_cast<Poco::UInt8>(it->second.size());
			TXTRecordSetValue(&ref, it->first.c_str(), valueSize, valueSize == 0 ? 0 : it->second.c_str());
		}
	}
	const void* txtRecord = TXTRecordGetBytesPtr(&ref);
	Poco::UInt16 txtSize = TXTRecordGetLength(&ref);
	std::string result(reinterpret_cast<const char*>(txtRecord), txtSize);
	TXTRecordDeallocate(&ref);

	return result;
}


void BonjourResponderImpl::onRegisterServiceReply(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    DNSServiceErrorType errorCode,
    const char* name,
    const char* regtype,
    const char* domain)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		Service service(0, name, regtype, domain);
		DNSSDResponder::ServiceEventArgs args(ServiceHandle(sdRef), service);
		_owner.serviceRegistered(this, args);
	}
	else
	{
		Service service(0, name, regtype, domain);
		Error error(0, errorCode, describeError(errorCode));
		DNSSDResponder::ErrorEventArgs args(ServiceHandle(sdRef), service, error);
		_owner.serviceRegistrationFailed(this, args);
	}
}


} } } // namespace Poco::DNSSD::Bonjour


namespace Poco {
namespace DNSSD {


namespace
{
	Poco::DNSSD::Bonjour::BonjourResponderImplFactory implFactory;
}


void initializeDNSSD()
{
	Poco::DNSSD::DNSSDResponder::registerImplFactory(implFactory);
}


void uninitializeDNSSD()
{
	Poco::DNSSD::DNSSDResponder::unregisterImplFactory();
}


} } // namespace Poco::DNSSD
