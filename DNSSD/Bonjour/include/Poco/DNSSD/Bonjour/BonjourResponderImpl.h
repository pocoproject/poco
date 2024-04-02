//
// BonjourResponderImpl.h
//
// $Id: //poco/1.7/DNSSD/Bonjour/include/Poco/DNSSD/Bonjour/BonjourResponderImpl.h#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  BonjourResponderImpl
//
// Definition of the BonjourResponderImpl class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Bonjour_BonjourResponderImpl_INCLUDED
#define DNSSD_Bonjour_BonjourResponderImpl_INCLUDED


#include "Poco/DNSSD/Bonjour/Bonjour.h"
#include "Poco/DNSSD/Bonjour/BonjourBrowserImpl.h"
#include "Poco/DNSSD/Bonjour/EventLoop.h"
#include "Poco/DNSSD/DNSSDResponderImpl.h"
#include "Poco/DNSSD/DNSSDResponder.h"
#include "Poco/Thread.h"


namespace Poco {
namespace DNSSD {
namespace Bonjour {


class DNSSD_Bonjour_API BonjourResponderImpl: public Poco::DNSSD::DNSSDResponderImpl
	/// The DNSSDResponderImpl implementation for Bonjour.
{
public:
	BonjourResponderImpl(Poco::DNSSD::DNSSDResponder& owner);
		/// Creates the BonjourResponder, using the given owner.

	~BonjourResponderImpl();
		/// Destroys the BonjourResponderImpl.

	// DNSSDResponderImpl
	DNSSDBrowser& browser() override;
	ServiceHandle registerService(const Service& service, int options) override;
	void unregisterService(ServiceHandle& serviceHandle) override;
	RecordHandle addRecord(ServiceHandle serviceHandle, const Record& record) override;
	void updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record) override;
	void removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle) override;
	void start() override;
	void stop() override;

	// Implementation
	static const char* describeError(int code);
		/// Returns a human-readable string describing the error.

	void onRegisterServiceReply(DNSServiceRef sdRef, DNSServiceFlags flags, DNSServiceErrorType errorCode, const char* name, const char* regtype, const char* domain);

protected:
	static std::string createTXTRecord(const Service::Properties& properties);

private:
	Poco::DNSSD::DNSSDResponder& _owner;
	EventLoop _eventLoop;
	Poco::Thread _eventLoopThread;
	BonjourBrowserImpl _browser;
	bool _running;
};


class DNSSD_Bonjour_API BonjourResponderImplFactory: public Poco::DNSSD::DNSSDResponderImplFactory
	/// A factory for BonjourResponderImplFactory objects.
{
public:
	DNSSDResponderImpl* createResponderImpl(Poco::DNSSD::DNSSDResponder& owner)
	{
		return new BonjourResponderImpl(owner);
	}
};


} } } // namespace Poco::DNSSD::Bonjour


#endif // DNSSD_Bonjour_BonjourResponderImpl_INCLUDED
