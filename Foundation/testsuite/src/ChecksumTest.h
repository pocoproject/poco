//
// ChecksumTest.h
//
// Definition of the ChecksumTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ChecksumTest_INCLUDED
#define ChecksumTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class ChecksumTest: public CppUnit::TestCase
{
public:
	ChecksumTest() = delete;
	explicit ChecksumTest(const std::string& name);
	~ChecksumTest() override;

	void testCRC32();
	void testAdler32();
	void testLargeInput();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ChecksumTest_INCLUDED
