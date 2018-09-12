//
// SharedLibraryTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SharedLibraryTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/SharedLibrary.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"


using Poco::SharedLibrary;
using Poco::NotFoundException;
using Poco::LibraryLoadException;
using Poco::LibraryAlreadyLoadedException;
using Poco::Path;


typedef int (*GimmeFiveFunc)();


SharedLibraryTest::SharedLibraryTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


SharedLibraryTest::~SharedLibraryTest()
{
}


void SharedLibraryTest::testSharedLibrary1()
{
	std::string self = Path(Path::self()).makeParent().toString();
	std::string path = self + "TestLibrary";
	path.append(SharedLibrary::suffix());
	SharedLibrary sl;
	assertTrue (!sl.isLoaded());
	sl.load(path);
	assertTrue (sl.getPath() == path);
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
	std::string self = Path(Path::self()).makeParent().toString();
	std::string path = self + "TestLibrary";
	path.append(SharedLibrary::suffix());
	SharedLibrary sl(path);
	assertTrue (sl.getPath() == path);
	assertTrue (sl.isLoaded());

	GimmeFiveFunc gimmeFive = (GimmeFiveFunc) sl.getSymbol("gimmeFive");
	assertTrue (gimmeFive() == 5);
	
	sl.unload();
	assertTrue (!sl.isLoaded());
}


void SharedLibraryTest::testSharedLibrary3()
{
	std::string path = "NonexistentLibrary";
	SharedLibrary sl;
	try
	{
		sl.load(path);
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

	std::string self = Path(Path::self()).makeParent().toString();
	path = self + "TestLibrary";
	path.append(SharedLibrary::suffix());
	sl.load(path);
	assertTrue (sl.isLoaded());
	
	try
	{
		sl.load(path);
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


void SharedLibraryTest::setUp()
{
}


void SharedLibraryTest::tearDown()
{
}


CppUnit::Test* SharedLibraryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SharedLibraryTest");

#ifndef _DEBUG // FIXME excluded from the Debug build temporarily for AppVeyor stability
	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary1);
	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary2);
	CppUnit_addTest(pSuite, SharedLibraryTest, testSharedLibrary3);
#endif

	return pSuite;
}
