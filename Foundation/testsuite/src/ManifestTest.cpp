//
// ManifestTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ManifestTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Manifest.h"
#include "Poco/MetaObject.h"
#include <set>


using Poco::Manifest;
using Poco::MetaObject;


class MfTestBase
{
};


class MfTestObject: public MfTestBase
{
};


ManifestTest::ManifestTest(const std::string& name): CppUnit::TestCase(name)
{
}


ManifestTest::~ManifestTest()
{
}


void ManifestTest::testManifest()
{
	Manifest<MfTestBase> manifest;
	assertTrue (manifest.empty());
	assertTrue (manifest.size() == 0);
	assertTrue (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject1")));
	assertTrue (!manifest.empty());
	assertTrue (manifest.size() == 1);
	assertTrue (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject2")));
	MetaObject<MfTestObject, MfTestBase>* pMeta = new MetaObject<MfTestObject, MfTestBase>("MfTestObject2");
	assertTrue (!manifest.insert(pMeta));
	delete pMeta;
	assertTrue (!manifest.empty());
	assertTrue (manifest.size() == 2);
	assertTrue (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject3")));
	assertTrue (manifest.size() == 3);

	assertTrue (manifest.find("MfTestObject1") != manifest.end());
	assertTrue (manifest.find("MfTestObject2") != manifest.end());
	assertTrue (manifest.find("MfTestObject3") != manifest.end());
	assertTrue (manifest.find("MfTestObject4") == manifest.end());
	
	std::set<std::string> classes;
	
	Manifest<MfTestBase>::Iterator it = manifest.begin();
	assertTrue (it != manifest.end());
	classes.insert(it->name());
	++it;
	assertTrue (it != manifest.end());
	classes.insert(it->name());
	++it;
	assertTrue (it != manifest.end());
	classes.insert(it->name());
	it++;
	assertTrue (it == manifest.end());
	
	assertTrue (classes.find("MfTestObject1") != classes.end());
	assertTrue (classes.find("MfTestObject2") != classes.end());
	assertTrue (classes.find("MfTestObject3") != classes.end());
	
	manifest.clear();
	assertTrue (manifest.empty());
	assertTrue (manifest.size() == 0);
	assertTrue (manifest.insert(new MetaObject<MfTestObject, MfTestBase>("MfTestObject4")));
	assertTrue (!manifest.empty());
	assertTrue (manifest.size() == 1);
	it = manifest.begin();
	assertTrue (it != manifest.end());
	assertTrue (std::string(it->name()) == "MfTestObject4");
	++it;
	assertTrue (it == manifest.end());
}


void ManifestTest::setUp()
{
}


void ManifestTest::tearDown()
{
}


CppUnit::Test* ManifestTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ManifestTest");

	CppUnit_addTest(pSuite, ManifestTest, testManifest);

	return pSuite;
}
