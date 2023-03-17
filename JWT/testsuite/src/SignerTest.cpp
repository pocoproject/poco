//
// SignerTest.cpp
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SignerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/JWT/Signer.h"
#include "Poco/JWT/JWTException.h"
#include <iostream>


using namespace Poco::JWT;


const std::string SignerTest::RSA_PRIVATE_KEY(
	"-----BEGIN RSA PRIVATE KEY-----\n"
	"MIIEpQIBAAKCAQEAyVsWBblTS6iuhZiScYwX3Q8O0q2rvbC9cw9fO9NE3XMjzSGf\n"
	"NMMFqRRgnivduUvpXwQxw711Va6Y/7o8xrRJ+LBflKBN7EEMdBbNyj7HvZrrkonW\n"
	"X/mO1uFo9Tf4Lt0hfomE1tqcXw5IGi4X1wXzvbUvDhFqmFcL3F7alIeSPPoZe5Kq\n"
	"4PFoVxhH8z+2u4Q3qME2NbYvJqYaaO+cCxX1JtdY230LL/rHXtH+EbhJpHJNGdLz\n"
	"zCJMdKZF3Aqz4J5lb1z6FYIekzJ20FvJ6it1tCQplQisEJyo2YNYCgzBUeAcJoXw\n"
	"o4OqsIgfpLWEoTBxm5WbarIwt6yVLRamHGXsZQIDAQABAoIBAQCF5F65gamExu7+\n"
	"AR1oCcsYgNnPeBKaeXsQlqkyA+NoChdFYVUXZZNPAjDZHLw8aeEOAKEC0unCBu7Q\n"
	"JcwiKWKCzu1PzGTkc8DPNRa0pJh3WRvKfoOhoDW+Z3c+kHAk4YBTPyipIgcXIk0J\n"
	"s5rArcGNZXybszMZh9BoQl7Ao4G3rzssIAn4lKhoHPkm4YhJqhboPb0esjTtQXZU\n"
	"QIbME3Psir2BgtL+dAW15DofmmvSwVFlsW9tM2QMrS7RtEDoDbEgdteAYGJe+BKg\n"
	"6+wH9fGR7Csacqfg5YWBYBC6zwMtOxYIYLuSA3MeWH529M7V26hog/cntl4CA8C4\n"
	"Q/vdfjrhAoGBAPzoP7rjkZkwiE49IHLjth7wMEpJJzb9O33jAtRRqz46gXRMO7y2\n"
	"lADzHkHmMaJThxtVSQleNHWHW/QZosEaVqw8ibfIeUNVnwLZBL02Rp0XyVnLp2zq\n"
	"bG8kQM5CQO9yrckQ4U7hE6Nb9goXR1DzojdllXjS0G2LuGNeZAjL3Pc5AoGBAMvR\n"
	"c0UAESCwQX93WyvJRQUsx/3qLWvm84o847W3Ouik4CPxWX6K77lHu8trwl0+sI4r\n"
	"+1l4IMli1GBYnp+h64kcIHRcdh8cdB2Q0SSovpbIfBQDmLLxH2eZcPsCkimrRvWw\n"
	"1wdPw3e56vn/uhOil1BBqjTdUel4cJkaQX8bDdKNAoGBALxjfGWIcsJ7xm0RV4R9\n"
	"XwI4xJ/xUgbCJ3iki00A8OBP0HKC/tSZ5DG458cK49oZAkE4DEmwJL+Pbs2r8vKI\n"
	"3hs6lROTf9DKjMIgSklvjrYiK9h5vWOCU/eON7/s1lYHRLDLpCmPu4MqU2I86ODC\n"
	"Owms3+S6lIulUlqiyz1KLnTxAoGBAKYrOjY8Krqi2JOKhNs6+bmRW+/o12VMYqwG\n"
	"noPNQgrgORk0sQkZTv6YvEPJtCn/bURMGV1FMj3eBFYUiiaNhZFATDlyFJ+ivAxK\n"
	"wCiKJvCZvYc3s2vYaAk3sUQEQZQVXwH6TiVY430eJ7PwyQ0vFvxIvPLiqeCV3/8C\n"
	"x/lP3kIVAoGAZbiwceB12RTDCL2jsczN4ZE16VyXvatI44NWtakbuxZoDJGhg5Uu\n"
	"N2xb6w2mkmaJ0FUi2+B7Vpv8vmH98Aylfev03J4qnJgaNVVEK2nlprkOyflHr7Bj\n"
	"fIG0bvyvwUNUy8f5idTqcucDi02zu3Sfr2+LocNUJgrbvswMIXUE3v8=\n"
	"-----END RSA PRIVATE KEY-----\n"
);


const std::string SignerTest::RSA_PUBLIC_KEY(
	"-----BEGIN PUBLIC KEY-----\n"
	"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyVsWBblTS6iuhZiScYwX\n"
	"3Q8O0q2rvbC9cw9fO9NE3XMjzSGfNMMFqRRgnivduUvpXwQxw711Va6Y/7o8xrRJ\n"
	"+LBflKBN7EEMdBbNyj7HvZrrkonWX/mO1uFo9Tf4Lt0hfomE1tqcXw5IGi4X1wXz\n"
	"vbUvDhFqmFcL3F7alIeSPPoZe5Kq4PFoVxhH8z+2u4Q3qME2NbYvJqYaaO+cCxX1\n"
	"JtdY230LL/rHXtH+EbhJpHJNGdLzzCJMdKZF3Aqz4J5lb1z6FYIekzJ20FvJ6it1\n"
	"tCQplQisEJyo2YNYCgzBUeAcJoXwo4OqsIgfpLWEoTBxm5WbarIwt6yVLRamHGXs\n"
	"ZQIDAQAB\n"
	"-----END PUBLIC KEY-----\n"
);


const std::string SignerTest::ECDSA_PRIVATE_KEY(
	"-----BEGIN PRIVATE KEY-----\n"
	"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgevZzL1gdAFr88hb2\n"
	"OF/2NxApJCzGCEDdfSp6VQO30hyhRANCAAQRWz+jn65BtOMvdyHKcvjBeBSDZH2r\n"
	"1RTwjmYSi9R/zpBnuQ4EiMnCqfMPWiZqB4QdbAd0E7oH50VpuZ1P087G\n"
	"-----END PRIVATE KEY-----\n"
);


const std::string SignerTest::ECDSA_PUBLIC_KEY(
	"-----BEGIN PUBLIC KEY-----\n"
	"MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEEVs/o5+uQbTjL3chynL4wXgUg2R9\n"
	"q9UU8I5mEovUf86QZ7kOBIjJwqnzD1omageEHWwHdBO6B+dFabmdT9POxg==\n"
	"-----END PUBLIC KEY-----\n"
);


SignerTest::SignerTest(const std::string& name):
	CppUnit::TestCase("SignerTest")
{
}


SignerTest::~SignerTest()
{
}


void SignerTest::setUp()
{
}


void SignerTest::tearDown()
{
}


void SignerTest::testSignHS256()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	std::string jwt = signer.sign(token, Signer::ALGO_HS256);

	assertTrue(jwt == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw");
}


void SignerTest::testSignHS384()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	std::string jwt = signer.sign(token, Signer::ALGO_HS384);

	assertTrue(jwt == "eyJhbGciOiJIUzM4NCIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.9NsI7ahPhCd3itTewXb0GNZi08fuUHXLx0qeBscteMXJiug1PyQ_teA9v7zLgg1W");
}


void SignerTest::testSignHS512()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	std::string jwt = signer.sign(token, Signer::ALGO_HS512);

	assertTrue(jwt == "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.WG4y8U_bDN4T3Vu3L5Q5C4pqssrH4wqBtdrFLVuS8k-BLycCq8_bjYGgo7BCzVt4DFXs3BFUIJQdWBzuJwXHtg");
}


void SignerTest::testVerifyHS256()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw");

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "HS256");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyHS384()
{
	std::string jwt("eyJhbGciOiJIUzM4NCIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.9NsI7ahPhCd3itTewXb0GNZi08fuUHXLx0qeBscteMXJiug1PyQ_teA9v7zLgg1W");

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	signer.addAlgorithm(Signer::ALGO_HS384);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "HS384");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "9NsI7ahPhCd3itTewXb0GNZi08fuUHXLx0qeBscteMXJiug1PyQ_teA9v7zLgg1W");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyHS512()
{
	std::string jwt("eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.WG4y8U_bDN4T3Vu3L5Q5C4pqssrH4wqBtdrFLVuS8k-BLycCq8_bjYGgo7BCzVt4DFXs3BFUIJQdWBzuJwXHtg");

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	signer.addAlgorithm(Signer::ALGO_HS512);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "HS512");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "WG4y8U_bDN4T3Vu3L5Q5C4pqssrH4wqBtdrFLVuS8k-BLycCq8_bjYGgo7BCzVt4DFXs3BFUIJQdWBzuJwXHtg");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyFailSignature()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.wn9G7NwFEOjIh-7hfCFDZA1aJeQmf7I7YvzCBcdenGw");

	Signer signer("0123456789ABCDEF0123456789ABCDEF");
	try
	{
		Token token = signer.verify(jwt);
		fail("Verification must fail");
	}
	catch (SignatureVerificationException&)
	{
	}
}


void SignerTest::testVerifyFailKey()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw");

	Signer signer("0123456789ABCDEF0123456789ABFAIL");
	try
	{
		Token token = signer.verify(jwt);
		fail("Verification must fail");
	}
	catch (SignatureVerificationException&)
	{
	}
}


void SignerTest::testSignRS256()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(RSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_RS256);

	assertTrue(jwt == "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.a27BSSEBTaQZFA1tVX4IZHgyG5HIXcJVZpbpB5LQ_rPTalJjvhDDuWC1dM0G0tUACrzPtUN4BhSd-dygJsX4b35DnWm_gPUNDI3HMm7Ck52mM_2Y6445B6aa_pPPuFk6AWql8WWLzQqo9kjQh8AmbMw2A9bciA1smEEsHVw4-VX1tEtupbhJsXO2FnwkQNhJF_Pp4nuX282UV_4DtZ9LW3jLoEYFytKrM4fhkNKVMY52Cn0DJA89fQYe7098gduCjzqoGtaoKKDngbADn2h_1P8VLZrZEd4UROEHviVLm_qxHrWY8-tB0L7i_JMXxw1qMKAavWA-WbnNDdXpOn_o2Q");
}


void SignerTest::testSignRS384()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(RSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_RS384);

	assertTrue(jwt == "eyJhbGciOiJSUzM4NCIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.L-34N4v5kLa94Llz-XakGIwL9M00ERciAzZSqxgGIJ2dw9VrIodfK-U00wZZwSA2UEZWIm-LJ7wQBiuUw8oMl_fYsufT8W6dWiGZQ2c24AjGKwpXmypPKjh5yRnylkK-8ZRC1AJuZDsY8DJE7vse1w2eAE_Jw0XRJ-u_lq9Hgxz58ZonV1YzUdyVPtD3gWdhyjnlzPCH7lQM4copVUFN6mFTZzt4WQ2i1O1qW1cD_F4Jul9_5z5BYe7-bK3DoV79AgfbEewdnc4yatLQWMIAkrc2LM_tFe83ABhFYhM0qIH8nOuk3WKyKwtjh15f3h3Fn-JnriSfcC79v-M5UpEsZg");
}


void SignerTest::testSignRS512()
{
	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(RSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_RS512);

	assertTrue(jwt == "eyJhbGciOiJSUzUxMiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.XQTBYo2zqxcyUGs0H-74tfBY6l8PxBQK7-IAJ1NgEVIeoMDX3zQJu5BQX2_VhjOESOPqGNN-FtiNLD1G-LCvSV1fxJwIVEilT7CTBs5iNii6Jrpha5YPnzETqBiz1zdnyNh_QVbtdRIv2ORlp_OIYNZJrxiRfOGvm2_Z3htDoqgv_Lm8SZqelOntox96GrV6GaXhpKBbLjBSU-XPkSOcm5VuXDCz8tltJ_d5cKxbFDUtS6FBYNMaLEqIL4-_aJU_Ld5TcPQT7MqWlHHZZufA5zzmfKEEgddco6uzCBLOz3B6E4Z5VZDoweCM5R7hnLiZOlK0kYsFoaDCVcK_TZhDNw");
}


void SignerTest::testVerifyRS256()
{
	std::string jwt("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.a27BSSEBTaQZFA1tVX4IZHgyG5HIXcJVZpbpB5LQ_rPTalJjvhDDuWC1dM0G0tUACrzPtUN4BhSd-dygJsX4b35DnWm_gPUNDI3HMm7Ck52mM_2Y6445B6aa_pPPuFk6AWql8WWLzQqo9kjQh8AmbMw2A9bciA1smEEsHVw4-VX1tEtupbhJsXO2FnwkQNhJF_Pp4nuX282UV_4DtZ9LW3jLoEYFytKrM4fhkNKVMY52Cn0DJA89fQYe7098gduCjzqoGtaoKKDngbADn2h_1P8VLZrZEd4UROEHviVLm_qxHrWY8-tB0L7i_JMXxw1qMKAavWA-WbnNDdXpOn_o2Q");

	std::istringstream publicKeyStream(RSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_RS256);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "RS256");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "a27BSSEBTaQZFA1tVX4IZHgyG5HIXcJVZpbpB5LQ_rPTalJjvhDDuWC1dM0G0tUACrzPtUN4BhSd-dygJsX4b35DnWm_gPUNDI3HMm7Ck52mM_2Y6445B6aa_pPPuFk6AWql8WWLzQqo9kjQh8AmbMw2A9bciA1smEEsHVw4-VX1tEtupbhJsXO2FnwkQNhJF_Pp4nuX282UV_4DtZ9LW3jLoEYFytKrM4fhkNKVMY52Cn0DJA89fQYe7098gduCjzqoGtaoKKDngbADn2h_1P8VLZrZEd4UROEHviVLm_qxHrWY8-tB0L7i_JMXxw1qMKAavWA-WbnNDdXpOn_o2Q");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyRS384()
{
	std::string jwt("eyJhbGciOiJSUzM4NCIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.L-34N4v5kLa94Llz-XakGIwL9M00ERciAzZSqxgGIJ2dw9VrIodfK-U00wZZwSA2UEZWIm-LJ7wQBiuUw8oMl_fYsufT8W6dWiGZQ2c24AjGKwpXmypPKjh5yRnylkK-8ZRC1AJuZDsY8DJE7vse1w2eAE_Jw0XRJ-u_lq9Hgxz58ZonV1YzUdyVPtD3gWdhyjnlzPCH7lQM4copVUFN6mFTZzt4WQ2i1O1qW1cD_F4Jul9_5z5BYe7-bK3DoV79AgfbEewdnc4yatLQWMIAkrc2LM_tFe83ABhFYhM0qIH8nOuk3WKyKwtjh15f3h3Fn-JnriSfcC79v-M5UpEsZg");

	std::istringstream publicKeyStream(RSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_RS384);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "RS384");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "L-34N4v5kLa94Llz-XakGIwL9M00ERciAzZSqxgGIJ2dw9VrIodfK-U00wZZwSA2UEZWIm-LJ7wQBiuUw8oMl_fYsufT8W6dWiGZQ2c24AjGKwpXmypPKjh5yRnylkK-8ZRC1AJuZDsY8DJE7vse1w2eAE_Jw0XRJ-u_lq9Hgxz58ZonV1YzUdyVPtD3gWdhyjnlzPCH7lQM4copVUFN6mFTZzt4WQ2i1O1qW1cD_F4Jul9_5z5BYe7-bK3DoV79AgfbEewdnc4yatLQWMIAkrc2LM_tFe83ABhFYhM0qIH8nOuk3WKyKwtjh15f3h3Fn-JnriSfcC79v-M5UpEsZg");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyRS512()
{
	std::string jwt("eyJhbGciOiJSUzUxMiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.XQTBYo2zqxcyUGs0H-74tfBY6l8PxBQK7-IAJ1NgEVIeoMDX3zQJu5BQX2_VhjOESOPqGNN-FtiNLD1G-LCvSV1fxJwIVEilT7CTBs5iNii6Jrpha5YPnzETqBiz1zdnyNh_QVbtdRIv2ORlp_OIYNZJrxiRfOGvm2_Z3htDoqgv_Lm8SZqelOntox96GrV6GaXhpKBbLjBSU-XPkSOcm5VuXDCz8tltJ_d5cKxbFDUtS6FBYNMaLEqIL4-_aJU_Ld5TcPQT7MqWlHHZZufA5zzmfKEEgddco6uzCBLOz3B6E4Z5VZDoweCM5R7hnLiZOlK0kYsFoaDCVcK_TZhDNw");

	std::istringstream publicKeyStream(RSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::RSAKey> pKey = new Poco::Crypto::RSAKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_RS512);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "RS512");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "XQTBYo2zqxcyUGs0H-74tfBY6l8PxBQK7-IAJ1NgEVIeoMDX3zQJu5BQX2_VhjOESOPqGNN-FtiNLD1G-LCvSV1fxJwIVEilT7CTBs5iNii6Jrpha5YPnzETqBiz1zdnyNh_QVbtdRIv2ORlp_OIYNZJrxiRfOGvm2_Z3htDoqgv_Lm8SZqelOntox96GrV6GaXhpKBbLjBSU-XPkSOcm5VuXDCz8tltJ_d5cKxbFDUtS6FBYNMaLEqIL4-_aJU_Ld5TcPQT7MqWlHHZZufA5zzmfKEEgddco6uzCBLOz3B6E4Z5VZDoweCM5R7hnLiZOlK0kYsFoaDCVcK_TZhDNw");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testSignVerifyES256()
{
	// Note: ECDSA is a strange beast and does not return a "known" signature.
	// That's why we do the signing and verification in a single test.

	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(ECDSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_ES256);

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer verifier(pKey);
	verifier.addAlgorithm(Signer::ALGO_ES256);
	try
	{
		Token token2 = verifier.verify(jwt);
		assertTrue(token2.getAlgorithm() == "ES256");
		assertTrue(token2.getType() == "JWT");
		assertTrue(token2.getSubject() == "1234567890");
		assertTrue(token2.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token2.payload().getValue<std::string>("name") == "John Doe");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyES256()
{
	std::string jwt("eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.kLfRdCmR-qewMgzhCtqJrXVoagoh7es0yWsn3VunuS51FMBBcxLTKRDfdgHih0os4gvBdLMYkJu61_IQqoIYZw");

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_ES256);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "ES256");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "kLfRdCmR-qewMgzhCtqJrXVoagoh7es0yWsn3VunuS51FMBBcxLTKRDfdgHih0os4gvBdLMYkJu61_IQqoIYZw");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testSignVerifyES384()
{
	// Note: ECDSA is a strange beast and does not return a "known" signature.
	// That's why we do the signing and verification in a single test.

	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(ECDSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_ES384);

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer verifier(pKey);
	verifier.addAlgorithm(Signer::ALGO_ES384);
	try
	{
		Token token2 = verifier.verify(jwt);
		assertTrue(token2.getAlgorithm() == "ES384");
		assertTrue(token2.getType() == "JWT");
		assertTrue(token2.getSubject() == "1234567890");
		assertTrue(token2.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token2.payload().getValue<std::string>("name") == "John Doe");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyES384()
{
	std::string jwt("eyJhbGciOiJFUzM4NCIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.ROGmzbopY2GfjSUKih4MmgZ5_1jLQdEc2db3ITYCDOZSHzeGM_14KtY-61qvx4BXxmeUiXfoInPZWlA75VL6dA");

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_ES384);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "ES384");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "ROGmzbopY2GfjSUKih4MmgZ5_1jLQdEc2db3ITYCDOZSHzeGM_14KtY-61qvx4BXxmeUiXfoInPZWlA75VL6dA");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testSignVerifyES512()
{
	// Note: ECDSA is a strange beast and does not return a "known" signature.
	// That's why we do the signing and verification in a single test.

	Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::istringstream privateKeyStream(ECDSA_PRIVATE_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(0, &privateKeyStream);

	Signer signer(pKey);
	std::string jwt = signer.sign(token, Signer::ALGO_ES512);

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer verifier(pKey);
	verifier.addAlgorithm(Signer::ALGO_ES512);
	try
	{
		Token token2 = verifier.verify(jwt);
		assertTrue(token2.getAlgorithm() == "ES512");
		assertTrue(token2.getType() == "JWT");
		assertTrue(token2.getSubject() == "1234567890");
		assertTrue(token2.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token2.payload().getValue<std::string>("name") == "John Doe");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


void SignerTest::testVerifyES512()
{
	std::string jwt("eyJhbGciOiJFUzUxMiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.8AYb9WDk3x2U-69Hi2DHou06L8VavXJlMqyH8dF-uiekm926CNM7D3pkgnWD6e_OfV_p2XIkdfIV018PjZtfuA");

	std::istringstream publicKeyStream(ECDSA_PUBLIC_KEY);
	Poco::SharedPtr<Poco::Crypto::ECKey> pKey = new Poco::Crypto::ECKey(&publicKeyStream);

	Signer signer(pKey);
	signer.addAlgorithm(Signer::ALGO_ES512);
	try
	{
		Token token = signer.verify(jwt);
		assertTrue(token.getAlgorithm() == "ES512");
		assertTrue(token.getType() == "JWT");
		assertTrue(token.getSubject() == "1234567890");
		assertTrue(token.getIssuedAt().epochTime() == 1516239022);
		assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
		assertTrue(token.signature() == "8AYb9WDk3x2U-69Hi2DHou06L8VavXJlMqyH8dF-uiekm926CNM7D3pkgnWD6e_OfV_p2XIkdfIV018PjZtfuA");
	}
	catch (JWTException&)
	{
		fail("Verification must succeed");
	}
}


CppUnit::Test* SignerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SignerTest");

	CppUnit_addTest(pSuite, SignerTest, testSignHS256);
	CppUnit_addTest(pSuite, SignerTest, testSignHS384);
	CppUnit_addTest(pSuite, SignerTest, testSignHS512);
	CppUnit_addTest(pSuite, SignerTest, testVerifyHS256);
	CppUnit_addTest(pSuite, SignerTest, testVerifyHS384);
	CppUnit_addTest(pSuite, SignerTest, testVerifyHS512);
	CppUnit_addTest(pSuite, SignerTest, testVerifyFailSignature);
	CppUnit_addTest(pSuite, SignerTest, testVerifyFailKey);
	CppUnit_addTest(pSuite, SignerTest, testSignRS256);
	CppUnit_addTest(pSuite, SignerTest, testSignRS384);
	CppUnit_addTest(pSuite, SignerTest, testSignRS512);
	CppUnit_addTest(pSuite, SignerTest, testVerifyRS256);
	CppUnit_addTest(pSuite, SignerTest, testVerifyRS384);
	CppUnit_addTest(pSuite, SignerTest, testVerifyRS512);
	CppUnit_addTest(pSuite, SignerTest, testSignVerifyES256);
	CppUnit_addTest(pSuite, SignerTest, testSignVerifyES384);
	CppUnit_addTest(pSuite, SignerTest, testSignVerifyES512);
	CppUnit_addTest(pSuite, SignerTest, testVerifyES256);
	CppUnit_addTest(pSuite, SignerTest, testVerifyES384);
	CppUnit_addTest(pSuite, SignerTest, testVerifyES512);

	return pSuite;
}
