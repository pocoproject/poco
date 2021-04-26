//
// EndpointTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "EndpointTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/Endpoint.h"
#include "Poco/Net/NetException.h"


using Poco::Net::Endpoint;
using Poco::Net::IPAddress;
using Poco::Net::InvalidAddressException;
using Poco::Net::HostNotFoundException;
using Poco::Net::ServiceNotFoundException;
using Poco::Net::NoAddressFoundException;
using Poco::Net::AddressFamilyMismatchException;
using Poco::InvalidArgumentException;


EndpointTest::EndpointTest(const std::string& name): CppUnit::TestCase(name)
{
}


EndpointTest::~EndpointTest()
{
}


void EndpointTest::testEndpoint()
{
	Endpoint wild;
	assertTrue (wild.host().isWildcard());
	assertTrue (wild.port() == 0);

	Endpoint sa1("192.168.1.100", 100);
	assertTrue (sa1.af() == AF_INET);
	assertTrue (sa1.family() == Endpoint::IPv4);
	assertTrue (sa1.host().toString() == "192.168.1.100");
	assertTrue (sa1.port() == 100);
	assertTrue (sa1.toString() == "192.168.1.100:100");

	Endpoint sa2("192.168.1.100", "100");
	assertTrue (sa2.host().toString() == "192.168.1.100");
	assertTrue (sa2.port() == 100);

#if !defined(_WIN32_WCE)
	Endpoint sa3("192.168.1.100", "ftp");
	assertTrue (sa3.host().toString() == "192.168.1.100");
	assertTrue (sa3.port() == 21);
#endif

	try
	{
		Endpoint sa3("192.168.1.100", "f00bar");
		fail("bad service name - must throw");
	}
	catch (ServiceNotFoundException&)
	{
	}

	Endpoint sa4("pocoproject.org", 80);
	assertTrue (sa4.host().toString() == "54.93.62.90");
	assertTrue (sa4.port() == 80);

	try
	{
		Endpoint sa5("192.168.2.260", 80);
		fail("invalid address - must throw");
	}
	catch (HostNotFoundException&)
	{
	}
	catch (NoAddressFoundException&)
	{
	}

	try
	{
		Endpoint sa6("192.168.2.120", "80000");
		fail("invalid port - must throw");
	}
	catch (ServiceNotFoundException&)
	{
	}

	Endpoint sa7("192.168.2.120:88");
	assertTrue (sa7.host().toString() == "192.168.2.120");
	assertTrue (sa7.port() == 88);

	Endpoint sa8("[192.168.2.120]:88");
	assertTrue (sa8.host().toString() == "192.168.2.120");
	assertTrue (sa8.port() == 88);

	try
	{
		Endpoint sa9("[192.168.2.260]");
		fail("invalid address - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		Endpoint sa9("[192.168.2.260:88");
		fail("invalid address - must throw");
	}
	catch (InvalidArgumentException&)
	{
	}

	Endpoint sa10("www6.pocoproject.org", 80);
	assertTrue (sa10.host().toString() == "54.93.62.90" || sa10.host().toString() == "[2001:4801:7828:101:be76:4eff:fe10:1455]");

	Endpoint sa11(Endpoint::IPv4, "www6.pocoproject.org", 80);
	assertTrue (sa11.host().toString() == "54.93.62.90");

#ifdef POCO_NET_ENABLE_IPv6
	try
	{
		Endpoint sa12(Endpoint::IPv6, "www6.pocoproject.org", 80);
		assertTrue (sa12.host().toString() == "2001:4801:7828:101:be76:4eff:fe10:1455");
	}
	catch (AddressFamilyMismatchException&)
	{
		// may happen if no IPv6 address is configured on the system
	}
#endif
}


void EndpointTest::testSocketRelationals()
{
	Endpoint sa1("192.168.1.100", 100);
    Endpoint sa2("192.168.1.100:100");
	assertTrue (sa1 == sa2);

    Endpoint sa3("192.168.1.101", "99");
	assertTrue (sa2 < sa3);

	Endpoint sa4("192.168.1.101", "102");
	assertTrue (sa3 < sa4);
}


void EndpointTest::testEndpoint6()
{
#ifdef POCO_NET_ENABLE_IPv6
	Endpoint sa1("FE80::E6CE:8FFF:FE4A:EDD0", 100);
	assertTrue (sa1.af() == AF_INET6);
	assertTrue (sa1.family() == Endpoint::IPv6);
	assertTrue (sa1.host().toString() == "fe80::e6ce:8fff:fe4a:edd0");
	assertTrue (sa1.port() == 100);
	assertTrue (sa1.toString() == "[fe80::e6ce:8fff:fe4a:edd0]:100");

	Endpoint sa2("[FE80::E6CE:8FFF:FE4A:EDD0]:100");
	assertTrue (sa2.af() == AF_INET6);
	assertTrue (sa2.family() == Endpoint::IPv6);
	assertTrue (sa2.host().toString() == "fe80::e6ce:8fff:fe4a:edd0");
	assertTrue (sa2.port() == 100);
	assertTrue (sa2.toString() == "[fe80::e6ce:8fff:fe4a:edd0]:100");
#endif
}


void EndpointTest::testEndpointUnixLocal()
{
#ifdef POCO_NET_ENABLE_UNIX_SOCKET
	Endpoint sa1(Endpoint::UNIX_LOCAL, "/tmp/sock1");
	assertTrue (sa1.af() == AF_UNIX);
	assertTrue (sa1.family() == Endpoint::UNIX_LOCAL);
	assertTrue (sa1.toString() == "/tmp/sock1");

	Endpoint sa2(Endpoint::UNIX_LOCAL, "/tmp/sock2");
	assertTrue (sa1 != sa2);
	assertTrue (sa1 < sa2);

	Endpoint sa3(Endpoint::UNIX_LOCAL, "/tmp/sock1");
	assertTrue (sa1 == sa3);
	assertTrue (!(sa1 < sa3));

	Endpoint sa4("/tmp/sock1");
	assertTrue (sa1 == sa4);
	assertTrue (sa4.toString() == "/tmp/sock1");
#endif
}


void EndpointTest::setUp()
{
}


void EndpointTest::tearDown()
{
}


CppUnit::Test* EndpointTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EndpointTest");

	CppUnit_addTest(pSuite, EndpointTest, testEndpoint);
	CppUnit_addTest(pSuite, EndpointTest, testSocketRelationals);
	CppUnit_addTest(pSuite, EndpointTest, testEndpoint6);
	CppUnit_addTest(pSuite, EndpointTest, testEndpointUnixLocal);

	return pSuite;
}
