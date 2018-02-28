//
// DirectoryIteratorsTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DirectoryIteratorsTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/SortedDirectoryIterator.h"
#include "Poco/RecursiveDirectoryIterator.h"
#include "Poco/FileStream.h"
#include "Poco/Exception.h"

#include <iostream>

#if defined(POCO_OS_FAMILY_UNIX)
#include <errno.h>
#include <sys/stat.h>
#endif

using namespace Poco;


#if defined(POCO_OS_FAMILY_UNIX)

static void setReadable(const std::string& path, bool flag)
{
	poco_assert(!path.empty());

	struct stat st;
	if (stat(path.c_str(), &st) != 0)
	{
		throw FileException("stat error", path, errno);
	}
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IRUSR | S_IRGRP | S_IROTH;
	}
	else
	{
		mode_t rmask = S_IRUSR | S_IRGRP | S_IROTH;
		mode = st.st_mode & ~rmask;
	}
	if (chmod(path.c_str(), mode) != 0)
	{
		throw FileException("chmod", path, errno);
	}
}

#else

static void setReadable(const std::string& path, bool flag)
{
	poco_assert(!path.empty());
}

#endif


class TemporarilyHidePath
{
public:
	TemporarilyHidePath(std::string path)
		: _path(path)
	{
		setReadable(_path, false);
	}

	~TemporarilyHidePath()
	{
		setReadable(_path, true);
	}
private:
	std::string _path;
};


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


void DirectoryIteratorsTest::testSimpleRecursiveDirectoryIteratorOnError()
{
	Path p = path();
	SimpleRecursiveDirectoryIterator dirIterator(p);
	dirIterator.onError(*this, &DirectoryIteratorsTest::onError);
	SimpleRecursiveDirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	Path second(p);
	second.pushDirectory("first");
	second.pushDirectory("second");
	std::string errorPath(second.toString());
	TemporarilyHidePath hidePath(errorPath);

	while (dirIterator != end)
	{
		file = dirIterator->path();
		++dirIterator;
		result.push_back(file);
	}

#if defined(POCO_OS_FAMILY_UNIX)
	assert(_onErrorPath.size() > 0);
	if (second.separator() != *_onErrorPath.rbegin())
		_onErrorPath += second.separator();
	if (second.separator() != *errorPath.rbegin())
		errorPath += second.separator();

	assertEquals(_onErrorPath, errorPath);
	assertEquals(14, (long) result.size());
#else
	assertEquals(20, (long) result.size());
#endif
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


void DirectoryIteratorsTest::testSiblingsFirstRecursiveDirectoryIteratorOnError()
{
	Path p = path();
	SiblingsFirstRecursiveDirectoryIterator dirIterator(p);
	dirIterator.onError(*this, &DirectoryIteratorsTest::onError);
	SimpleRecursiveDirectoryIterator end;
	std::vector<std::string> result;
	std::string file;

	Path first(p);
	first.pushDirectory("first");
	std::string errorPath(first.toString());
	TemporarilyHidePath hidePath(errorPath);

	while (dirIterator != end)
	{
		file = dirIterator->path();
		++dirIterator;
		result.push_back(file);
	}

#if defined(POCO_OS_FAMILY_UNIX)
	assert(_onErrorPath.size() > 0);
	if (first.separator() != *_onErrorPath.rbegin())
		_onErrorPath += first.separator();
	if (first.separator() != *errorPath.rbegin())
		errorPath += first.separator();

	assertEquals(_onErrorPath, errorPath);
	assertEquals(7, (long) result.size());
#else
	assertEquals(20, (long) result.size());
#endif
}


void DirectoryIteratorsTest::setUp()
{
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


void DirectoryIteratorsTest::onError(const void* pSender, const std::string& path)
{
	_onErrorPath = path;
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
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSimpleRecursiveDirectoryIteratorOnError);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSiblingsFirstRecursiveDirectoryIterator);
	CppUnit_addTest(pSuite, DirectoryIteratorsTest, testSiblingsFirstRecursiveDirectoryIteratorOnError);

	return pSuite;
}
