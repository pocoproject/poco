//
// HashingTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HashingTestSuite.h"
#if defined(POCO_TEST_DEPRECATED)
#include "HashTableTest.h"
#include "SimpleHashTableTest.h"
#endif
#include "LinearHashTableTest.h"
#include "HashSetTest.h"
#include "HashMapTest.h"


CppUnit::Test* HashingTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HashingTestSuite");

#if defined(POCO_TEST_DEPRECATED)
	pSuite->addTest(HashTableTest::suite());
	pSuite->addTest(SimpleHashTableTest::suite());
#endif
	pSuite->addTest(LinearHashTableTest::suite());
	pSuite->addTest(HashSetTest::suite());
	pSuite->addTest(HashMapTest::suite());

	return pSuite;
}
