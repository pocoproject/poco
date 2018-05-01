//
// SharedMemoryTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SharedMemoryTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/SharedMemory.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Exception.h"
#include "Poco/Environment.h"
#include <sstream>
#include <iostream>


using Poco::SharedMemory;


SharedMemoryTest::SharedMemoryTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


SharedMemoryTest::~SharedMemoryTest()
{
}


void SharedMemoryTest::testCreate()
{
	SharedMemory mem("hi", 4096, SharedMemory::AM_WRITE);
	assertTrue (mem.end()-mem.begin() == 4096);
	mem.begin()[0] = 'A';
	mem.end()[-1] = 'Z';
}


void SharedMemoryTest::testCreateFromFile()
{
	Poco::Path p = findDataFile("data", "testdata.txt");
	Poco::File f(p);
	assertTrue (f.exists() && f.isFile());
	SharedMemory mem(f, SharedMemory::AM_READ);
	assertTrue (mem.end() > mem.begin()); // valid?
	assertTrue (mem.end() - mem.begin() == f.getSize());
	assertTrue (mem.begin()[0] == 'A');
	assertTrue (mem.end()[-5] == 'Z');
}


Poco::Path SharedMemoryTest::findDataFile(const std::string& directory, const std::string& file)
{
	std::ostringstream ostr;
	ostr << directory << '/' << file;
	std::string validDir(ostr.str());
	Poco::Path pathPattern(validDir);
	if (Poco::File(pathPattern).exists())
	{
		return pathPattern;
	}

	ostr.str("");
	ostr << "/Foundation/testsuite/" << directory << '/' << file;
	validDir = Poco::Environment::get("POCO_BASE") + ostr.str();
	pathPattern = validDir;

	if (!Poco::File(pathPattern).exists())
	{
		std::cout << "Can't find " << validDir << std::endl;
		throw Poco::NotFoundException("cannot locate directory containing valid Zip test files");
	}
	return pathPattern;
}


void SharedMemoryTest::setUp()
{
}


void SharedMemoryTest::tearDown()
{
}


CppUnit::Test* SharedMemoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SharedMemoryTest");

#if !defined(POCO_NO_SHAREDMEMORY)
	CppUnit_addTest(pSuite, SharedMemoryTest, testCreate);
	CppUnit_addTest(pSuite, SharedMemoryTest, testCreateFromFile);
#endif
	return pSuite;
}
