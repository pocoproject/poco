//
// AvahiResponderImpl.h
//
// $Id: //poco/1.7/DNSSD/Avahi/include/Poco/DNSSD/Avahi/AvahiResponderImpl.h#1 $
//
// Library: DNSSD/Avahi
// Package: Implementation
// Module:  AvahiResponderImpl
//
// Definition of the AvahiResponderImpl class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Avahi_AvahiResponderImpl_INCLUDED
#define DNSSD_Avahi_AvahiResponderImpl_INCLUDED


#include "Poco/DNSSD/Avahi/Avahi.h"
#include "Poco/DNSSD/Avahi/AvahiBrowserImpl.h"
#include "Poco/DNSSD/DNSSDResponderImpl.h"
#include "Poco/DNSSD/DNSSDResponder.h"
#include "Poco/ScopedLock.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <map>
#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/simple-watch.h>


namespace Poco {
namespace DNSSD {
namespace Avahi {


class AvahiBrowserImpl;

class DNSSD_Avahi_API AvahiResponderImpl: public Poco::DNSSD::DNSSDResponderImpl, public Poco::Runnable
	/// The DNSSDResponderImpl implementation for Avahi.
{
public:
	typedef Poco::ScopedLock<AvahiResponderImpl> ScopedLock;

	AvahiResponderImpl(Poco::DNSSD::DNSSDResponder& owner);
		/// Creates the AvahiResponder, using the given owner.

	~AvahiResponderImpl();
		/// Destroys the AvahiResponderImpl.

	// DNSSDResponderImpl
	DNSSDBrowser& browser();
	ServiceHandle registerService(const Service& service, int options);
	void unregisterService(ServiceHandle& serviceHandle);
	RecordHandle addRecord(ServiceHandle serviceHandle, const Record& record);
	void updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record);
	void removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle);
	void start();
	void stop();

	// Runnable
	void run();

	// Implementation
	static const char* describeError(int code);
		/// Returns a human-readable string describing the error.

	void onClientStateChange(AvahiClientState state);
	void onGroupStateChange(AvahiEntryGroup* avahiGroup, AvahiEntryGroupState state);
	void lock();
	void unlock();

protected:
	enum
	{
		START_TIMEOUT = 5000 // milliseconds
	};

	struct RecordInfo
	{
		Record record;
		int    id;
	};
	typedef std::vector<RecordInfo> RecordVec;

	struct ServiceInfo
	{
		Service   service;
		RecordVec records;
		int       options;
	};
	typedef std::map<ServiceHandle, ServiceInfo> ServiceMap;

	void reregisterServices();
	void setupEntryGroup(AvahiEntryGroup* avahiGroup, const Service& service, const RecordVec& records, int options, bool rename);
	static AvahiStringList* createTXTRecord(const Service::Properties& properties);

private:
	Poco::DNSSD::DNSSDResponder& _owner;
	AvahiBrowserImpl _browser;
	AvahiSimplePoll* _avahiPoll;
	AvahiClient* _avahiClient;
	Poco::Event _avahiClientReady;
	ServiceMap _services;
	bool _running;
	int _nextRecordId;
	Poco::Mutex _mutex;
	Poco::Thread _pollThread;

	friend class AvahiBrowserImpl;
};


class DNSSD_Avahi_API AvahiResponderImplFactory: public Poco::DNSSD::DNSSDResponderImplFactory
	/// A factory for AvahiResponderImplFactory objects.
{
public:
	DNSSDResponderImpl* createResponderImpl(Poco::DNSSD::DNSSDResponder& owner)
	{
		return new AvahiResponderImpl(owner);
	}
};


} } } // namespace Poco::DNSSD::Avahi


#endif // DNSSD_Avahi_AvahiResponderImpl_INCLUDED
