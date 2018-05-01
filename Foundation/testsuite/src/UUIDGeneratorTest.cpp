//
// UUIDGeneratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UUIDGeneratorTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "Poco/SHA1Engine.h"
#include <set>


using Poco::UUIDGenerator;
using Poco::UUID;


UUIDGeneratorTest::UUIDGeneratorTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


UUIDGeneratorTest::~UUIDGeneratorTest()
{
}


void UUIDGeneratorTest::testTimeBased()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();
	
	std::set<UUID> uuids;
	for (int i = 0; i < 1000; ++i)
	{
		UUID uuid = gen.create();
		assertTrue (uuid.version() == UUID::UUID_TIME_BASED);
		assertTrue (uuids.find(uuid) == uuids.end());
		uuids.insert(uuid);
	}
}


void UUIDGeneratorTest::testRandom()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();

	std::set<UUID> uuids;
	for (int i = 0; i < 1000; ++i)
	{
		UUID uuid = gen.createRandom();
		assertTrue (uuid.version() == UUID::UUID_RANDOM);
		assertTrue (uuids.find(uuid) == uuids.end());
		uuids.insert(uuid);
	}
}


void UUIDGeneratorTest::testNameBased()
{
	UUIDGenerator& gen = UUIDGenerator::defaultGenerator();

	UUID uuid1 = gen.createFromName(UUID::uri(), "http://www.appinf.com/uuid");
	assertTrue (uuid1.version() == UUID::UUID_NAME_BASED);
	assertTrue (uuid1.variant() == 2);

	UUID uuid2 = gen.createFromName(UUID::uri(), "http://www.appinf.com/uuid2");
	assertTrue (uuid2 != uuid1);

	UUID uuid3 = gen.createFromName(UUID::dns(), "www.appinf.com");
	assertTrue (uuid3 != uuid1);

	UUID uuid4 = gen.createFromName(UUID::oid(), "1.3.6.1.4.1");
	assertTrue (uuid4 != uuid1);

	UUID uuid5 = gen.createFromName(UUID::x500(), "cn=Guenter Obiltschnig, ou=People, o=Applied Informatics, c=at");
	assertTrue (uuid5 != uuid1);

	UUID uuid6 = gen.createFromName(UUID::uri(), "http://www.appinf.com/uuid");
	assertTrue (uuid6 == uuid1);

	Poco::SHA1Engine sha1;
	UUID uuid7 = gen.createFromName(UUID::uri(), "http://www.appinf.com/uuid", sha1);
	assertTrue (uuid7.version() == UUID::UUID_NAME_BASED_SHA1);
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
