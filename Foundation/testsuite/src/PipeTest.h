//
// PipeTest.h
//
// Definition of the PipeTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PipeTest_INCLUDED
#define PipeTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class PipeTest: public CppUnit::TestCase
{
public:
	PipeTest(const std::string& name);
	~PipeTest();

	void testReadWrite();
	void testClose();
	void testCloseRead();
	void testCloseWrite();
	void testCopy();
	void testCloseRace();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // PipeTest_INCLUDED
