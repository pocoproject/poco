//
// DNSSDBrowser.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/DNSSDBrowser.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSDBrowser
//
// Definition of the DNSSDBrowser class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_DNSSDBrowser_INCLUDED
#define DNSSD_DNSSDBrowser_INCLUDED


#include "Poco/DNSSD/DNSSD.h"
#include "Poco/DNSSD/Service.h"
#include "Poco/DNSSD/Domain.h"
#include "Poco/DNSSD/Record.h"
#include "Poco/DNSSD/Error.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace DNSSD {


class DNSSD_API DNSSDBrowser
	/// The DNSSDBrowser class allows browsing for services, domains and records,
	/// resolving services and error handling. Browse and resolve operations
	/// are asynchronous. Discovered services, records and domains, as well as
	/// error conditions are reported via events.
	///
	/// A DNSSDBrowser object is never created directly. It can be obtained
	/// from the DNSSDResponder by calling the DNSSDResponder::serviceBrowser()
	/// method.
	///
	/// A note on implementing event handlers: An event handler should
	/// complete its work and return as quick as possible, otherwise it may
	/// interfere with DNS-SD processing. An event handler should never
	/// wait for other events to happen. Specifically, it must never
	/// wait for an other DNSSDBrowser event to happen, as this will
	/// result in a deadlock.
{
public:
	enum Options
		/// Options for browsing, resolving and DNS queries.
	{
		RESOLVE_ON_DISCOVERED_INTERFACE = 0x01,
			/// Attempt to resolve the service on the interface it was discovered on only.

		RESOLVE_ON_ALL_INTERFACES       = 0x02,
			/// Attempt to resolve the service on all interfaces.

		BROWSE_FORCE_MULTICAST          = 0x04,
			/// Force query to be performed with a link-local mDNS query,
			/// even if the name is an apparently non-local name (i.e. a
			/// name not ending in ".local.").

		BROWSE_LONG_LIVED_QUERY         = 0x08
			/// Create a "long-lived" unicast query in a non-local domain.
			/// Without enabling this option, unicast queries will be one-shot --
			/// that is, only answers available at the time of the call
			/// will be returned. By setting this flag, answers that become available
			/// after the initial call is made will generate
			/// events. This flag has no effect on link-local multicast queries.
	};

	enum BrowseFlags
		/// Flags reported in event arguments.
	{
		BROWSE_MORE_COMING = 0x01
			/// This flag signals that more events will follow
			/// immediately. For the application this means for
			/// example that updates to the user interface can be
			/// delayed until an event is fired that does not have
			/// this flag set.
	};

	struct CommonEventArgs
	{
		CommonEventArgs(BrowseHandle h, int f):
			browseHandle(h),
			flags(f)
		{
		}

		BrowseHandle browseHandle;
		int          flags;
	};

	struct ServiceEventArgs: public CommonEventArgs
	{
		ServiceEventArgs(BrowseHandle h, int f, const Service& s):
			CommonEventArgs(h, f),
			service(s)
		{
		}

		const Service& service;
	};

	struct DomainEventArgs: public CommonEventArgs
	{
		DomainEventArgs(BrowseHandle h, int f, const Domain& d):
			CommonEventArgs(h, f),
			domain(d)
		{
		}

		const Domain& domain;
	};

	struct RecordEventArgs: public CommonEventArgs
	{
		RecordEventArgs(BrowseHandle h, int f, const Record& r):
			CommonEventArgs(h, f),
			record(r)
		{
		}

		const Record& record;
	};

	struct ResolveHostEventArgs: public CommonEventArgs
	{
		ResolveHostEventArgs(BrowseHandle h, int f, const std::string& n, const Poco::Net::IPAddress& a, Poco::Int32 i, Poco::UInt32 t):
			CommonEventArgs(h, f),
			host(n),
			address(a),
			networkInterface(i),
			ttl(t)
		{
		}

		std::string          host;
		Poco::Net::IPAddress address;
		Poco::Int32          networkInterface;
		Poco::UInt32         ttl;
	};

	struct ErrorEventArgs: public CommonEventArgs
	{
		ErrorEventArgs(BrowseHandle h, int f, const Error& e):
			CommonEventArgs(h, f),
			error(e)
		{
		}

		const Error& error;
	};

	Poco::BasicEvent<const ErrorEventArgs> browseError;
		/// Fired when an error occures while browsing for services.
		///
		/// The specific error condition can be found by looking at the
		/// Error object in the argument.

	Poco::BasicEvent<const ServiceEventArgs> serviceFound;
		/// Fired when a service has been found.
		///
		/// The Service object given in the argument can be passed to resolve()
		/// to resolve the service, and to obtain the service's host and port
		/// number.

	Poco::BasicEvent<const ServiceEventArgs> serviceRemoved;
		/// Fired when a service has been removed and is no longer available.
		///
		/// The Service object given in the argument can be passed to resolve()
		/// to resolve the service, and to obtain the service's host and port
		/// number.

	Poco::BasicEvent<const ErrorEventArgs> resolveError;
		/// Fired when an error occures while resolving a service.
		///
		/// The specific error condition can be found by looking at the
		/// ServiceError object in the argument.

	Poco::BasicEvent<const ServiceEventArgs> serviceResolved;
		/// Fired when a service has been successfully resolved.

	Poco::BasicEvent<const DomainEventArgs> browseDomainFound;
		/// Fired when a browse domain has been found.

	Poco::BasicEvent<const DomainEventArgs> browseDomainRemoved;
		/// Fired when a browse domain has been removed and is no longer available.

	Poco::BasicEvent<const ErrorEventArgs> browseDomainError;
		/// Fired when an error occures while browsing for browse domains.
		///
		/// The specific error condition can be found by looking at the
		/// Error object in the argument.

	Poco::BasicEvent<const DomainEventArgs> registrationDomainFound;
		/// Fired when a registration domain has been found.

	Poco::BasicEvent<const DomainEventArgs> registrationDomainRemoved;
		/// Fired when a registration domain has been removed and is no longer available.

	Poco::BasicEvent<const ErrorEventArgs> registrationDomainError;
		/// Fired when an error occures while browsing for registration domains.
		///
		/// The specific error condition can be found by looking at the
		/// Error object in the argument.

	Poco::BasicEvent<const RecordEventArgs> recordFound;
		/// Fired when a record has been found.

	Poco::BasicEvent<const RecordEventArgs> recordRemoved;
		/// Fired when a record has been removed and is no longer available.

	Poco::BasicEvent<const ErrorEventArgs> recordError;
		/// Fired when an error occures while browsing for a record type.
		///
		/// The specific error condition can be found by looking at the
		/// Error object in the argument.

	Poco::BasicEvent<const ResolveHostEventArgs> hostResolved;
		/// Fired when a host name has been resolved.

	Poco::BasicEvent<const ErrorEventArgs> hostResolveError;
		/// Fired when an error occurs while resolving a host name.
		///
		/// The specific error condition can be found by looking at the
		/// Error object in the argument.

	virtual BrowseHandle browse(const std::string& regType, const std::string& domain, int options = 0, Poco::Int32 networkInterface = 0) = 0;
		/// Browse for a service type on a specific network interface, given by its index in networkInterface.
		/// An interface with index 0 can be specified to search on all interfaces.
		///
		///    - regType specifies the service type and protocol, separated by a dot
		///      (e.g., "_ftp._tcp"). The transport protocol must be either "_tcp" or "_udp".
		///      Optionally, a single subtype may be specified to perform filtered browsing:
		///      e.g. browsing for "_primarytype._tcp,_subtype" will discover only those
		///      instances of "_primarytype._tcp" that were registered specifying "_subtype"
		///      in their list of registered subtypes.
		///    - domain specifies the domain on which to browse for services.
		///      If an empty string is specified, the default domain(s) will be browsed.
		///    - options should be 0, as no browse options are currently supported.
		///
		/// Results will be reported asynchronously via the serviceFound, serviceRemoved and serviceError events.
		///
		/// The returned BrowseHandle can be passed to cancel() to cancel browsing.
		///
		/// Note: It is possible to enumerate all service types registered on the local network
		/// by browsing for the special regType "_services._dns-sd._udp". Available service types will
		/// be reported with the Service object's name containing the service type (e.g., "_ftp")
		/// and the Service's type containing the protocol and domain (e.g., "_tcp.local.").

	virtual BrowseHandle resolve(const Service& service, int options = RESOLVE_ON_DISCOVERED_INTERFACE) = 0;
		/// Resolves the service specified by the given Service object. The given
		/// Service object must be one obtained via the serviceFound event.
		///
		/// Results will be sent asnychronously via the serviceResolved and resolveError events.
		///
		/// If RESOLVE_ON_ALL_INTERFACES is specified in options, resolving will be attempted
		/// on all network interfaces. Otherwise, if RESOLVE_ON_DISCOVERED_INTERFACE is specified,
		/// resolving will only be attempted on the interfaces on which the service has
		/// been discovered.
		///
		/// The returned BrowseHandle can be passed to cancel() to cancel resolving.
		/// However, this should be done before either the serviceResolved or resolveError event
		/// has been fired. In practice, this has high potential for a race condition,
		/// so calling cancel() with a BrowseHandle returned from this function is
		/// not recommended.
		/// After either the serviceResolved or resolveError event has been
		/// fired, the returned BrowseHandle is no longer valid.

	virtual BrowseHandle enumerateBrowseDomains(Poco::Int32 networkInterface = 0) = 0;
		/// Discovers domains for browse operations on the given interface, given by its index.
		/// An interface with index 0 can be specified to search on all interfaces.
		///
		/// Results will be sent asnychronously via the browseDomainFound, browseDomainRemoved and browseDomainError events.

	virtual BrowseHandle enumerateRegistrationDomains(Poco::Int32 networkInterface = 0) = 0;
		/// Discovers domains for service registration on the given interface, given by its index.
		/// An interface with index 0 can be specified to search on all interfaces.
		///
		/// Results will be sent asnychronously via the registrationDomainFound, registrationDomainRemoved and registrationDomainError events.
		///
		/// The returned BrowseHandle can be passed to cancel() to cancel resolving.

	virtual BrowseHandle queryRecord(const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz = Record::RC_IN, int options = 0, Poco::Int32 networkInterface = 0) = 0;
		/// Starts a query for a DNS record, specified by name, type and clazz, on the specified interface, given by its index.
		/// An interface with index 0 can be specified to search on all interfaces.
		///
		///    - name specifies the full domain name of the record.
		///    - type specifies the record's type (e.g., RT_PTR).
		///      Suitable values can be found in the RecordType enumeration.
		///    - clazz specifies the record's class (usually RC_IN).
		///    - options can be 0, BROWSE_FORCE_MULTICAST or BROWSE_LONG_LIVED_QUERY.
		///
		/// Results will be sent asnychronously via the recordFound, recordRemoved and recordError events.
		///
		/// The returned BrowseHandle can be passed to cancel() to cancel the query.

	virtual BrowseHandle resolveHost(const std::string& host, int options = 0, Poco::Int32 networkInterface = 0) = 0;
		/// Resolves the given host name. The given host name should have
		/// been obtained by resolving a service.
		/// An interface with index 0 can be specified to search on all interfaces.
		///
		///    - host specifies the hostname to resolve. It should be a hostname
		///      obtained by resolving a service.
		///    - options can be 0 or BROWSE_FORCE_MULTICAST.
		///
		/// This should be used instead of Poco::Net::DNS to resolve host names
		/// via Multicast DNS, if the system's domain name resolver does not
		/// support Multicast DNS.
		///
		/// Results will be sent asnychronously via the hostResolved and hostResolveError events.
		///
		/// The returned BrowseHandle can be passed to cancel() to cancel resolving.
		/// However, this should be done before either the hostResolved or hostResolveError event
		/// has been fired. In practice, this has high potential for a race condition,
		/// so calling cancel() with a BrowseHandle returned from this function is
		/// not recommended.
		/// After either the hostResolved or hostResolveError event has been
		/// fired, the returned BrowseHandle is no longer valid.

	virtual void cancel(BrowseHandle& browseHandle) = 0;
		/// Cancels the browse or resolve activity associated with the given BrowseHandle.
		///
		/// The BrowseHandle is invalidated.

protected:
	DNSSDBrowser();
	virtual ~DNSSDBrowser();

private:
	DNSSDBrowser(const DNSSDBrowser&);
	DNSSDBrowser& operator = (const DNSSDBrowser&);
};


} } // namespace Poco::DNSSD


#endif // DNSSD_DNSSDBrowser_INCLUDED
