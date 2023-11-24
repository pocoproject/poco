//
// ClassLoaderTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ClassLoaderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "TestPlugin.h"


using Poco::ClassLoader;
using Poco::Manifest;
using Poco::SharedLibrary;
using Poco::AbstractMetaObject;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::Path;
using Poco::File;


ClassLoaderTest::ClassLoaderTest(const std::string& name): CppUnit::TestCase(name)
{
}


ClassLoaderTest::~ClassLoaderTest()
{
}


void ClassLoaderTest::testClassLoader1()
{
	std::string libraryPath = getFullName("TestLibrary");
	ClassLoader<TestPlugin> cl;

	assertTrue (cl.begin() == cl.end());
	assertNullPtr (cl.findClass("PluginA"));
	assertNullPtr (cl.findManifest(libraryPath));

	assertTrue (!cl.isLibraryLoaded(libraryPath));

	try
	{
		const ClassLoader<TestPlugin>::Meta& POCO_UNUSED meta = cl.classFor("PluginA");
		fail("not found - must throw exception");
	}
	catch (NotFoundException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception");
	}

	try
	{
		const ClassLoader<TestPlugin>::Manif& POCO_UNUSED manif = cl.manifestFor(libraryPath);
		fail("not found - must throw exception");
	}
	catch (NotFoundException&)
	{
	}
	catch (...)
	{
		failmsg("wrong exception");
	}
}


void ClassLoaderTest::testClassLoader2()
{
	std::string libraryPath = getFullName("TestLibrary");
	ClassLoader<TestPlugin> cl;
	cl.loadLibrary(libraryPath);

	assertTrue (cl.begin() != cl.end());
	assertNotNullPtr (cl.findClass("PluginA"));
	assertNotNullPtr (cl.findClass("PluginB"));
	assertNotNullPtr (cl.findClass("PluginC"));
	assertNotNullPtr (cl.findManifest(libraryPath));

	assertTrue (cl.isLibraryLoaded(libraryPath));
	assertTrue (cl.manifestFor(libraryPath).size() == 3);

	ClassLoader<TestPlugin>::Iterator it = cl.begin();
	assertTrue (it != cl.end());
	assertTrue (it->first == libraryPath);
	assertTrue (it->second->size() == 3);
	++it;
	assertTrue (it == cl.end());

	TestPlugin* pPluginA = cl.classFor("PluginA").create();
	assertTrue (pPluginA->name() == "PluginA");
	assertTrue (!cl.classFor("PluginA").isAutoDelete(pPluginA));
	delete pPluginA;

	TestPlugin* pPluginB = cl.classFor("PluginB").create();
	assertTrue (pPluginB->name() == "PluginB");
	delete pPluginB;

	pPluginB = cl.create("PluginB");
	assertTrue (pPluginB->name() == "PluginB");
	delete pPluginB;

	assertTrue (cl.canCreate("PluginA"));
	assertTrue (cl.canCreate("PluginB"));
	assertTrue (!cl.canCreate("PluginC"));

	TestPlugin& pluginC = cl.instance("PluginC");
	assertTrue (pluginC.name() == "PluginC");

	try
	{
		TestPlugin& POCO_UNUSED plgB = cl.instance("PluginB");
		fail("not a singleton - must throw");
	}
	catch (InvalidAccessException&)
	{
	}

	try
	{
		TestPlugin* POCO_UNUSED pPluginC = cl.create("PluginC");
		fail("cannot create a singleton - must throw");
	}
	catch (InvalidAccessException&)
	{
	}

	try
	{
		const AbstractMetaObject<TestPlugin>& meta = cl.classFor("PluginC");
		meta.autoDelete(&(meta.instance()));
		fail("cannot take ownership of a singleton - must throw");
	}
	catch (InvalidAccessException&)
	{
	}

	const AbstractMetaObject<TestPlugin>& meta1 = cl.classFor("PluginC");
	assertTrue (meta1.isAutoDelete(&(meta1.instance())));

	// the following must not produce memory leaks
	const AbstractMetaObject<TestPlugin>& meta2 = cl.classFor("PluginA");
	meta2.autoDelete(meta2.create());
	meta2.autoDelete(meta2.create());

	TestPlugin* pPlugin = meta2.create();
	meta2.autoDelete(pPlugin);
	assertTrue (meta2.isAutoDelete(pPlugin));
	meta2.destroy(pPlugin);
	assertTrue (!meta2.isAutoDelete(pPlugin));

	cl.unloadLibrary(libraryPath);
}


void ClassLoaderTest::testClassLoader3()
{
	std::string libraryPath = getFullName("TestLibrary");
	ClassLoader<TestPlugin> cl;
	cl.loadLibrary(libraryPath);
	cl.loadLibrary(libraryPath);
	cl.unloadLibrary(libraryPath);

	assertTrue (cl.manifestFor(libraryPath).size() == 3);

	ClassLoader<TestPlugin>::Iterator it = cl.begin();
	assertTrue (it != cl.end());
	assertTrue (it->first == libraryPath);
	assertTrue (it->second->size() == 3);
	++it;
	assertTrue (it == cl.end());

	cl.unloadLibrary(libraryPath);
	assertNullPtr (cl.findManifest(libraryPath));
}


std::string ClassLoaderTest::getFullName(const std::string& libName)
{
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


void ClassLoaderTest::setUp()
{
}


void ClassLoaderTest::tearDown()
{
}


CppUnit::Test* ClassLoaderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ClassLoaderTest");

	CppUnit_addTest(pSuite, ClassLoaderTest, testClassLoader1);
	CppUnit_addTest(pSuite, ClassLoaderTest, testClassLoader2);
	CppUnit_addTest(pSuite, ClassLoaderTest, testClassLoader3);

	return pSuite;
}
