//
// NameValueCollectionTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NameValueCollectionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Exception.h"


using Poco::Net::NameValueCollection;
using Poco::NotFoundException;


NameValueCollectionTest::NameValueCollectionTest(const std::string& name): CppUnit::TestCase(name)
{
}


NameValueCollectionTest::~NameValueCollectionTest()
{
}


void NameValueCollectionTest::testNameValueCollection()
{
	NameValueCollection nvc;

	assertTrue (nvc.empty());
	assertTrue (nvc.size() == 0);

	nvc.set("name", "value");
	assertTrue (!nvc.empty());
	assertTrue (nvc["name"] == "value");
	assertTrue (nvc["Name"] == "value");

	nvc.set("name2", "value2");
	assertTrue (nvc.get("name2") == "value2");
	assertTrue (nvc.get("NAME2") == "value2");

	assertTrue (nvc.size() == 2);

	try
	{
		std::string value = nvc.get("name3");
		fail("not found - must throw");
	}
	catch (NotFoundException&)
	{
	}

	try
	{
		std::string value = nvc["name3"];
		fail("not found - must throw");
	}
	catch (NotFoundException&)
	{
	}

	assertTrue (nvc.get("name", "default") == "value");
	assertTrue (nvc.get("name3", "default") == "default");

	assertTrue (nvc.has("name"));
	assertTrue (nvc.has("name2"));
	assertTrue (!nvc.has("name3"));

	nvc.add("name3", "value3");
	assertTrue (nvc.get("name3") == "value3");

	nvc.add("name3", "value31");

	NameValueCollection::ConstIterator it = nvc.find("Name3");
	assertTrue (it != nvc.end());
	std::string v1 = it->second;
	assertTrue (it->first == "name3");
	++it;
	assertTrue (it != nvc.end());
	std::string v2 = it->second;
	assertTrue (it->first == "name3");

	assertTrue ((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2 == "value3"));

	nvc.erase("name3");
	assertTrue (!nvc.has("name3"));
	assertTrue (nvc.find("name3") == nvc.end());

	it = nvc.begin();
	assertTrue (it != nvc.end());
	++it;
	assertTrue (it != nvc.end());
	++it;
	assertTrue (it == nvc.end());

	nvc.clear();
	assertTrue (nvc.empty());

	assertTrue (nvc.size() == 0);
}


void NameValueCollectionTest::setUp()
{
}


void NameValueCollectionTest::tearDown()
{
}


CppUnit::Test* NameValueCollectionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NameValueCollectionTest");

	CppUnit_addTest(pSuite, NameValueCollectionTest, testNameValueCollection);

	return pSuite;
}
