//
// HMACEngineTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HMACEngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA2Engine.h"
#include "Poco/Exception.h"


using Poco::HMACEngine;
using Poco::MD5Engine;
using Poco::SHA2Engine224;
using Poco::SHA2Engine256;
using Poco::SHA2Engine384;
using Poco::SHA2Engine512;
using Poco::DigestEngine;


HMACEngineTest::HMACEngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


HMACEngineTest::~HMACEngineTest()
{
}


void HMACEngineTest::testHMAC_MD5()
{
	// test vectors from RFC 2104

	std::string key(16, 0x0b);
	std::string data("Hi There");
	HMACEngine<MD5Engine> hmac1(key);
	hmac1.update(data);
	std::string digest = DigestEngine::digestToHex(hmac1.digest());
	assertTrue (digest == "9294727a3638bb1c13f48ef8158bfc9d");

	key  = "Jefe";
	data = "what do ya want for nothing?";
	HMACEngine<MD5Engine> hmac2(key);
	hmac2.update(data);
	digest = DigestEngine::digestToHex(hmac2.digest());
	assertTrue (digest == "750c783e6ab0b503eaa86e310a5db738");

	key  = std::string(16, std::string::value_type(0xaa));
	data = std::string(50, std::string::value_type(0xdd));
	HMACEngine<MD5Engine> hmac3(key);
	hmac3.update(data);
	digest = DigestEngine::digestToHex(hmac3.digest());
	assertTrue (digest == "56be34521d144c88dbb8c733f0e8b3f6");
}


void HMACEngineTest::testHMAC_SHA2_224()
{
	// test vectors from RFC 4231

	std::string key(20, 0x0b);
	std::string data("Hi There");
	HMACEngine<SHA2Engine224> hmac1(key);
	hmac1.update(data);
	std::string digest = DigestEngine::digestToHex(hmac1.digest());
	assertTrue (digest == "896fb1128abbdf196832107cd49df33f47b4b1169912ba4f53684b22");

	key  = "Jefe";
	data = "what do ya want for nothing?";
	HMACEngine<SHA2Engine224> hmac2(key);
	hmac2.update(data);
	digest = DigestEngine::digestToHex(hmac2.digest());
	assertTrue (digest == "a30e01098bc6dbbf45690f3a7e9e6d0f8bbea2a39e6148008fd05e44");

	key  = std::string(20, std::string::value_type(0xaa));
	data = std::string(50, std::string::value_type(0xdd));
	HMACEngine<SHA2Engine224> hmac3(key);
	hmac3.update(data);
	digest = DigestEngine::digestToHex(hmac3.digest());
	assertTrue (digest == "7fb3cb3588c6c1f6ffa9694d7d6ad2649365b0c1f65d69d1ec8333ea");

	std::vector<char> str = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
	};
	key  = std::string(str.begin(), str.end()); // 25 bytes
	data = std::string(50, (char)0xcd);
	HMACEngine<SHA2Engine224> hmac4(key);
	hmac4.update(data);
	digest = DigestEngine::digestToHex(hmac4.digest());
	assertTrue (digest == "6c11506874013cac6a2abc1bb382627cec6a90d86efc012de7afec5a");

	key  = std::string(20, std::string::value_type(0x0c));
	data = std::string("Test With Truncation");
	HMACEngine<SHA2Engine224> hmac5(key);
	hmac5.update(data);
	digest = DigestEngine::digestToHex(hmac5.digest(), 16);
	assertTrue (digest == "0e2aea68a90c8d37c988bcdb9fca6fa8");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("Test Using Larger Than Block-Size Key - Hash Key First");
	HMACEngine<SHA2Engine224> hmac6(key);
	hmac6.update(data);
	digest = DigestEngine::digestToHex(hmac6.digest());
	assertTrue (digest == "95e9a0db962095adaebe9b2d6f0dbce2d499f112f2d2b7273fa6870e");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("This is a test using a larger than block-size key "
		"and a larger than block-size data. The key needs to be hashed "
		"before being used by the HMAC algorithm.");
	HMACEngine<SHA2Engine224> hmac7(key);
	hmac7.update(data);
	digest = DigestEngine::digestToHex(hmac7.digest());
	assertTrue (digest == "3a854166ac5d9f023f54d517d0b39dbd946770db9c2b95c9f6f565d1");

	HMACEngine<SHA2Engine224> hmac8(key);
	hmac8.update(data);
	try
	{
		const DigestEngine::Digest& d = hmac8.digest();
		digest = DigestEngine::digestToHex(d, d.size()+1);
		fail("Invalid digest length, must throw.");
	}
	catch(const Poco::InvalidArgumentException&) {}
}


void HMACEngineTest::testHMAC_SHA2_256()
{
	// test vectors from RFC 4231

	std::string key(20, 0x0b);
	std::string data("Hi There");
	HMACEngine<SHA2Engine256> hmac1(key);
	hmac1.update(data);
	std::string digest = DigestEngine::digestToHex(hmac1.digest());
	assertTrue (digest == "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7");

	key  = "Jefe";
	data = "what do ya want for nothing?";
	HMACEngine<SHA2Engine256> hmac2(key);
	hmac2.update(data);
	digest = DigestEngine::digestToHex(hmac2.digest());
	assertTrue (digest == "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");

	key  = std::string(20, std::string::value_type(0xaa));
	data = std::string(50, std::string::value_type(0xdd));
	HMACEngine<SHA2Engine256> hmac3(key);
	hmac3.update(data);
	digest = DigestEngine::digestToHex(hmac3.digest());
	assertTrue (digest == "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe");

	std::vector<char> str = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
	};
	key  = std::string(str.begin(), str.end()); // 25 bytes
	data = std::string(50, (char)0xcd);
	HMACEngine<SHA2Engine256> hmac4(key);
	hmac4.update(data);
	digest = DigestEngine::digestToHex(hmac4.digest());
	assertTrue (digest == "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b");

	key  = std::string(20, std::string::value_type(0x0c));
	data = std::string("Test With Truncation");
	HMACEngine<SHA2Engine256> hmac5(key);
	hmac5.update(data);
	digest = DigestEngine::digestToHex(hmac5.digest(), 16);
	assertTrue (digest == "a3b6167473100ee06e0c796c2955552b");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("Test Using Larger Than Block-Size Key - Hash Key First");
	HMACEngine<SHA2Engine256> hmac6(key);
	hmac6.update(data);
	digest = DigestEngine::digestToHex(hmac6.digest());
	assertTrue (digest == "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("This is a test using a larger than block-size key "
		"and a larger than block-size data. The key needs to be hashed "
		"before being used by the HMAC algorithm.");
	HMACEngine<SHA2Engine256> hmac7(key);
	hmac7.update(data);
	digest = DigestEngine::digestToHex(hmac7.digest());
	assertTrue (digest == "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2");

	HMACEngine<SHA2Engine256> hmac8(key);
	hmac8.update(data);
	try
	{
		const DigestEngine::Digest& d = hmac8.digest();
		digest = DigestEngine::digestToHex(d, d.size()+1);
		fail("Invalid digest length, must throw.");
	}
	catch(const Poco::InvalidArgumentException&) {}
}


void HMACEngineTest::testHMAC_SHA2_384()
{
	// test vectors from RFC 4231

	std::string key(20, 0x0b);
	std::string data("Hi There");
	HMACEngine<SHA2Engine384> hmac1(key);
	hmac1.update(data);
	std::string digest = DigestEngine::digestToHex(hmac1.digest());
	assertTrue (digest == "afd03944d84895626b0825f4ab46907f"
		"15f9dadbe4101ec682aa034c7cebc59c"
		"faea9ea9076ede7f4af152e8b2fa9cb6");

	key  = "Jefe";
	data = "what do ya want for nothing?";
	HMACEngine<SHA2Engine384> hmac2(key);
	hmac2.update(data);
	digest = DigestEngine::digestToHex(hmac2.digest());
	assertTrue (digest == "af45d2e376484031617f78d2b58a6b1b"
		"9c7ef464f5a01b47e42ec3736322445e"
		"8e2240ca5e69e2c78b3239ecfab21649");

	key  = std::string(20, std::string::value_type(0xaa));
	data = std::string(50, std::string::value_type(0xdd));
	HMACEngine<SHA2Engine384> hmac3(key);
	hmac3.update(data);
	digest = DigestEngine::digestToHex(hmac3.digest());
	assertTrue (digest == "88062608d3e6ad8a0aa2ace014c8a86f"
		"0aa635d947ac9febe83ef4e55966144b"
		"2a5ab39dc13814b94e3ab6e101a34f27");

	std::vector<char> str = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
	};
	key  = std::string(str.begin(), str.end()); // 25 bytes
	data = std::string(50, (char)0xcd);
	HMACEngine<SHA2Engine384> hmac4(key);
	hmac4.update(data);
	digest = DigestEngine::digestToHex(hmac4.digest());
	assertTrue (digest == "3e8a69b7783c25851933ab6290af6ca7"
		"7a9981480850009cc5577c6e1f573b4e"
		"6801dd23c4a7d679ccf8a386c674cffb");

	key  = std::string(20, std::string::value_type(0x0c));
	data = std::string("Test With Truncation");
	HMACEngine<SHA2Engine384> hmac5(key);
	hmac5.update(data);
	digest = DigestEngine::digestToHex(hmac5.digest(), 16);
	assertTrue (digest == "3abf34c3503b2a23a46efc619baef897");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("Test Using Larger Than Block-Size Key - Hash Key First");
	HMACEngine<SHA2Engine384> hmac6(key);
	hmac6.update(data);
	digest = DigestEngine::digestToHex(hmac6.digest());
	assertTrue (digest == "4ece084485813e9088d2c63a041bc5b4"
                  "4f9ef1012a2b588f3cd11f05033ac4c6"
                  "0c2ef6ab4030fe8296248df163f44952");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("This is a test using a larger than block-size key "
		"and a larger than block-size data. The key needs to be hashed "
		"before being used by the HMAC algorithm.");
	HMACEngine<SHA2Engine384> hmac7(key);
	hmac7.update(data);
	digest = DigestEngine::digestToHex(hmac7.digest());
	assertTrue (digest == "6617178e941f020d351e2f254e8fd32c"
		"602420feb0b8fb9adccebb82461e99c5"
		"a678cc31e799176d3860e6110c46523e");

	HMACEngine<SHA2Engine384> hmac8(key);
	hmac8.update(data);
	try
	{
		const DigestEngine::Digest& d = hmac8.digest();
		digest = DigestEngine::digestToHex(d, d.size()+1);
		fail("Invalid digest length, must throw.");
	}
	catch(const Poco::InvalidArgumentException&) {}
}


void HMACEngineTest::testHMAC_SHA2_512()
{
	// test vectors from RFC 4231

	std::string key(20, 0x0b);
	std::string data("Hi There");
	HMACEngine<SHA2Engine512> hmac1(key);
	hmac1.update(data);
	std::string digest = DigestEngine::digestToHex(hmac1.digest());
	assertTrue (digest == "87aa7cdea5ef619d4ff0b4241a1d6cb0"
		"2379f4e2ce4ec2787ad0b30545e17cde"
		"daa833b7d6b8a702038b274eaea3f4e4"
		"be9d914eeb61f1702e696c203a126854");

	key  = "Jefe";
	data = "what do ya want for nothing?";
	HMACEngine<SHA2Engine512> hmac2(key);
	hmac2.update(data);
	digest = DigestEngine::digestToHex(hmac2.digest());
	assertTrue (digest == "164b7a7bfcf819e2e395fbe73b56e0a3"
		"87bd64222e831fd610270cd7ea250554"
		"9758bf75c05a994a6d034f65f8f0e6fd"
		"caeab1a34d4a6b4b636e070a38bce737");

	key  = std::string(20, std::string::value_type(0xaa));
	data = std::string(50, std::string::value_type(0xdd));
	HMACEngine<SHA2Engine512> hmac3(key);
	hmac3.update(data);
	digest = DigestEngine::digestToHex(hmac3.digest());
	assertTrue (digest == "fa73b0089d56a284efb0f0756c890be9"
		"b1b5dbdd8ee81a3655f83e33b2279d39"
		"bf3e848279a722c806b485a47e67c807"
		"b946a337bee8942674278859e13292fb");

	std::vector<char> str = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19
	};
	key  = std::string(str.begin(), str.end()); // 25 bytes
	data = std::string(50, (char)0xcd);
	HMACEngine<SHA2Engine512> hmac4(key);
	hmac4.update(data);
	digest = DigestEngine::digestToHex(hmac4.digest());
	assertTrue (digest == "b0ba465637458c6990e5a8c5f61d4af7"
		"e576d97ff94b872de76f8050361ee3db"
		"a91ca5c11aa25eb4d679275cc5788063"
		"a5f19741120c4f2de2adebeb10a298dd");

	key  = std::string(20, std::string::value_type(0x0c));
	data = std::string("Test With Truncation");
	HMACEngine<SHA2Engine512> hmac5(key);
	hmac5.update(data);
	digest = DigestEngine::digestToHex(hmac5.digest(), 16);
	assertTrue (digest == "415fad6271580a531d4179bc891d87a6");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("Test Using Larger Than Block-Size Key - Hash Key First");
	HMACEngine<SHA2Engine512> hmac6(key);
	hmac6.update(data);
	digest = DigestEngine::digestToHex(hmac6.digest());
	assertTrue (digest == "80b24263c7c1a3ebb71493c1dd7be8b4"
                  "9b46d1f41b4aeec1121b013783f8f352"
                  "6b56d037e05f2598bd0fd2215d6a1e52"
                  "95e64f73f63f0aec8b915a985d786598");

	key  = std::string(131, std::string::value_type(0xaa));
	data = std::string("This is a test using a larger than block-size key "
		"and a larger than block-size data. The key needs to be hashed "
		"before being used by the HMAC algorithm.");
	HMACEngine<SHA2Engine512> hmac7(key);
	hmac7.update(data);
	digest = DigestEngine::digestToHex(hmac7.digest());
	assertTrue (digest == "e37b6a775dc87dbaa4dfa9f96e5e3ffd"
		"debd71f8867289865df5a32d20cdc944"
		"b6022cac3c4982b10d5eeb55c3e4de15"
		"134676fb6de0446065c97440fa8c6a58");

	HMACEngine<SHA2Engine512> hmac8(key);
	hmac8.update(data);
	try
	{
		const DigestEngine::Digest& d = hmac8.digest();
		digest = DigestEngine::digestToHex(d, d.size()+1);
		fail("Invalid digest length, must throw.");
	}
	catch(const Poco::InvalidArgumentException&) {}
}


void HMACEngineTest::setUp()
{
}


void HMACEngineTest::tearDown()
{
}


CppUnit::Test* HMACEngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HMACEngineTest");

	CppUnit_addTest(pSuite, HMACEngineTest, testHMAC_MD5);
	CppUnit_addTest(pSuite, HMACEngineTest, testHMAC_SHA2_224);
	CppUnit_addTest(pSuite, HMACEngineTest, testHMAC_SHA2_256);
	CppUnit_addTest(pSuite, HMACEngineTest, testHMAC_SHA2_384);
	CppUnit_addTest(pSuite, HMACEngineTest, testHMAC_SHA2_512);

	return pSuite;
}
