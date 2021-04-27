//
// Endpoint.h
//
// Library: Net
// Package: NetCore
// Module:  Endpoint
//
// Definition of the Endpoint class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_Endpoint_INCLUDED
#define Net_Endpoint_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/Endpoint.h"
#include "Poco/Format.h"
#include <ostream>


// force no local sockets
#define POCO_NET_NO_UNIX_SOCKET
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NET_NO_UNIX_SOCKET)
#define POCO_HAVE_UNIX_SOCKET
#elif defined(POCO_HAVE_UNIX_SOCKET)
#undef POCO_HAVE_UNIX_SOCKET
#endif


namespace Poco {

class BinaryReader;
class BinaryWriter;

namespace Net {

class IPAddress;

class Net_API Endpoint
	/// This class represents an internet (IP) endpoint (socket)
	/// address. The address can belong either to the
	/// IPv4 or the IPv6 address family and consists of a
	/// host address and a port number.
{
public:
	// The following declarations keep the Family type
	// backwards compatible with the previously used
	// enum declaration.
	using Family = AddressFamily::Family;
	static const Family IPv4 = AddressFamily::IPv4;
#if defined(POCO_HAVE_IPv6)
	static const Family IPv6 = AddressFamily::IPv6;
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
	static const Family UNIX_LOCAL = AddressFamily::UNIX_LOCAL;
#endif

	Endpoint();
		/// Creates a wildcard (all zero) IPv4 Endpoint.

	explicit Endpoint(Family family);
		/// Creates a Endpoint with unspecified (wildcard) IP address
		/// of the given family.

	Endpoint(const IPAddress& hostAddress, Poco::UInt16 portNumber);
		/// Creates a Endpoint from an IP address and given port number.

	explicit Endpoint(Poco::UInt16 port);
		/// Creates a Endpoint with unspecified (wildcard) IP address
		/// and given port number.

	Endpoint(Family family, Poco::UInt16 port);
		/// Creates a Endpoint with unspecified (wildcard) IP address
		/// of the given family, and given port number.

	Endpoint(const std::string& hostAddress, Poco::UInt16 portNumber);
		/// Creates a Endpoint from an IP address and given port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.

	Endpoint(Family family, const std::string& hostAddress, Poco::UInt16 portNumber);
		/// Creates a Endpoint from an IP address and given port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.
		///
		/// If a domain name is given in hostAddress, it is resolved and the address
		/// matching the given family is used. If no address matching the given family
		/// is found, or the IP address given in hostAddress does not match the given
		/// family, an AddressFamilyMismatchException is thrown.

	Endpoint(const std::string& hostAddress, const std::string& portNumber);
		/// Creates a Endpoint from an IP address and the
		/// service name or port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.
		///
		/// The given port must either be a decimal port number, or
		/// a service name.

	Endpoint(Family family, const std::string& hostAddress, const std::string& portNumber);
		/// Creates a Endpoint from an IP address and the
		/// service name or port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.
		///
		/// The given port must either be a decimal port number, or
		/// a service name.
		///
		/// If a domain name is given in hostAddress, it is resolved and the address
		/// matching the given family is used. If no address matching the given family
		/// is found, or the IP address given in hostAddress does not match the given
		/// family, an AddressFamilyMismatchException is thrown.

	explicit Endpoint(const std::string& hostAndPort);
		/// Creates a Endpoint from an IP address or host name and the
		/// port number/service name. Host name/address and port number must
		/// be separated by a colon. In case of an IPv6 address,
		/// the address part must be enclosed in brackets.
		///
		/// Examples:
		///	 192.168.1.10:80
		///	 [::ffff:192.168.1.120]:2040
		///	 www.appinf.com:8080
		///
		/// On POSIX platforms supporting UNIX_LOCAL sockets, hostAndPort
		/// can also be the absolute path of a local socket, starting with a
		/// slash, e.g. "/tmp/local.socket".

	Endpoint(Family family, const std::string& addr);
		/// Creates a Endpoint of the given family from a
		/// string representation of the address, which is
		/// either an IP address and port number, separated by
		/// a colon for IPv4 or IPv6 addresses, or a path for
		/// UNIX_LOCAL sockets.

	Endpoint(const Endpoint& addr);
		/// Creates a Endpoint by copying another one.

	Endpoint(const struct sockaddr* addr, poco_socklen_t length);
		/// Creates a Endpoint from a native socket address.

	~Endpoint() = default;
		/// Destroys the Endpoint.

	Endpoint& operator = (const Endpoint& socketAddress);
		/// Assigns another Endpoint.

	IPAddress host() const;
		/// Returns the host IP address.

	Poco::UInt16 port() const;
		/// Returns the port number.

	poco_socklen_t length() const;
		/// Returns the length of the internal native socket address.

	const struct sockaddr* addr() const;
		/// Returns a pointer to the internal native socket address.

	int af() const;
		/// Returns the address family (AF_INET or AF_INET6) of the address.

	std::string toString() const;
		/// Returns a string representation of the address.

	Family family() const;
		/// Returns the address family of the host's address.

	bool operator < (const Endpoint& socketAddress) const;
	bool operator == (const Endpoint& socketAddress) const;
	bool operator != (const Endpoint& socketAddress) const;

	enum
	{
		MAX_ADDRESS_LENGTH =
#if defined(POCO_HAVE_UNIX_SOCKET)
			sizeof(struct sockaddr_un)
#elif defined(POCO_HAVE_IPv6)
			sizeof(struct sockaddr_in6)
#else
			sizeof(struct sockaddr_in)
#endif
			/// Maximum length in bytes of a socket address.
	};

protected:
	void init(const IPAddress& hostAddress, Poco::UInt16 portNumber);
	void init(const std::string& hostAddress, Poco::UInt16 portNumber);
	void init(Family family, const std::string& hostAddress, Poco::UInt16 portNumber);
	void init(Family family, const std::string& address);
	void init(const std::string& hostAndPort);
	Poco::UInt16 resolveService(const std::string& service);

private:

	void unsupportedFamilyError(const std::string& func) const;
	const char* path() const;

	void makeIPv4();
	void makeIPv4(const sockaddr_in*);
	void makeIPv4(const IPAddress& hostAddress, Poco::UInt16 portNumber);

#if defined(POCO_HAVE_IPv6)
	void makeIPv6(const sockaddr_in6*);
	void makeIPv6(const IPAddress& hostAddress, Poco::UInt16 portNumber);
#endif

#if defined(POCO_HAVE_UNIX_SOCKET)
	void makeLocal(const sockaddr_un* sockAddr);
	void makeLocal(const std::string& path);
#endif

	union SockAddr
	{
		struct sockaddr	 base;
		struct sockaddr_in  v4;
#if defined(POCO_HAVE_IPv6)
		struct sockaddr_in6 v6;
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		struct sockaddr_un  local;
#endif
	} _addr;
};


//
// inlines
//

inline 	bool Endpoint::operator == (const Endpoint& socketAddress) const
{
#if defined(POCO_HAVE_UNIX_SOCKET)
	if (family() == UNIX_LOCAL)
		return toString() == socketAddress.toString();
	else
#endif
		return host() == socketAddress.host() && port() == socketAddress.port();
}


inline bool Endpoint::operator != (const Endpoint& socketAddress) const
{
	return !(operator == (socketAddress));
}


inline const struct sockaddr* Endpoint::addr() const
{
	return &_addr.base;
}


inline void Endpoint::unsupportedFamilyError(const std::string& func) const
{
	throw Poco::InvalidAccessException(Poco::format("%s(): unknown, disabled or unsupported sa_family: %d",
													func, (int)_addr.base.sa_family));
}


} } // namespace Poco::Net


Net_API Poco::BinaryWriter& operator << (Poco::BinaryWriter& writer, const Poco::Net::Endpoint& value);
Net_API Poco::BinaryReader& operator >> (Poco::BinaryReader& reader, Poco::Net::Endpoint& value);
Net_API std::ostream& operator << (std::ostream& ostr, const Poco::Net::Endpoint& address);


#endif // Net_Endpoint_INCLUDED
