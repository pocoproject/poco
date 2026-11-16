//
// DirectoryIteratorsTest.h
//
// Definition of the DirectoryIteratorsTest class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DirectoryIteratorsTest_INCLUDED
#define DirectoryIteratorsTest_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Path.h"
#include "CppUnit/TestCase.h"
#include <string>
#include <vector>


class DirectoryIteratorsTest: public CppUnit::TestCase
{
public:
	DirectoryIteratorsTest(const std::string& name);
	~DirectoryIteratorsTest();

	void testDirectoryIterator();
	void testSortedDirectoryIterator();
	void testSimpleRecursiveDirectoryIterator();
	void testSiblingsFirstRecursiveDirectoryIterator();
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
	void testSimpleRecursiveDirectoryIteratorOnError();
	void testSiblingsFirstRecursiveDirectoryIteratorOnError();
#endif

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	Poco::Path path() const;
	void createSubdir(Poco::Path& p);
	void onError(const void* pSender, const std::string& path);

private:
	std::vector<std::string> _errorPaths;
};


#endif // DirectoryIteratorsTest_INCLUDED
