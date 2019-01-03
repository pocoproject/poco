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
#include "Poco/CppUnit/TestCase.h"


class DirectoryIteratorsTest: public CppUnit::TestCase
{
public:
	DirectoryIteratorsTest(const std::string& name);
	~DirectoryIteratorsTest();

	void testDirectoryIterator();
	void testSortedDirectoryIterator();
	void testSimpleRecursiveDirectoryIterator();
	void testSimpleRecursiveDirectoryIteratorOnError();
	void testSiblingsFirstRecursiveDirectoryIterator();
	void testSiblingsFirstRecursiveDirectoryIteratorOnError();
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
	
protected:
	Poco::Path path() const;
	void createSubdir(Poco::Path& p);
	void onError(const void* pSender, const std::string& path);
	std::string _onErrorPath;

private:
};


#endif // DirectoryIteratorsTest_INCLUDED
