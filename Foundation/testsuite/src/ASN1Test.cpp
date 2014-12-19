//
// ASN1Test.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/ASN1Test.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ASN1Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ASN1Codec.h"
#include "Poco/ASN1Types.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::ASN1Codec;
using Poco::ASN1;
using Poco::ASN1Type;
using namespace Poco::ASN1Types;

ASN1Test::ASN1Test(const std::string& name): CppUnit::TestCase(name)
{
}


ASN1Test::~ASN1Test()
{
}


void ASN1Test::testEncoder()
{
	{
		std::ostringstream str(std::stringstream::out | std::stringstream::binary);
		ASN1Codec encoder;
		ASN1::Ptr s(new OctetString("ENCODE ME"));
		encoder.encode(s, str);
		assert (str.str() == std::string("\x04\x09" "ENCODE ME"));
	}

	{
		std::ostringstream str(std::stringstream::out | std::stringstream::binary);
		ASN1Codec encoder;
		ASN1::Ptr s(new Integer(206672));
		encoder.encode(s, str);
		assert (str.str() == std::string("\x02\x03\x03\x27\x50"));
	}
}


void ASN1Test::testDecoder()
{
	{
		std::stringstream str(std::stringstream::in | std::stringstream::out | std::stringstream::binary);
		str << "\x04\x09" "ENCODE ME";
		ASN1Codec encoder;
		ASN1::Ptr s = encoder.decode(str);
		assert(s->getType().rawValue() == ASN1Type::OctetString);
		assert(s.cast<OctetString>()->getValue() == "ENCODE ME");
	}

	{
		std::stringstream str(std::stringstream::in | std::stringstream::out | std::stringstream::binary);
		str << "\x02\x03\x03\x27\x50";
		ASN1Codec encoder;
		ASN1::Ptr s = encoder.decode(str);
		assert(s->getType().rawValue() == ASN1Type::Integer);
		assert(s.cast<Integer>()->getValue() == 206672);
	}
}


void ASN1Test::testEncodeDecode()
{
	/*
	{
		std::stringstream str;
		Base32Encoder encoder(str);
		encoder << "The quick brown fox ";
		encoder << "jumped over the lazy dog.";
		encoder.close();
		Base32Decoder decoder(str);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assert (s == "The quick brown fox jumped over the lazy dog.");
	}
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::stringstream str;
		Base32Encoder encoder(str);
		encoder.write(src.data(), (std::streamsize) src.size());
		encoder.close();
		Base32Decoder decoder(str);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assert (s == src);
	}
	*/
}


void ASN1Test::setUp()
{
}


void ASN1Test::tearDown()
{
}


CppUnit::Test* ASN1Test::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ASN1Test");

	CppUnit_addTest(pSuite, ASN1Test, testEncoder);
	CppUnit_addTest(pSuite, ASN1Test, testDecoder);
	CppUnit_addTest(pSuite, ASN1Test, testEncodeDecode);

	return pSuite;
}
