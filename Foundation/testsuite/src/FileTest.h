//
// FileTest.h
//
// Definition of the FileTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FileTest_INCLUDED
#define FileTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class FileTest: public CppUnit::TestCase
{
public:
	FileTest(const std::string& name);
	~FileTest();

	void testFileAttributes1();
	void testCreateFile();
	void testExists();
	void testFileAttributes2();
	void testFileAttributes3();
	void testCompare();
	void testSwap();
	void testSize();
	void testDirectory();
	void testCopy();
	void testCopyFailIfDestinationFileExists();
	void testMove();
	void testMoveFailIfDestinationFileExists();
	void testCopyDirectory();
	void testCopyDirectoryFailIfExists();
	void testRename();
	void testRenameFailIfExists();
	void testRootDir();
	void testLongPath();
	void testUnixFileExtension();
	void testTemporaryFile();
	void testGetExecutablePathNonExistent();
	void testGetExecutablePathResolve();
	void testGetExecutablePathAbsolute();
	void testGetExecutablePathNonExecutable();
	void testGetExecutablePathEmpty();
	void testGetExecutablePathDirectory();
	void testGetExecutablePathDirectoryShadow();
	void testGetExecutablePathRelative();
	void testGetExecutablePathPATHEXT();
#if defined(POCO_OS_FAMILY_UNIX)
	void testGetExecutablePathThreadSafety();
#endif

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FileTest_INCLUDED
