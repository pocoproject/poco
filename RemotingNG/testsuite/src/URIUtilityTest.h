//
// URIUtilityTest.h
//
// Definition of the URIUtilityTest class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef URIUtilityTest_INCLUDED
#define URIUtilityTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"


class URIUtilityTest: public CppUnit::TestCase
{
public:
	URIUtilityTest(const std::string& name);
	~URIUtilityTest() override;

	void testMatchPathBasic();
	void testMatchPathPlaceholder();
	void testMatchPathAlternation();
	void testMatchPathAlternationNamed();
	void testMatchPathMixed();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
};


#endif // URIUtilityTest_INCLUDED
