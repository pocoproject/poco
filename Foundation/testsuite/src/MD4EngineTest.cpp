//
// MD4EngineTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MD4EngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MD4Engine.h"
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
#include <sys/mman.h>
#endif


using Poco::MD4Engine;
using Poco::DigestEngine;


MD4EngineTest::MD4EngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


MD4EngineTest::~MD4EngineTest()
{
}


void MD4EngineTest::testMD4()
{
	MD4Engine engine;

	// test vectors from RFC 1320

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "31d6cfe0d16ae931b73c59d7e0c089c0");

	engine.update("a");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "bde52cb31de33e46245e05fbdbd6fb24");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "a448017aaf21d8525fc10ae87aa6729d");

	engine.update("message digest");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d9130a8164549fe818874806e1c7014b");

	engine.update("abcdefghijklmnopqrstuvwxyz");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d79e1c308aa5bbcdeea8ed63df412da9");

	engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "043f8582f241db351ce627e153e7f0e4");

	engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "e33b4ddc9c38f2199c3e7b164fcc0536");
}


void MD4EngineTest::testLargeInput()
{
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
	// more than 4 GB of zero bytes in a single update() call; on Linux, reads
	// of a read-only anonymous mapping go to the shared zero page and use no memory
	const std::size_t size = (std::size_t(1) << 32) + 100;
	void* p = mmap(nullptr, size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	assertTrue (p != MAP_FAILED);

	MD4Engine engine;
	engine.update(p, size);
	munmap(p, size);
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "8f1366bdef03f801eef254a579f617b6");
#endif
}


void MD4EngineTest::setUp()
{
}


void MD4EngineTest::tearDown()
{
}


CppUnit::Test* MD4EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MD4EngineTest");

	CppUnit_addTest(pSuite, MD4EngineTest, testMD4);
	CppUnit_addLongTest(pSuite, MD4EngineTest, testLargeInput);

	return pSuite;
}
