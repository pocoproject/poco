//
// URIUtilityTest.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "URIUtilityTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/URIUtility.h"


using Poco::RemotingNG::URIUtility;


URIUtilityTest::URIUtilityTest(const std::string& name): CppUnit::TestCase(name)
{
}


URIUtilityTest::~URIUtilityTest()
{
}


void URIUtilityTest::testMatchPathBasic()
{
	// Exact match
	assert(URIUtility::matchPath("/api/test", "/api/test"));
	assert(!URIUtility::matchPath("/api/test", "/api/other"));
	assert(!URIUtility::matchPath("/api/test", "/api/test/extra"));
	assert(!URIUtility::matchPath("/api/test/extra", "/api/test"));

	// Empty paths
	assert(URIUtility::matchPath("", ""));
	assert(!URIUtility::matchPath("/", ""));
	assert(!URIUtility::matchPath("", "/"));
}


void URIUtilityTest::testMatchPathPlaceholder()
{
	// Basic placeholder
	assert(URIUtility::matchPath("/api/user/123", "/api/user/{id}"));
	assert(URIUtility::matchPath("/api/user/abc", "/api/user/{id}"));
	assert(!URIUtility::matchPath("/api/user", "/api/user/{id}"));
	assert(!URIUtility::matchPath("/api/user/123/extra", "/api/user/{id}"));

	// Multiple placeholders
	assert(URIUtility::matchPath("/api/user/123/post/456", "/api/user/{userId}/post/{postId}"));
	assert(!URIUtility::matchPath("/api/user/123/post", "/api/user/{userId}/post/{postId}"));
}


void URIUtilityTest::testMatchPathAlternation()
{
	// Standalone alternation (without parameter name)
	assert(URIUtility::matchPath("/api/get", "/api/(get|post|put)"));
	assert(URIUtility::matchPath("/api/post", "/api/(get|post|put)"));
	assert(URIUtility::matchPath("/api/put", "/api/(get|post|put)"));
	assert(!URIUtility::matchPath("/api/delete", "/api/(get|post|put)"));
	assert(!URIUtility::matchPath("/api/", "/api/(get|post|put)"));

	// Alternation with path continuation
	assert(URIUtility::matchPath("/api/get/resource", "/api/(get|post)/resource"));
	assert(URIUtility::matchPath("/api/post/resource", "/api/(get|post)/resource"));
	assert(!URIUtility::matchPath("/api/put/resource", "/api/(get|post)/resource"));
}


void URIUtilityTest::testMatchPathAlternationNamed()
{
	// Named alternation with parameter extraction: {name:(option1|option2|...)}
	assert(URIUtility::matchPath("/devs/sql/query", "/devs/sql/{endpoint:(query|databases|config)}"));
	assert(URIUtility::matchPath("/devs/sql/databases", "/devs/sql/{endpoint:(query|databases|config)}"));
	assert(URIUtility::matchPath("/devs/sql/config", "/devs/sql/{endpoint:(query|databases|config)}"));
	assert(!URIUtility::matchPath("/devs/sql/invalid", "/devs/sql/{endpoint:(query|databases|config)}"));
	assert(!URIUtility::matchPath("/devs/sql/", "/devs/sql/{endpoint:(query|databases|config)}"));

	// Five options (real-world case)
	assert(URIUtility::matchPath("/devs/sql/query", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
	assert(URIUtility::matchPath("/devs/sql/databases", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
	assert(URIUtility::matchPath("/devs/sql/queries", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
	assert(URIUtility::matchPath("/devs/sql/config", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
	assert(URIUtility::matchPath("/devs/sql/pending", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
	assert(!URIUtility::matchPath("/devs/sql/other", "/devs/sql/{endpoint:(query|databases|queries|config|pending)}"));
}


void URIUtilityTest::testMatchPathMixed()
{
	// Named alternation followed by regular placeholder
	assert(URIUtility::matchPath("/devs/sql/query/item123", "/devs/sql/{endpoint:(query|config)}/{id}"));
	assert(URIUtility::matchPath("/devs/sql/config/item456", "/devs/sql/{endpoint:(query|config)}/{id}"));
	assert(!URIUtility::matchPath("/devs/sql/other/item123", "/devs/sql/{endpoint:(query|config)}/{id}"));

	// Regular placeholder followed by named alternation
	assert(URIUtility::matchPath("/api/123/read", "/api/{id}/{action:(read|write|delete)}"));
	assert(URIUtility::matchPath("/api/456/write", "/api/{id}/{action:(read|write|delete)}"));
	assert(!URIUtility::matchPath("/api/789/execute", "/api/{id}/{action:(read|write|delete)}"));

	// Mix of standalone alternation and placeholder
	assert(URIUtility::matchPath("/v1/users/42", "/(v1|v2)/users/{id}"));
	assert(URIUtility::matchPath("/v2/users/42", "/(v1|v2)/users/{id}"));
	assert(!URIUtility::matchPath("/v3/users/42", "/(v1|v2)/users/{id}"));
}


void URIUtilityTest::setUp()
{
}


void URIUtilityTest::tearDown()
{
}


CppUnit::Test* URIUtilityTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("URIUtilityTest");

	CppUnit_addTest(pSuite, URIUtilityTest, testMatchPathBasic);
	CppUnit_addTest(pSuite, URIUtilityTest, testMatchPathPlaceholder);
	CppUnit_addTest(pSuite, URIUtilityTest, testMatchPathAlternation);
	CppUnit_addTest(pSuite, URIUtilityTest, testMatchPathAlternationNamed);
	CppUnit_addTest(pSuite, URIUtilityTest, testMatchPathMixed);

	return pSuite;
}
