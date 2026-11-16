//
// AutoSecBufferDescTest.h
//
// Definition of the AutoSecBufferDescTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AutoSecBufferDescTest_INCLUDED
#define AutoSecBufferDescTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class AutoSecBufferDescTest: public CppUnit::TestCase
{
public:
	AutoSecBufferDescTest() = delete;
	explicit AutoSecBufferDescTest(const std::string& name);
	~AutoSecBufferDescTest() override;

	void setUp() override;
	void tearDown() override;

	void testSecurityWin32Defined();

	static CppUnit::Test* suite();
};


#endif // AutoSecBufferDescTest_INCLUDED
