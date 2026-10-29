//
// SHA1EngineTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SHA1EngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SHA1Engine.h"
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
#include <sys/mman.h>
#endif


using Poco::SHA1Engine;
using Poco::DigestEngine;


SHA1EngineTest::SHA1EngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


SHA1EngineTest::~SHA1EngineTest()
{
}


void SHA1EngineTest::testSHA1()
{
	SHA1Engine engine;

	// test vectors from FIPS 180-1

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "a9993e364706816aba3e25717850c26c9cd0d89d");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}


void SHA1EngineTest::testLargeInput()
{
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
	// more than 4 GB of zero bytes in a single update() call; on Linux, reads
	// of a read-only anonymous mapping go to the shared zero page and use no memory
	constexpr std::size_t size = (std::size_t(1) << 32) + 100;
	void* p = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	assertTrue (p != MAP_FAILED);

	SHA1Engine engine;
	engine.update(p, size);
	::munmap(p, size);
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "ed4e242fbb152330b464d8812afead7ba2e2a07a");
#endif
}


void SHA1EngineTest::setUp()
{
}


void SHA1EngineTest::tearDown()
{
}


CppUnit::Test* SHA1EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SHA1EngineTest");

	CppUnit_addTest(pSuite, SHA1EngineTest, testSHA1);
	CppUnit_addLongTest(pSuite, SHA1EngineTest, testLargeInput);

	return pSuite;
}
