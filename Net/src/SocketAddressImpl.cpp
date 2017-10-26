//
// SocketAddressImpl.cpp
//
// Library: Net
// Package: NetCore
// Module:  SocketAddressImpl
//
// Copyright (c) 2005-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddressImpl.h"
#include "Poco/Net/SocketDefs.h"
#include <cstring>


namespace Poco {
namespace Net {
namespace Impl {


//
// SocketAddressImpl
//


SocketAddressImpl::SocketAddressImpl()
{
}


SocketAddressImpl::~SocketAddressImpl()
{
}


//
// IPv4SocketAddressImpl
//


IPv4SocketAddressImpl::IPv4SocketAddressImpl()
{
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	poco_set_sin_len(&_addr);
}


IPv4SocketAddressImpl::IPv4SocketAddressImpl(const struct sockaddr_in* addr)
{
	std::memcpy(&_addr, addr, sizeof(_addr));
}


IPv4SocketAddressImpl::IPv4SocketAddressImpl(const void* addr, UInt16 port)
{
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	std::memcpy(&_addr.sin_addr, addr, sizeof(_addr.sin_addr));
	_addr.sin_port = port;
}


#ifdef __clang__
inline IPAddress IPv4SocketAddressImpl::host() const
{
	return IPAddress(&_addr.sin_addr, sizeof(_addr.sin_addr));
}


inline UInt16 IPv4SocketAddressImpl::port() const
{
	return _addr.sin_port;
}


inline poco_socklen_t IPv4SocketAddressImpl::length() const
{
	return sizeof(_addr);
}


inline const struct sockaddr* IPv4SocketAddressImpl::addr() const
{
	return reinterpret_cast<const struct sockaddr*>(&_addr);
}


inline int IPv4SocketAddressImpl::af() const
{
	return _addr.sin_family;
}
#endif //__clang__


#if defined(POCO_HAVE_IPv6)


//
// IPv6SocketAddressImpl
//


IPv6SocketAddressImpl::IPv6SocketAddressImpl(const struct sockaddr_in6* addr)
{
	std::memcpy(&_addr, addr, sizeof(_addr));
}


IPv6SocketAddressImpl::IPv6SocketAddressImpl(const void* addr, UInt16 port)
{
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin6_family = AF_INET6;
	poco_set_sin6_len(&_addr);
	std::memcpy(&_addr.sin6_addr, addr, sizeof(_addr.sin6_addr));
	_addr.sin6_port = port;
}


IPv6SocketAddressImpl::IPv6SocketAddressImpl(const void* addr, UInt16 port, UInt32 scope)
{
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin6_family = AF_INET6;
	poco_set_sin6_len(&_addr);
	std::memcpy(&_addr.sin6_addr, addr, sizeof(_addr.sin6_addr));
	_addr.sin6_port = port;
	_addr.sin6_scope_id = scope;
}


#ifdef __clang__
inline IPAddress IPv6SocketAddressImpl::host() const
{
	return IPAddress(&_addr.sin6_addr, sizeof(_addr.sin6_addr), _addr.sin6_scope_id);
}


inline UInt16 IPv6SocketAddressImpl::port() const
{
	return _addr.sin6_port;
}


inline poco_socklen_t IPv6SocketAddressImpl::length() const
{
	return sizeof(_addr);
}


inline const struct sockaddr* IPv6SocketAddressImpl::addr() const
{
	return reinterpret_cast<const struct sockaddr*>(&_addr);
}


inline int IPv6SocketAddressImpl::af() const
{
	return _addr.sin6_family;
}
#endif //__clang__


#endif // POCO_HAVE_IPv6


} } } // namespace Poco::Net::Impl
