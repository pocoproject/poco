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
#include "TestPlugin.h"


using Poco::ClassLoader;
using Poco::Manifest;
using Poco::SharedLibrary;
using Poco::AbstractMetaObject;
using Poco::NotFoundException;
using Poco::InvalidAccessException;


ClassLoaderTest::ClassLoaderTest(const std::string& name): CppUnit::TestCase(name)
{
}


ClassLoaderTest::~ClassLoaderTest()
{
}


void ClassLoaderTest::testClassLoader1()
{
	std::string path = "TestLibrary";
	path.append(SharedLibrary::suffix());

	ClassLoader<TestPlugin> cl;

	assertTrue (cl.begin() == cl.end());
	assertNullPtr (cl.findClass("PluginA"));
	assertNullPtr (cl.findManifest(path));
	
	assertTrue (!cl.isLibraryLoaded(path));
	
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
		const ClassLoader<TestPlugin>::Manif& POCO_UNUSED manif = cl.manifestFor(path);
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
	std::string path = "TestLibrary";
	path.append(SharedLibrary::suffix());

	ClassLoader<TestPlugin> cl;
	cl.loadLibrary(path);

	assertTrue (cl.begin() != cl.end());
	assertNotNullPtr (cl.findClass("PluginA"));
	assertNotNullPtr (cl.findClass("PluginB"));
	assertNotNullPtr (cl.findClass("PluginC"));
	assertNotNullPtr (cl.findManifest(path));
	
	assertTrue (cl.isLibraryLoaded(path));
	assertTrue (cl.manifestFor(path).size() == 3);
	
	ClassLoader<TestPlugin>::Iterator it = cl.begin();
	assertTrue (it != cl.end());
	assertTrue (it->first == path);
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

	cl.unloadLibrary(path);
}


void ClassLoaderTest::testClassLoader3()
{
	std::string path = "TestLibrary";
	path.append(SharedLibrary::suffix());

	ClassLoader<TestPlugin> cl;
	cl.loadLibrary(path);
	cl.loadLibrary(path);
	cl.unloadLibrary(path);
	
	assertTrue (cl.manifestFor(path).size() == 3);
	
	ClassLoader<TestPlugin>::Iterator it = cl.begin();
	assertTrue (it != cl.end());
	assertTrue (it->first == path);
	assertTrue (it->second->size() == 3);
	++it;
	assertTrue (it == cl.end());
	
	cl.unloadLibrary(path);
	assertNullPtr (cl.findManifest(path));
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
