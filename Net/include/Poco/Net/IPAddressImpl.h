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

	~IPAddressImpl() override;

	virtual IPAddressImpl* clone() const = 0;
	virtual std::string toString() const = 0;
	virtual poco_socklen_t length() const = 0;
	virtual const void* addr() const = 0;
	virtual Family family() const = 0;
	virtual int af() const = 0;
	virtual Poco::UInt32 scope() const = 0;
	virtual bool isWildcard() const	= 0;
	virtual bool isBroadcast() const = 0;
	virtual bool isLoopback() const = 0;
	virtual bool isMulticast() const = 0;
	virtual bool isLinkLocal() const = 0;
	virtual bool isSiteLocal() const = 0;
	virtual bool isIPv4Mapped() const = 0;
	virtual bool isIPv4Compatible() const = 0;
	virtual bool isWellKnownMC() const = 0;
	virtual bool isNodeLocalMC() const = 0;
	virtual bool isLinkLocalMC() const = 0;
	virtual bool isSiteLocalMC() const = 0;
	virtual bool isOrgLocalMC() const = 0;
	virtual bool isGlobalMC() const = 0;
	virtual void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet) = 0;
	virtual unsigned prefixLength() const = 0;

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
	std::string toString() const override;
	poco_socklen_t length() const override;
	const void* addr() const override;
	Family family() const override;
	int af() const override;
	unsigned prefixLength() const override;
	Poco::UInt32 scope() const override;
	bool isWildcard() const override;
	bool isBroadcast() const override;
	bool isLoopback() const override;
	bool isMulticast() const override;
	bool isLinkLocal() const override;
	bool isSiteLocal() const override;
	bool isIPv4Compatible() const override;
	bool isIPv4Mapped() const override;
	bool isWellKnownMC() const override;
	bool isNodeLocalMC() const override;
	bool isLinkLocalMC() const override;
	bool isSiteLocalMC() const override;
	bool isOrgLocalMC() const override;
	bool isGlobalMC() const override;
	static IPv4AddressImpl parse(const std::string& addr);
	void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet) override;
	IPAddressImpl* clone() const override;
	IPv4AddressImpl operator & (const IPv4AddressImpl& addr) const;
	IPv4AddressImpl operator | (const IPv4AddressImpl& addr) const;
	IPv4AddressImpl operator ^ (const IPv4AddressImpl& addr) const;
	IPv4AddressImpl operator ~ () const;
	bool operator == (const IPv4AddressImpl& addr) const;
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
	std::string toString() const override;
	poco_socklen_t length() const override;
	const void* addr() const override;
	Family family() const override;
	int af() const override;
	unsigned prefixLength() const override;
	Poco::UInt32 scope() const override;
	bool isWildcard() const override;
	bool isBroadcast() const override;
	bool isLoopback() const override;
	bool isMulticast() const override;
	bool isLinkLocal() const override;
	bool isSiteLocal() const override;
	bool isIPv4Compatible() const override;
	bool isIPv4Mapped() const override;
	bool isWellKnownMC() const override;
	bool isNodeLocalMC() const override;
	bool isLinkLocalMC() const override;
	bool isSiteLocalMC() const override;
	bool isOrgLocalMC() const override;
	bool isGlobalMC() const override;
	static IPv6AddressImpl parse(const std::string& addr);
	void mask(const IPAddressImpl* pMask, const IPAddressImpl* pSet) override;
	IPAddressImpl* clone() const override;
	IPv6AddressImpl operator & (const IPv6AddressImpl& addr) const;
	IPv6AddressImpl operator | (const IPv6AddressImpl& addr) const;
	IPv6AddressImpl operator ^ (const IPv6AddressImpl& addr) const;
	IPv6AddressImpl operator ~ () const;
	bool operator == (const IPv6AddressImpl& addr) const;
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
