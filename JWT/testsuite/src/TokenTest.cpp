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
	assert (token.getAlgorithm() == "HS256");
	assert (token.getType() == "JWT");
	assert (token.getSubject() == "1234567890");
	assert (token.getIssuedAt().epochTime() == 1516239022);
	assert (token.payload().getValue<std::string>("name") == "John Doe");
	assert (token.signature() == "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");
}


void TokenTest::testParseNoSig()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.");

	Token token(jwt);
	assert (token.getAlgorithm() == "HS256");
	assert (token.getType() == "JWT");
	assert (token.getSubject() == "1234567890");
	assert (token.getIssuedAt().epochTime() == 1516239022);
	assert (token.payload().getValue<std::string>("name") == "John Doe");
	assert (token.signature() == "");
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
	assert (jwt == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.");
}


void TokenTest::testAssign()
{
	std::string jwt("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c");

	Token token(jwt);
	Token token2(token);

	assert (token.toString() == token2.toString());
}


void TokenTest::testAudience()
{
	Token token;

	token.setAudience("11111");
	assert (token.payload().getValue<std::string>(Token::CLAIM_AUDIENCE) == "11111");

	std::vector<std::string> audience = token.getAudience();
	assert (audience.size() == 1);
	assert (audience[0] == "11111");

	audience.push_back("22222");
	token.setAudience(audience);

	assert (token.payload().isArray(Token::CLAIM_AUDIENCE));
	audience = token.getAudience();
	assert (audience.size() == 2);
	assert (audience[0] == "11111");
	assert (audience[1] == "22222");
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
