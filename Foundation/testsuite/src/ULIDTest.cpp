//
// ULIDTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ULIDTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ULID.h"
#include "Poco/Exception.h"


ULIDTest::ULIDTest(const std::string& name): CppUnit::TestCase(name)
{
}


ULIDTest::~ULIDTest()
{
}


void ULIDTest::testParse()
{
	Poco::ULID ulid("01BX5ZZKBKACTAV9WEVGEMMVRY");
	assertTrue (ulid.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");

	ulid.parse("01bx5zzkbkactav9wevgemmvry");
	assertTrue (ulid.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");

	ulid.parse("olbx5zzkbkactav9wevgemmvry");
	assertTrue (ulid.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");

	ulid.parse("01KCSHHDYBY3J5VRSXHMKNYSE6");
	assertTrue (ulid.toString() == "01KCSHHDYBY3J5VRSXHMKNYSE6");

	ulid.parse("7ZZZZZZZZZZZZZZZZZZZZZZZZZ");
	assertTrue (ulid.toString() == "7ZZZZZZZZZZZZZZZZZZZZZZZZZ");

	ulid.parse("00000000000000000000000000");
	assertTrue (ulid.toString() == "00000000000000000000000000");

	try
	{
		ulid.parse("80000000000000000000000000");
		fail("out of range ULID - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	try
	{
		ulid.parse("01BX5ZZKBKACTAV9WEVGEMMVR");
		fail("malformed ULID - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	try
	{
		ulid.parse("01BX5ZZKBKAUTAV9WEVGEMMVRY");
		fail("malformed ULID - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	try
	{
		ulid.parse("");
		fail("malformed ULID - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void ULIDTest::testBuffer()
{
	Poco::ULID ulid("01BX5ZZKBKACTAV9WEVGEMMVRY");
	char buffer[16];
	ulid.copyTo(buffer);
	Poco::ULID ulid2;
	ulid2.copyFrom(buffer);
	assertTrue (ulid2.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");
}


void ULIDTest::testCompare()
{
	Poco::ULID null;
	assertTrue (null.isNull());
	assertTrue (Poco::ULID::null().isNull());

	Poco::ULID ulid1 = null;
	Poco::ULID ulid2;
	assertTrue (ulid1.isNull());
	assertTrue (ulid1 == null);
	assertFalse (ulid1 != null);
	assertTrue (ulid1 >= null);
	assertTrue (ulid1 <= null);
	assertFalse (ulid1 > null);
	assertFalse (ulid1 < null);

	ulid1.parse("01BX5ZZKBKACTAV9WEVGEMMVRY");
	ulid2.parse("01BX5ZZKBKACTAV9WEVGEMMVRZ");
	assertTrue (ulid1 < ulid2);
	assertTrue (ulid1 <= ulid2);
	assertTrue (ulid2 > ulid1);
	assertTrue (ulid2 >= ulid1);
	assertFalse (ulid1 == ulid2);
	assertFalse (ulid2 == ulid1);
}


void ULIDTest::testSwap()
{
	Poco::ULID ulid1("01BX5ZZKBKACTAV9WEVGEMMVRY");
	Poco::ULID ulid2("01BX5ZZKBKACTAV9WEVGEMMVS1");
	ulid1.swap(ulid2);
	assertTrue (ulid1.toString() == "01BX5ZZKBKACTAV9WEVGEMMVS1");
	assertTrue (ulid2.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");
}


void ULIDTest::testTryParse()
{
	Poco::ULID ulid;
	assertTrue (ulid.tryParse("01BX5ZZKBKACTAV9WEVGEMMVRY"));
	assertTrue (ulid.toString() == "01BX5ZZKBKACTAV9WEVGEMMVRY");

	Poco::ULID notUlid;
	assertTrue (!notUlid.tryParse("not a uuid"));
	assertTrue (notUlid.isNull());
}


void ULIDTest::setUp()
{
}


void ULIDTest::tearDown()
{
}


CppUnit::Test* ULIDTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ULIDTest");

	CppUnit_addTest(pSuite, ULIDTest, testParse);
	CppUnit_addTest(pSuite, ULIDTest, testBuffer);
	CppUnit_addTest(pSuite, ULIDTest, testCompare);
	CppUnit_addTest(pSuite, ULIDTest, testSwap);
	CppUnit_addTest(pSuite, ULIDTest, testTryParse);

	return pSuite;
}
