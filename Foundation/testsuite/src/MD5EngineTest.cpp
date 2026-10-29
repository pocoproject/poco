//
// MD5EngineTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MD5EngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MD5Engine.h"
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
#include <sys/mman.h>
#endif


using Poco::MD5Engine;
using Poco::DigestEngine;


MD5EngineTest::MD5EngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


MD5EngineTest::~MD5EngineTest()
{
}


void MD5EngineTest::testMD5()
{
	MD5Engine engine;

	// test vectors from RFC 1321

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d41d8cd98f00b204e9800998ecf8427e");

	engine.update("a");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "0cc175b9c0f1b6a831c399e269772661");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "900150983cd24fb0d6963f7d28e17f72");

	engine.update("message digest");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "f96b697d7cb7938d525a2f31aaf161d0");

	engine.update("abcdefghijklmnopqrstuvwxyz");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

	engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");

	engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "57edf4a22be3c955ac49da2e2107b67a");
}


void MD5EngineTest::testConstantTimeEquals()
{
	DigestEngine::Digest d1 = DigestEngine::digestFromHex("d41d8cd98f00b204e9800998ecf8427e");
	DigestEngine::Digest d2 = DigestEngine::digestFromHex("d41d8cd98f00b204e9800998ecf8427e");
	DigestEngine::Digest d3 = DigestEngine::digestFromHex("0cc175b9c0f1b6a831c399e269772661");

	assertTrue (DigestEngine::constantTimeEquals(d1, d2));
	assertTrue (!DigestEngine::constantTimeEquals(d1, d3));

	assertTrue (DigestEngine::constantTimeEquals(std::string("d41d8cd98f00b204"), std::string("d41d8cd98f00b204")));
	assertTrue (!DigestEngine::constantTimeEquals(std::string("d41d8cd98f00b204"), std::string("d41d8cd98f00b205")));
	assertTrue (!DigestEngine::constantTimeEquals(std::string("d41d8cd98f00b204"), std::string("d41d8cd98f00b20")));
	assertTrue (DigestEngine::constantTimeEquals(std::string(), std::string()));
}


void MD5EngineTest::testLargeInput()
{
#if (POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID) && defined(POCO_PTR_IS_64_BIT)
	// more than 4 GB of zero bytes in a single update() call; on Linux, reads
	// of a read-only anonymous mapping go to the shared zero page and use no memory
	constexpr std::size_t size = (std::size_t(1) << 32) + 100;
	void* p = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	assertTrue (p != MAP_FAILED);

	MD5Engine engine;
	engine.update(p, size);
	::munmap(p, size);
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "3601846a07f37ff8fbbeed3a1a7999b7");
#endif
}


void MD5EngineTest::setUp()
{
}


void MD5EngineTest::tearDown()
{
}


CppUnit::Test* MD5EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MD5EngineTest");

	CppUnit_addTest(pSuite, MD5EngineTest, testMD5);
	CppUnit_addLongTest(pSuite, MD5EngineTest, testLargeInput);
	CppUnit_addTest(pSuite, MD5EngineTest, testConstantTimeEquals);

	return pSuite;
}
