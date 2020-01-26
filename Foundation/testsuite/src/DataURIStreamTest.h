//
// DataURIStreamTest.h
//
// Definition of the DataURItreamTest class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataURIStreamTest_INCLUDED
#define DataURIStreamTest_INCLUDED


#include "CppUnit/TestCase.h"


class DataURIStreamTest: public CppUnit::TestCase
{
public:
	DataURIStreamTest(const std::string& name);
	~DataURIStreamTest();

	void testWithBase64();
	void testWithoutBase64();
	void testZeroLengthData();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // DataURIStreamTest_INCLUDED
