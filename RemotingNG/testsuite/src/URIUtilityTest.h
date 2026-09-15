//
// URIUtilityTest.h
//
// Definition of the URIUtilityTest class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef URIUtilityTest_INCLUDED
#define URIUtilityTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"


class URIUtilityTest: public CppUnit::TestCase
{
public:
	URIUtilityTest(const std::string& name);
	~URIUtilityTest();

	void testMatchPathBasic();
	void testMatchPathPlaceholder();
	void testMatchPathAlternation();
	void testMatchPathAlternationNamed();
	void testMatchPathMixed();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // URIUtilityTest_INCLUDED
