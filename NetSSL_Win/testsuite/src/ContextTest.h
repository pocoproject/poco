//
// ContextTest.h
//
// Definition of the ContextTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ContextTest_INCLUDED
#define ContextTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class ContextTest: public CppUnit::TestCase
{
public:
	ContextTest() = delete;
	explicit ContextTest(const std::string& name);
	~ContextTest() override;

	void testImportPKCS12FileWithChain();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ContextTest_INCLUDED
