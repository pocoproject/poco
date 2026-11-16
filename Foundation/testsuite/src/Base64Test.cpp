//
// Base64Test.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Base64Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::Base64Encoder;
using Poco::Base64Decoder;
using Poco::DataFormatException;


namespace
{
	std::string decode(const std::string& encoded, int options = 0)
		/// Returns the decoded data, or the message of the DataFormatException thrown.
	{
		std::istringstream istr(encoded);
		Base64Decoder decoder(istr, options);
		decoder.exceptions(std::ios::badbit);
		std::string result;
		try
		{
			Poco::StreamCopier::copyToString(decoder, result);
		}
		catch (DataFormatException& exc)
		{
			return "exception: " + exc.message();
		}
		return result;
	}
}


Base64Test::Base64Test(const std::string& name): CppUnit::TestCase(name)
{
}


Base64Test::~Base64Test()
{
}


void Base64Test::testEncoder()
{
	{
		std::ostringstream str;
		Base64Encoder encoder(str);
		encoder << std::string("\00\01\02\03\04\05", 6);
		encoder.close();
		assertTrue (str.str() == "AAECAwQF");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str);
		encoder << std::string("\00\01\02\03", 4);
		encoder.close();
		assertTrue (str.str() == "AAECAw==");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str);
		encoder << "ABCDEF";
		encoder.close();
		assertTrue (str.str() == "QUJDREVG");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str);
		encoder << "!@#$%^&*()_~<>";
		encoder.close();
		assertTrue (str.str() == "IUAjJCVeJiooKV9+PD4=");
	}
}


void Base64Test::testEncoderURL()
{
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING);
		encoder << std::string("\00\01\02\03\04\05", 6);
		encoder.close();
		assertTrue (str.str() == "AAECAwQF");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING);
		encoder << std::string("\00\01\02\03", 4);
		encoder.close();
		assertTrue (str.str() == "AAECAw==");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING);
		encoder << "ABCDEF";
		encoder.close();
		assertTrue (str.str() == "QUJDREVG");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING);
		encoder << "!@#$%^&*()_~<>";
		encoder.close();
		assertTrue (str.str() == "IUAjJCVeJiooKV9-PD4=");
	}
}


void Base64Test::testEncoderNoPadding()
{
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
		encoder << std::string("\00\01\02\03\04\05", 6);
		encoder.close();
		assertTrue (str.str() == "AAECAwQF");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
		encoder << std::string("\00\01\02\03", 4);
		encoder.close();
		assertTrue (str.str() == "AAECAw");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
		encoder << "ABCDEF";
		encoder.close();
		assertTrue (str.str() == "QUJDREVG");
	}
	{
		std::ostringstream str;
		Base64Encoder encoder(str, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
		encoder << "!@#$%^&*()_~<>";
		encoder.close();
		assertTrue (str.str() == "IUAjJCVeJiooKV9-PD4");
	}
}


void Base64Test::testDecoder()
{
	{
		std::istringstream istr("AAECAwQF");
		Base64Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == 5);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAwQ=");
		Base64Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAw==");
		Base64Decoder decoder(istr);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("QUJDREVG");
		Base64Decoder decoder(istr);
		std::string s;
		decoder >> s;
		assertTrue (s == "ABCDEF");
		assertTrue (decoder.eof());
		assertTrue (!decoder.fail());
	}
	{
		std::istringstream istr("QUJ\r\nDRE\r\nVG");
		Base64Decoder decoder(istr);
		std::string s;
		decoder >> s;
		assertTrue (s == "ABCDEF");
		assertTrue (decoder.eof());
		assertTrue (!decoder.fail());
	}
	{
		std::istringstream istr("QUJD#REVG");
		Base64Decoder decoder(istr);
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
	{
		// istream::read() goes through xsgetn; badbit must be set
		// when a mid-read decoder error occurs.
		std::istringstream istr("QUJD#REVG");
		Base64Decoder decoder(istr);
		char buf[16];
		try
		{
			decoder.read(buf, sizeof(buf));
			assertTrue (decoder.bad());
		}
		catch (DataFormatException&)
		{
		}
		assertTrue (!decoder.eof());
	}
	{
		// exceptions(badbit) opt-in must still rethrow the decoder
		// exception when the failure occurs mid-read via xsgetn.
		std::istringstream istr("QUJD#REVG");
		Base64Decoder decoder(istr);
		decoder.exceptions(std::ios::badbit);
		char buf[16];
		bool threw = false;
		try
		{
			decoder.read(buf, sizeof(buf));
		}
		catch (DataFormatException&)
		{
			threw = true;
		}
		assertTrue (threw);
	}
}


void Base64Test::testDecoderURL()
{
	{
		std::istringstream istr("AAECAwQF");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == 5);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAwQ=");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAw==");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("QUJDREVG");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		std::string s;
		decoder >> s;
		assertTrue (s == "ABCDEF");
		assertTrue (decoder.eof());
		assertTrue (!decoder.fail());
	}
	{
		std::istringstream istr("QUJ\r\nDRE\r\nVG");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		try
		{
			std::string s;
			decoder >> s;
			assertTrue (decoder.bad());
		}
		catch (DataFormatException&)
		{
		}
	}
	{
		std::istringstream istr("QUJD#REVG");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
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
	{
		std::istringstream istr("IUAjJCVeJiooKV9-PD4=");
		Base64Decoder decoder(istr, Poco::BASE64_URL_ENCODING);
		std::string s;
		decoder >> s;
		assertTrue (s == "!@#$%^&*()_~<>");
		assertTrue (decoder.eof());
		assertTrue (!decoder.fail());
	}
}


void Base64Test::testDecoderNoPadding()
{
	{
		std::istringstream istr("AAECAwQF");
		Base64Decoder decoder(istr, Poco::BASE64_NO_PADDING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == 5);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAwQ");
		Base64Decoder decoder(istr, Poco::BASE64_NO_PADDING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == 4);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
	{
		std::istringstream istr("AAECAw");
		Base64Decoder decoder(istr, Poco::BASE64_NO_PADDING);
		assertTrue (decoder.good() && decoder.get() == 0);
		assertTrue (decoder.good() && decoder.get() == 1);
		assertTrue (decoder.good() && decoder.get() == 2);
		assertTrue (decoder.good() && decoder.get() == 3);
		assertTrue (decoder.good() && decoder.get() == -1);
	}
}


void Base64Test::testEncodeDecode()
{
	{
		std::stringstream str;
		Base64Encoder encoder(str);
		encoder << "The quick brown fox ";
		encoder << "jumped over the lazy dog.";
		encoder.close();
		Base64Decoder decoder(str);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assertTrue (s == "The quick brown fox jumped over the lazy dog.");
	}
	{
		std::string src;
		for (int i = 0; i < 255; ++i) src += char(i);
		std::stringstream str;
		Base64Encoder encoder(str);
		encoder.write(src.data(), (std::streamsize) src.size());
		encoder.close();
		Base64Decoder decoder(str);
		std::string s;
		int c = decoder.get();
		while (c != -1) { s += char(c); c = decoder.get(); }
		assertTrue (s == src);
	}
}


void Base64Test::testDecoderMalformed()
{
	assertEqual (std::string("abc"), decode("YWJj"));
	assertEqual (std::string("ab"), decode("YWI="));
	assertEqual (std::string("a"), decode("YQ=="));
	assertEqual (std::string("a"), decode("YQ==\r\n"));
	assertEqual (std::string("exception: unexpected Base64 padding at position 4"), decode("YWJj===="));
	assertEqual (std::string("exception: Base64 data after padding: 'j' at position 3"), decode("YW=j"));
	assertEqual (std::string("exception: Base64 data after padding: 'Y' at position 4"), decode("YQ==YQ=="));
	assertEqual (std::string("exception: incomplete Base64 group at end of input"), decode("Y"));
	assertEqual (std::string("exception: incomplete Base64 group at end of input"), decode("YWJjY"));
	assertEqual (std::string("exception: invalid Base64 character '*' at position 2"), decode("YW*j"));
	assertEqual (std::string("exception: invalid Base64 character '*' at position 4"), decode("YW\r\n*j"));
	assertEqual (std::string("exception: invalid Base64 character 0x01 at position 2"), decode("YW\x01j"));

	assertEqual (std::string("a"), decode("YQ", Poco::BASE64_NO_PADDING));
	assertEqual (std::string("ab"), decode("YWI", Poco::BASE64_NO_PADDING));
	assertEqual (std::string("exception: incomplete Base64 group at end of input"), decode("Y", Poco::BASE64_NO_PADDING));
}


void Base64Test::setUp()
{
}


void Base64Test::tearDown()
{
}


CppUnit::Test* Base64Test::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("Base64Test");

	CppUnit_addTest(pSuite, Base64Test, testEncoder);
	CppUnit_addTest(pSuite, Base64Test, testEncoderURL);
	CppUnit_addTest(pSuite, Base64Test, testEncoderNoPadding);
	CppUnit_addTest(pSuite, Base64Test, testDecoder);
	CppUnit_addTest(pSuite, Base64Test, testDecoderURL);
	CppUnit_addTest(pSuite, Base64Test, testDecoderNoPadding);
	CppUnit_addTest(pSuite, Base64Test, testEncodeDecode);
	CppUnit_addTest(pSuite, Base64Test, testDecoderMalformed);

	return pSuite;
}
