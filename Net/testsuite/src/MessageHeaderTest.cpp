//
// MessageHeaderTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "MessageHeaderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include <cstring>
#include <new>
#include <sstream>


using Poco::Net::MessageHeader;
using Poco::Net::NameValueCollection;
using Poco::Net::MessageException;


MessageHeaderTest::MessageHeaderTest(const std::string& name): CppUnit::TestCase(name)
{
}


MessageHeaderTest::~MessageHeaderTest()
{
}


void MessageHeaderTest::testWrite()
{
	MessageHeader mh;
	mh.set("name1", "value1");
	mh.set("name2", "value2");
	mh.set("name3", "value3");

	std::ostringstream ostr;
	mh.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "name1: value1\r\nname2: value2\r\nname3: value3\r\n");
}


void MessageHeaderTest::testRead1()
{
	std::string s("name1: value1\r\nname2: value2\r\nname3: value3\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value2");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testRead2()
{
	std::string s("name1: value1\nname2: value2\nname3: value3\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value2");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testRead3()
{
	std::string s("name1: value1\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 1);
	assertTrue (mh["name1"] == "value1");
}



void MessageHeaderTest::testRead4()
{
	std::string s("name1: value1\r\nname2: value2\r\n\r\nsomedata");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 2);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value2");
	int ch = istr.get();
	assertTrue (ch == '\r');
	ch = istr.get();
	assertTrue (ch == '\n');
	ch = istr.get();
	assertTrue (ch == 's');
}


void MessageHeaderTest::testRead5()
{
	std::string s("name1:\r\nname2: value2\r\nname3: value3  \r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "");
	assertTrue (mh["name2"] == "value2");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testReadFolding1()
{
	std::string s("name1: value1\r\nname2: value21\r\n value22\r\nname3: value3\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value21 value22");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testReadFolding2()
{
	std::string s("name1: value1\nname2: value21\n\tvalue22\nname3: value3\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value21\tvalue22");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testReadFolding3()
{
	std::string s("name1: value1\r\nname2: value21\r\n value22\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 2);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value21 value22");
}


void MessageHeaderTest::testReadFolding4()
{
	std::string s("name1: value1\r\nname2: value21\r\n value22\r\n value23");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 2);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value21 value22 value23");
}


void MessageHeaderTest::testReadFolding5()
{
	std::string s("name1: value1\r\nname2: value21\r\n value22\r\n value23\r\nname3: value3");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.size() == 3);
	assertTrue (mh["name1"] == "value1");
	assertTrue (mh["name2"] == "value21 value22 value23");
	assertTrue (mh["name3"] == "value3");
}


void MessageHeaderTest::testReadInvalid1()
{
	std::string s("name1: value1\r\nname2: value21\r\n value22\r\n value23\r\n");
	s.append(300, 'x');
	std::istringstream istr(s);
	MessageHeader mh;
	try
	{
		mh.read(istr);
		fail("malformed message - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testReadInvalid2()
{
	std::string s("name1: value1\r\nname2: ");
	s.append(9000, 'x');
	std::istringstream istr(s);
	MessageHeader mh;
	try
	{
		mh.read(istr);
		fail("malformed message - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testSplitElements()
{
	std::string s;
	std::vector<std::string> v;
	MessageHeader::splitElements(s, v);
	assertTrue (v.empty());

	s = "foo";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 1);
	assertTrue (v[0] == "foo");

	s = "  foo ";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 1);
	assertTrue (v[0] == "foo");

	s = "foo,bar";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "foo");
	assertTrue (v[1] == "bar");

	s = "foo,,bar";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "foo");
	assertTrue (v[1] == "bar");

	MessageHeader::splitElements(s, v, false);
	assertTrue (v.size() == 3);
	assertTrue (v[0] == "foo");
	assertTrue (v[1] == "");
	assertTrue (v[2] == "bar");

	s = "foo;param=\"a,b\",bar;param=\"c,d\"";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "foo;param=\"a,b\"");
	assertTrue (v[1] == "bar;param=\"c,d\"");

	s = "foo; param=\"a,b\",  bar; param=\"c,d\"";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "foo; param=\"a,b\"");
	assertTrue (v[1] == "bar; param=\"c,d\"");

	s = "foo, bar, f00, baz";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 4);
	assertTrue (v[0] == "foo");
	assertTrue (v[1] == "bar");
	assertTrue (v[2] == "f00");
	assertTrue (v[3] == "baz");

	s = "a,b,c";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 3);
	assertTrue (v[0] == "a");
	assertTrue (v[1] == "b");
	assertTrue (v[2] == "c");

	s = "a=\"value=\\\\\\\"foo, bar\\\\\\\"\",b=foo";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "a=\"value=\\\"foo, bar\\\"\"");
	assertTrue (v[1] == "b=foo");

	s = "a=\\\",b=\\\"";
	MessageHeader::splitElements(s, v);
	assertTrue (v.size() == 2);
	assertTrue (v[0] == "a=\"");
	assertTrue (v[1] == "b=\"");

}


void MessageHeaderTest::testSplitParameters()
{
	std::string s;
	std::string v;
	NameValueCollection p;

	MessageHeader::splitParameters(s, v, p);
	assertTrue (v.empty());
	assertTrue (p.empty());

	s = "multipart/related";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "multipart/related");
	assertTrue (p.empty());

	s = "multipart/related; boundary=MIME_boundary_01234567";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "multipart/related");
	assertTrue (p.size() == 1);
	assertTrue (p["boundary"] == "MIME_boundary_01234567");

	s = "multipart/related; boundary=\"MIME_boundary_76543210\"";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "multipart/related");
	assertTrue (p.size() == 1);
	assertTrue (p["boundary"] == "MIME_boundary_76543210");

	s = "text/plain; charset=us-ascii";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "text/plain");
	assertTrue (p.size() == 1);
	assertTrue (p["charset"] == "us-ascii");

	s = "value; p1=foo; p2=bar";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "value");
	assertTrue (p.size() == 2);
	assertTrue (p["p1"] == "foo");
	assertTrue (p["p2"] == "bar");

	s = "value; p1=\"foo; bar\"";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "value");
	assertTrue (p.size() == 1);
	assertTrue (p["p1"] == "foo; bar");

	s = "value ; p1=foo ; p2=bar ";
	MessageHeader::splitParameters(s, v, p);
	assertTrue (v == "value");
	assertTrue (p.size() == 2);
	assertTrue (p["p1"] == "foo");
	assertTrue (p["p2"] == "bar");
}


void MessageHeaderTest::testFieldLimit()
{
	std::string s("name1: value1\r\nname2: value2\r\nname3: value3\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.setFieldLimit(2);
	try
	{
		mh.read(istr);
		fail("Field limit exceeded - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testNameLengthLimit()
{
	std::string s("name1: value1\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.setNameLengthLimit(2);
	try
	{
		mh.read(istr);
		fail("Name length limit exceeded - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testValueLengthLimit()
{
	std::string s("name1: value1\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.setValueLengthLimit(2);
	try
	{
		mh.read(istr);
		fail("Value length limit exceeded - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testLongValue()
{
	std::string longValue(10000, 'x');
	std::string s("long: " + longValue + "\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.setValueLengthLimit(10000);
	mh.read(istr);
	assertTrue (mh.get("long") == longValue);
}


void MessageHeaderTest::testDecodeWord()
{
	std::string coded("this is pure ASCII");
	std::string decoded = MessageHeader::decodeWord(coded, "ISO-8859-1");
	assertTrue (decoded == coded);

	coded = "(=?ISO-8859-1?Q?a?= =?ISO-8859-1?Q?b?=)";
	decoded = MessageHeader::decodeWord(coded, "ISO-8859-1");
	assertTrue (decoded == "(ab)");

	coded = "(=?ISO-8859-1?Q?a?= <0> =?ISO-8859-1?Q?b?=)";
	decoded = MessageHeader::decodeWord(coded, "ISO-8859-1");
	assertTrue (decoded == "(a <0> b)");

	coded = "Hello =?UTF-8?B?RnJhbmNpcw==?=, good bye";
	decoded = MessageHeader::decodeWord(coded, "ISO-8859-1");
	assertTrue (decoded == "Hello Francis, good bye");

	coded = "application/pdf; name=\"=?utf-8?Q?RUG_Regler-_und_Ger=C3=A4tebau_Gm?= =?utf-8?Q?bH_Angebot_Erneuerung_=C3=9CE.pdf?=\"";
	decoded = MessageHeader::decodeWord(coded, "UTF-8");
	assertTrue (decoded == "application/pdf; name=\"RUG Regler- und Gerätebau GmbH Angebot Erneuerung ÜE.pdf\"");

	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?=") == "Hello");
	assertTrue (MessageHeader::decodeWord("=?base64?SGVsbG8=?=") == "=?base64?SGVsbG8=?=");
	assertTrue (MessageHeader::decodeWord("https://example.com/?a=?b") == "https://example.com/?a=?b");
	assertTrue (MessageHeader::decodeWord("=?x?y?z?=") == "=?x?y?z?=");
	assertTrue (MessageHeader::decodeWord("https://example.com/?a=?b&c=?d") == "https://example.com/?a=?b&c=?d");
	assertTrue (MessageHeader::decodeWord("prefix =?invalid and =?UTF-8?B?SGVsbG8=?= suffix") == "prefix =?invalid and Hello suffix");
	assertTrue (MessageHeader::decodeWord("=?=") == "=?=");
	assertTrue (MessageHeader::decodeWord("=?a?=") == "=?a?=");
	assertTrue (MessageHeader::decodeWord("=?a?b?=") == "=?a?b?=");
	assertTrue (MessageHeader::decodeWord("=?x?y?z?= =?UTF-8?B?SGVsbG8=?=") == "=?x?y?z?= Hello");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?x?y?z?=") == "Hello =?x?y?z?=");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?X?SGVsbG8=?= =?UTF-8?B?SGVsbG8=?=") == "=?UTF-8?X?SGVsbG8=?= Hello");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?UTF-8?B?V29ybGQ=?=") == "HelloWorld");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?=   =?UTF-8?B?V29ybGQ=?=") == "HelloWorld");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?=\r\n =?UTF-8?B?V29ybGQ=?=") == "HelloWorld");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= middle =?UTF-8?B?V29ybGQ=?=") == "Hello middle World");
	assertTrue (MessageHeader::decodeWord("prefix =?UTF-8?B?not!valid!?= suffix") == "prefix =?UTF-8?B?not!valid!?= suffix");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?not!valid!?= =?UTF-8?B?SGVsbG8=?=") == "=?UTF-8?B?not!valid!?= Hello");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?UTF-8?B?not!valid!?=") == "Hello =?UTF-8?B?not!valid!?=");
	assertTrue (MessageHeader::decodeWord("=?UTF-8?Q?bad=ZZ?= =?UTF-8?B?SGVsbG8=?=") == "=?UTF-8?Q?bad=ZZ?= Hello");

	std::istringstream in("X-Value: =?base64?SGVsbG8=?=\r\nX-Link: <https://example.com/?a=?b>\r\n\r\n");
	MessageHeader header;
	header.read(in);
	assertTrue (header.get("X-Value") == "=?base64?SGVsbG8=?=");
	assertTrue (header.get("X-Link") == "<https://example.com/?a=?b>");

	std::string out;
	MessageHeader::decodeRFC2047("=?x?y?z?=", out);
	assertTrue (out == "=?x?y?z?=");
	MessageHeader::decodeRFC2047("x?y?z", out);
	assertTrue (out == "x?y?z");
	MessageHeader::decodeRFC2047("=?UTF-8?B?SGVsbG8=?=", out);
	assertTrue (out == "Hello");
	MessageHeader::decodeRFC2047("UTF-8?B?SGVsbG8=", out);
	assertTrue (out == "Hello");
	MessageHeader::decodeRFC2047("=?UTF-8?B?not!valid!?=", out);
	assertTrue (out == "=?UTF-8?B?not!valid!?=");
	MessageHeader::decodeRFC2047("=?UTF-8?Q?bad=ZZ?=", out);
	assertTrue (out == "=?UTF-8?Q?bad=ZZ?=");
	MessageHeader::decodeRFC2047("=?UTF-8?Q?bad=2?=", out);
	assertTrue (out == "=?UTF-8?Q?bad=2?=");
}

// Sample HTTP reuest header
static std::string httpRequestHeader{
R"(GET / HTTP/2
Host: stackoverflow.com
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/119.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: en-GB,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Upgrade-Insecure-Requests: 1
X-Encoded-Header-A: (=?ISO-8859-1?Q?a?= =?ISO-8859-1?Q?b?=)
X-Encoded-Header-B: Hello =?UTF-8?B?RnJhbmNpcw==?=, good bye

)"
};

void MessageHeaderTest::testAutoDecode()
{
	{
		std::istringstream istr(httpRequestHeader);
		MessageHeader mh;
		mh.read(istr);

		assertEquals(mh.get("X-Encoded-Header-A"), "(ab)");
		assertEquals(mh.get("X-Encoded-Header-B"), "Hello Francis, good bye");

		assertEquals(mh.getDecoded("X-Encoded-Header-A"), "(ab)");
		assertEquals(mh.getDecoded("X-Encoded-Header-B"), "Hello Francis, good bye");
	}
	{
		std::istringstream istr(httpRequestHeader);
		MessageHeader mh;
		mh.setAutoDecode(false);
		mh.read(istr);

		assertEquals(mh.get("X-Encoded-Header-A"), "(=?ISO-8859-1?Q?a?= =?ISO-8859-1?Q?b?=)");
		assertEquals(mh.get("X-Encoded-Header-B"), "Hello =?UTF-8?B?RnJhbmNpcw==?=, good bye");

		assertEquals(mh.getDecoded("X-Encoded-Header-A"), "(ab)");
		assertEquals(mh.getDecoded("X-Encoded-Header-B"), "Hello Francis, good bye");
	}
}


void MessageHeaderTest::setUp()
{
}


void MessageHeaderTest::tearDown()
{
}


void MessageHeaderTest::testNoDecodeFramingHeaders()
{
	// Auto-decoding must not reach the headers that decide the framing,
	// otherwise an encoded word becomes "chunked" only after a front-end has
	// already framed the message on the undecoded value.
	std::string s("Subject: =?utf-8?B?Y2h1bmtlZA==?=\r\n"
		"Transfer-Encoding: =?utf-8?B?Y2h1bmtlZA==?=\r\n"
		"Content-Length: =?utf-8?B?MTA=?=\r\n\r\n");
	std::istringstream istr(s);
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.get("Subject") == "chunked");
	assertTrue (mh.get("Transfer-Encoding") == "=?utf-8?B?Y2h1bmtlZA==?=");
	assertTrue (mh.get("Content-Length") == "=?utf-8?B?MTA=?=");
}


void MessageHeaderTest::testStrictFieldName()
{
	const std::string invalid[] = {
		"Content-Length : 5\r\n\r\n",
		"Transfer-Encoding\t: chunked\r\n\r\n",
		"Content-Length\r: 5\r\n\r\n",
		"Na\x01me: value\r\n\r\n",
		"N\xC3\xA4me: value\r\n\r\n",
		"X(a): value\r\n\r\n",
		"X/a: value\r\n\r\n",
		": value\r\n\r\n"
	};
	for (const auto& s: invalid)
	{
		std::istringstream istr(s);
		MessageHeader mh;
		mh.setStrictParsing(true);
		try
		{
			mh.read(istr);
			failmsg("invalid field name must throw: " + s);
		}
		catch (MessageException&)
		{
		}
	}

	std::istringstream istr("Content-Length: 5\r\nX-Name_1.~!#$%&'*+^`|: value\r\n\r\n");
	MessageHeader mh;
	mh.setStrictParsing(true);
	mh.read(istr);
	assertTrue (mh.size() == 2);
	assertTrue (mh.get("Content-Length") == "5");
	assertTrue (mh.get("X-Name_1.~!#$%&'*+^`|") == "value");

	std::istringstream lenient("Content-Length : 5\r\n\r\n");
	MessageHeader mhLenient;
	mhLenient.read(lenient);
	assertTrue (mhLenient.get("Content-Length ") == "5");
}


void MessageHeaderTest::testStrictValueWhitespace()
{
	std::istringstream istr("Transfer-Encoding:\vchunked\r\nContent-Length: \t5\f \r\n\r\n");
	MessageHeader mh;
	mh.setStrictParsing(true);
	mh.read(istr);
	assertTrue (mh.get("Transfer-Encoding") == "\vchunked");
	assertTrue (mh.get("Content-Length") == "5\f");

	std::istringstream lenient("Transfer-Encoding:\vchunked\r\nContent-Length: \t5\f \r\n\r\n");
	MessageHeader mhLenient;
	mhLenient.read(lenient);
	assertTrue (mhLenient.get("Transfer-Encoding") == "chunked");
	assertTrue (mhLenient.get("Content-Length") == "5");
}


void MessageHeaderTest::testFieldLimitLinesWithoutColon()
{
	std::string s;
	for (int i = 0; i < 200; ++i) s += "x\r\n";
	s += "name: value\r\n\r\n";
	std::istringstream istr(s);
	MessageHeader mh;
	try
	{
		mh.read(istr);
		fail("Field limit exceeded - must throw");
	}
	catch (MessageException&)
	{
	}
}


void MessageHeaderTest::testWriteInvalidFieldValue()
{
	const std::string invalid[] = {
		"a\r\nSet-Cookie: b",
		"a\nb",
		"a\rb",
		"a\r\n",
		"a\r\n ",
		"a\r\n \r\n b",
		"a\r\n\t\r\nb",
		std::string("a\0b", 3),
		"a\x01" "b",
		"a\x7F"
	};
	for (const auto& value: invalid)
	{
		MessageHeader mh;
		mh.set("Name1", "value1");
		mh.set("Name2", value);
		std::ostringstream ostr;
		try
		{
			mh.write(ostr);
			fail("invalid field value must throw");
		}
		catch (MessageException&)
		{
		}
		assertTrue (ostr.str().empty());
	}
}


void MessageHeaderTest::testWriteInvalidFieldName()
{
	const std::string invalid[] = {
		"",
		"Na me",
		"Name:",
		"Na\tme",
		"Na\x01me",
		"N\xC3\xA4me"
	};
	for (const auto& name: invalid)
	{
		MessageHeader mh;
		mh.set("Name1", "value1");
		mh.set(name, "value");
		std::ostringstream ostr;
		try
		{
			mh.write(ostr);
			failmsg("invalid field name must throw: " + name);
		}
		catch (MessageException&)
		{
		}
		assertTrue (ostr.str().empty());
	}
}


void MessageHeaderTest::testWriteValidFields()
{
	MessageHeader mh;
	mh.set("X-Name_1.~!#$%&'*+^`|", "a\tb");
	mh.set("Subject", "=?UTF-8?Q?a?=\r\n =?UTF-8?Q?b?=");
	mh.set("Text", "N\xC3\xA4me");
	mh.set("X-A/B(c)", "a\r\n \t b");
	std::ostringstream ostr;
	mh.write(ostr);
	assertTrue (ostr.str() == "X-Name_1.~!#$%&'*+^`|: a\tb\r\nSubject: =?UTF-8?Q?a?=\r\n =?UTF-8?Q?b?=\r\nText: N\xC3\xA4me\r\nX-A/B(c): a\r\n \t b\r\n");
}


void MessageHeaderTest::testReadDecodedControlCharacters()
{
	std::istringstream istr(
		"X-Q: =?UTF-8?Q?a=0D=0AY:_b?=\r\n"
		"X-B: =?UTF-8?B?YQ0KWTogYg==?=\r\n"
		"X-Nul: =?UTF-8?Q?a=00b?=\r\n"
		"X-Tab: =?UTF-8?Q?a=09b?=\r\n"
		"\r\n");
	MessageHeader mh;
	mh.read(istr);
	assertTrue (mh.get("X-Q") == "=?UTF-8?Q?a=0D=0AY:_b?=");
	assertTrue (mh.get("X-B") == "=?UTF-8?B?YQ0KWTogYg==?=");
	assertTrue (mh.get("X-Nul") == "=?UTF-8?Q?a=00b?=");
	assertTrue (mh.get("X-Tab") == "a\tb");

	std::ostringstream ostr;
	mh.write(ostr);
	assertTrue (ostr.str().find("\r\nY:") == std::string::npos);
}


void MessageHeaderTest::testAutoDecodeCopy()
{
	const std::string header("X-A: =?UTF-8?B?SGVsbG8=?=\r\n\r\n");
	// The storage is filled with the opposite of the source's decode flags,
	// so the copy must take both from the source.
	alignas(MessageHeader) unsigned char storage[sizeof(MessageHeader)];
	for (const bool autoDecode: {false, true})
	{
		std::istringstream istr(header);
		MessageHeader src;
		src.setAutoDecode(autoDecode);
		src.read(istr);

		std::memset(storage, autoDecode ? 0x00 : 0x01, sizeof(storage));
		MessageHeader* pCopy = new (storage) MessageHeader(src);
		const bool copyAutoDecode = pCopy->getAutoDecode();
		const std::string copyDecoded = pCopy->getDecoded("X-A");
		pCopy->~MessageHeader();
		assertTrue (copyAutoDecode == autoDecode);
		assertTrue (copyDecoded == "Hello");

		// The target holds a decode state that differs from the source.
		std::istringstream assignedIstr(header);
		MessageHeader assigned;
		assigned.setAutoDecode(!autoDecode);
		assigned.read(assignedIstr);
		assigned = src;
		assertTrue (assigned.getAutoDecode() == autoDecode);
		assertTrue (assigned.getDecoded("X-A") == "Hello");
	}
}


void MessageHeaderTest::testStrictParsingCopy()
{
	for (const bool strict: {false, true})
	{
		MessageHeader src;
		src.setStrictParsing(strict);

		MessageHeader copy(src);
		assertTrue (copy.getStrictParsing() == strict);

		MessageHeader assigned;
		assigned.setStrictParsing(!strict);
		assigned = src;
		assertTrue (assigned.getStrictParsing() == strict);
	}
}


CppUnit::Test* MessageHeaderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MessageHeaderTest");

	CppUnit_addTest(pSuite, MessageHeaderTest, testWrite);
	CppUnit_addTest(pSuite, MessageHeaderTest, testRead1);
	CppUnit_addTest(pSuite, MessageHeaderTest, testRead2);
	CppUnit_addTest(pSuite, MessageHeaderTest, testRead3);
	CppUnit_addTest(pSuite, MessageHeaderTest, testRead4);
	CppUnit_addTest(pSuite, MessageHeaderTest, testRead5);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadFolding1);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadFolding2);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadFolding3);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadFolding4);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadFolding5);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadInvalid1);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadInvalid2);
	CppUnit_addTest(pSuite, MessageHeaderTest, testSplitElements);
	CppUnit_addTest(pSuite, MessageHeaderTest, testSplitParameters);
	CppUnit_addTest(pSuite, MessageHeaderTest, testFieldLimit);
	CppUnit_addTest(pSuite, MessageHeaderTest, testNameLengthLimit);
	CppUnit_addTest(pSuite, MessageHeaderTest, testValueLengthLimit);
	CppUnit_addTest(pSuite, MessageHeaderTest, testLongValue);
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWord);
	CppUnit_addTest(pSuite, MessageHeaderTest, testNoDecodeFramingHeaders);
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecode);
	CppUnit_addTest(pSuite, MessageHeaderTest, testStrictFieldName);
	CppUnit_addTest(pSuite, MessageHeaderTest, testStrictValueWhitespace);
	CppUnit_addTest(pSuite, MessageHeaderTest, testFieldLimitLinesWithoutColon);
	CppUnit_addTest(pSuite, MessageHeaderTest, testWriteInvalidFieldValue);
	CppUnit_addTest(pSuite, MessageHeaderTest, testWriteInvalidFieldName);
	CppUnit_addTest(pSuite, MessageHeaderTest, testWriteValidFields);
	CppUnit_addTest(pSuite, MessageHeaderTest, testReadDecodedControlCharacters);
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecodeCopy);
	CppUnit_addTest(pSuite, MessageHeaderTest, testStrictParsingCopy);

	return pSuite;
}
