//
// MongoDBTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MongoDBTestSuite.h"
#include "MongoDBTest.h"
#include "BSONTest.h"


CppUnit::Test* MongoDBTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MongoDBTestSuite");

	pSuite->addTest(BSONTest::suite());

	CppUnit::Test* mongoTests = MongoDBTest::suite();
	if (mongoTests != nullptr)
	{
		pSuite->addTest(mongoTests);
	}

	return pSuite;
}
