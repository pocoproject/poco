//
// DigestStreamTest.cpp
//
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DigestStreamTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include <sstream>


using Poco::DigestInputStream;
using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;


DigestStreamTest::DigestStreamTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


DigestStreamTest::~DigestStreamTest()
{
}


void DigestStreamTest::testInputStream()
{
	std::istringstream istr("abcdefghijklmnopqrstuvwxyz");
	MD5Engine md5;
	DigestInputStream ds(md5, istr);
	std::string s;
	ds >> s;
	assertTrue (DigestEngine::digestToHex(md5.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");
	assertTrue (s == "abcdefghijklmnopqrstuvwxyz");
}


void DigestStreamTest::testOutputStream1()
{
	MD5Engine md5;
	DigestOutputStream ds(md5);
	ds << "abcdefghijklmnopqrstuvwxyz";
	ds.close();
	assertTrue (DigestEngine::digestToHex(md5.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

	ds << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	ds << "abcdefghijklmnopqrstuvwxyz0123456789";
	ds.close();
	assertTrue (DigestEngine::digestToHex(md5.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");
}


void DigestStreamTest::testOutputStream2()
{
	MD5Engine md5;
	std::ostringstream ostr;
	DigestOutputStream ds(md5, ostr);
	ds << "abcdefghijklmnopqrstuvwxyz";
	ds.close();
	assertTrue (DigestEngine::digestToHex(md5.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");
	assertTrue (ostr.str() == "abcdefghijklmnopqrstuvwxyz");
}


void DigestStreamTest::testToFromHex()
{
	std::string digest("c3fcd3d76192e4007dfb496cca67e13b");
	Poco::DigestEngine::Digest dig = DigestEngine::digestFromHex(digest);
	std::string digest2 = DigestEngine::digestToHex(dig);
	assertTrue (digest == digest2);
}


void DigestStreamTest::setUp()
{
}


void DigestStreamTest::tearDown()
{
}


CppUnit::Test* DigestStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DigestStreamTest");

	CppUnit_addTest(pSuite, DigestStreamTest, testInputStream);
	CppUnit_addTest(pSuite, DigestStreamTest, testOutputStream1);
	CppUnit_addTest(pSuite, DigestStreamTest, testOutputStream2);
	CppUnit_addTest(pSuite, DigestStreamTest, testToFromHex);
	
	return pSuite;
}
