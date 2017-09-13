//
// X509CertificateTest.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/X509CertificateTest.cpp#1 $
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "X509CertificateTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Net/X509Certificate.h"
#include <openssl/opensslv.h>
#include <sstream>

using namespace Poco::Net;


static const std::string BADSSL_PEM(
	"-----BEGIN CERTIFICATE-----\n"
	"MIIHGjCCBgKgAwIBAgIQC2as9L3V1BSoMGn+fB4NRjANBgkqhkiG9w0BAQ0FADBN\n"
	"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMScwJQYDVQQDEx5E\n"
	"aWdpQ2VydCBTSEEyIFNlY3VyZSBTZXJ2ZXIgQ0EwHhcNMTcwMzE4MDAwMDAwWhcN\n"
	"MjAwMzI1MTIwMDAwWjBnMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5p\n"
	"YTEVMBMGA1UEBxMMV2FsbnV0IENyZWVrMRUwEwYDVQQKEwxMdWNhcyBHYXJyb24x\n"
	"FTATBgNVBAMMDCouYmFkc3NsLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCC\n"
	"AQoCggEBAMIE7PiM7gTCs9hQ1XBYzJMY61yoaEmwIrX5lZ6xKyx2PmzAS2BMTOqy\n"
	"tMAPgLaw+XLJhgL5XEFdEyt/ccRLvOmULlA3pmccYYz2QULFRtMWhyefdOsKnRFS\n"
	"JiFzbIRMeVXk0WvoBj1IFVKtsyjbqv9u/2CVSndrOfEk0TG23U3AxPxTuW1CrbV8\n"
	"/q71FdIzSOciccfCFHpsKOo3St/qbLVytH5aohbcabFXRNsKEqveww9HdFxBIuGa\n"
	"+RuT5q0iBikusbpJHAwnnqP7i/dAcgCskgjZjFeEU4EFy+b+a1SYQCeFxxC7c3Dv\n"
	"aRhBB0VVfPlkPz0sw6l865MaTIbRyoUCAwEAAaOCA9owggPWMB8GA1UdIwQYMBaA\n"
	"FA+AYRyCMWHVLyjnjUY4tCzhxtniMB0GA1UdDgQWBBSd7sF7gQs6R2lxGH0RN5O8\n"
	"pRs/+zAjBgNVHREEHDAaggwqLmJhZHNzbC5jb22CCmJhZHNzbC5jb20wDgYDVR0P\n"
	"AQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjBrBgNVHR8E\n"
	"ZDBiMC+gLaArhilodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vc3NjYS1zaGEyLWc1\n"
	"LmNybDAvoC2gK4YpaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL3NzY2Etc2hhMi1n\n"
	"NS5jcmwwTAYDVR0gBEUwQzA3BglghkgBhv1sAQEwKjAoBggrBgEFBQcCARYcaHR0\n"
	"cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQUzAIBgZngQwBAgMwfAYIKwYBBQUHAQEE\n"
	"cDBuMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wRgYIKwYB\n"
	"BQUHMAKGOmh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFNIQTJT\n"
	"ZWN1cmVTZXJ2ZXJDQS5jcnQwDAYDVR0TAQH/BAIwADCCAfcGCisGAQQB1nkCBAIE\n"
	"ggHnBIIB4wHhAHcApLkJkLQYWBSHuxOizGdwCjw1mAT5G9+443fNDsgN3BAAAAFa\n"
	"37GUmAAABAMASDBGAiEAlzAI5gaOsmMH2II7yr08jlqKgxeZT5zAtd9obFeUfMgC\n"
	"IQChMLfMCBbgv8Fo+7YoGzXJrNJJXDDBC+R/miHp2PA8BwB2AFYUBpov18Ls0/Xh\n"
	"vUSyPsdGdrm8mRFcwO+UmFXWidDdAAABWt+xlX0AAAQDAEcwRQIgWBrOlMeBTCBJ\n"
	"ezx+6OwzmD1QSl7/Vq2k/xACoGInSjsCIQCii59Rjt8rZxNIHMQE6/lnYHWyCqEv\n"
	"rNAJiUOrJAxJ3QB1AO5Lvbd1zmC64UJpH6vhnmajD35fsHLYgwDEe4l6qP3LAAAB\n"
	"Wt+xlqkAAAQDAEYwRAIgEklGobMohZiTQwTgizi4Sfy2CQ/fL1K1EM2yZGrvSCAC\n"
	"IGkDXy8uSUh3S09pAODVLHeyMYX8fdGM3UU1uv8ReNH3AHcAu9nfvB+KcbWTlCOX\n"
	"qpJ7RzhXlQqrUugakJZkNo4e0YUAAAFa37GUwQAABAMASDBGAiEA2wazyXDaD1k3\n"
	"4UVG0AJH+HhNBb728PHxuTJYGffXCckCIQDOb7subG8AQJFc5zWKeKPhnHhP2R56\n"
	"28tXqM8mzCQGzzANBgkqhkiG9w0BAQ0FAAOCAQEAyA7NyduD8+zjNI/uclW9WkvG\n"
	"6Oolon0vvbs+BGvCl5iY/3BBIxNTWPeRBDE0P+zOkceqGqjENce/d1O5I/H+oFZR\n"
	"Y0fLjtILwg/BH5QKoRWJF/rqvu7wlSqnxlYGDducs2hY2WgtIl/BntMaDQXcyzXP\n"
	"O+Pjbq0JLb6/Enss558t3DNIKvtspDidxnQIPBzKBVx8prG1C0ng4S+HmZYg166W\n"
	"mShTOeNNQMYPIUr44ICQkw82gYuAsjsgxEEjAHo0zrkHlA3cnwXeERAX3qEUQ8WH\n"
	"Dw/Py3T1TqiODDqvLLE/sFYUwqDaSdaaOo/geYvJmfqOYXQoaz2t1ptYXdvsfg==\n"
	"-----END CERTIFICATE-----\n"
);


X509CertificateTest::X509CertificateTest(const std::string& name): CppUnit::TestCase(name)
{
}


X509CertificateTest::~X509CertificateTest()
{
}


void X509CertificateTest::testVerify()
{
	std::istringstream certStream(BADSSL_PEM);
	X509Certificate cert(certStream);

	assert (cert.verify("badssl.com"));
	assert (cert.verify("host.badssl.com"));

#if OPENSSL_VERSION_NUMBER < 0x10002000L
	assert (cert.verify("wrong.host.badssl.com"));
#else
	assert (!cert.verify("wrong.host.badssl.com"));
#endif
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

	CppUnit_addTest(pSuite, X509CertificateTest, testVerify);

	return pSuite;
}
