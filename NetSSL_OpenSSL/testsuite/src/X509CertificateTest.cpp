//
// X509CertificateTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "X509CertificateTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/X509Certificate.h"
#include <sstream>
#include <string>


using Poco::Net::X509Certificate;


namespace
{
	// subject CN=test, subjectAltName DNS:f*.example.com
	const std::string PARTIAL_WILDCARD_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBjzCCATagAwIBAgIUHipMSw5jcxotLddQSTzt/WGC6MgwCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAgFw0yNjEwMTIxMjI2MDVaGA8yMTI2MDkxODEyMjYw\n"
		"NVowDzENMAsGA1UEAwwEdGVzdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABB4l\n"
		"TbuJFY7lJeZ9OTrzqcTBJ7amHHBe39eNAJ9pWptg3n/0+tJILHpErhrhczWWZmJO\n"
		"m1WU+w5mmBn5u9HCmZWjbjBsMB0GA1UdDgQWBBT30KcfyzgM/2wZhwKudQsDfvzH\n"
		"FTAfBgNVHSMEGDAWgBT30KcfyzgM/2wZhwKudQsDfvzHFTAPBgNVHRMBAf8EBTAD\n"
		"AQH/MBkGA1UdEQQSMBCCDmYqLmV4YW1wbGUuY29tMAoGCCqGSM49BAMCA0cAMEQC\n"
		"IAKqZ7lPN9G5yokrLa9cU1ij9PVdANmSoS4q/hl2XYsGAiBGQoiGxUPDpd1dUnDX\n"
		"3KQh/EAMHJIanJX6YnG815mzeQ==\n"
		"-----END CERTIFICATE-----\n");

	// subject CN=test, subjectAltName DNS:*.example.com
	const std::string WILDCARD_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBjzCCATWgAwIBAgIUIC5QuL31zGe9beY5yunShZk5W40wCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAgFw0yNjEwMTIxMjI2MDVaGA8yMTI2MDkxODEyMjYw\n"
		"NVowDzENMAsGA1UEAwwEdGVzdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABKlN\n"
		"1MG5vuRg916APg0Kau3wceZrfvVzaOZ//jyDv0SEx8uy6XUedyPaPpU8PpJy2HDV\n"
		"Tplc3NgtnYdYOnijC4ijbTBrMB0GA1UdDgQWBBQ0ScUyq2bHRNvDQtm7sQtMhHP3\n"
		"QjAfBgNVHSMEGDAWgBQ0ScUyq2bHRNvDQtm7sQtMhHP3QjAPBgNVHRMBAf8EBTAD\n"
		"AQH/MBgGA1UdEQQRMA+CDSouZXhhbXBsZS5jb20wCgYIKoZIzj0EAwIDSAAwRQIg\n"
		"XtYRjNic21AklyCblYr3Ts7vS+bIXaXNt8j2ys6kdYACIQDBdkau8aBHIt0K1LsF\n"
		"rYB6/wioeK6zPeRJXjRrGZeF/Q==\n"
		"-----END CERTIFICATE-----\n");

	// subject CN=test, subjectAltName DNS:10.0.0.1
	const std::string DNS_NAME_ADDRESS_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBiTCCATCgAwIBAgIUZ1M46BhWoui4kufyYeuQKz+WucQwCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAgFw0yNjEwMTIxMjI2MDVaGA8yMTI2MDkxODEyMjYw\n"
		"NVowDzENMAsGA1UEAwwEdGVzdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABJhO\n"
		"pj6iFm8BCGacyuysaouUW1Wnz0HGPa+eJrGu22vviEtYcJlbZDfs5+4jfvGzTuPi\n"
		"12Ctr90+HUTF4DanWT6jaDBmMB0GA1UdDgQWBBT2YBcUN0Ws1ive/DxaEPsXC4uK\n"
		"JjAfBgNVHSMEGDAWgBT2YBcUN0Ws1ive/DxaEPsXC4uKJjAPBgNVHRMBAf8EBTAD\n"
		"AQH/MBMGA1UdEQQMMAqCCDEwLjAuMC4xMAoGCCqGSM49BAMCA0cAMEQCICkTMSpf\n"
		"wxAN6u2HZpZXErnK5bBdKoo6cuFzC5u/s/YbAiAPh1jI7vTw+828k8dH/AdPsWnC\n"
		"2Ap3kpOMJOMSg6kmqQ==\n"
		"-----END CERTIFICATE-----\n");

	// subject CN=10.0.0.1, no subjectAltName
	const std::string COMMON_NAME_ADDRESS_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBfTCCASOgAwIBAgIUGnqF7KwmQC8ikrEBo4Z09LSkSOkwCgYIKoZIzj0EAwIw\n"
		"EzERMA8GA1UEAwwIMTAuMC4wLjEwIBcNMjYxMDEyMTIyNjA1WhgPMjEyNjA5MTgx\n"
		"MjI2MDVaMBMxETAPBgNVBAMMCDEwLjAuMC4xMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n"
		"AQcDQgAE47TaN/15zpNvL5/YFcFQ6Sy0FkjBOLl627K1wwe+5dBy9JHf1fsJOlDz\n"
		"ekLzYvHVp0Vb3Ii6m7f1cqerMnwnNqNTMFEwHQYDVR0OBBYEFA00WjosN1nLKQcj\n"
		"z/8mFfNVs5QxMB8GA1UdIwQYMBaAFA00WjosN1nLKQcjz/8mFfNVs5QxMA8GA1Ud\n"
		"EwEB/wQFMAMBAf8wCgYIKoZIzj0EAwIDSAAwRQIhAPC9P8slzDjnR53KcUW+OjTC\n"
		"9fCaK9h/rUyMPRvVvgPCAiBhvnTwoJFq8P+i/MMDVKMdmpdQ2d3PcdCk5znjOBy5\n"
		"Eg==\n"
		"-----END CERTIFICATE-----\n");

	// subject CN=test, subjectAltName IP:10.0.0.1
	const std::string IP_ADDRESS_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBhjCCASygAwIBAgIUHJnA934IZ1ZmASxPDygRc8pOizkwCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAgFw0yNjEwMTIxMjI2MDVaGA8yMTI2MDkxODEyMjYw\n"
		"NVowDzENMAsGA1UEAwwEdGVzdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABHWe\n"
		"s2od1AoalGYMx+1DKepo8SNIg8n644iChWCsq07axsx8N9HAy4B4lBbIWSODElHj\n"
		"eY6Rbi5hroPAN5mIlvujZDBiMB0GA1UdDgQWBBQj8o5aOtAD7hwVjUFUcTbfh60K\n"
		"ZTAfBgNVHSMEGDAWgBQj8o5aOtAD7hwVjUFUcTbfh60KZTAPBgNVHRMBAf8EBTAD\n"
		"AQH/MA8GA1UdEQQIMAaHBAoAAAEwCgYIKoZIzj0EAwIDSAAwRQIgEi/gkvXvDXLh\n"
		"fOOKy5EimQEWNKeK/04CDbGiN+CH63ICIQDnBLcEzFB6BFJsdnRM6ZSylZMrRCs2\n"
		"Ead7AzC7vQg9PQ==\n"
		"-----END CERTIFICATE-----\n");

	// subject CN=www.example.com, no subjectAltName
	const std::string COMMON_NAME_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBijCCATGgAwIBAgIUDvc7h1iT6sMsEUaxuu4CYDwzmxcwCgYIKoZIzj0EAwIw\n"
		"GjEYMBYGA1UEAwwPd3d3LmV4YW1wbGUuY29tMCAXDTI2MTAxMjEyMjYwNVoYDzIx\n"
		"MjYwOTE4MTIyNjA1WjAaMRgwFgYDVQQDDA93d3cuZXhhbXBsZS5jb20wWTATBgcq\n"
		"hkjOPQIBBggqhkjOPQMBBwNCAARyKmzOQ3OWmln58AL7N2wXmzA1nojWxGm2wInI\n"
		"oNINldDMkqYhQ4oxSRcHLkAOiX1p8SmP9e15tBCUZv8Z6MXlo1MwUTAdBgNVHQ4E\n"
		"FgQUdSPj2sobN5mcMmVgn23SXD89W04wHwYDVR0jBBgwFoAUdSPj2sobN5mcMmVg\n"
		"n23SXD89W04wDwYDVR0TAQH/BAUwAwEB/zAKBggqhkjOPQQDAgNHADBEAiBknx/1\n"
		"5EMWud7YZgJyRpNToOhIqIYekQDVHlSxfgBHJgIgZH6k/RaCeb0RmEBzjViK5Xy5\n"
		"4IbhfFJ6W6/IDD48rAk=\n"
		"-----END CERTIFICATE-----\n");


	// subject CN=test, subjectAltName DNS:www.example.com, valid from 2020-01-01 to 2021-01-01
	const std::string EXPIRED_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBbzCCARSgAwIBAgIUNcoB3VeYDfYXg02ziFO70RZ65SgwCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAeFw0yMDAxMDEwMDAwMDBaFw0yMTAxMDEwMDAwMDBa\n"
		"MA8xDTALBgNVBAMMBHRlc3QwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAScyZar\n"
		"Pb5PWodTyAyL9mq+TQNb/y0RBHMAJwJ3KQdUlKGRbb0FcNEDeNDFOxxY3EP9h8MP\n"
		"bhM7KIJupyOpnE7Fo04wTDAdBgNVHQ4EFgQUA2o9knmX/3hHPd60VxOPIAKJj8Ew\n"
		"DwYDVR0TAQH/BAUwAwEB/zAaBgNVHREEEzARgg93d3cuZXhhbXBsZS5jb20wCgYI\n"
		"KoZIzj0EAwIDSQAwRgIhAPx/dAH6rRfy/f7ztUNP6Vcfkzaa9QpPahVTaTCgs5W1\n"
		"AiEAg6V6/NNofblh16352BHwRoTKW4aVHOW7VkEkQzSXNAc=\n"
		"-----END CERTIFICATE-----\n");


	// subject CN=server.example.com, issued by CN=Test CA (not included),
	// subjectAltName DNS:www.example.com, IP:10.0.0.1, IP:::1
	const std::string ISSUED_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBlTCCATygAwIBAgIUeoMt1lXfBFJ539SQ4tUHSyWGCmowCgYIKoZIzj0EAwIw\n"
		"EjEQMA4GA1UEAwwHVGVzdCBDQTAgFw0yNjExMDUxMDQ3MTZaGA8yMTI2MTAxMjEw\n"
		"NDcxNlowHTEbMBkGA1UEAwwSc2VydmVyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0C\n"
		"AQYIKoZIzj0DAQcDQgAElVnGZy5kbbXhCJRbKJF9EG5W9BqSbataauznHjWaZzWv\n"
		"9Qj0ifoATsmzKJKrL5BuY2DwW5/1zxibKcxwLJJyj6NjMGEwCQYDVR0TBAIwADAL\n"
		"BgNVHQ8EBAMCB4AwEwYDVR0lBAwwCgYIKwYBBQUHAwEwMgYDVR0RBCswKYIPd3d3\n"
		"LmV4YW1wbGUuY29thwQKAAABhxAAAAAAAAAAAAAAAAAAAAABMAoGCCqGSM49BAMC\n"
		"A0cAMEQCIH0c+J8rZP3mdhE0UkWe+i++4MM/nQaXkZ8zOJo0J62HAiBWOHEhrBkE\n"
		"UwlBs6LTRptrPzY7X/pwAxzlqMCMgJ+DOQ==\n"
		"-----END CERTIFICATE-----\n");


	// subject CN=test, subjectAltName DNS:www.example.com,
	// critical private extension 1.3.6.1.4.1.55555.1
	const std::string CRITICAL_EXTENSION_CERT(
		"-----BEGIN CERTIFICATE-----\n"
		"MIIBgzCCASqgAwIBAgIUZsSRsmuqW9eUJ4IZCuCCY9HSAq4wCgYIKoZIzj0EAwIw\n"
		"DzENMAsGA1UEAwwEdGVzdDAgFw0yNjExMDUxMTIzNTFaGA8yMTI2MTAxMjExMjM1\n"
		"MVowDzENMAsGA1UEAwwEdGVzdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABArA\n"
		"mGwjM1wBou91yZG57DT6abMIoc/Uqc+84HiS0CdaizSf5pWDSEJCvboP4MZeqlkl\n"
		"oB2gykhaZ0SyUFJ7jmijYjBgMB0GA1UdDgQWBBRiLuVO4zRcafS+jbtJFrfpcWNN\n"
		"djAPBgNVHRMBAf8EBTADAQH/MBoGA1UdEQQTMBGCD3d3dy5leGFtcGxlLmNvbTAS\n"
		"BgkrBgEEAYOyAwEBAf8EAgUAMAoGCCqGSM49BAMCA0cAMEQCIF7a1bcXisbzwCPJ\n"
		"NIKagjd078BL+Wq7PKToKKSm4umHAiA/MXyGMZZxbNkmXECpPRfjk7zkSSitwVh4\n"
		"GGXd4zLOXg==\n"
		"-----END CERTIFICATE-----\n");


	bool verify(const std::string& pem, const std::string& hostName)
	{
		std::istringstream istr(pem);
		X509Certificate cert(istr);
		return cert.verify(hostName);
	}
}


X509CertificateTest::X509CertificateTest(const std::string& name): CppUnit::TestCase(name)
{
}


X509CertificateTest::~X509CertificateTest()
{
}


void X509CertificateTest::testVerifyWildcard()
{
	assertTrue (verify(WILDCARD_CERT, "foo.example.com"));
	assertTrue (verify(WILDCARD_CERT, "FOO.Example.COM"));
	assertFalse (verify(WILDCARD_CERT, "example.com"));
	assertFalse (verify(WILDCARD_CERT, "foo.bar.example.com"));
	// the common name (test) is not considered
	assertFalse (verify(WILDCARD_CERT, "test"));
}


void X509CertificateTest::testVerifyPartialWildcard()
{
	assertFalse (verify(PARTIAL_WILDCARD_CERT, "foo.example.com"));
	assertFalse (verify(PARTIAL_WILDCARD_CERT, "f.example.com"));
}


void X509CertificateTest::testVerifyIPAddress()
{
	assertTrue (verify(IP_ADDRESS_CERT, "10.0.0.1"));
	assertFalse (verify(IP_ADDRESS_CERT, "10.0.0.2"));
	// the address is compared as POCO parses it, with a leading 0 for octal
	assertFalse (verify(IP_ADDRESS_CERT, "010.0.0.1"));
	assertTrue (verify(IP_ADDRESS_CERT, "012.0.0.1"));
}


void X509CertificateTest::testVerifyIPAddressAgainstDNSName()
{
	assertFalse (verify(DNS_NAME_ADDRESS_CERT, "10.0.0.1"));
}


void X509CertificateTest::testVerifyIPAddressAgainstCommonName()
{
	assertFalse (verify(COMMON_NAME_ADDRESS_CERT, "10.0.0.1"));
}


void X509CertificateTest::testVerifyIgnoresCommonName()
{
	assertFalse (verify(COMMON_NAME_CERT, "www.example.com"));
	assertFalse (verify(COMMON_NAME_CERT, "example.com"));
}


void X509CertificateTest::testVerifyEmptyHostName()
{
	assertFalse (verify(WILDCARD_CERT, ""));
}


void X509CertificateTest::testVerifyEmbeddedNul()
{
	// these two would match if read up to the NUL
	assertFalse (verify(WILDCARD_CERT, std::string("www.example.com\0.org", 20)));
	assertFalse (verify(IP_ADDRESS_CERT, std::string("10.0.0.1\0.example.com", 21)));
	// matched with OpenSSL, which ignores a trailing NUL
	assertFalse (verify(WILDCARD_CERT, std::string("foo.example.com\0", 16)));
	// matched in NetSSL_Win, whose wildcard match ignores the leftmost label
	assertFalse (verify(WILDCARD_CERT, std::string("a\0b.example.com", 15)));
}


void X509CertificateTest::testVerifyWildcardHostName()
{
	// matched with OpenSSL and in NetSSL_Win
	assertFalse (verify(WILDCARD_CERT, "*.example.com"));
	// matched with OpenSSL, which compares it literally with the partial wildcard
	assertFalse (verify(PARTIAL_WILDCARD_CERT, "f*.example.com"));
}


void X509CertificateTest::testVerifyEmptyLabel()
{
	// matched with OpenSSL, which reads a leading '.' as any subdomain
	assertFalse (verify(ISSUED_CERT, ".example.com"));
	// matched by neither backend; rejected like any empty label
	assertFalse (verify(WILDCARD_CERT, "www..example.com"));
}


void X509CertificateTest::testVerifyExpired()
{
	assertTrue (verify(EXPIRED_CERT, "www.example.com"));
}


void X509CertificateTest::testVerifyIssuedCertificate()
{
	assertTrue (verify(ISSUED_CERT, "www.example.com"));
	assertTrue (verify(ISSUED_CERT, "10.0.0.1"));
	assertFalse (verify(ISSUED_CERT, "other.example.com"));
	assertFalse (verify(ISSUED_CERT, "10.0.0.2"));
	assertFalse (verify(ISSUED_CERT, "server.example.com"));
#if defined(POCO_HAVE_IPv6)
	assertTrue (verify(ISSUED_CERT, "::1"));
	assertFalse (verify(ISSUED_CERT, "::2"));
#endif
}


void X509CertificateTest::testVerifyIgnoresOtherCertificateErrors()
{
	assertTrue (verify(CRITICAL_EXTENSION_CERT, "www.example.com"));
	assertFalse (verify(CRITICAL_EXTENSION_CERT, "other.example.com"));
}


void X509CertificateTest::setUp()
{
}


void X509CertificateTest::tearDown()
{
}


CppUnit::Test* X509CertificateTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("X509CertificateTest");

	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyWildcard);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyPartialWildcard);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIPAddress);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIPAddressAgainstDNSName);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIPAddressAgainstCommonName);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIgnoresCommonName);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyEmptyHostName);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyEmbeddedNul);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyWildcardHostName);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyEmptyLabel);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyExpired);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIssuedCertificate);
	CppUnit_addTest(pSuite, X509CertificateTest, testVerifyIgnoresOtherCertificateErrors);

	return pSuite;
}
