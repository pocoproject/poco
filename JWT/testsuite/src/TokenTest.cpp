//
// TokenTest.cpp
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TokenTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/JWT/Token.h"


using namespace Poco::JWT;


TokenTest::TokenTest(const std::string& name):
	CppUnit::TestCase("TokenTest")
{
}


TokenTest::~TokenTest()
{
}


void TokenTest::setUp()
{
}


void TokenTest::tearDown()
{
}


void TokenTest::testParse()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");

	Token token(jwt);
	assertTrue(token.getAlgorithm() == "HS256");
	assertTrue(token.getType() == "JWT");
	assertTrue(token.getSubject() == "1234567890");
	assertTrue(token.getIssuedAt().epochTime() == 1516239022);
	assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
	assertTrue(token.signature() == "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");
}


void TokenTest::testParseNoSig()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.");

	Token token(jwt);
	assertTrue(token.getAlgorithm() == "HS256");
	assertTrue(token.getType() == "JWT");
	assertTrue(token.getSubject() == "1234567890");
	assertTrue(token.getIssuedAt().epochTime() == 1516239022);
	assertTrue(token.payload().getValue<std::string>("name") == "John Doe");
	assertTrue(token.signature() == "");
}


void TokenTest::testSerialize()
{
	Token token;
	token.setAlgorithm("HS256");
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	std::string jwt = token.toString();
	assertTrue(jwt == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.");
}


void TokenTest::testAssign()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");

	Token token(jwt);
	Token token2(token);

	assertTrue(token.toString() == token2.toString());
}


void TokenTest::testAudience()
{
	Token token;

	token.setAudience("11111");
	assertTrue(token.payload().getValue<std::string>(Token::CLAIM_AUDIENCE) == "11111");

	std::vector<std::string> audience = token.getAudience();
	assertTrue(audience.size() == 1);
	assertTrue(audience[0] == "11111");

	audience.push_back("22222");
	token.setAudience(audience);

	assertTrue(token.payload().isArray(Token::CLAIM_AUDIENCE));
	audience = token.getAudience();
	assertTrue(audience.size() == 2);
	assertTrue(audience[0] == "11111");
	assertTrue(audience[1] == "22222");
}


CppUnit::Test* TokenTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TokenTest");

	CppUnit_addTest(pSuite, TokenTest, testParse);
	CppUnit_addTest(pSuite, TokenTest, testParseNoSig);
	CppUnit_addTest(pSuite, TokenTest, testSerialize);
	CppUnit_addTest(pSuite, TokenTest, testAssign);
	CppUnit_addTest(pSuite, TokenTest, testAudience);

	return pSuite;
}
