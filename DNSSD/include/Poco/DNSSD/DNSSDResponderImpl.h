//
// DNSSDResponderImpl.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/DNSSDResponderImpl.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSDResponderImpl
//
// Definition of the DNSSDResponderImpl class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_DNSSDResponderImpl_INCLUDED
#define DNSSD_DNSSDResponderImpl_INCLUDED


#include "Poco/DNSSD/DNSSD.h"


namespace Poco {
namespace DNSSD {


class DNSSDBrowser;
class DNSSDResponder;
class Service;
class Record;


class DNSSD_API DNSSDResponderImpl
	/// DNSSDResponderImpl subclasses implement the actual binding
	/// to the underlying DNSSD engine (e.g., Bonjour or Avahi).
{
public:
	virtual ~DNSSDResponderImpl();
		/// Destroys the DNSSDResponderImpl.

	virtual DNSSDBrowser& browser() = 0;
		/// Returns the DNSSDBrowser, which is used to
		/// discover and resolve services and domains.

	virtual ServiceHandle registerService(const Service& service, int options) = 0;
		/// Registers a service.
		///
		/// Service registration will be asynchronously. When the service
		/// has been registered successfully, a serviceRegistered event
		/// will be fired. Otherwise, a ServiceRegistrationFailed event
		/// will be fired.
		///
		/// Returns a ServiceHandle that can later be used to unregister
		/// the service, or to add DNS records to the service.

	virtual void unregisterService(ServiceHandle& serviceHandle) = 0;
		/// Unregisters the service specified by serviceHandle.
		///
		/// The ServiceHandle is invalidated.

	virtual RecordHandle addRecord(ServiceHandle serviceHandle, const Record& record) = 0;
		/// Add a record to a registered service. The name of the record will be the same as the
		/// registered service's name.
		///
		/// The record can later be updated or deregistered by passing the RecordHandle returned
		/// by this function to updateRecord() or removeRecord().

	virtual void updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record) = 0;
		/// Update a registered resource record. The record must either be:
		///   - the primary txt record of a service registered via registerService()
		///     (if recordHandle is a null handle), or
		///   - a record added to a registered service via addRecord().

	virtual void removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle) = 0;
		/// Remove a registered resource record. The record must either be:
		/// a record added to a registered service via addRecord().
		///
		/// The RecordHandle is invalidated.

	virtual void start() = 0;
		/// Starts the responder.
		///
		/// Must be called before services can be registered
		/// or before browsing for domains and services.

	virtual void stop() = 0;
		/// Stops the responder.

protected:
	DNSSDResponderImpl();

private:
	DNSSDResponderImpl(const DNSSDResponderImpl&);
	DNSSDResponderImpl& operator = (const DNSSDResponderImpl&);
};


class DNSSD_API DNSSDResponderImplFactory
	/// A factory for DNSSDResponderImpl objects.
	///
	/// A subclass of this class must be provided by DNSSD
	/// implementations and registered with the DNSSDResponder class.
{
public:
	virtual DNSSDResponderImpl* createResponderImpl(DNSSDResponder& owner) = 0;
		/// Creates a new DNSSDResponderImpl.

protected:
	virtual ~DNSSDResponderImplFactory();
};


} } // namespace Poco::DNSSD


#endif // DNSSD_DNSSDResponderImpl_INCLUDED
