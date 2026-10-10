//
// ULIDGeneratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ULIDGeneratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ULIDGenerator.h"
#include "Poco/ULID.h"
#include <set>


using Poco::ULIDGenerator;


ULIDGeneratorTest::ULIDGeneratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


ULIDGeneratorTest::~ULIDGeneratorTest()
{
}


void ULIDGeneratorTest::testCreate()
{
	ULIDGenerator& gen = ULIDGenerator::defaultGenerator();

	std::set<Poco::ULID> ulids;
	for (int i = 0; i < 1000; ++i)
	{
		Poco::ULID ulid = gen.create();
		assertTrue (ulids.find(ulid) == ulids.end());
		ulids.insert(ulid);
	}
}


void ULIDGeneratorTest::testCreateTime()
{
	const Poco::UInt64 time = static_cast<Poco::UInt64>(1766090811)*1000;

	ULIDGenerator gen;
	std::set<Poco::ULID> ulids;
	for (int i = 0; i < 1000; ++i)
	{
		try
		{
			Poco::ULID ulid = gen.create(time);
			assertTrue (ulids.find(ulid) == ulids.end());
			ulids.insert(ulid);
			assertTrue (ulid.time() == time);
		}
		catch (Poco::RuntimeException&)
		{
		}
	}
}


void ULIDGeneratorTest::setUp()
{
}


void ULIDGeneratorTest::tearDown()
{
}


CppUnit::Test* ULIDGeneratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ULIDGeneratorTest");

	CppUnit_addTest(pSuite, ULIDGeneratorTest, testCreate);
	CppUnit_addTest(pSuite, ULIDGeneratorTest, testCreateTime);

	return pSuite;
}
