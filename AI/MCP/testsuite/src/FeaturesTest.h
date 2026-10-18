//
// FeaturesTest.h
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FeaturesTest_INCLUDED
#define FeaturesTest_INCLUDED


#include "CppUnit/TestCase.h"


class FeaturesTest: public CppUnit::TestCase
{
public:
	FeaturesTest(const std::string& name);
	~FeaturesTest();

	void testResourcesList();
	void testResourceReadText();
	void testResourceReadBlob();
	void testResourceNotFound();
	void testResourceReaderThrows();
	void testResourceTemplatesEmpty();
	void testPromptsList();
	void testPromptGet();
	void testPromptArgumentErrors();
	void testCursorRefused();
	void testRegistrationRefused();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // FeaturesTest_INCLUDED
