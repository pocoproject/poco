//
// SessionPoolTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SessionPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SessionPoolContainer.h"
#include "Poco/Thread.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Connector.h"


using namespace Poco::Data::Keywords;
using Poco::Thread;
using Poco::AutoPtr;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::Data::Session;
using Poco::Data::SessionPool;
using Poco::Data::SessionPoolContainer;
using Poco::Data::SessionPoolExhaustedException;
using Poco::Data::SessionPoolExistsException;
using Poco::Data::SessionUnavailableException;


SessionPoolTest::SessionPoolTest(const std::string& name): CppUnit::TestCase(name)
{
	Poco::Data::Test::Connector::addToFactory();
}


SessionPoolTest::~SessionPoolTest()
{
	Poco::Data::Test::Connector::removeFromFactory();
}


void SessionPoolTest::testSessionPool()
{
	SessionPool pool("test", "cs", 1, 4, 2, 10);

	pool.setFeature("f1", true);
	assertTrue (pool.getFeature("f1"));
	try { pool.getFeature("g1"); fail ("must fail"); }
	catch ( Poco::NotFoundException& ) { }

	pool.setProperty("p1", 1);
	assertTrue (1 == Poco::AnyCast<int>(pool.getProperty("p1")));
	try { pool.getProperty("r1"); fail ("must fail"); }
	catch ( Poco::NotFoundException& ) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 4);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());
	Session s1(pool.get());

	assertTrue (s1.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s1.getProperty("p1")));

	try { pool.setFeature("f1", true); fail ("must fail"); }
	catch ( Poco::InvalidAccessException& ) { }

	try { pool.setProperty("p1", 1); fail ("must fail"); }
	catch ( Poco::InvalidAccessException& ) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 1);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 3);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s2(pool.get("f1", false));
	assertTrue (!s2.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s2.getProperty("p1")));

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	{
		Session s3(pool.get("p1", 2));
		assertTrue (s3.getFeature("f1"));
		assertTrue (2 == Poco::AnyCast<int>(s3.getProperty("p1")));

		assertTrue (pool.capacity() == 4);
		assertTrue (pool.allocated() == 3);
		assertTrue (pool.idle() == 0);
		assertTrue (pool.connTimeout() == 10);
		assertTrue (pool.available() == 1);
		assertTrue (pool.dead() == 0);
		assertTrue (pool.allocated() == pool.used() + pool.idle());
	}

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 1);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s4(pool.get());
	assertTrue (s4.getFeature("f1"));
	assertTrue (1 == Poco::AnyCast<int>(s4.getProperty("p1")));

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s5(pool.get());

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 0);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	try
	{
		Session s6(pool.get());
		fail("pool exhausted - must throw");
	}
	catch (SessionPoolExhaustedException&) { }

	s5.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 1);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	try
	{
		s5 << "DROP TABLE IF EXISTS Test", now;
		fail("session unusable - must throw");
	}
	catch (SessionUnavailableException&) { }

	s4.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 4);
	assertTrue (pool.idle() == 2);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Thread::sleep(5000); // time to clean up idle sessions

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	Session s6(pool.get());

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 3);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 1);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	s6.setFeature("connected", false);
	assertTrue (pool.dead() == 1);

	s6.close();
	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 2);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 2);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());

	assertTrue (pool.isActive());
	pool.shutdown();
	assertTrue (!pool.isActive());
	try
	{
		Session s7(pool.get());
		fail("pool shut down - must throw");
	}
	catch (InvalidAccessException&) { }

	assertTrue (pool.capacity() == 4);
	assertTrue (pool.allocated() == 0);
	assertTrue (pool.idle() == 0);
	assertTrue (pool.connTimeout() == 10);
	assertTrue (pool.available() == 0);
	assertTrue (pool.dead() == 0);
	assertTrue (pool.allocated() == pool.used() + pool.idle());
}


void SessionPoolTest::testSessionPoolContainer()
{
	SessionPoolContainer spc;
	AutoPtr<SessionPool> pPool = new SessionPool("TeSt", "Cs");
	spc.add(pPool);
	assertTrue (pPool->isActive());
	assertTrue (spc.isActive("test", "cs"));
	assertTrue (spc.isActive("test:///cs"));
	assertTrue (spc.has("test:///cs"));
	assertTrue (1 == spc.count());

	Poco::Data::Session sess = spc.get("test:///cs");
	assertTrue ("test" == sess.impl()->connectorName());
	assertTrue ("Cs" == sess.impl()->connectionString());
	assertTrue ("test:///Cs" == sess.uri());

	try { spc.add(pPool); fail ("must fail"); }
	catch (SessionPoolExistsException&) { }
	pPool->shutdown();
	assertTrue (!pPool->isActive());
	assertTrue (!spc.isActive("test", "cs"));
	assertTrue (!spc.isActive("test:///cs"));
	spc.remove(pPool->name());
	assertTrue (!spc.has("test:///cs"));
	assertTrue (!spc.isActive("test", "cs"));
	assertTrue (!spc.isActive("test:///cs"));
	assertTrue (0 == spc.count());
	try { spc.get("test"); fail ("must fail"); }
	catch (NotFoundException&) { }

	spc.add("tEsT", "cs");
	spc.add("TeSt", "cs");//duplicate request, must be silently ignored
	assertTrue (1 == spc.count());
	spc.remove("TesT:///cs");
	assertTrue (0 == spc.count());
	try { spc.get("test"); fail ("must fail"); }
	catch (NotFoundException&) { }
}


void SessionPoolTest::setUp()
{
}


void SessionPoolTest::tearDown()
{
}


CppUnit::Test* SessionPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SessionPoolTest");

	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPool);
	CppUnit_addTest(pSuite, SessionPoolTest, testSessionPoolContainer);

	return pSuite;
}
