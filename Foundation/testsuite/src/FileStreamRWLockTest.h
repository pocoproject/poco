//
// FileStreamRWLockTest.h
//
// Definition of the FileStreamRWLockTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FileStreamRWLockTest_INCLUDED
#define FileStreamRWLockTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class FileStreamRWLockTest: public CppUnit::TestCase
{
public:
	FileStreamRWLockTest(const std::string& name);
	~FileStreamRWLockTest();

	void testFSLock();
	void testFSTryLock();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FileStreamRWLockTest_INCLUDED
