//
// MailRecipientTest.h
//
// Definition of the MailRecipientTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MailRecipientTest_INCLUDED
#define MailRecipientTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class MailRecipientTest: public CppUnit::TestCase
{
public:
	MailRecipientTest() = delete;
	explicit MailRecipientTest(const std::string& name);
	~MailRecipientTest() override;

	void testMove();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // MailRecipientTest_INCLUDED
