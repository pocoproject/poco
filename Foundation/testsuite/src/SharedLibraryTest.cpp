//
// SharedLibraryTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SharedLibraryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SharedLibrary.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"


using Poco::SharedLibrary;
using Poco::NotFoundException;
using Poco::LibraryLoadException;
using Poco::LibraryAlreadyLoadedException;
using Poco::Path;
using Poco::File;


typedef int (*GimmeFiveFunc)();


SharedLibraryTest::SharedLibraryTest(const std::string& name): CppUnit::TestCase(name)
{
}


SharedLibraryTest::~SharedLibraryTest()
{
}


void SharedLibraryTest::testSharedLibrary1()
{
	std::string libraryPath = getFullName("TestLibrary");
	SharedLibrary sl;
	assertTrue (!sl.isLoaded());
	sl.load(libraryPath);
	assertTrue (sl.getPath() == libraryPath);
	assertTrue (sl.isLoaded());
	assertTrue (sl.hasSymbol("pocoBuildManifest"));
	assertTrue (sl.hasSymbol("pocoInitializeLibrary"));
	assertTrue (sl.hasSymbol("pocoUninitializeLibrary"));
	assertTrue (sl.hasSymbol("gimmeFive"));
	assertTrue (!sl.hasSymbol("fooBar123"));

	void* p1 = sl.getSymbol("pocoBuildManifest");
	assertNotNullPtr(p1);
	try
	{
		p1 = sl.getSymbol("fooBar123");
		failmsg("no such symbol - must throw exception");
	}
	catch (NotFoundException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception");
	}
	sl.unload();
	assertTrue (!sl.isLoaded());
}


void SharedLibraryTest::testSharedLibrary2()
{
	std::string libraryPath = getFullName("TestLibrary");
	SharedLibrary sl(libraryPath);
	assertTrue (sl.getPath() == libraryPath);
	assertTrue (sl.isLoaded());

	GimmeFiveFunc gimmeFive = (GimmeFiveFunc) sl.getSymbol("gimmeFive");
	assertTrue (gimmeFive() == 5);

	sl.unload();
	assertTrue (!sl.isLoaded());
}


void SharedLibraryTest::testSharedLibrary3()
{
	std::string libraryPath = "NonexistentLibrary";
	libraryPath.append(libraryPath);
	SharedLibrary sl;
	try
	{
		sl.load(libraryPath);
		failmsg("no such library - must throw exception");
	}
	catch (LibraryLoadException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception");
	}
	assertTrue (!sl.isLoaded());

	libraryPath = getFullName("TestLibrary");
	sl.load(libraryPath);
	assertTrue (sl.isLoaded());

	try
	{
		sl.load(libraryPath);
		failmsg("library already loaded - must throw exception");
	}
	catch (LibraryAlreadyLoadedException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception");
	}
	assertTrue (sl.isLoaded());

	sl.unload();
	assertTrue (!sl.isLoaded());
}


std::string SharedLibraryTest::getFullName(const std::string& libName)
{
	// make
	std::string name = Path::expand("$POCO_BASE");
	char c = Path::separator();
	std::string OSNAME = Path::expand("$OSNAME");
	std::string OSARCH = Path::expand("$OSARCH");
	name.append(1, c)
		.append(Poco::format("Foundation%ctestsuite%cbin%c", c, c, c))
		.append(Poco::format("%s%c%s%c", OSNAME, c, OSARCH, c))
		.append(libName).append(SharedLibrary::suffix());

	// CMake
	if (!File(name).exists())
	{
		name = Path::expand("$POCO_BASE");
		name.append(Poco::format("%ccmake-build%cbin%c", c, c, c))
			.append(libName).append(SharedLibrary::suffix());
	}

	if (!File(name).exists())
		name = libName + SharedLibrary::suffix();

	return name;
}


void SharedLibraryTest::setUp()
{
}


void SharedLibraryTest::tearDown()
{
}


CppUnit::Test* SharedLibraryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SharedLibraryTest");

	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary1);
	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary2);
	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary3);

	return pSuite;
}
