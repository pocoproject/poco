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


void Base32Test::testEncoderHex()
{
	// RFC 4648 test vectors
	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder.close();
		assertTrue (str.str() == "");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "f";
		encoder.close();
		assertTrue (str.str() == "CO======");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "fo";
		encoder.close();
		assertTrue (str.str() == "CPNG====");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "foo";
		encoder.close();
		assertTrue (str.str() == "CPNMU===");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "foob";
		encoder.close();
		assertTrue (str.str() == "CPNMUOG=");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "fooba";
		encoder.close();
		assertTrue (str.str() == "CPNMUOJ1");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET | Poco::BASE32_USE_PADDING);
		encoder << "foobar";
		encoder.close();
		assertTrue (str.str() == "CPNMUOJ1E8======");
	}
}


void Base32Test::testEncoderCrockford()
{
	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder.close();
		assertTrue (str.str() == "");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "f";
		encoder.close();
		assertTrue (str.str() == "CR");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "fo";
		encoder.close();
		assertTrue (str.str() == "CSQG");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "foo";
		encoder.close();
		assertTrue (str.str() == "CSQPY");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "foob";
		encoder.close();
		assertTrue (str.str() == "CSQPYRG");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "fooba";
		encoder.close();
		assertTrue (str.str() == "CSQPYRK1");
	}

	{
		std::ostringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "foobar";
		encoder.close();
		assertTrue (str.str() == "CSQPYRK1E8");
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


void Base32Test::testDecoderHex()
{
	{
		std::istringstream istr("CO======");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "f");
	}
	{
		std::istringstream istr("CPNG====");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "fo");
	}
	{
		std::istringstream istr("CPNMU===");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foo");
	}
	{
		std::istringstream istr("CPNMUOG=");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foob");
	}
	{
		std::istringstream istr("CPNMUOJ1");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "fooba");
	}
	{
		std::istringstream istr("CPNMUOJ1E8======");
		Base32Decoder decoder(istr, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foobar");
	}
}


void Base32Test::testDecoderCrockford()
{
	{
		std::istringstream istr("CR");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "f");
	}
	{
		std::istringstream istr("CSQG");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "fo");
	}
	{
		std::istringstream istr("CSQPY");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foo");
	}
	{
		std::istringstream istr("CSQPYRG");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foob");
	}
	{
		std::istringstream istr("CSQPYRK1");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "fooba");
	}
	{
		std::istringstream istr("CSQPYRK1E8");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foobar");
	}
	{
		// alternative decode symbols (lowercase, 1/l)
		std::istringstream istr("csqpyrkle8");
		Base32Decoder decoder(istr, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		decoder >> s;
		assertTrue (s == "foobar");
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


void Base32Test::testEncodeDecodeHex()
{
	{
		std::stringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET);
		encoder << "The quick brown fox ";
		encoder << "jumped over the lazy dog.";
		encoder.close();
		Base32Decoder decoder(str, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assertTrue (s == "The quick brown fox jumped over the lazy dog.");
	}
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::stringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_HEX_ALPHABET);
		encoder.write(src.data(), (std::streamsize) src.size());
		encoder.close();
		Base32Decoder decoder(str, Poco::BASE32_USE_HEX_ALPHABET);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assertTrue (s == src);
	}
}


void Base32Test::testEncodeDecodeCrockford()
{
	{
		std::stringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder << "The quick brown fox ";
		encoder << "jumped over the lazy dog.";
		encoder.close();
		Base32Decoder decoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assertTrue (s == "The quick brown fox jumped over the lazy dog.");
	}
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::stringstream str;
		Base32Encoder encoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
		encoder.write(src.data(), (std::streamsize) src.size());
		encoder.close();
		Base32Decoder decoder(str, Poco::BASE32_USE_CROCKFORD_ALPHABET);
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
	CppUnit_addTest(pSuite, Base32Test, testEncoderHex);
	CppUnit_addTest(pSuite, Base32Test, testEncoderCrockford);
	CppUnit_addTest(pSuite, Base32Test, testDecoder);
	CppUnit_addTest(pSuite, Base32Test, testDecoderHex);
	CppUnit_addTest(pSuite, Base32Test, testDecoderCrockford);
	CppUnit_addTest(pSuite, Base32Test, testEncodeDecode);
	CppUnit_addTest(pSuite, Base32Test, testEncodeDecodeHex);
	CppUnit_addTest(pSuite, Base32Test, testEncodeDecodeCrockford);

	return pSuite;
}
