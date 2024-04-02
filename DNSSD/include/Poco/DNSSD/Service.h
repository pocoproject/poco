//
// Service.h
//
// $Id$
//
// Library: DNSSD
// Package: Core
// Module:  Service
//
// Definition of the Service class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Service_INCLUDED
#define DNSSD_Service_INCLUDED


#include "Poco/DNSSD/DNSSD.h"
#include "Poco/Net/NameValueCollection.h"


namespace Poco {
namespace DNSSD {


class DNSSD_API Service
	/// Service holds information for a registered or resolved service.
{
public:
	typedef Poco::Net::NameValueCollection Properties;
		/// The Properties of a resolved service contains the
		/// contents of the associated TXT record.
		///
		/// The TXT record contains key-value pairs in
		/// the form <key>=<value>. Each pair is preceded
		/// by a length byte giving the total length
		/// of the pair. Thus the total length of key
		/// plus value must not exceed 254 bytes.
		/// Keys are case insensitive and must consists
		/// entirely of printable US-ASCII characters.
		/// The length of a key should not exceed nine bytes,
		/// to keep packet sizes small. Values can contain
		/// arbitrary bytes. A value can also be empty; in
		/// this case the '=' character can be omitted in
		/// the record.

	Service();
		/// Creates an empty Service.

	Service(Poco::Int32 networkInterface, const std::string& name, const std::string& type, const std::string& domain);
		/// Creates the Service using the given information.
		///
		///     - networkInterface specifies the index of the interface the service was discovered on.
		///       When registering a service, can be set to 0 to register the service on all
		///       available interfaces.
		///     - name specifies the human-readable service name.
		///       When registering a service, can be left empty. In this case
		///       the computer's name is used. Must be 1 - 63 bytes of UTF-8 text.
		///     - type specifies the registration type of the service, consisting of service type
		///       and network protocol (delimited by a dot, as in "_ftp._tcp"),
		///       and an optional subtype (e.g., "_primarytype._tcp,_subtype").
		///       The protocol is always either "_tcp" or "_udp".
		///     - domain specifies the name of the domain the service is registered on.
		///       When registering a service, can be left empty to register in the default domain.

	Service(const std::string& type, Poco::UInt16 port, const Properties& properties = Properties());
		/// Creates the Service using the given information. This is the easiest way
		/// to create a Service instance for registration.
		///
		/// The service will be registered on all available network interfaces. The computer's name
		/// will be used as the service name. The default domain is used, and the computer's host name
		/// is used as host name for registering.
		///
		///     - type specifies the registration type of the service, consisting of service type
		///       and network protocol (delimited by a dot, as in "_ftp._tcp"),
		///       and an optional subtype (e.g., "_primarytype._tcp,_subtype").
		///       The protocol is always either "_tcp" or "_udp".
		///     - properties contains the contents of the service's TXT record.

	Service(Poco::Int32 networkInterface, const std::string& name, const std::string& fullName, const std::string& type, const std::string& domain, const std::string& host, Poco::UInt16 port);
		/// Creates the Service using the given information.
		///
		///     - networkInterface specifies the index of the interface the service was discovered on.
		///       When registering a service, can be set to 0 to register the service on all
		///       available interfaces.
		///     - name specifies the human-readable service name.
		///       When registering a service, can be left empty. In this case
		///       the computer's name is used. Must be 1 - 63 bytes of UTF-8 text.
		///     - fullName specifies the full service name in the form "<servicename>.<protocol>.<domain>.".
		///       When registering a service, this should be left empty.
		///     - type specifies the registration type of the service, consisting of service type
		///       and network protocol (delimited by a dot, as in "_ftp._tcp"),
		///       and an optional subtype (e.g., "_primarytype._tcp,_subtype").
		///       The protocol is always either "_tcp" or "_udp".
		///     - domain specifies the name of the domain the service is registered on.
		///       When registering a service, can be left empty to register in the default domain.
		///     - host specifies the name of the host providing the service.
		///       When registering a service, can be left empty to use the machine's default host name.
		///     - port specifies the port number on which the service is available.

	Service(Poco::Int32 networkInterface, const std::string& name, const std::string& fullName, const std::string& type, const std::string& domain, const std::string& host, Poco::UInt16 port, const Properties& properties);
		/// Creates the Service using the given information.
		///
		///     - networkInterface specifies the index of the interface the service was discovered on.
		///       When registering a service, can be set to 0 to register the service on all
		///       available interfaces.
		///     - name specifies the human-readable service name.
		///       When registering a service, can be left empty. In this case
		///       the computer's name is used. Must be 1 - 63 bytes of UTF-8 text.
		///     - fullName specifies the full service name in the form "<servicename>.<protocol>.<domain>.".
		///       When registering a service, this should be left empty.
		///     - type specifies the registration type of the service, consisting of service type
		///       and network protocol (delimited by a dot, as in "_ftp._tcp"),
		///       and an optional subtype (e.g., "_primarytype._tcp,_subtype").
		///       The protocol is always either "_tcp" or "_udp".
		///     - domain specifies the name of the domain the service is registered on.
		///       When registering a service, can be left empty to register in the default domain.
		///     - host specifies the name of the host providing the service.
		///       When registering a service, can be left empty to use the machine's default host name.
		///     - port specifies the port number on which the service is available.
		///     - properties contains the contents of the service's TXT record.

	~Service();
		/// Destroys the Service.

	Poco::Int32 networkInterface() const;
		/// The id of the interface on which the remote service is running, or zero
		/// if the service is available on all interfaces.

	const std::string& name() const;
		/// The name of the service.

	const std::string& fullName() const;
		/// Returns the full name of the service.
		///
		/// The format of the full name is "<servicename>.<protocol>.<domain>.".
		/// This name is escaped following standard DNS rules.
		/// The full name will be empty for an unresolved service.

	const std::string& type() const;
		/// The registration type of the service, consisting of service type
		/// and network protocol (delimited by a dot, as in "_ftp._tcp"),
		/// and an optional subtype (e.g., "_primarytype._tcp,_subtype").
		///
		/// The protocol is always either "_tcp" or "_udp".

	const std::string& domain() const;
		/// The domain the service is registered on.

	const std::string& host() const;
		/// Returns the host name of the host providing the service.
		///
		/// Will be empty for an unresolved service.

	Poco::UInt16 port() const;
		/// Returns the port number on which the service is available.
		///
		/// Will be 0 for an unresolved service.

	const Properties& properties() const;
		/// Returns the contents of the TXT record associated with the service.
		///
		/// Will be empty for an unresolved service.

	Properties& properties();
		/// Returns the contents of the TXT record associated with the service.
		///
		/// Will be empty for an unresolved service.

private:
	Poco::Int32   _networkInterface;
	std::string   _name;
	std::string   _fullName;
	std::string   _type;
	std::string   _domain;
	std::string   _host;
	Poco::UInt16  _port;
	Properties    _properties;
};


//
// inlines
//
inline Poco::Int32 Service::networkInterface() const
{
	return _networkInterface;
}


inline const std::string& Service::name() const
{
	return _name;
}


inline const std::string& Service::fullName() const
{
	return _fullName;
}


inline const std::string& Service::type() const
{
	return _type;
}


inline const std::string& Service::domain() const
{
	return _domain;
}


inline const std::string& Service::host() const
{
	return _host;
}


inline Poco::UInt16 Service::port() const
{
	return _port;
}


inline const Service::Properties& Service::properties() const
{
	return _properties;
}


inline Service::Properties& Service::properties()
{
	return _properties;
}


} } // namespace Poco::DNSSD


#endif // DNSSD_Service_INCLUDED
