//
// MailRecipientTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MailRecipientTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MailRecipient.h"
#include <type_traits>
#include <utility>


using Poco::Net::MailRecipient;


MailRecipientTest::MailRecipientTest(const std::string& name): CppUnit::TestCase(name)
{
}


MailRecipientTest::~MailRecipientTest() = default;


void MailRecipientTest::testMove()
{
	assertTrue (std::is_nothrow_move_constructible_v<MailRecipient>);
	assertTrue (std::is_nothrow_move_assignable_v<MailRecipient>);

	MailRecipient recipient(MailRecipient::CC_RECIPIENT, "jane.doe@no.where", "Jane Doe");
	MailRecipient moved(std::move(recipient));
	assertTrue (moved.getType() == MailRecipient::CC_RECIPIENT);
	assertEqual ("jane.doe@no.where", moved.getAddress());
	assertEqual ("Jane Doe", moved.getRealName());

	MailRecipient assigned(MailRecipient::BCC_RECIPIENT, "john.doe@no.where");
	assigned = std::move(moved);
	assertTrue (assigned.getType() == MailRecipient::CC_RECIPIENT);
	assertEqual ("jane.doe@no.where", assigned.getAddress());
	assertEqual ("Jane Doe", assigned.getRealName());
}


void MailRecipientTest::setUp()
{
}


void MailRecipientTest::tearDown()
{
}


CppUnit::Test* MailRecipientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MailRecipientTest");

	CppUnit_addTest(pSuite, MailRecipientTest, testMove);

	return pSuite;
}
