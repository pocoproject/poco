//
// DNSSDResponder.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/DNSSDResponder.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSDResponder
//
// Definition of the DNSSDResponder class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_DNSSDResponder_INCLUDED
#define DNSSD_DNSSDResponder_INCLUDED


#include "Poco/DNSSD/DNSSD.h"
#include "Poco/DNSSD/Service.h"
#include "Poco/DNSSD/Record.h"
#include "Poco/DNSSD/Error.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace DNSSD {


class DNSSDBrowser;
class DNSSDResponderImpl;
class DNSSDResponderImplFactory;


class DNSSD_API DNSSDResponder
	/// DNSSDResponder provides a unified interface to the underlying
	/// DNS Service Discovery implementation, which can be Apple's Bonjour
	/// or Avahi.
	///
	/// An application should not create more than one instance of
	/// the DNSSDResponder class.
	///
	/// To register a service with the DNSSDResponder, and thus to
	/// announce a service on the network, create an instance
	/// of Service with appropriate values. Then pass this object
	/// to registerService(). Example:
	///
	///     DNSSDResponder dnssdResponder;
	///     dnssdResponder.start():
	///     ...
	///     Service::Properties props;
	///     Service myService(0, "My Service", "_mysvc._tcp", "", "", 1234, props);
	///     ServiceHandle myServiceHandle = dnssdResponder.registerService(myService);
	///
	/// Note that service registration is asynchronous, so the serviceRegistered
	/// and serviceRegistrationFailed events must be observed to ensure the
	/// service has actually been registered.
	///
	/// A registered service can be unregistered, by passing its ServiceHandle
	/// to unregisterService().
	///
	/// Before a DNSSDResponder instance can be created, registerImplFactory() must
	/// be called to register a DNSSDResponderImplFactory. This is done by
	/// calling the initializeDNSSD() function provided by an implementation library
	/// (e.g., Bonjour or Avahi).
{
public:
	enum RegistrationOptions
		/// Options for service registration.
	{
		REG_LOCAL_ONLY    = 0x01, /// Service is visible on local host only.
		REG_NO_AUTORENAME = 0x02, /// Do not allow automatic renaming in case another service with the same name exists.
		REG_NON_BROWSABLE = 0x04  /// Service is not visible when browsing, but can be resolved (only for DNSSD version < 1096.0.2).
	};

	struct ServiceEventArgs
	{
		ServiceEventArgs(ServiceHandle h, const Service& s):
			serviceHandle(h),
			service(s)
		{
		}

		ServiceHandle  serviceHandle;
		const Service& service;
	};

	struct ErrorEventArgs
	{
		ErrorEventArgs(ServiceHandle h, const Service& s, const Error& e):
			serviceHandle(h),
			service(s),
			error(e)
		{
		}

		ServiceHandle  serviceHandle;
		const Service& service;
		const Error&   error;
	};

	Poco::BasicEvent<const ServiceEventArgs> serviceRegistered;
		/// Fired after the service has been registered successfully.
		///
		/// If auto-rename has been enabled, the service name may be different from the
		/// name originally specified.

	Poco::BasicEvent<const ErrorEventArgs> serviceRegistrationFailed;
		/// Fired when service registration fails.

	DNSSDResponder();
		/// Creates a DNSSDResponder.

	~DNSSDResponder();
		/// Destroys the DNSSDResponder.

	DNSSDBrowser& browser();
		/// Returns the DNSServiceBrowser, which is used to
		/// discover and resolve services and domains.

	ServiceHandle registerService(const Service& service, int options = 0);
		/// Registers a service, specified by the given Service object.
		///
		/// Valid option values are defined in the RegistrationOptions enumeration.
		///
		/// Service registration will be asynchronously. When the service
		/// has been registered successfully, a serviceRegistered event
		/// will be fired. Otherwise, a ServiceRegistrationFailed event
		/// will be fired.
		///
		/// Returns a ServiceHandle that can later be used to unregister
		/// the service, or to add DNS records to the service.

	void unregisterService(ServiceHandle& serviceHandle);
		/// Unregisters the service specified by serviceHandle.
		///
		/// The ServiceHandle is invalidated.

	RecordHandle addRecord(ServiceHandle serviceHandle, const Record& record);
		/// Add a record to a registered service. The name of the record will be the same as the
		/// registered service's name.
		///
		/// The record can later be updated or deregistered by passing the RecordHandle returned
		/// by this function to updateRecord() or removeRecord().

	void updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record);
		/// Update a registered resource record. The record must either be:
		///   - the primary txt record of a service registered via registerService()
		///     (if recordHandle is a null handle), or
		///   - a record added to a registered service via addRecord().

	void removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle);
		/// Remove a registered resource record. The record must be
		/// a record added to a registered service via addRecord().
		///
		/// The RecordHandle is invalidated.

	void start();
		/// Starts the responder.
		///
		/// Must be called before services can be registered
		/// or before browsing for domains and services.

	void stop();
		/// Stops the responder.

	static void registerImplFactory(DNSSDResponderImplFactory& factory);
		/// Registers the factory for creating DNSSDResponderImpl objects.
		///
		/// A factory must be registered before the first instance of
		/// DNSSDResponder is created.

	static void unregisterImplFactory();
		/// Unregisters the currently registered DNSSDResponderImplFactory.

private:
	DNSSDResponder(const DNSSDResponder&);
	DNSSDResponder& operator = (const DNSSDResponder&);

	DNSSDResponderImpl* _pImpl;

	static DNSSDResponderImplFactory* _pImplFactory;
};


} } // namespace Poco::DNSSD


#endif // DNSSD_DNSSDResponder_INCLUDED
