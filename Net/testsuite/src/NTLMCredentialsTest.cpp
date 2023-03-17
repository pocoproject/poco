//
// NTLMCredentialsTest.cpp
//
// Copyright (c) 2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NTLMCredentialsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/NTLMCredentials.h"
#include "Poco/DigestEngine.h"


using Poco::Net::NTLMCredentials;


NTLMCredentialsTest::NTLMCredentialsTest(const std::string& name): CppUnit::TestCase(name)
{
}


NTLMCredentialsTest::~NTLMCredentialsTest()
{
}


void NTLMCredentialsTest::testNonce()
{
	std::vector<unsigned char> nonce1 = NTLMCredentials::createNonce();
	assertTrue (nonce1.size() == 8);

	std::vector<unsigned char> nonce2 = NTLMCredentials::createNonce();
	assertTrue (nonce2.size() == 8);

	assertTrue (nonce1 != nonce2);
}


void NTLMCredentialsTest::testPasswordHash()
{
	std::vector<unsigned char> passHash = NTLMCredentials::createPasswordHash("SecREt01");
	std::string passHashHex = Poco::DigestEngine::digestToHex(passHash);
	assertTrue (passHashHex == "cd06ca7c7e10c99b1d33b7485a2ed808");
}


void NTLMCredentialsTest::testNTLMv2Hash()
{
	std::vector<unsigned char> ntlm2Hash = NTLMCredentials::createNTLMv2Hash("user", "DOMAIN", "SecREt01");
	std::string ntlm2HashHex = Poco::DigestEngine::digestToHex(ntlm2Hash);
	assertTrue (ntlm2HashHex == "04b8e0ba74289cc540826bab1dee63ae");
}


void NTLMCredentialsTest::testLMv2Response()
{
	static const unsigned char CHALLENGE[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	static const unsigned char NONCE[] = {0xff, 0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44};

	std::vector<unsigned char> challenge(CHALLENGE, CHALLENGE + 8);
	std::vector<unsigned char> nonce(NONCE, NONCE + 8);

	std::vector<unsigned char> ntlm2Hash = NTLMCredentials::createNTLMv2Hash("user", "DOMAIN", "SecREt01");
	std::vector<unsigned char> lm2Response = NTLMCredentials::createLMv2Response(ntlm2Hash, challenge, nonce);

	std::string lm2ResponseHex = Poco::DigestEngine::digestToHex(lm2Response);
	assertTrue (lm2ResponseHex == "d6e6152ea25d03b7c6ba6629c2d6aaf0ffffff0011223344");
}


void NTLMCredentialsTest::testNTLMv2Response()
{
	static const unsigned char CHALLENGE[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	static const unsigned char NONCE[] = {0xff, 0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44};
	static const unsigned char TARGET_INFO[] = {
		0x02, 0x00, 0x0c, 0x00, 0x44, 0x00, 0x4f, 0x00,
		0x4d, 0x00, 0x41, 0x00, 0x49, 0x00, 0x4e, 0x00,
		0x01, 0x00, 0x0c, 0x00, 0x53, 0x00, 0x45, 0x00,
		0x52, 0x00, 0x56, 0x00, 0x45, 0x00, 0x52, 0x00,
		0x04, 0x00, 0x14, 0x00, 0x64, 0x00, 0x6f, 0x00,
		0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00,
		0x2e, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00,
		0x03, 0x00, 0x22, 0x00, 0x73, 0x00, 0x65, 0x00,
		0x72, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00,
		0x2e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x6d, 0x00,
		0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x2e, 0x00,
		0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x00, 0x00,
		0x00, 0x00
	};

	std::vector<unsigned char> challenge(CHALLENGE, CHALLENGE + 8);
	std::vector<unsigned char> nonce(NONCE, NONCE + 8);
	std::vector<unsigned char> targetInfo(TARGET_INFO, TARGET_INFO + sizeof(TARGET_INFO));

	std::vector<unsigned char> ntlm2Hash = NTLMCredentials::createNTLMv2Hash("user", "DOMAIN", "SecREt01");
	Poco::UInt64 timestamp = Poco::UInt64(12700317600)*10000000;

	std::vector<unsigned char> ntlm2Response = NTLMCredentials::createNTLMv2Response(
		ntlm2Hash,
		challenge,
		nonce,
		targetInfo,
		timestamp);

	std::string ntlm2ResponseHex = Poco::DigestEngine::digestToHex(ntlm2Response);

	assertTrue (ntlm2ResponseHex ==
		"cbabbca713eb795d04c97abc01ee4983"
		"01010000000000000090d336b734c301"
		"ffffff00112233440000000002000c00"
		"44004f004d00410049004e0001000c00"
		"53004500520056004500520004001400"
		"64006f006d00610069006e002e006300"
		"6f006d00030022007300650072007600"
		"650072002e0064006f006d0061006900"
		"6e002e0063006f006d00000000000000"
		"0000"
	);
}


void NTLMCredentialsTest::testFormatNegotiateMessage()
{
	NTLMCredentials::NegotiateMessage msg1;
	msg1.flags = 0;

	std::vector<unsigned char> msg1Buffer = NTLMCredentials::formatNegotiateMessage(msg1);
	std::string msg1BufferHex = Poco::DigestEngine::digestToHex(msg1Buffer);

	assertTrue (msg1BufferHex == "4e544c4d53535000010000000582080000000000180000000000000018000000");

	msg1.domain = "DOMAIN";
	msg1Buffer = NTLMCredentials::formatNegotiateMessage(msg1);
	msg1BufferHex = Poco::DigestEngine::digestToHex(msg1Buffer);

	assertTrue (msg1BufferHex == "4e544c4d5353500001000000059208000c000c0018000000000000002400000044004f004d00410049004e00");

	msg1.workstation = "WORKST";
	msg1Buffer = NTLMCredentials::formatNegotiateMessage(msg1);
	msg1BufferHex = Poco::DigestEngine::digestToHex(msg1Buffer);

	assertTrue (msg1BufferHex == "4e544c4d535350000100000005b208000c000c00180000000c000c002400000044004f004d00410049004e0057004f0052004b0053005400");
}


void  NTLMCredentialsTest::testParseChallengeMessage()
{
	const unsigned char BUFFER[] = {
		0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00,
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
	};

	NTLMCredentials::ChallengeMessage msg2;
	bool ok = NTLMCredentials::parseChallengeMessage(BUFFER, sizeof(BUFFER), msg2);

	assertTrue (ok);
	assertTrue (msg2.flags == (NTLMCredentials::NTLM_FLAG_NEGOTIATE_OEM | NTLMCredentials::NTLM_FLAG_NEGOTIATE_NTLM));
	assertTrue (msg2.challenge.size() == 8);
	assertTrue (msg2.targetInfo.empty());

	assertTrue (msg2.challenge[0] == 0x01);
	assertTrue (msg2.challenge[1] == 0x23);
	assertTrue (msg2.challenge[7] == 0xef);
}


void  NTLMCredentialsTest::testParseChallengeMessageWithTargetInfo()
{
	const unsigned char BUFFER[] = {
		0x4e, 0x54, 0x4c, 0x4d, 0x53, 0x53, 0x50, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00,
		0x30, 0x00, 0x00, 0x00, 0x01, 0x02, 0x81, 0x00,
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x62, 0x00, 0x62, 0x00, 0x3c, 0x00, 0x00, 0x00,
		0x44, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x41, 0x00,
		0x49, 0x00, 0x4e, 0x00, 0x02, 0x00, 0x0c, 0x00,
		0x44, 0x00, 0x4f, 0x00, 0x4d, 0x00, 0x41, 0x00,
		0x49, 0x00, 0x4e, 0x00, 0x01, 0x00, 0x0c, 0x00,
		0x53, 0x00, 0x45, 0x00, 0x52, 0x00, 0x56, 0x00,
		0x45, 0x00, 0x52, 0x00, 0x04, 0x00, 0x14, 0x00,
		0x64, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00,
		0x69, 0x00, 0x6e, 0x00, 0x2e, 0x00, 0x63, 0x00,
		0x6f, 0x00, 0x6d, 0x00, 0x03, 0x00, 0x22, 0x00,
		0x73, 0x00, 0x65, 0x00, 0x72, 0x00, 0x76, 0x00,
		0x65, 0x00, 0x72, 0x00, 0x2e, 0x00, 0x64, 0x00,
		0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00,
		0x6e, 0x00, 0x2e, 0x00, 0x63, 0x00, 0x6f, 0x00,
		0x6d, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	NTLMCredentials::ChallengeMessage msg2;
	bool ok = NTLMCredentials::parseChallengeMessage(BUFFER, sizeof(BUFFER), msg2);

	assertTrue (ok);
	assertTrue (msg2.flags == (NTLMCredentials::NTLM_FLAG_NEGOTIATE_UNICODE | NTLMCredentials::NTLM_FLAG_NEGOTIATE_NTLM | NTLMCredentials::NTLM_FLAG_NEGOTIATE_TARGET | NTLMCredentials::NTLM_FLAG_TARGET_DOMAIN));
	assertTrue (msg2.challenge.size() == 8);
	assertTrue (msg2.target == "DOMAIN");

	assertTrue (msg2.targetInfo.size() == 98);

	assertTrue (msg2.challenge[0] == 0x01);
	assertTrue (msg2.challenge[1] == 0x23);
	assertTrue (msg2.challenge[7] == 0xef);

	assertTrue (msg2.targetInfo[0] == 0x02);
	assertTrue (msg2.targetInfo[1] == 0x00);
	assertTrue (msg2.targetInfo[97] == 0x00);
}


void NTLMCredentialsTest::testFormatAuthenticateMessage()
{
	const unsigned char LM[] = {
		0xc3, 0x37, 0xcd, 0x5c, 0xbd, 0x44, 0xfc, 0x97,
		0x82, 0xa6, 0x67, 0xaf, 0x6d, 0x42, 0x7c, 0x6d,
		0xe6, 0x7c, 0x20, 0xc2, 0xd3, 0xe7, 0x7c, 0x56
	};
	const unsigned char NTLM[] = {
		0x25, 0xa9, 0x8c, 0x1c, 0x31, 0xe8, 0x18, 0x47,
		0x46, 0x6b, 0x29, 0xb2, 0xdf, 0x46, 0x80, 0xf3,
		0x99, 0x58, 0xfb, 0x8c, 0x21, 0x3a, 0x9c, 0xc6
	};

	NTLMCredentials::AuthenticateMessage msg3;
	msg3.flags = NTLMCredentials::NTLM_FLAG_NEGOTIATE_UNICODE | NTLMCredentials::NTLM_FLAG_NEGOTIATE_NTLM;
	msg3.target = "DOMAIN";
	msg3.username = "user";
	msg3.workstation = "WORKSTATION";
	msg3.lmResponse.assign(LM, LM + sizeof(LM));
	msg3.ntlmResponse.assign(NTLM, NTLM + sizeof(NTLM));

	std::vector<unsigned char> msg3Buffer = NTLMCredentials::formatAuthenticateMessage(msg3);
	std::string msg3BufferHex = Poco::DigestEngine::digestToHex(msg3Buffer);

	assertTrue (msg3BufferHex ==
		"4e544c4d5353500003000000180018004000000018001800"
		"580000000c000c0070000000080008007c00000016001600"
		"84000000000000009a00000001020000c337cd5cbd44fc97"
		"82a667af6d427c6de67c20c2d3e77c5625a98c1c31e81847"
		"466b29b2df4680f39958fb8c213a9cc644004f004d004100"
		"49004e00750073006500720057004f0052004b0053005400"
		"4100540049004f004e00"
	);
}


void NTLMCredentialsTest::setUp()
{
}


void NTLMCredentialsTest::tearDown()
{
}


CppUnit::Test* NTLMCredentialsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NTLMCredentialsTest");

	CppUnit_addTest(pSuite, NTLMCredentialsTest, testNonce);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testPasswordHash);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testNTLMv2Hash);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testLMv2Response);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testNTLMv2Response);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testFormatNegotiateMessage);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testParseChallengeMessage);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testParseChallengeMessageWithTargetInfo);
	CppUnit_addTest(pSuite, NTLMCredentialsTest, testFormatAuthenticateMessage);

	return pSuite;
}
