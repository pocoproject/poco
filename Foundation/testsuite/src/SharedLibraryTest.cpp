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
#include "Poco/Glob.h"
#include "Poco/Format.h"
#include "Poco/Environment.h"
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <iostream>


using Poco::SharedLibrary;
using Poco::NotFoundException;
using Poco::LibraryLoadException;
using Poco::LibraryAlreadyLoadedException;
using Poco::Path;
using Poco::File;
using Poco::Glob;
using Poco::Environment;


typedef int (*GimmeFiveFunc)();


SharedLibraryTest::SharedLibraryTest(const std::string& name): CppUnit::TestCase(name)
{
}


SharedLibraryTest::~SharedLibraryTest()
{
}


void SharedLibraryTest::testSharedLibrary1()
{
#ifdef POCO_STATIC
	return; // Skip test in static builds where TestLibrary is not built
#endif

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
#ifdef POCO_STATIC
	return; // Skip test in static builds where TestLibrary is not built
#endif

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
#ifdef POCO_STATIC
	return; // Skip test in static builds where TestLibrary is not built
#endif

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


void SharedLibraryTest::testMissingDependencies()
{
#ifdef POCO_STATIC
	return; // Skip test in static builds where TestLibrary is not built
#endif

	// Test with a valid library - should return empty list (all dependencies found)
	std::string libraryPath = getFullName("TestLibrary");
	std::vector<std::string> missing = SharedLibrary::findMissingDependencies(libraryPath);
	// TestLibrary should have all its dependencies available
	for (const auto& dep : missing)
		std::cout << "TestLibrary missing: " << dep << std::endl;
	assertTrue (missing.empty());

	// Test with non-existent file - should return the file itself as missing
	missing = SharedLibrary::findMissingDependencies("NonexistentFile.dll");
	assertTrue (missing.size() == 1);
	assertTrue (missing[0] == "NonexistentFile.dll");

	// Test with empty path - should return empty path as missing
	missing = SharedLibrary::findMissingDependencies("");
	assertTrue (missing.size() == 1);
	assertTrue (missing[0].empty());

	// Test with a library that has missing dependencies
	std::string missingDepsLib = getFullName("TestLibraryMissingDeps");
	if (File(missingDepsLib).exists())
	{
		// Delete all NonExistentLibrary files (including versioned ones and symlinks) to ensure the test is valid
		// Try both with and without "lib" prefix since CMake and make builds differ
		// Also check the lib/ directory for make builds (libraries have absolute paths embedded)
		std::set<std::string> libFiles;
		Path selfPath(Path::self());
		selfPath.setFileName("");
		std::string baseDir = selfPath.toString();
		std::replace(baseDir.begin(), baseDir.end(), '\\', '/');
		Glob::glob(baseDir + "NonExistentLibrary*", libFiles);
		Glob::glob(baseDir + "libNonExistentLibrary*", libFiles);

		// For make builds, also check POCO_BASE/lib/OSNAME/OSARCH/
		std::string pocoBase = Environment::get("POCO_BASE", "");
		if (!pocoBase.empty())
		{
			std::string OSNAME = Environment::get("OSNAME", "");
			std::string OSARCH = Environment::get("OSARCH", "");
			std::string libDir = pocoBase + "/lib/" + OSNAME + "/" + OSARCH + "/";
			Glob::glob(libDir + "NonExistentLibrary*", libFiles);
			Glob::glob(libDir + "libNonExistentLibrary*", libFiles);
		}

		for (const auto& libFile : libFiles)
		{
			File f(libFile);
			if (f.exists() || f.isLink())
				f.remove();
		}

		missing = SharedLibrary::findMissingDependencies(missingDepsLib);
		// Should report NonExistentLibrary as missing
		assertTrue (!missing.empty());
		bool foundNonExistent = false;
		for (const auto& dep : missing)
		{
			if (dep.find("NonExistentLibrary") != std::string::npos)
			{
				foundNonExistent = true;
				break;
			}
		}
		assertTrue (foundNonExistent);
		for (const auto& dep : missing)
			std::cout << "Missing library: " << dep;
	}
}


std::string SharedLibraryTest::getFullName(const std::string& libName)
{
	// Get the directory where the test executable is located
	Path selfPath(Path::self());
	selfPath.setFileName("");
	std::string name = selfPath.toString() + libName + SharedLibrary::suffix();

	if (File(name).exists())
		return name;

	// Fallback: make build layout
	std::string pocoBase = Environment::get("POCO_BASE", "");
	if (!pocoBase.empty())
	{
		char c = Path::separator();
		std::string OSNAME = Environment::get("OSNAME", "");
		std::string OSARCH = Environment::get("OSARCH", "");
		name = pocoBase;
		name.append(1, c)
			.append(Poco::format("Foundation%ctestsuite%cbin%c", c, c, c))
			.append(Poco::format("%s%c%s%c", OSNAME, c, OSARCH, c))
			.append(libName).append(SharedLibrary::suffix());

		if (File(name).exists())
			return name;
	}

	// Last resort: just the library name
	return libName + SharedLibrary::suffix();
}


std::string SharedLibraryTest::getLibFullName(const std::string& libName)
{
	std::string suffix = SharedLibrary::suffix();

	// Get the directory where the test executable is located
	Path selfPath(Path::self());
	selfPath.setFileName("");

#if defined(_WIN32)
	std::string name = selfPath.toString() + libName + suffix;
#else
	// On Unix, shared libraries have a "lib" prefix
	std::string name = selfPath.toString() + "lib" + libName + suffix;
#endif

	if (File(name).exists())
		return name;

#if !defined(_WIN32)
	// Fallback: make build layout - libraries in lib/ directory with lib prefix
	std::string pocoBase = Environment::get("POCO_BASE", "");
	if (!pocoBase.empty())
	{
		char c = Path::separator();
		std::string OSNAME = Environment::get("OSNAME", "");
		std::string OSARCH = Environment::get("OSARCH", "");
		name = pocoBase;
		name.append(1, c)
			.append(Poco::format("lib%c%s%c%s%c", c, OSNAME, c, OSARCH, c))
			.append("lib").append(libName).append(suffix);

		if (File(name).exists())
			return name;
	}
#endif

	// Last resort: just the library name
	return libName + suffix;
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
	CppUnit_addTest(pSuite, SharedLibraryTest, testMissingDependencies);

	return pSuite;
}
