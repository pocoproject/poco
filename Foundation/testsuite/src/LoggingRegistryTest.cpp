//
// LoggingRegistryTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LoggingRegistryTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/AutoPtr.h"


using Poco::LoggingRegistry;
using Poco::Channel;
using Poco::ConsoleChannel;
using Poco::Formatter;
using Poco::PatternFormatter;
using Poco::AutoPtr;


LoggingRegistryTest::LoggingRegistryTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


LoggingRegistryTest::~LoggingRegistryTest()
{
}


void LoggingRegistryTest::testRegister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();

	reg.clear();

	Channel::Ptr pC1 = new ConsoleChannel();
	Channel::Ptr pC2 = new ConsoleChannel();
	Formatter::Ptr pF1 = new PatternFormatter("");
	Formatter::Ptr pF2 = new PatternFormatter("");

	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);

	Channel::Ptr pC = reg.channelForName("c1");
	assertTrue (pC1 == pC);
	pC = reg.channelForName("c2");
	assertTrue (pC2 == pC);

	Formatter* pF = reg.formatterForName("f1");
	assertTrue (pF1 == pF);
	pF = reg.formatterForName("f2");
	assertTrue (pF2 == pF);

	try
	{
		pC = reg.channelForName("c3");
		fail("not found - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void LoggingRegistryTest::testReregister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();

	reg.clear();

	Channel::Ptr pC1 = new ConsoleChannel();
	Channel::Ptr pC2 = new ConsoleChannel();
	Channel::Ptr pC1b = new ConsoleChannel();
	AutoPtr<Formatter> pF1 = new PatternFormatter("");
	AutoPtr<Formatter> pF2 = new PatternFormatter("");
	AutoPtr<Formatter> pF1b = new PatternFormatter("");

	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);

	reg.registerChannel("c1", pC1b);

	Channel::Ptr pC = reg.channelForName("c1");
	assertTrue (pC1b == pC);

	pC = reg.channelForName("c2");
	assertTrue (pC2 == pC);

	reg.registerFormatter("f1", pF1b);

	Formatter::Ptr pF = reg.formatterForName("f1");
	assertTrue (pF1b == pF);
	pF = reg.formatterForName("f2");
	assertTrue (pF2 == pF);
}


void LoggingRegistryTest::testUnregister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();

	reg.clear();

	Channel::Ptr pC1 = new ConsoleChannel();
	Channel::Ptr pC2 = new ConsoleChannel();
	AutoPtr<Formatter> pF1 = new PatternFormatter("");
	AutoPtr<Formatter> pF2 = new PatternFormatter("");

	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);

	reg.unregisterChannel("c1");
	reg.unregisterFormatter("f2");

	try
	{
		Channel::Ptr pC = reg.channelForName("c1");
		fail("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}

	try
	{
		Formatter::Ptr pF = reg.formatterForName("f2");
		fail("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void LoggingRegistryTest::setUp()
{
}


void LoggingRegistryTest::tearDown()
{
}


CppUnit::Test* LoggingRegistryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggingRegistryTest");

	CppUnit_addTest(pSuite, LoggingRegistryTest, testRegister);
	CppUnit_addTest(pSuite, LoggingRegistryTest, testReregister);
	CppUnit_addTest(pSuite, LoggingRegistryTest, testUnregister);

	return pSuite;
}
