//
// DNSTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DNSTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#if defined(POCO_OS_FAMILY_UNIX)
#include <netdb.h>
#endif
#include <cerrno>
#include <type_traits>


using Poco::Net::DNS;
using Poco::Net::IPAddress;
using Poco::Net::HostEntry;
using Poco::Net::InvalidAddressException;
using Poco::Net::HostNotFoundException;
using Poco::Net::ServiceNotFoundException;
using Poco::Net::NoAddressFoundException;
using Poco::Net::DNSException;
using Poco::NumberFormatter;


namespace
{
	class DNSErrorProbe: public Poco::Net::DNS
	{
	public:
		DNSErrorProbe() = delete;
		~DNSErrorProbe() = default;

		using DNS::aierror;
	};
}


DNSTest::DNSTest(const std::string& name): CppUnit::TestCase(name)
{
}


DNSTest::~DNSTest()
{
}


void DNSTest::testHostByName()
{
	HostEntry he1 = DNS::hostByName("aliastest.pocoproject.org");
	// different systems report different canonical names, unfortunately.
	assertTrue (he1.name() == "dnstest.pocoproject.org" || he1.name() == "aliastest.pocoproject.org");
#if !defined(POCO_HAVE_ADDRINFO)
	// getaddrinfo() does not report any aliases
	assertTrue (!he1.aliases().empty());
	assertTrue (he1.aliases()[0] == "aliastest.pocoproject.org");
#endif
	assertTrue (he1.addresses().size() >= 1);
	assertTrue (he1.addresses()[0].toString() == "1.2.3.4");

	try
	{
		HostEntry he1 = DNS::hostByName("nohost.pocoproject.org");
		fail("host not found - must throw");
	}
	catch (HostNotFoundException&)
	{
	}
	catch (NoAddressFoundException&)
	{
	}
}


void DNSTest::testHostByAddress()
{
	IPAddress ip1("80.122.195.86");
	HostEntry he1 = DNS::hostByAddress(ip1);
	assertTrue (he1.name() == "mailhost.appinf.com");
	assertTrue (he1.aliases().empty());
	assertTrue (he1.addresses().size() >= 1);
	assertTrue (he1.addresses()[0].toString() == "80.122.195.86");

	IPAddress ip2("10.0.244.253");
	try
	{
		HostEntry he2 = DNS::hostByAddress(ip2);
		fail("host not found - must throw");
	}
	catch (HostNotFoundException&)
	{
	}
	catch (NoAddressFoundException&)
	{
	}
}


void DNSTest::testResolve()
{
	HostEntry he1 = DNS::hostByName("localhost");

	auto a = he1.addresses();
	sort(a.begin(), a.end());
	auto itA = std::unique(a.begin(), a.end());
	assertTrue (itA == a.end());

	auto b = he1.aliases();
	sort(b.begin(), b.end());
	auto itB = std::unique(b.begin(), b.end());
	assertTrue (itB == b.end());
}


void DNSTest::testEncodeIDN()
{
	std::string idn("d\xc3\xb6m\xc3\xa4in.example"); // d"om"ain.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--dmin-moa0i.example");

	idn = ".d\xc3\xb6m\xc3\xa4in.example"; // .d"om"ain.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == ".xn--dmin-moa0i.example");

	idn = "d\xc3\xb6m\xc3\xa4in.example."; // .d"om"ain.example.
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--dmin-moa0i.example.");

	idn = "d\xc3\xb6m\xc3\xa4in"; // d"om"ain
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--dmin-moa0i");

	idn = "\xc3\xa4""aaa.example"; // "aaaa.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--aaa-pla.example");

	idn = "a\xc3\xa4""aa.example"; // a"aaa.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--aaa-qla.example");

	idn = "foo.\xc3\xa2""bcd\xc3\xa9""f.example"; // foo.^abcd'ef.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "foo.xn--bcdf-9na9b.example");

	idn = "\xe2\x98\x83.example"; // <snowman>.example
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--n3h.example");

	idn = "\xe2\x98\x83."; // <snowman>.
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--n3h.");

	idn = "\xe2\x98\x83"; // <snowman>
	assertTrue (DNS::isIDN(idn));
	assertTrue (DNS::encodeIDN(idn) == "xn--n3h");

	std::string dn = "www.pocoproject.org";
	assertTrue (!DNS::isIDN(dn));
	assertTrue (DNS::encodeIDN(dn) == "www.pocoproject.org");
}


void DNSTest::testDecodeIDN()
{
	std::string enc("xn--dmin-moa0i.example");
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "d\xc3\xb6m\xc3\xa4in.example"); // d"om"ain.example

	enc = ".xn--dmin-moa0i.example";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == ".d\xc3\xb6m\xc3\xa4in.example"); // .d"om"ain.example

	enc = "xn--dmin-moa0i.example.";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "d\xc3\xb6m\xc3\xa4in.example."); // d"om"ain.example.

	enc = "xn--dmin-moa0i";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "d\xc3\xb6m\xc3\xa4in"); // d"om"ain

	enc = "foo.xn--bcdf-9na9b.example";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "foo.\xc3\xa2""bcd\xc3\xa9""f.example"); // foo.^abcd'ef.example

	enc = "xn--n3h.example";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "\xe2\x98\x83.example"); // <snowman>.example

	enc = "xn--n3h.";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "\xe2\x98\x83."); // <snowman>.

	enc = "xn--n3h";
	assertTrue (DNS::isEncodedIDN(enc));
	assertTrue (DNS::decodeIDN(enc) == "\xe2\x98\x83"); // <snowman>

	std::string dn = "www.pocoproject.org";
	assertTrue (!DNS::isEncodedIDN(dn));
	assertTrue (DNS::decodeIDN(dn) == "www.pocoproject.org");
}


void DNSTest::testSystemError()
{
#if defined(EAI_SYSTEM)
	h_errno = HOST_NOT_FOUND;
	errno = EMFILE;
	try
	{
		DNSErrorProbe::aierror(EAI_SYSTEM, "example.com");
		fail("must throw");
	}
	catch (HostNotFoundException&)
	{
		failmsg("EAI_SYSTEM error was taken from h_errno instead of errno");
	}
	catch (DNSException& exc)
	{
		assertEqual (EMFILE, exc.code());
	}
#endif
}


void DNSTest::testHostByNameEmbeddedNul()
{
	for (const std::string& name: {std::string("localhost\0junk", 14), std::string("127.0.0.1\0junk", 14)})
	{
		try
		{
			HostEntry he = DNS::hostByName(name);
			fail("host name with embedded NUL - must throw");
		}
		catch (HostNotFoundException&)
		{
		}
	}
}


void DNSTest::testAIError()
{
#if defined(POCO_HAVE_IPv6) || defined(POCO_HAVE_ADDRINFO)
	const std::string host("host.example");
	bool thrown = false;
	try
	{
		DNSErrorProbe::aierror(EAI_MEMORY, host);
	}
	catch (DNSException& exc)
	{
		thrown = true;
		const std::string text = exc.displayText();
		assertEqual (EAI_MEMORY, exc.code());
		assertTrue (text.find("EAI " + NumberFormatter::format(EAI_MEMORY) + ": ") != std::string::npos);
		assertTrue (text.find(host) != std::string::npos);
		// The host stays on the line of the error text; a trace build appends lines after it.
		assertTrue (text.find_first_of("\r\n") > text.find(host));
#if !defined(_WIN32)
		assertTrue (text.find(gai_strerror(EAI_MEMORY)) != std::string::npos);
#endif
	}
	assertTrue (thrown);
#endif

#if POCO_OS == POCO_OS_LINUX
	constexpr unsigned UNKNOWN_HINT_FLAG = 0x40000000;
	const std::string localhost("localhost");
	try
	{
		// glibc rejects the unknown flag before any lookup, so no network is needed;
		// macOS ignores it and resolves the host.
		HostEntry he = DNS::hostByName(localhost, UNKNOWN_HINT_FLAG);
		fail("invalid hint flags - must throw");
	}
	catch (DNSException& exc)
	{
		const std::string text = exc.displayText();
		assertTrue (text.find(localhost) != std::string::npos);
		assertTrue (text.find(gai_strerror(EAI_BADFLAGS)) != std::string::npos);
	}
#endif
}


void DNSTest::testHostEntryMoveNoexcept()
{
	static_assert(std::is_nothrow_move_constructible_v<HostEntry>);
	static_assert(std::is_nothrow_move_assignable_v<HostEntry>);

	HostEntry he1 = DNS::hostByName("localhost");
	const HostEntry::AddressList addresses = he1.addresses();
	assertTrue (!addresses.empty());

	HostEntry he2(std::move(he1));
	assertTrue (he2.addresses() == addresses);

	HostEntry he3;
	he3 = std::move(he2);
	assertTrue (he3.addresses() == addresses);
}


void DNSTest::setUp()
{
}


void DNSTest::tearDown()
{
}


CppUnit::Test* DNSTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DNSTest");

	CppUnit_addTest(pSuite, DNSTest, testHostByName);
	CppUnit_addTest(pSuite, DNSTest, testHostByAddress);
	CppUnit_addTest(pSuite, DNSTest, testResolve);
	CppUnit_addTest(pSuite, DNSTest, testEncodeIDN);
	CppUnit_addTest(pSuite, DNSTest, testDecodeIDN);
	CppUnit_addTest(pSuite, DNSTest, testSystemError);
	CppUnit_addTest(pSuite, DNSTest, testHostByNameEmbeddedNul);
	CppUnit_addTest(pSuite, DNSTest, testAIError);
	CppUnit_addTest(pSuite, DNSTest, testHostEntryMoveNoexcept);

	return pSuite;
}
