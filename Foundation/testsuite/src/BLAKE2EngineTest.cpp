//
// BLAKE2EngineTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/BLAKE2EngineTest.cpp#1 $
//
// Code of the BLAKE2EngineTest class.
//
// test vectors from RFC 7693
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "BLAKE2EngineTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/BLAKE2Engine.h"

using Poco::BLAKE2Engine;
using Poco::DigestEngine;

BLAKE2EngineTest::BLAKE2EngineTest(const std::string& rName): CppUnit::TestCase(rName)
{
}

BLAKE2EngineTest::~BLAKE2EngineTest()
{
}

void BLAKE2EngineTest::testBLAKE2_224()
{
	BLAKE2Engine engine(BLAKE2Engine::BLAKE2b_224);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "9bd237b02a29e43bdd6738afa5b53ff0eee178d6210b618e4511aec8");
}

void BLAKE2EngineTest::testBLAKE2_256()
{
	BLAKE2Engine engine(BLAKE2Engine::BLAKE2b_256);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "bddd813c634239723171ef3fee98579b94964e3bb1cb3e427262c8c068d52319");
}

void BLAKE2EngineTest::testBLAKE2_384()
{
	BLAKE2Engine engine(BLAKE2Engine::BLAKE2b_384);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "6f56a82c8e7ef526dfe182eb5212f7db9df1317e57815dbda46083fc30f54ee6c66ba83be64b302d7cba6ce15bb556f4");
}

void BLAKE2EngineTest::testBLAKE2_512()
{
	BLAKE2Engine engine(BLAKE2Engine::BLAKE2b_512);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d17d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923");
}

void BLAKE2EngineTest::setUp()
{
}

void BLAKE2EngineTest::tearDown()
{
}

CppUnit::Test* BLAKE2EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BLAKE2EngineTest");

	CppUnit_addTest(pSuite, BLAKE2EngineTest, testBLAKE2_224);
	CppUnit_addTest(pSuite, BLAKE2EngineTest, testBLAKE2_256);
	CppUnit_addTest(pSuite, BLAKE2EngineTest, testBLAKE2_384);
	CppUnit_addTest(pSuite, BLAKE2EngineTest, testBLAKE2_512);

	return pSuite;
}
