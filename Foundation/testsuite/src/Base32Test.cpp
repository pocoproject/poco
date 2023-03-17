//
// Base32Test.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Base32Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Base32Encoder.h"
#include "Poco/Base32Decoder.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::Base32Encoder;
using Poco::Base32Decoder;
using Poco::DataFormatException;


Base32Test::Base32Test(const std::string& name): CppUnit::TestCase(name)
{
}


Base32Test::~Base32Test()
{
}


void Base32Test::testEncoder()
{
	{
		std::ostringstream str;
		Base32Encoder encoder(str);
		encoder << std::string("\00\01\02\03\04\05", 6);
		encoder.close();
		assertTrue (str.str() == "AAAQEAYEAU======");
	}
	{
		std::ostringstream str;
		Base32Encoder encoder(str);
		encoder << std::string("\00\01\02\03", 4);
		encoder.close();
		assertTrue (str.str() == "AAAQEAY=");
	}
	{
		std::ostringstream str;
		Base32Encoder encoder(str, false);
		encoder << "ABCDEF";
		encoder.close();
		assertTrue (str.str() == "IFBEGRCFIY");
	}
	{
		std::ostringstream str;
		Base32Encoder encoder(str);
		encoder << "ABCDEF";
		encoder.close();
		assertTrue (str.str() == "IFBEGRCFIY======");
	}
	{
		std::ostringstream str;
		Base32Encoder encoder(str);
		encoder << "ABCDE";
		encoder.close();
		assertTrue (str.str() == "IFBEGRCF");
	}
}


void Base32Test::testDecoder()
{
	{
		std::istringstream istr("AAAQEAYEAU======");
		Base32Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == 5);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAAQEAYE");
		Base32Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAAQEAY=");
		Base32Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("IFBEGRCFIY======");
		Base32Decoder decoder(istr);
		std::string s;
		decoder >> s;
		assertTrue (s == "ABCDEF");
		assertTrue (decoder.eof());
		assertTrue (!decoder.fail());
	}
	{
		std::istringstream istr("QUJD#REVG");
		Base32Decoder decoder(istr);
		std::string s;
		try
		{
			decoder >> s;
			assertTrue (decoder.bad());
		}
		catch (DataFormatException&)
		{
		}
		assertTrue (!decoder.eof());
	}
}


void Base32Test::testEncodeDecode()
{
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
		assertTrue (s == "The quick brown fox jumped over the lazy dog.");
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
		assertTrue (s == src);
	}
}


void Base32Test::setUp()
{
}


void Base32Test::tearDown()
{
}


CppUnit::Test* Base32Test::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("Base32Test");

	CppUnit_addTest(pSuite, Base32Test, testEncoder);
	CppUnit_addTest(pSuite, Base32Test, testDecoder);
	CppUnit_addTest(pSuite, Base32Test, testEncodeDecode);

	return pSuite;
}
