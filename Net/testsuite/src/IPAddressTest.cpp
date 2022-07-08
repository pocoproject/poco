//
// IPAddressTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "IPAddressTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetException.h"


using Poco::Net::IPAddress;
using Poco::Net::InvalidAddressException;


IPAddressTest::IPAddressTest(const std::string& name): CppUnit::TestCase(name)
{
}


IPAddressTest::~IPAddressTest()
{
}


void IPAddressTest::testStringConv()
{
	IPAddress ia01 = IPAddress("127.0.0.1");
	IPAddress ia1(std::move(ia01));
	assertTrue (ia1.family() == IPAddress::IPv4);
	assertTrue (ia1.toString() == "127.0.0.1");
	
	IPAddress ia02 = IPAddress("192.168.1.120");
	IPAddress ia2(std::move(ia02));
	assertTrue (ia2.family() == IPAddress::IPv4);
	assertTrue (ia2.toString() == "192.168.1.120");

	IPAddress ia3("255.255.255.255");
	assertTrue (ia3.family() == IPAddress::IPv4);
	assertTrue (ia3.toString() == "255.255.255.255");

	IPAddress ia4("0.0.0.0");
	assertTrue (ia4.family() == IPAddress::IPv4);
	assertTrue (ia4.toString() == "0.0.0.0");

	IPAddress ia5(24,  IPAddress::IPv4);
	assertTrue (ia5.family() == IPAddress::IPv4);
	assertTrue (ia5.toString() == "255.255.255.0");
}


void IPAddressTest::testStringConv6()
{
#ifdef POCO_HAVE_IPv6
	IPAddress ia00 = IPAddress("::1");
	IPAddress ia0(std::move(ia00));
	assertTrue (ia0.family() == IPAddress::IPv6);
	assertTrue (ia0.toString() == "::1");

	IPAddress ia01 = IPAddress("1080:0:0:0:8:600:200a:425c");
	IPAddress ia1(std::move(ia01));
	assertTrue (ia1.family() == IPAddress::IPv6);
	assertTrue (ia1.toString() == "1080::8:600:200a:425c");
	
	IPAddress ia02 = IPAddress("1080::8:600:200A:425C");
	IPAddress ia2(std::move(ia02));
	assertTrue (ia2.family() == IPAddress::IPv6);
	assertTrue (ia2.toString() == "1080::8:600:200a:425c");

	IPAddress ia3("::192.168.1.120");
	assertTrue (ia3.family() == IPAddress::IPv6);
	assertTrue (ia3.toString() == "::192.168.1.120");

	IPAddress ia4("::ffff:192.168.1.120");
	assertTrue (ia4.family() == IPAddress::IPv6);
	assertTrue (ia4.toString() == "::ffff:192.168.1.120");

	IPAddress ia5(64, IPAddress::IPv6);
	assertTrue (ia5.family() == IPAddress::IPv6);
	assertTrue (ia5.toString() == "ffff:ffff:ffff:ffff::");

	IPAddress ia6(32, IPAddress::IPv6);
	assertTrue (ia6.family() == IPAddress::IPv6);
	assertTrue (ia6.toString() == "ffff:ffff::");

	IPAddress ia7("::");
	assertTrue (ia7.family() == IPAddress::IPv6);
	assertTrue (ia7.toString() == "::");
#endif
}


void IPAddressTest::testParse()
{
	IPAddress ip;
	assertTrue (IPAddress::tryParse("0.0.0.0", ip));
	assertTrue (IPAddress::tryParse("255.255.255.255", ip));
	assertTrue (IPAddress::tryParse("192.168.1.120", ip));
	assertTrue (!IPAddress::tryParse("192.168.1.280", ip));

	ip = IPAddress::parse("192.168.1.120");
	try
	{
		ip = IPAddress::parse("192.168.1.280");
		fail("bad address - must throw");
	}
	catch (InvalidAddressException&)
	{
	}

#ifdef POCO_HAVE_IPv6
	assertTrue (IPAddress::tryParse("::", ip));
	assertFalse (IPAddress::tryParse(":::", ip));
	assertTrue (IPAddress::tryParse("0::", ip));
	assertTrue (IPAddress::tryParse("0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0:0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0:0:0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0:0:0:0:0::", ip));
	assertTrue (IPAddress::tryParse("0:0:0:0:0:0:0:0", ip));
	assertFalse (IPAddress::tryParse("0:0:0:0:0:0:0:0:", ip));
	assertFalse (IPAddress::tryParse("::0:0::", ip));
	assertFalse (IPAddress::tryParse("::0::0::", ip));

	assertTrue (IPAddress::tryParse("::1", ip));
	assertTrue (IPAddress::tryParse("1080:0:0:0:8:600:200a:425c", ip));
	assertTrue (IPAddress::tryParse("1080::8:600:200a:425c", ip));
	assertTrue (IPAddress::tryParse("1080::8:600:200A:425C", ip));
	assertTrue (IPAddress::tryParse("1080::8:600:200a:425c", ip));
	assertTrue (IPAddress::tryParse("::192.168.1.120", ip));
	assertTrue (IPAddress::tryParse("::ffff:192.168.1.120", ip));
	assertTrue (IPAddress::tryParse("::ffff:192.168.1.120", ip));
	assertTrue (IPAddress::tryParse("ffff:ffff:ffff:ffff::", ip));
	assertTrue (IPAddress::tryParse("ffff:ffff::", ip));
#endif
}


void IPAddressTest::testClassification()
{
	IPAddress ip1("0.0.0.0"); // wildcard
	assertTrue (ip1.isWildcard());
	assertTrue (!ip1.isBroadcast());
	assertTrue (!ip1.isLoopback());
	assertTrue (!ip1.isMulticast());
	assertTrue (!ip1.isUnicast());
	assertTrue (!ip1.isLinkLocal());
	assertTrue (!ip1.isSiteLocal());
	assertTrue (!ip1.isWellKnownMC());
	assertTrue (!ip1.isNodeLocalMC());
	assertTrue (!ip1.isLinkLocalMC());
	assertTrue (!ip1.isSiteLocalMC());
	assertTrue (!ip1.isOrgLocalMC());
	assertTrue (!ip1.isGlobalMC());

	IPAddress ip2("255.255.255.255"); // broadcast
	assertTrue (!ip2.isWildcard());
	assertTrue (ip2.isBroadcast());
	assertTrue (!ip2.isLoopback());
	assertTrue (!ip2.isMulticast());
	assertTrue (!ip2.isUnicast());
	assertTrue (!ip2.isLinkLocal());
	assertTrue (!ip2.isSiteLocal());
	assertTrue (!ip2.isWellKnownMC());
	assertTrue (!ip2.isNodeLocalMC());
	assertTrue (!ip2.isLinkLocalMC());
	assertTrue (!ip2.isSiteLocalMC());
	assertTrue (!ip2.isOrgLocalMC());
	assertTrue (!ip2.isGlobalMC());

	IPAddress ip3("127.0.0.1"); // loopback
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (ip3.isLoopback());
	assertTrue (!ip3.isMulticast());
	assertTrue (ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (!ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (!ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());

	IPAddress ip4("80.122.195.86"); // unicast
	assertTrue (!ip4.isWildcard());
	assertTrue (!ip4.isBroadcast());
	assertTrue (!ip4.isLoopback());
	assertTrue (!ip4.isMulticast());
	assertTrue (ip4.isUnicast());
	assertTrue (!ip4.isLinkLocal());
	assertTrue (!ip4.isSiteLocal());
	assertTrue (!ip4.isWellKnownMC());
	assertTrue (!ip4.isNodeLocalMC());
	assertTrue (!ip4.isLinkLocalMC());
	assertTrue (!ip4.isSiteLocalMC());
	assertTrue (!ip4.isOrgLocalMC());
	assertTrue (!ip4.isGlobalMC());

	IPAddress ip5("169.254.1.20"); // link local unicast
	assertTrue (!ip5.isWildcard());
	assertTrue (!ip5.isBroadcast());
	assertTrue (!ip5.isLoopback());
	assertTrue (!ip5.isMulticast());
	assertTrue (ip5.isUnicast());
	assertTrue (ip5.isLinkLocal());
	assertTrue (!ip5.isSiteLocal());
	assertTrue (!ip5.isWellKnownMC());
	assertTrue (!ip5.isNodeLocalMC());
	assertTrue (!ip5.isLinkLocalMC());
	assertTrue (!ip5.isSiteLocalMC());
	assertTrue (!ip5.isOrgLocalMC());
	assertTrue (!ip5.isGlobalMC());

	IPAddress ip6("192.168.1.120"); // site local unicast
	assertTrue (!ip6.isWildcard());
	assertTrue (!ip6.isBroadcast());
	assertTrue (!ip6.isLoopback());
	assertTrue (!ip6.isMulticast());
	assertTrue (ip6.isUnicast());
	assertTrue (!ip6.isLinkLocal());
	assertTrue (ip6.isSiteLocal());
	assertTrue (!ip6.isWellKnownMC());
	assertTrue (!ip6.isNodeLocalMC());
	assertTrue (!ip6.isLinkLocalMC());
	assertTrue (!ip6.isSiteLocalMC());
	assertTrue (!ip6.isOrgLocalMC());
	assertTrue (!ip6.isGlobalMC());

	IPAddress ip7("10.0.0.138"); // site local unicast
	assertTrue (!ip7.isWildcard());
	assertTrue (!ip7.isBroadcast());
	assertTrue (!ip7.isLoopback());
	assertTrue (!ip7.isMulticast());
	assertTrue (ip7.isUnicast());
	assertTrue (!ip7.isLinkLocal());
	assertTrue (ip7.isSiteLocal());
	assertTrue (!ip7.isWellKnownMC());
	assertTrue (!ip7.isNodeLocalMC());
	assertTrue (!ip7.isLinkLocalMC());
	assertTrue (!ip7.isSiteLocalMC());
	assertTrue (!ip7.isOrgLocalMC());
	assertTrue (!ip7.isGlobalMC());

	IPAddress ip8("172.18.1.200"); // site local unicast
	assertTrue (!ip8.isWildcard());
	assertTrue (!ip8.isBroadcast());
	assertTrue (!ip8.isLoopback());
	assertTrue (!ip8.isMulticast());
	assertTrue (ip8.isUnicast());
	assertTrue (!ip8.isLinkLocal());
	assertTrue (ip8.isSiteLocal());
	assertTrue (!ip8.isWellKnownMC());
	assertTrue (!ip8.isNodeLocalMC());
	assertTrue (!ip8.isLinkLocalMC());
	assertTrue (!ip8.isSiteLocalMC());
	assertTrue (!ip8.isOrgLocalMC());
	assertTrue (!ip8.isGlobalMC());
}


void IPAddressTest::testMCClassification()
{
	IPAddress ip1("224.0.0.100"); // well-known multicast
	assertTrue (!ip1.isWildcard());
	assertTrue (!ip1.isBroadcast());
	assertTrue (!ip1.isLoopback());
	assertTrue (ip1.isMulticast());
	assertTrue (!ip1.isUnicast());
	assertTrue (!ip1.isLinkLocal());
	assertTrue (!ip1.isSiteLocal());
	assertTrue (ip1.isWellKnownMC());
	assertTrue (!ip1.isNodeLocalMC());
	assertTrue (ip1.isLinkLocalMC()); // well known are in the range of link local
	assertTrue (!ip1.isSiteLocalMC());
	assertTrue (!ip1.isOrgLocalMC());
	assertTrue (!ip1.isGlobalMC());

	IPAddress ip2("224.1.0.100"); // link local unicast
	assertTrue (!ip2.isWildcard());
	assertTrue (!ip2.isBroadcast());
	assertTrue (!ip2.isLoopback());
	assertTrue (ip2.isMulticast());
	assertTrue (!ip2.isUnicast());
	assertTrue (!ip2.isLinkLocal());
	assertTrue (!ip2.isSiteLocal());
	assertTrue (!ip2.isWellKnownMC());
	assertTrue (!ip2.isNodeLocalMC());
	assertTrue (ip2.isLinkLocalMC());
	assertTrue (!ip2.isSiteLocalMC());
	assertTrue (!ip2.isOrgLocalMC());
	assertTrue (ip2.isGlobalMC()); // link local fall in the range of global

	IPAddress ip3("239.255.0.100"); // site local unicast
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (!ip3.isLoopback());
	assertTrue (ip3.isMulticast());
	assertTrue (!ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (!ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());

	IPAddress ip4("239.192.0.100"); // org local unicast
	assertTrue (!ip4.isWildcard());
	assertTrue (!ip4.isBroadcast());
	assertTrue (!ip4.isLoopback());
	assertTrue (ip4.isMulticast());
	assertTrue (!ip4.isUnicast());
	assertTrue (!ip4.isLinkLocal());
	assertTrue (!ip4.isSiteLocal());
	assertTrue (!ip4.isWellKnownMC());
	assertTrue (!ip4.isNodeLocalMC());
	assertTrue (!ip4.isLinkLocalMC());
	assertTrue (!ip4.isSiteLocalMC());
	assertTrue (ip4.isOrgLocalMC());
	assertTrue (!ip4.isGlobalMC());

	IPAddress ip5("224.2.127.254"); // global unicast
	assertTrue (!ip5.isWildcard());
	assertTrue (!ip5.isBroadcast());
	assertTrue (!ip5.isLoopback());
	assertTrue (ip5.isMulticast());
	assertTrue (!ip5.isUnicast());
	assertTrue (!ip5.isLinkLocal());
	assertTrue (!ip5.isSiteLocal());
	assertTrue (!ip5.isWellKnownMC());
	assertTrue (!ip5.isNodeLocalMC());
	assertTrue (ip5.isLinkLocalMC()); // link local fall in the range of global
	assertTrue (!ip5.isSiteLocalMC());
	assertTrue (!ip5.isOrgLocalMC());
	assertTrue (ip5.isGlobalMC());
}


void IPAddressTest::testClassification6()
{
#ifdef POCO_HAVE_IPv6
	IPAddress ip1("::"); // wildcard
	assertTrue (ip1.isWildcard());
	assertTrue (!ip1.isBroadcast());
	assertTrue (!ip1.isLoopback());
	assertTrue (!ip1.isMulticast());
	assertTrue (!ip1.isUnicast());
	assertTrue (!ip1.isLinkLocal());
	assertTrue (!ip1.isSiteLocal());
	assertTrue (!ip1.isWellKnownMC());
	assertTrue (!ip1.isNodeLocalMC());
	assertTrue (!ip1.isLinkLocalMC());
	assertTrue (!ip1.isSiteLocalMC());
	assertTrue (!ip1.isOrgLocalMC());
	assertTrue (!ip1.isGlobalMC());

	IPAddress ip3("::1"); // loopback
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (ip3.isLoopback());
	assertTrue (!ip3.isMulticast());
	assertTrue (ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (!ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (!ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());

	IPAddress ip4("2001:0db8:85a3:0000:0000:8a2e:0370:7334"); // unicast
	assertTrue (!ip4.isWildcard());
	assertTrue (!ip4.isBroadcast());
	assertTrue (!ip4.isLoopback());
	assertTrue (!ip4.isMulticast());
	assertTrue (ip4.isUnicast());
	assertTrue (!ip4.isLinkLocal());
	assertTrue (!ip4.isSiteLocal());
	assertTrue (!ip4.isWellKnownMC());
	assertTrue (!ip4.isNodeLocalMC());
	assertTrue (!ip4.isLinkLocalMC());
	assertTrue (!ip4.isSiteLocalMC());
	assertTrue (!ip4.isOrgLocalMC());
	assertTrue (!ip4.isGlobalMC());

	IPAddress ip5("fe80::21f:5bff:fec6:6707"); // link local unicast
	assertTrue (!ip5.isWildcard());
	assertTrue (!ip5.isBroadcast());
	assertTrue (!ip5.isLoopback());
	assertTrue (!ip5.isMulticast());
	assertTrue (ip5.isUnicast());
	assertTrue (ip5.isLinkLocal());
	assertTrue (!ip5.isSiteLocal());
	assertTrue (!ip5.isWellKnownMC());
	assertTrue (!ip5.isNodeLocalMC());
	assertTrue (!ip5.isLinkLocalMC());
	assertTrue (!ip5.isSiteLocalMC());
	assertTrue (!ip5.isOrgLocalMC());
	assertTrue (!ip5.isGlobalMC());

	IPAddress ip10("fe80::12"); // link local unicast
	assertTrue (!ip10.isWildcard());
	assertTrue (!ip10.isBroadcast());
	assertTrue (!ip10.isLoopback());
	assertTrue (!ip10.isMulticast());
	assertTrue (ip10.isUnicast());
	assertTrue (ip10.isLinkLocal());
	assertTrue (!ip10.isSiteLocal());
	assertTrue (!ip10.isWellKnownMC());
	assertTrue (!ip10.isNodeLocalMC());
	assertTrue (!ip10.isLinkLocalMC());
	assertTrue (!ip10.isSiteLocalMC());
	assertTrue (!ip10.isOrgLocalMC());
	assertTrue (!ip10.isGlobalMC());

	IPAddress ip6("fec0::21f:5bff:fec6:6707"); // site local unicast (RFC 4291)
	assertTrue (!ip6.isWildcard());
	assertTrue (!ip6.isBroadcast());
	assertTrue (!ip6.isLoopback());
	assertTrue (!ip6.isMulticast());
	assertTrue (ip6.isUnicast());
	assertTrue (!ip6.isLinkLocal());
	assertTrue (ip6.isSiteLocal());
	assertTrue (!ip6.isWellKnownMC());
	assertTrue (!ip6.isNodeLocalMC());
	assertTrue (!ip6.isLinkLocalMC());
	assertTrue (!ip6.isSiteLocalMC());
	assertTrue (!ip6.isOrgLocalMC());
	assertTrue (!ip6.isGlobalMC());

	IPAddress ip7("fc00::21f:5bff:fec6:6707"); // site local unicast (RFC 4193)
	assertTrue (!ip7.isWildcard());
	assertTrue (!ip7.isBroadcast());
	assertTrue (!ip7.isLoopback());
	assertTrue (!ip7.isMulticast());
	assertTrue (ip7.isUnicast());
	assertTrue (!ip7.isLinkLocal());
	assertTrue (ip7.isSiteLocal());
	assertTrue (!ip7.isWellKnownMC());
	assertTrue (!ip7.isNodeLocalMC());
	assertTrue (!ip7.isLinkLocalMC());
	assertTrue (!ip7.isSiteLocalMC());
	assertTrue (!ip7.isOrgLocalMC());
	assertTrue (!ip7.isGlobalMC());

	IPAddress ip8("::ffff:127.0.0.1"); // IPv4-mapped loopback
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (ip3.isLoopback());
	assertTrue (!ip3.isMulticast());
	assertTrue (ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (!ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (!ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());

	IPAddress ip9("::ffff:127.255.255.254"); // IPv4-mapped loopback
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (ip3.isLoopback());
	assertTrue (!ip3.isMulticast());
	assertTrue (ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (!ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (!ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());
#endif
}


void IPAddressTest::testMCClassification6()
{
#ifdef POCO_HAVE_IPv6
	IPAddress ip1("ff02:0:0:0:0:0:0:c"); // well-known link-local multicast
	assertTrue (!ip1.isWildcard());
	assertTrue (!ip1.isBroadcast());
	assertTrue (!ip1.isLoopback());
	assertTrue (ip1.isMulticast());
	assertTrue (!ip1.isUnicast());
	assertTrue (!ip1.isLinkLocal());
	assertTrue (!ip1.isSiteLocal());
	assertTrue (ip1.isWellKnownMC());
	assertTrue (!ip1.isNodeLocalMC());
	assertTrue (ip1.isLinkLocalMC());
	assertTrue (!ip1.isSiteLocalMC());
	assertTrue (!ip1.isOrgLocalMC());
	assertTrue (!ip1.isGlobalMC());

	IPAddress ip2("ff01:0:0:0:0:0:0:FB"); // node-local unicast
	assertTrue (!ip2.isWildcard());
	assertTrue (!ip2.isBroadcast());
	assertTrue (!ip2.isLoopback());
	assertTrue (ip2.isMulticast());
	assertTrue (!ip2.isUnicast());
	assertTrue (!ip2.isLinkLocal());
	assertTrue (!ip2.isSiteLocal());
	assertTrue (ip2.isWellKnownMC());
	assertTrue (ip2.isNodeLocalMC());
	assertTrue (!ip2.isLinkLocalMC());
	assertTrue (!ip2.isSiteLocalMC());
	assertTrue (!ip2.isOrgLocalMC());
	assertTrue (!ip2.isGlobalMC());

	IPAddress ip3("ff05:0:0:0:0:0:0:FB"); // site local unicast
	assertTrue (!ip3.isWildcard());
	assertTrue (!ip3.isBroadcast());
	assertTrue (!ip3.isLoopback());
	assertTrue (ip3.isMulticast());
	assertTrue (!ip3.isUnicast());
	assertTrue (!ip3.isLinkLocal());
	assertTrue (!ip3.isSiteLocal());
	assertTrue (ip3.isWellKnownMC());
	assertTrue (!ip3.isNodeLocalMC());
	assertTrue (!ip3.isLinkLocalMC());
	assertTrue (ip3.isSiteLocalMC());
	assertTrue (!ip3.isOrgLocalMC());
	assertTrue (!ip3.isGlobalMC());

	IPAddress ip4("ff18:0:0:0:0:0:0:FB"); // org local unicast
	assertTrue (!ip4.isWildcard());
	assertTrue (!ip4.isBroadcast());
	assertTrue (!ip4.isLoopback());
	assertTrue (ip4.isMulticast());
	assertTrue (!ip4.isUnicast());
	assertTrue (!ip4.isLinkLocal());
	assertTrue (!ip4.isSiteLocal());
	assertTrue (!ip4.isWellKnownMC());
	assertTrue (!ip4.isNodeLocalMC());
	assertTrue (!ip4.isLinkLocalMC());
	assertTrue (!ip4.isSiteLocalMC());
	assertTrue (ip4.isOrgLocalMC());
	assertTrue (!ip4.isGlobalMC());

	IPAddress ip5("ff1f:0:0:0:0:0:0:FB"); // global unicast
	assertTrue (!ip5.isWildcard());
	assertTrue (!ip5.isBroadcast());
	assertTrue (!ip5.isLoopback());
	assertTrue (ip5.isMulticast());
	assertTrue (!ip5.isUnicast());
	assertTrue (!ip5.isLinkLocal());
	assertTrue (!ip5.isSiteLocal());
	assertTrue (!ip5.isWellKnownMC());
	assertTrue (!ip5.isNodeLocalMC());
	assertTrue (!ip5.isLinkLocalMC());
	assertTrue (!ip5.isSiteLocalMC());
	assertTrue (!ip5.isOrgLocalMC());
	assertTrue (ip5.isGlobalMC());
#endif
}


void IPAddressTest::testRelationals()
{
	IPAddress ip1("192.168.1.120");
	IPAddress ip2(ip1);
	IPAddress ip3;
	IPAddress ip4("10.0.0.138");

	assertTrue (ip1 != ip4);
	assertTrue (ip1 == ip2);
	assertTrue (!(ip1 != ip2));
	assertTrue (!(ip1 == ip4));
	assertTrue (ip1 > ip4);
	assertTrue (ip1 >= ip4);
	assertTrue (ip4 < ip1);
	assertTrue (ip4 <= ip1);
	assertTrue (!(ip1 < ip4));
	assertTrue (!(ip1 <= ip4));
	assertTrue (!(ip4 > ip1));
	assertTrue (!(ip4 >= ip1));

	ip3 = ip1;
	assertTrue (ip1 == ip3);
	ip3 = ip4;
	assertTrue (ip1 != ip3);
	assertTrue (ip3 == ip4);
}


void IPAddressTest::testWildcard()
{
	IPAddress wildcard = IPAddress::wildcard();
	assertTrue (wildcard.isWildcard());
	assertTrue (wildcard.toString() == "0.0.0.0");
}


void IPAddressTest::testBroadcast()
{
	IPAddress broadcast = IPAddress::broadcast();
	assertTrue (broadcast.isBroadcast());
	assertTrue (broadcast.toString() == "255.255.255.255");
}


void IPAddressTest::testPrefixCons()
{
	IPAddress ia1(15, IPAddress::IPv4);
	assertTrue (ia1.toString() == "255.254.0.0");

#ifdef POCO_HAVE_IPv6
	IPAddress ia2(62, IPAddress::IPv6);
	assertTrue (ia2.toString() == "ffff:ffff:ffff:fffc::");
#endif
}


void IPAddressTest::testPrefixLen()
{
	IPAddress ia1(15, IPAddress::IPv4);
	assertTrue (ia1.prefixLength() == 15);

	IPAddress ia2(16, IPAddress::IPv4);
	assertTrue (ia2.prefixLength() == 16);

	IPAddress ia3(23, IPAddress::IPv4);
	assertTrue (ia3.prefixLength() == 23);

	IPAddress ia4(24, IPAddress::IPv4);
	assertTrue (ia4.prefixLength() == 24);

	IPAddress ia5(25, IPAddress::IPv4);
	assertTrue (ia5.prefixLength() == 25);

#ifdef POCO_HAVE_IPv6
	IPAddress ia6(62, IPAddress::IPv6);
	assertTrue (ia6.prefixLength() == 62);

	IPAddress ia7(63, IPAddress::IPv6);
	assertTrue (ia7.prefixLength() == 63);

	IPAddress ia8(64, IPAddress::IPv6);
	assertTrue (ia8.prefixLength() == 64);

	IPAddress ia9(65, IPAddress::IPv6);
	assertTrue (ia9.prefixLength() == 65);
#endif
}


void IPAddressTest::testOperators()
{
	IPAddress ip("10.0.0.51");
	IPAddress mask(24, IPAddress::IPv4);

	IPAddress net = ip & mask;
	assertTrue (net.toString() == "10.0.0.0");

	IPAddress host("0.0.0.51");
	assertTrue ((net | host) == ip);

	assertTrue ((~mask).toString() == "0.0.0.255");
}


void IPAddressTest::testRelationals6()
{
#ifdef POCO_HAVE_IPv6
#endif
}


void IPAddressTest::testByteOrderMacros()
{
	Poco::UInt16 a16 = 0xDEAD;
	assertTrue (poco_ntoh_16(a16) == ntohs(a16));
	assertTrue (poco_hton_16(a16) == htons(a16));
	Poco::UInt32 a32 = 0xDEADBEEF;
	assertTrue (poco_ntoh_32(a32) == ntohl(a32));
	assertTrue (poco_hton_32(a32) == htonl(a32));
}


void IPAddressTest::setUp()
{
}


void IPAddressTest::tearDown()
{
}


CppUnit::Test* IPAddressTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("IPAddressTest");

	CppUnit_addTest(pSuite, IPAddressTest, testStringConv);
	CppUnit_addTest(pSuite, IPAddressTest, testStringConv6);
	CppUnit_addTest(pSuite, IPAddressTest, testParse);
	CppUnit_addTest(pSuite, IPAddressTest, testClassification);
	CppUnit_addTest(pSuite, IPAddressTest, testMCClassification);
	CppUnit_addTest(pSuite, IPAddressTest, testClassification6);
	CppUnit_addTest(pSuite, IPAddressTest, testMCClassification6);
	CppUnit_addTest(pSuite, IPAddressTest, testRelationals);
	CppUnit_addTest(pSuite, IPAddressTest, testRelationals6);
	CppUnit_addTest(pSuite, IPAddressTest, testWildcard);
	CppUnit_addTest(pSuite, IPAddressTest, testBroadcast);
	CppUnit_addTest(pSuite, IPAddressTest, testPrefixCons);
	CppUnit_addTest(pSuite, IPAddressTest, testPrefixLen);
	CppUnit_addTest(pSuite, IPAddressTest, testOperators);
	CppUnit_addTest(pSuite, IPAddressTest, testByteOrderMacros);

	return pSuite;
}
