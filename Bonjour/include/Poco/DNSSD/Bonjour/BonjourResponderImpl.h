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
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
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


class POCO_DNSSD_Bonjour_API BonjourResponderImpl: public Poco::DNSSD::DNSSDResponderImpl
	/// The DNSSDResponderImpl implementation for Bonjour.
{
public:
	BonjourResponderImpl(Poco::DNSSD::DNSSDResponder& owner);
		/// Creates the BonjourResponder, using the given owner.
		
	~BonjourResponderImpl();
		/// Destroys the BonjourResponderImpl.

	// DNSSDResponderImpl
	DNSSDBrowser& browser();
	ServiceHandle registerService(const Service& service, int options);	
	void unregisterService(ServiceHandle& serviceHandle);	
	RecordHandle addRecord(ServiceHandle serviceHandle, const Record& record);
	void updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record);
	void removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle);
	void start();
	void stop();

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


class POCO_DNSSD_Bonjour_API BonjourResponderImplFactory: public Poco::DNSSD::DNSSDResponderImplFactory
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
