//
// SHA3EngineTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/SHA3EngineTest.cpp#1 $
//
// Code of the SHA3EngineTest class.
//
// test vectors from FIPS 180-1
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "SHA3EngineTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/SHA3Engine.h"

using Poco::SHA3Engine;
using Poco::DigestEngine;

SHA3EngineTest::SHA3EngineTest(const std::string& rName): CppUnit::TestCase(rName)
{
}

SHA3EngineTest::~SHA3EngineTest()
{
}

void SHA3EngineTest::testSHA3_224()
{
	SHA3Engine engine(SHA3Engine::SHA3_224);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "e642824c3f8cf24ad09234ee7d3c766fc9a3a5168d0c94ad73b46fdf");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assert(DigestEngine::digestToHex(engine.digest()) == "8a24108b154ada21c9fd5574494479ba5c7e7ab76ef264ead0fcce33");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assert(DigestEngine::digestToHex(engine.digest()) == "543e6868e1666c1a643630df77367ae5a62a85070a51c14cbf665cbc");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assert(DigestEngine::digestToHex(engine.digest()) == "d69335b93325192e516a912e6d19a15cb51c6ed5c15243e7a7fd653c");
}

void SHA3EngineTest::testSHA3_256()
{
	SHA3Engine engine(SHA3Engine::SHA3_256);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assert(DigestEngine::digestToHex(engine.digest()) == "41c0dba2a9d6240849100376a8235e2c82e1b9998a999e21db32dd97496d3376");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assert(DigestEngine::digestToHex(engine.digest()) == "916f6061fe879741ca6469b43971dfdb28b1a32dc36cb3254e812be27aad1d18");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assert(DigestEngine::digestToHex(engine.digest()) == "5c8875ae474a3634ba4fd55ec85bffd661f32aca75c6d699d0cdcb6c115891c1");
}

void SHA3EngineTest::testSHA3_384()
{
	SHA3Engine engine(SHA3Engine::SHA3_384);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "ec01498288516fc926459f58e2c6ad8df9b473cb0fc08c2596da7cf0e49be4b298d88cea927ac7f539f1edf228376d25");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assert(DigestEngine::digestToHex(engine.digest()) == "991c665755eb3a4b6bbdfb75c78a492e8c56a22c5c4d7e429bfdbc32b9d4ad5aa04a1f076e62fea19eef51acd0657c22");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assert(DigestEngine::digestToHex(engine.digest()) == "79407d3b5916b59c3e30b09822974791c313fb9ecc849e406f23592d04f625dc8c709b98b43b3852b337216179aa7fc7");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assert(DigestEngine::digestToHex(engine.digest()) == "eee9e24d78c1855337983451df97c8ad9eedf256c6334f8e948d252d5e0e76847aa0774ddb90a842190d2c558b4b8340");
}

void SHA3EngineTest::testSHA3_512()
{
	SHA3Engine engine(SHA3Engine::SHA3_512);

	engine.update("abc");
	assert(DigestEngine::digestToHex(engine.digest()) == "b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e10e116e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assert(DigestEngine::digestToHex(engine.digest()) == "04a371e84ecfb5b8b77cb48610fca8182dd457ce6f326a0fd3d7ec2f1e91636dee691fbe0c985302ba1b0d8dc78c086346b533b49c030d99a27daf1139d6e75e");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assert(DigestEngine::digestToHex(engine.digest()) == "afebb2ef542e6579c50cad06d2e578f9f8dd6881d7dc824d26360feebf18a4fa73e3261122948efcfd492e74e82e2189ed0fb440d187f382270cb455f21dd185");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assert(DigestEngine::digestToHex(engine.digest()) == "3c3a876da14034ab60627c077bb98f7e120a2a5370212dffb3385a18d4f38859ed311d0a9d5141ce9cc5c66ee689b266a8aa18ace8282a0e0db596c90b0a7b87");
}

void SHA3EngineTest::setUp()
{
}

void SHA3EngineTest::tearDown()
{
}

CppUnit::Test* SHA3EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SHA3EngineTest");

	CppUnit_addTest(pSuite, SHA3EngineTest, testSHA3_224);
	CppUnit_addTest(pSuite, SHA3EngineTest, testSHA3_256);
	CppUnit_addTest(pSuite, SHA3EngineTest, testSHA3_384);
	CppUnit_addTest(pSuite, SHA3EngineTest, testSHA3_512);

	return pSuite;
}
