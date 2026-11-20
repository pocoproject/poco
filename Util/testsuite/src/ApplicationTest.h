//
// ApplicationTest.h
//
// Definition of the ApplicationTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ApplicationTest_INCLUDED
#define ApplicationTest_INCLUDED


#include "Poco/Util/Util.h"
#include "CppUnit/TestCase.h"


class ApplicationTest: public CppUnit::TestCase
{
public:
	ApplicationTest() = delete;
	explicit ApplicationTest(const std::string& name);
	~ApplicationTest() override;

	void testConfigDirFollowsFirstLoadedFile();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ApplicationTest_INCLUDED
