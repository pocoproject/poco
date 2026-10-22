//
// SecureSMTPClientSessionTest.h
//
// Definition of the SecureSMTPClientSessionTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SecureSMTPClientSessionTest_INCLUDED
#define SecureSMTPClientSessionTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class SecureSMTPClientSessionTest: public CppUnit::TestCase
{
public:
	SecureSMTPClientSessionTest() = delete;
	explicit SecureSMTPClientSessionTest(const std::string& name);
	~SecureSMTPClientSessionTest() override;

	void testStartTLSSendsHostName();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // SecureSMTPClientSessionTest_INCLUDED
