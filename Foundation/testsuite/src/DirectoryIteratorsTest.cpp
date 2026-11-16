//
// DirectoryIteratorsTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DirectoryIteratorsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/SortedDirectoryIterator.h"
#include "Poco/RecursiveDirectoryIterator.h"
#include "Poco/FileStream.h"
#include "Poco/Exception.h"

#include <iostream>
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#endif

using namespace Poco;


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
namespace
{
	class UnreadableDirectory
		/// Removes all permissions from a directory and restores
		/// its original mode when destroyed.
	{
	public:
		UnreadableDirectory() = delete;
		UnreadableDirectory(const UnreadableDirectory&) = delete;
		UnreadableDirectory& operator = (const UnreadableDirectory&) = delete;

		explicit UnreadableDirectory(const std::string& path);
		~UnreadableDirectory();

	private:
		std::string _path;
		mode_t _mode = 0;
	};


	UnreadableDirectory::UnreadableDirectory(const std::string& path):
		_path(path)
	{
		struct stat st;
		if (::stat(_path.c_str(), &st) != 0) throw FileException("cannot read the mode", _path, errno);
		_mode = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
		if (::chmod(_path.c_str(), 0) != 0) throw FileException("cannot change the mode", _path, errno);
	}


	UnreadableDirectory::~UnreadableDirectory()
	{
		::chmod(_path.c_str(), _mode);
	}
}
#endif


DirectoryIteratorsTest::DirectoryIteratorsTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


DirectoryIteratorsTest::~DirectoryIteratorsTest()
{
}


void DirectoryIteratorsTest::testDirectoryIterator()
{
	Path p = path();
	DirectoryIterator dirIterator(p);
	DirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	while (dirIterator != end)
	{
		file = dirIterator->path();
		++dirIterator;
		result.push_back(file);
	}

	assertEquals(7, (long) result.size());
}


void DirectoryIteratorsTest::testSortedDirectoryIterator()
{
	Path p = path();
	SortedDirectoryIterator dirIterator(p);
	SortedDirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	while (dirIterator != end)
	{
		file = Path(dirIterator->path()).getFileName();
		++dirIterator;
		result.push_back(file);
	}

	assertEquals(7, (long) result.size());
	assertEquals("first", result[0]);
	assertEquals("1", result[1]);
	assertEquals("2", result[2]);
	assertEquals("A", result[3]);
	assertEquals("B", result[4]);
	assertEquals("c", result[5]);
	assertEquals("d", result[6]);
}


void DirectoryIteratorsTest::testSimpleRecursiveDirectoryIterator()
{
	Path p = path();
	SimpleRecursiveDirectoryIterator dirIterator(p);
	SimpleRecursiveDirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	while (dirIterator != end)
	{
		file = dirIterator->path();
		++dirIterator;
		result.push_back(file);
	}

	assertEquals(20, (long) result.size());
}


void DirectoryIteratorsTest::testSiblingsFirstRecursiveDirectoryIterator()
{
	Path p = path();
	SiblingsFirstRecursiveDirectoryIterator dirIterator(p);
	SiblingsFirstRecursiveDirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	while (dirIterator != end)
	{
		file = dirIterator->path();
		++dirIterator;
		result.push_back(file);
	}

	assertEquals(20, (long) result.size());
}


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
void DirectoryIteratorsTest::testSimpleRecursiveDirectoryIteratorOnError()
{
	// Root reads a directory regardless of its mode.
	if (::geteuid() == 0) return;

	Path p = path();
	Path second(p);
	second.pushDirectory("first");
	second.pushDirectory("second");
	UnreadableDirectory unreadable(second.toString());

	SimpleRecursiveDirectoryIterator dirIterator(p);
	dirIterator.onError(*this, &DirectoryIteratorsTest::onError);
	SimpleRecursiveDirectoryIterator end;
	std::vector<std::string> result;

	while (dirIterator != end)
	{
		result.push_back(dirIterator->path());
		++dirIterator;
	}

	assertEqual(14, result.size());
	assertEqual(1, _errorPaths.size());
	assertEqual(second.makeFile().toString(), _errorPaths[0]);
}


void DirectoryIteratorsTest::testSiblingsFirstRecursiveDirectoryIteratorOnError()
{
	// Root reads a directory regardless of its mode.
	if (::geteuid() == 0) return;

	Path p = path();
	Path first(p);
	first.pushDirectory("first");
	UnreadableDirectory unreadable(first.toString());

	SiblingsFirstRecursiveDirectoryIterator dirIterator(p);
	dirIterator.onError(*this, &DirectoryIteratorsTest::onError);
	SiblingsFirstRecursiveDirectoryIterator end;
	std::vector<std::string> result;

	while (dirIterator != end)
	{
		result.push_back(dirIterator->path());
		++dirIterator;
	}

	assertEqual(7, result.size());
	assertEqual(1, _errorPaths.size());
	assertEqual(first.makeFile().toString(), _errorPaths[0]);
}
#endif


void DirectoryIteratorsTest::setUp()
{
	_errorPaths.clear();

	File d(path());
	if (d.exists()) d.remove(true);

	/*
	  Build Directory Tree like this:

	  DirectoryIteratorsTest
		|-- 1
		|-- 2
		|-- A
		|-- B
		|-- c
		|-- d
		`-- first
			|-- 1
			|-- 2
			|-- A
			|-- B
			|-- c
			|-- d
			`-- second
				|-- 1
				|-- 2
				|-- A
				|-- B
				|-- c
				`-- d

		2 directories, 18 files
	 */
	Path p = path();
	createSubdir(p);

	p.pushDirectory("first");
	createSubdir(p);

	p.pushDirectory("second");
	createSubdir(p);
}


void DirectoryIteratorsTest::createSubdir(Path& p)
{
	File d(p);
	d.createDirectories();
	FileStream f1(p.toString() + "d");
	FileStream f2(p.toString() + "1");
	FileStream f3(p.toString() + "A");
	FileStream f4(p.toString() + "2");
	FileStream f5(p.toString() + "B");
	FileStream f6(p.toString() + "c");
}


void DirectoryIteratorsTest::tearDown()
{
	try
	{
		File d(path());
		d.remove(true);
	}
	catch (...)
	{
	}
}


void DirectoryIteratorsTest::onError(const void*, const std::string& path)
{
	_errorPaths.push_back(path);
}


Poco::Path DirectoryIteratorsTest::path() const
{
	Path p(Path::current());
	p.pushDirectory("DirectoryIteratorsTest");
	return p;
}


CppUnit::Test* DirectoryIteratorsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DirectoryIteratorsTest");

	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testDirectoryIterator);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSortedDirectoryIterator);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSimpleRecursiveDirectoryIterator);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSiblingsFirstRecursiveDirectoryIterator);
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSimpleRecursiveDirectoryIteratorOnError);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSiblingsFirstRecursiveDirectoryIteratorOnError);
#endif

	return pSuite;
}
