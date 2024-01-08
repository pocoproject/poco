//
// SHA2EngineTest.cpp
//
// Code of the SHA2EngineTest class.
//
// test vectors from FIPS 180-1
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SHA2EngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SHA2Engine.h"


using Poco::SHA2Engine;
using Poco::DigestEngine;


SHA2EngineTest::SHA2EngineTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


SHA2EngineTest::~SHA2EngineTest()
{
}


void SHA2EngineTest::testSHA224()
{
#if defined(__sparc_v9__) || defined(__ppc64__) || defined(__powerpc__) || defined(__s390x__) || defined(__hppa__)
	return;
#endif
	SHA2Engine engine(SHA2Engine::SHA_224);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67");
}


void SHA2EngineTest::testSHA256()
{
#if defined(__sparc_v9__) || defined(__ppc64__) || defined(__powerpc__) || defined(__s390x__) || defined(__hppa__)
	return;
#endif
	SHA2Engine engine(SHA2Engine::SHA_256);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
}


void SHA2EngineTest::testSHA384()
{
#if defined(__sparc_v9__) || defined(__ppc64__) || defined(__powerpc__) || defined(__s390x__) || defined(__hppa__)
	return;
#endif
	SHA2Engine engine(SHA2Engine::SHA_384);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985");
}


void SHA2EngineTest::testSHA512()
{
#if defined(__sparc_v9__) || defined(__ppc64__) || defined(__powerpc__) || defined(__s390x__) || defined(__hppa__)
	return;
#endif
	SHA2Engine engine(SHA2Engine::SHA_512);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b");
}

void SHA2EngineTest::testSHA512_224()
{
	SHA2Engine engine(SHA2Engine::SHA_512_224);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "4634270f707b6a54daae7530460842e20e37ed265ceee9a43e8924aa");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "e5302d6d54bb242275d1e7622d68df6eb02dedd13f564c13dbda2174");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "23fec5bb94d60b23308192640b0c453335d664734fe40e7268674af9");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "37ab331d76f0d36de422bd0edeb22a28accd487b7a8453ae965dd287");
}

void SHA2EngineTest::testSHA512_256()
{
	SHA2Engine engine(SHA2Engine::SHA_512_256);

	engine.update("");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a");

	engine.update("abc");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "53048e2681941ef99b2e29b76b4c7dabe4c2d0c634fc6d46e0e2f13107e7af23");

	engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "bde8e1f9f19bb9fd3406c90ec6bc47bd36d8ada9f11880dbc8a22a7078b6a461");

	engine.update("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "3928e184fb8690f840da3988121d31be65cb9d3ef83ee6146feac861e19b563a");

	for (int i = 0; i < 1000000; ++i)
		engine.update('a');
	assertTrue (DigestEngine::digestToHex(engine.digest()) == "9a59a052930187a97038cae692f30708aa6491923ef5194394dc68d56c74fb21");
}

void SHA2EngineTest::setUp()
{
}


void SHA2EngineTest::tearDown()
{
}


CppUnit::Test* SHA2EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SHA2EngineTest");

	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA224);
	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA256);
	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA384);
	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA512);
	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA512_224);
	CppUnit_addTest(pSuite, SHA2EngineTest, testSHA512_256);
	return pSuite;
}
