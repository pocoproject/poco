//
// Endpoint.cpp
//
// Library: Net
// Package: NetCore
// Module:  Endpoint
//
// Copyright (c) 2005-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Endpoint.h"

#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/RefCountedObject.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include <algorithm>
#include <cstring>


using Poco::RefCountedObject;
using Poco::NumberParser;
using Poco::NumberFormatter;
using Poco::UInt16;
using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


struct AFLT
{
	bool operator () (const IPAddress& a1, const IPAddress& a2)
	{
		return a1.af() < a2.af();
	}
};


//
// Endpoint
//


#if !defined(_MSC_VER) || defined(__STDC__)
// Go home MSVC, you're drunk...
// See http://stackoverflow.com/questions/5899857/multiple-definition-error-for-static-const-class-members
const Endpoint::Family Endpoint::IPv4;
#if defined(POCO_HAVE_IPv6)
const Endpoint::Family Endpoint::IPv6;
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
const Endpoint::Family Endpoint::UNIX_LOCAL;
#endif
#endif


Endpoint::Endpoint(): _addr()
{
	makeIPv4();
}


Endpoint::Endpoint(Family fam): _addr()
{
		init(IPAddress(fam), 0);
}


Endpoint::Endpoint(const IPAddress& hostAddress, Poco::UInt16 portNumber): _addr()
{
	init(hostAddress, portNumber);
}


Endpoint::Endpoint(Poco::UInt16 portNumber): _addr()
{
	init(IPAddress(), portNumber);
}


Endpoint::Endpoint(Family fam, Poco::UInt16 portNumber): _addr()
{
	init(IPAddress(fam), portNumber);
}


Endpoint::Endpoint(const std::string& hostAddress, Poco::UInt16 portNumber): _addr()
{
	init(hostAddress, portNumber);
}


Endpoint::Endpoint(Family fam, const std::string& hostAddress, Poco::UInt16 portNumber): _addr()
{
	init(fam, hostAddress, portNumber);
}


Endpoint::Endpoint(const std::string& hostAddress, const std::string& portNumber): _addr()
{
	init(hostAddress, resolveService(portNumber));
}


Endpoint::Endpoint(Family fam, const std::string& hostAddress, const std::string& portNumber): _addr()
{
	init(fam, hostAddress, resolveService(portNumber));
}


Endpoint::Endpoint(Family fam, const std::string& addr): _addr()
{
	init(fam, addr);
}


Endpoint::Endpoint(const std::string& hostAndPort): _addr()
{
	init(hostAndPort);
}


Endpoint::Endpoint(const Endpoint& socketAddress): _addr()
{
	if (socketAddress.family() == IPv4)
		makeIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
#if defined(POCO_HAVE_IPv6)
	else if (socketAddress.family() == IPv6)
		makeIPv6(reinterpret_cast<const sockaddr_in6*>(socketAddress.addr()));
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
	else if (socketAddress.family() == UNIX_LOCAL)
		makeLocal(reinterpret_cast<const sockaddr_un*>(socketAddress.addr()));
#endif
}


Endpoint::Endpoint(const struct sockaddr* sockAddr, poco_socklen_t length): _addr()
{
	if (length == sizeof(struct sockaddr_in) && sockAddr->sa_family == Family::IPv4)
		makeIPv4(reinterpret_cast<const struct sockaddr_in*>(sockAddr));
#if defined(POCO_HAVE_IPv6)
	else if (length == sizeof(struct sockaddr_in6) && sockAddr->sa_family == Family::IPv6)
		makeIPv6(reinterpret_cast<const struct sockaddr_in6*>(sockAddr));
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
	else if (length > 0 && length <= sizeof(struct sockaddr_un) && sockAddr->sa_family == AF_UNIX)
		makeLocal(reinterpret_cast<const sockaddr_un*>(sockAddr));
#endif
	else throw Poco::InvalidArgumentException("Invalid address length or family passed to Endpoint()");
}


bool Endpoint::operator < (const Endpoint& socketAddress) const
{
	if (family() < socketAddress.family()) return true;
	if (family() > socketAddress.family()) return false;
#if defined(POCO_HAVE_UNIX_SOCKET)
	if (family() == UNIX_LOCAL) return toString() < socketAddress.toString();
#endif
	if (host() < socketAddress.host()) return true;
	if (host() > socketAddress.host()) return false;
	return (port() < socketAddress.port());
}


Endpoint& Endpoint::operator = (const Endpoint& socketAddress)
{
	if (&socketAddress != this)
	{
		if (socketAddress.family() == IPv4)
			makeIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
#if defined(POCO_HAVE_IPv6)
		else if (socketAddress.family() == IPv6)
			makeIPv6(reinterpret_cast<const sockaddr_in6*>(socketAddress.addr()));
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		else if (socketAddress.family() == UNIX_LOCAL)
			makeLocal(reinterpret_cast<const sockaddr_un*>(socketAddress.addr()));
#endif
	}
	return *this;
}


IPAddress Endpoint::host() const
{
	switch (family())
	{
		case Family::IPv4:
			return IPAddress(&_addr.v4.sin_addr, sizeof(_addr.v4.sin_addr));
#if defined(POCO_HAVE_IPv6)
		case Family::IPv6:
			return IPAddress(&_addr.v6.sin6_addr, sizeof(_addr.v6.sin6_addr), _addr.v6.sin6_scope_id);
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		case Family::UNIX_LOCAL:
			throw Poco::InvalidAccessException("local socket address does not have host IP address");
#endif
		default:
			unsupportedFamilyError(__func__);
	}
	return IPAddress();
}


UInt16 Endpoint::port() const
{
	switch (family())
	{
		case Family::IPv4:
			return ntohs(_addr.v4.sin_port);
#if defined(POCO_HAVE_IPv6)
		case Family::IPv6:
			return ntohs(_addr.v6.sin6_port);
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		case Family::UNIX_LOCAL:
			throw Poco::InvalidAccessException("local socket address does not have port number");
#endif
		default: unsupportedFamilyError(__func__);
	}
	return 0;
}


poco_socklen_t Endpoint::length() const
{
	switch (family())
	{
		case Family::IPv4:
			return sizeof(_addr.v4);
#if defined(POCO_HAVE_IPv6)
		case Family::IPv6:
			return sizeof(_addr.v6);
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		case Family::UNIX_LOCAL:
			return sizeof(_addr.local);
#endif
		default: unsupportedFamilyError(__func__);
	}
	return 0;
}


int Endpoint::af() const
{
	return family();
}


Endpoint::Family Endpoint::family() const
{
	switch (_addr.base.sa_family)
	{
		case AF_INET:
			return Family::IPv4;
#if defined(POCO_HAVE_IPv6)
		 case AF_INET6:
			 return Family::IPv6;
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		 case AF_UNIX:
			 return Family::UNIX_LOCAL;
#endif
		default: unsupportedFamilyError(__func__);
	}
	return Family::UNKNOWN;
}


const char* Endpoint::path() const
{
	switch (family())
	{
		case Family::IPv4:
			throw Poco::InvalidAccessException("IPv4 socket address does not have path");
#if defined(POCO_HAVE_IPv6)
		case Family::IPv6:
			throw Poco::InvalidAccessException("IPv6 socket address does not have path");
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		case Family::UNIX_LOCAL:
			return _addr.local.sun_path;
#endif
		default: unsupportedFamilyError(__func__);
	}
	return "";
}


std::string Endpoint::toString() const
{
	std::string result;
	switch (family())
	{
		case Family::IPv4:
			result.append(host().toString());
			result.append(":");
			NumberFormatter::append(result, port());
			break;
#if defined(POCO_HAVE_IPv6)
		case Family::IPv6:
			result.append("[");
			result.append(host().toString());
			result.append("]");
			result.append(":");
			NumberFormatter::append(result, port());
			break;
#endif
#if defined(POCO_HAVE_UNIX_SOCKET)
		 case Family::UNIX_LOCAL:
			 result = path();
			 break;
#endif
		default:
			unsupportedFamilyError(__func__);
	}
	return result;
}


void Endpoint::init(const IPAddress& hostAddress, Poco::UInt16 portNumber)
{
	if (hostAddress.family() == IPAddress::IPv4)
		makeIPv4(hostAddress, portNumber);
#if defined(POCO_HAVE_IPv6)
	else if (hostAddress.family() == IPAddress::IPv6)
		makeIPv6(hostAddress, portNumber);
#endif
	else throw Poco::NotImplementedException("unsupported IP address family");
}


void Endpoint::init(const std::string& hostAddress, Poco::UInt16 portNumber)
{
	IPAddress ip;
	if (IPAddress::tryParse(hostAddress, ip))
	{
		init(ip, portNumber);
	}
	else
	{
		HostEntry he = DNS::hostByName(hostAddress);
		HostEntry::AddressList addresses = he.addresses();
		if (addresses.size() > 0)
		{
#if defined(POCO_HAVE_IPv6) && defined(POCO_SOCKETADDRESS_PREFER_IPv4)
			// if we get both IPv4 and IPv6 addresses, prefer IPv4
			std::stable_sort(addresses.begin(), addresses.end(), AFLT());
#endif
			init(addresses[0], portNumber);
		}
		else throw HostNotFoundException("No address found for host", hostAddress);
	}
}


void Endpoint::init(Family fam, const std::string& hostAddress, Poco::UInt16 portNumber)
{
	IPAddress ip;
	if (IPAddress::tryParse(hostAddress, ip))
	{
		if (ip.family() != fam) throw AddressFamilyMismatchException(hostAddress);
		init(ip, portNumber);
	}
	else
	{
		HostEntry he = DNS::hostByName(hostAddress);
		HostEntry::AddressList addresses = he.addresses();
		if (addresses.size() > 0)
		{
			for (const auto& addr: addresses)
			{
				if (addr.family() == fam)
				{
					init(addr, portNumber);
					return;
				}
			}
			throw AddressFamilyMismatchException(hostAddress);
		}
		else throw HostNotFoundException("No address found for host", hostAddress);
	}
}


void Endpoint::init(Family fam, const std::string& address)
{
#if defined(POCO_HAVE_UNIX_SOCKET)
	if (fam == UNIX_LOCAL)
	{
		makeLocal(address);
	}
	else
#endif
	{
		std::string host;
		std::string port;
		std::string::const_iterator it  = address.begin();
		std::string::const_iterator end = address.end();

		if (*it == '[')
		{
			++it;
			while (it != end && *it != ']') host += *it++;
			if (it == end) throw InvalidArgumentException("Malformed IPv6 address");
			++it;
		}
		else
		{
			while (it != end && *it != ':') host += *it++;
		}
		if (it != end && *it == ':')
		{
			++it;
			while (it != end) port += *it++;
		}
		else throw InvalidArgumentException("Missing port number");
		init(fam, host, resolveService(port));
	}
}


void Endpoint::init(const std::string& hostAndPort)
{
	poco_assert (!hostAndPort.empty());

	std::string host;
	std::string port;
	std::string::const_iterator it  = hostAndPort.begin();
	std::string::const_iterator end = hostAndPort.end();

#if defined(POCO_HAVE_UNIX_SOCKET)
	if (*it == '/')
	{
		makeLocal(hostAndPort);
		return;
	}
#endif
	if (*it == '[')
	{
		++it;
		while (it != end && *it != ']') host += *it++;
		if (it == end) throw InvalidArgumentException("Malformed IPv6 address");
		++it;
	}
	else
	{
		while (it != end && *it != ':') host += *it++;
	}
	if (it != end && *it == ':')
	{
		++it;
		while (it != end) port += *it++;
	}
	else throw InvalidArgumentException("Missing port number");
	init(host, resolveService(port));
}


Poco::UInt16 Endpoint::resolveService(const std::string& service)
{
	unsigned port;
	if (NumberParser::tryParseUnsigned(service, port) && port <= 0xFFFF)
	{
		return (UInt16) port;
	}
	else
	{
#if defined(POCO_VXWORKS)
		throw ServiceNotFoundException(service);
#else
		struct servent* se = getservbyname(service.c_str(), NULL);
		if (se)
			return ntohs(se->s_port);
		else
			throw ServiceNotFoundException(service);
#endif
	}
}


void Endpoint::makeIPv4()
{
	_addr.v4.sin_family = Family::IPv4;
	poco_set_sin_len(&_addr.v4);
}


void Endpoint::makeIPv4(const sockaddr_in* sockAddr)
{
	poco_check_ptr(sockAddr);
	std::memcpy(&_addr.v4, sockAddr, sizeof(_addr.v4));
}


void Endpoint::makeIPv4(const IPAddress& hostAddress, Poco::UInt16 portNumber)
{
	_addr.v4.sin_family = Family::IPv4;
	poco_set_sin_len(&_addr.v4);
	std::memcpy(&_addr.v4.sin_addr, hostAddress.addr(), sizeof(_addr.v4.sin_addr));
	_addr.v4.sin_port = htons(portNumber);
}


#if defined(POCO_HAVE_IPv6)
void Endpoint::makeIPv6(const sockaddr_in6* sockAddr)
{
	poco_check_ptr(sockAddr);
	std::memcpy(&_addr.v6, sockAddr, sizeof(_addr.v6));
}


void Endpoint::makeIPv6(const IPAddress& hostAddress, Poco::UInt16 portNumber)
{
   _addr.v6.sin6_family = Family::IPv6;
	poco_set_sin6_len(&_addr.v6);
	std::memcpy(&_addr.v6.sin6_addr, hostAddress.addr(), sizeof(_addr.v6.sin6_addr));
	_addr.v6.sin6_port = htons(portNumber);
	_addr.v6.sin6_scope_id = hostAddress.scope();
}
#endif // POCO_HAVE_IPv6


#if defined(POCO_HAVE_UNIX_SOCKET)
void Endpoint::makeLocal(const sockaddr_un* sockAddr)
{
	std::memcpy(&_addr.local, sockAddr, sizeof(struct sockaddr_un));
}


void Endpoint::makeLocal(const std::string& path)
{
	poco_assert (path.length() < sizeof(_addr.local.sun_path));

	poco_set_sun_len(&_addr.local, path.length() + sizeof(struct sockaddr_un) - sizeof(_addr.local.sun_path) + 1);
	_addr.local.sun_family = AF_UNIX;
	std::strcpy(_addr.local.sun_path, path.c_str());
}
#endif // POCO_HAVE_UNIX_SOCKET


} } // namespace Poco::Net


Poco::BinaryWriter& operator << (Poco::BinaryWriter& writer, const Poco::Net::Endpoint& value)
{
	writer << value.host();
	writer << value.port();
	return writer;
}


Poco::BinaryReader& operator >> (Poco::BinaryReader& reader, Poco::Net::Endpoint& value)
{
	Poco::Net::IPAddress host;
	reader >> host;
	Poco::UInt16 port;
	reader >> port;
	value = Poco::Net::Endpoint(host, port);
	return reader;
}


std::ostream& operator << (std::ostream& ostr, const Poco::Net::Endpoint& address)
{
	ostr << address.toString();
	return ostr;
}
