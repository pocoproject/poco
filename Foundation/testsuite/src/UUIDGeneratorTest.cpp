//
// UUIDGeneratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UUIDGeneratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "Poco/SHA1Engine.h"
#include <set>


using Poco::UUIDGenerator;


UUIDGeneratorTest::UUIDGeneratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


UUIDGeneratorTest::~UUIDGeneratorTest()
{
}


void UUIDGeneratorTest::testTimeBased()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();

    std::set<Poco::UUID> uuids;
	for (int i = 0; i < 1000; ++i)
	{
        Poco::UUID uuid = gen.create();
        assertTrue (uuid.version() == Poco::UUID::UUID_TIME_BASED);
		assertTrue (uuids.find(uuid) == uuids.end());
		uuids.insert(uuid);
	}
}


void UUIDGeneratorTest::testRandom()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();

    std::set<Poco::UUID> uuids;
	for (int i = 0; i < 1000; ++i)
	{
        Poco::UUID uuid = gen.createRandom();
        assertTrue (uuid.version() == Poco::UUID::UUID_RANDOM);
		assertTrue (uuids.find(uuid) == uuids.end());
		uuids.insert(uuid);
	}
}


void UUIDGeneratorTest::testNameBased()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();

    Poco::UUID uuid1 = gen.createFromName(Poco::UUID::uri(), "http://www.appinf.com/uuid");
    assertTrue (uuid1.version() == Poco::UUID::UUID_NAME_BASED);
	assertTrue (uuid1.variant() == 2);

    Poco::UUID uuid2 = gen.createFromName(Poco::UUID::uri(), "http://www.appinf.com/uuid2");
	assertTrue (uuid2 != uuid1);

    Poco::UUID uuid3 = gen.createFromName(Poco::UUID::dns(), "www.appinf.com");
	assertTrue (uuid3 != uuid1);

    Poco::UUID uuid4 = gen.createFromName(Poco::UUID::oid(), "1.3.6.1.4.1");
	assertTrue (uuid4 != uuid1);

    Poco::UUID uuid5 = gen.createFromName(Poco::UUID::x500(), "cn=Guenter Obiltschnig, ou=People, o=Applied Informatics, c=at");
	assertTrue (uuid5 != uuid1);

    Poco::UUID uuid6 = gen.createFromName(Poco::UUID::uri(), "http://www.appinf.com/uuid");
	assertTrue (uuid6 == uuid1);

	Poco::SHA1Engine sha1;
    Poco::UUID uuid7 = gen.createFromName(Poco::UUID::uri(), "http://www.appinf.com/uuid", sha1);
    assertTrue (uuid7.version() == Poco::UUID::UUID_NAME_BASED_SHA1);
	assertTrue (uuid7.variant() == 2);
	assertTrue (uuid7 != uuid1);
}


void UUIDGeneratorTest::setUp()
{
}


void UUIDGeneratorTest::tearDown()
{
}


CppUnit::Test* UUIDGeneratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UUIDGeneratorTest");

	CppUnit_addTest(pSuite, UUIDGeneratorTest, testTimeBased);
	CppUnit_addTest(pSuite, UUIDGeneratorTest, testRandom);
	CppUnit_addTest(pSuite, UUIDGeneratorTest, testNameBased);

	return pSuite;
}
