//
// IPAddressImpl.h
//
// Library: Net
// Package: NetCore
// Module:  IPAddressImpl
//
// Definition of the IPAddressImpl class.
//
// Copyright (c) 2005-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_IPAddressImpl_INCLUDED
#define Net_IPAddressImpl_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/RefCountedObject.h"
#include <vector>


namespace Poco {
namespace Net {
namespace Impl {


class IPAddressImpl : public Poco::RefCountedObject
{
public:
	using Family = AddressFamily::Family;

	virtual ~IPAddressImpl();

	[[nodiscard]]
	virtual IPAddressImpl* clone() const = 0;
	[[nodiscard]]
	virtual std::string toString() const = 0;
	[[nodiscard]]
	virtual poco_socklen_t length() const = 0;
	[[nodiscard]]
	virtual const void* addr() const = 0;
	[[nodiscard]]
	virtual Family family() const = 0;
	[[nodiscard]]
	virtual int af() const = 0;
	[[nodiscard]]
	virtual Poco::UInt32 scope() const = 0;
	[[nodiscard]]
	virtual bool isWildcard() const	= 0;
	[[nodiscard]]
	virtual bool isBroadcast() const = 0;
	[[nodiscard]]
	virtual bool isLoopback() const = 0;
	[[nodiscard]]
	virtual bool isMulticast() const = 0;
	[[nodiscard]]
	virtual bool isLinkLocal() const = 0;
	[[nodiscard]]
	virtual bool isSiteLocal() const = 0;
	[[nodiscard]]
	virtual bool isIPv4Mapped() const = 0;
	[[nodiscard]]
	virtual bool isIPv4Compatible() const = 0;
	[[nodiscard]]
	virtual bool isWellKnownMC() const = 0;
	[[nodiscard]]
	virtual bool isNodeLocalMC() const = 0;
	[[nodiscard]]
	virtual bool isLinkLocalMC() const = 0;
	[[nodiscard]]
	virtual bool isSiteLocalMC() const = 0;
	[[nodiscard]]
	virtual bool isOrgLocalMC() const = 0;
	[[nodiscard]]
	virtual bool isGlobalMC() const = 0;
	virtual void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet) = 0;
	[[nodiscard]]
	virtual unsigned int prefixLength() const = 0;

protected:
	IPAddressImpl();

private:
	IPAddressImpl(const IPAddressImpl&);
	IPAddressImpl& operator = (const IPAddressImpl&);
};


//
// IPv4AddressImpl
//

class IPv4AddressImpl: public IPAddressImpl
{
public:
	IPv4AddressImpl();
	IPv4AddressImpl(const void* addr);
	IPv4AddressImpl(unsigned prefix);
	IPv4AddressImpl(const IPv4AddressImpl& addr);
	IPv4AddressImpl& operator = (const IPv4AddressImpl&);
	[[nodiscard]]
	std::string toString() const;
	[[nodiscard]]
	poco_socklen_t length() const;
	[[nodiscard]]
	const void* addr() const;
	[[nodiscard]]
	Family family() const;
	[[nodiscard]]
	int af() const;
	[[nodiscard]]
	unsigned int prefixLength() const;
	[[nodiscard]]
	Poco::UInt32 scope() const;
	[[nodiscard]]
	bool isWildcard() const;
	[[nodiscard]]
	bool isBroadcast() const;
	[[nodiscard]]
	bool isLoopback() const;
	[[nodiscard]]
	bool isMulticast() const;
	[[nodiscard]]
	bool isLinkLocal() const;
	[[nodiscard]]
	bool isSiteLocal() const;
	[[nodiscard]]
	bool isIPv4Compatible() const;
	[[nodiscard]]
	bool isIPv4Mapped() const;
	[[nodiscard]]
	bool isWellKnownMC() const;
	[[nodiscard]]
	bool isNodeLocalMC() const;
	[[nodiscard]]
	bool isLinkLocalMC() const;
	[[nodiscard]]
	bool isSiteLocalMC() const;
	[[nodiscard]]
	bool isOrgLocalMC() const;
	[[nodiscard]]
	bool isGlobalMC() const;
	[[nodiscard]]
	static IPv4AddressImpl parse(const std::string& addr);
	void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet);
	[[nodiscard]]
	IPAddressImpl* clone() const;
	[[nodiscard]]
	IPv4AddressImpl operator & (const IPv4AddressImpl& addr) const;
	[[nodiscard]]
	IPv4AddressImpl operator | (const IPv4AddressImpl& addr) const;
	[[nodiscard]]
	IPv4AddressImpl operator ^ (const IPv4AddressImpl& addr) const;
	[[nodiscard]]
	IPv4AddressImpl operator ~ () const;
	[[nodiscard]]
	bool operator == (const IPv4AddressImpl& addr) const;
	[[nodiscard]]
	bool operator != (const IPv4AddressImpl& addr) const;

private:
	struct in_addr _addr;
};


#if defined(POCO_HAVE_IPv6)


//
// IPv6AddressImpl
//

class IPv6AddressImpl: public IPAddressImpl
{
public:
	IPv6AddressImpl();
	IPv6AddressImpl(const void* addr);
	IPv6AddressImpl(const void* addr, Poco::UInt32 scope);
	IPv6AddressImpl(unsigned prefix);
	[[nodiscard]]
	std::string toString() const;
	[[nodiscard]]
	poco_socklen_t length() const;
	[[nodiscard]]
	const void* addr() const;
	[[nodiscard]]
	Family family() const;
	[[nodiscard]]
	int af() const;
	[[nodiscard]]
	unsigned int prefixLength() const;
	[[nodiscard]]
	Poco::UInt32 scope() const;
	[[nodiscard]]
	bool isWildcard() const;
	[[nodiscard]]
	bool isBroadcast() const;
	[[nodiscard]]
	bool isLoopback() const;
	[[nodiscard]]
	bool isMulticast() const;
	[[nodiscard]]
	bool isLinkLocal() const;
	[[nodiscard]]
	bool isSiteLocal() const;
	[[nodiscard]]
	bool isIPv4Compatible() const;
	[[nodiscard]]
	bool isIPv4Mapped() const;
	[[nodiscard]]
	bool isWellKnownMC() const;
	[[nodiscard]]
	bool isNodeLocalMC() const;
	[[nodiscard]]
	bool isLinkLocalMC() const;
	[[nodiscard]]
	bool isSiteLocalMC() const;
	[[nodiscard]]
	bool isOrgLocalMC() const;
	[[nodiscard]]
	bool isGlobalMC() const;
	[[nodiscard]]
	static IPv6AddressImpl parse(const std::string& addr);
	void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet);
	[[nodiscard]]
	IPAddressImpl* clone() const;
	[[nodiscard]]
	IPv6AddressImpl operator & (const IPv6AddressImpl& addr) const;
	[[nodiscard]]
	IPv6AddressImpl operator | (const IPv6AddressImpl& addr) const;
	[[nodiscard]]
	IPv6AddressImpl operator ^ (const IPv6AddressImpl& addr) const;
	[[nodiscard]]
	IPv6AddressImpl operator ~ () const;
	[[nodiscard]]
	bool operator == (const IPv6AddressImpl& addr) const;
	[[nodiscard]]
	bool operator != (const IPv6AddressImpl& addr) const;
	IPv6AddressImpl(const IPv6AddressImpl& addr);
	IPv6AddressImpl& operator = (const IPv6AddressImpl&);

private:
	struct in6_addr _addr;
	unsigned int    _scope;
};


#endif // POCO_HAVE_IPv6


} } } // namespace Poco::Net::Impl


#endif // Net_IPAddressImpl_INCLUDED
