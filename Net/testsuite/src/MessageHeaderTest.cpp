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
#include <utility>


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
}


void MessageHeaderTest::testDecodeWordNotEncoded()
{
	assertEqual ("=?base64?SGVsbG8=?=", MessageHeader::decodeWord("=?base64?SGVsbG8=?="));
	assertEqual ("https://example.com/?a=?b", MessageHeader::decodeWord("https://example.com/?a=?b"));
	assertEqual ("=?x?y?z?=", MessageHeader::decodeWord("=?x?y?z?="));
	assertEqual ("https://example.com/?a=?b&c=?d", MessageHeader::decodeWord("https://example.com/?a=?b&c=?d"));
	assertEqual ("prefix =?invalid and Hello suffix", MessageHeader::decodeWord("prefix =?invalid and =?UTF-8?B?SGVsbG8=?= suffix"));
	assertEqual ("=?=", MessageHeader::decodeWord("=?="));
	assertEqual ("=?a?=", MessageHeader::decodeWord("=?a?="));
	assertEqual ("=?a?b?=", MessageHeader::decodeWord("=?a?b?="));

	std::istringstream in("X-Value: =?base64?SGVsbG8=?=\r\nX-Link: <https://example.com/?a=?b>\r\n\r\n");
	MessageHeader header;
	header.read(in);
	assertEqual ("=?base64?SGVsbG8=?=", header.get("X-Value"));
	assertEqual ("<https://example.com/?a=?b>", header.get("X-Link"));
}


void MessageHeaderTest::testDecodeWordMalformed()
{
	assertEqual ("=?UTF-8?B?not!valid!?=", MessageHeader::decodeWord("=?UTF-8?B?not!valid!?="));
	assertEqual ("prefix =?UTF-8?B?not!valid!?= suffix", MessageHeader::decodeWord("prefix =?UTF-8?B?not!valid!?= suffix"));
	assertEqual ("=?UTF-8?B?SGVsbG8=A?=", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=A?="));
	assertEqual ("=?UTF-8?B?SGVsbG8=QQ==?=", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=QQ==?="));
	assertEqual ("=?UTF-8?B?SGVsbG8?=", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8?="));
	assertEqual ("=?UTF-8?Q?bad=ZZ?=", MessageHeader::decodeWord("=?UTF-8?Q?bad=ZZ?="));
	assertEqual ("=?UTF-8?Q?bad=2?=", MessageHeader::decodeWord("=?UTF-8?Q?bad=2?="));
	assertEqual ("=?UTF-8?Q?" "?=", MessageHeader::decodeWord("=?UTF-8?Q?" "?="));
	assertEqual ("=?UTF-8?B?" "?=", MessageHeader::decodeWord("=?UTF-8?B?" "?="));
	assertEqual ("=?bad/charset?Q?abc?=", MessageHeader::decodeWord("=?bad/charset?Q?abc?="));
}


void MessageHeaderTest::testDecodeWordAdjacent()
{
	assertEqual ("Hello", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?="));
	assertEqual ("HelloWorld", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?UTF-8?B?V29ybGQ=?="));
	assertEqual ("HelloWorld", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?=   =?UTF-8?B?V29ybGQ=?="));
	assertEqual ("HelloWorld", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?=\r\n =?UTF-8?B?V29ybGQ=?="));
	assertEqual ("Hello middle World", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= middle =?UTF-8?B?V29ybGQ=?="));
	assertEqual ("=?x?y?z?= Hello", MessageHeader::decodeWord("=?x?y?z?= =?UTF-8?B?SGVsbG8=?="));
	assertEqual ("Hello =?x?y?z?=", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?x?y?z?="));
	assertEqual ("=?UTF-8?X?SGVsbG8=?= Hello", MessageHeader::decodeWord("=?UTF-8?X?SGVsbG8=?= =?UTF-8?B?SGVsbG8=?="));
	assertEqual ("=?UTF-8?B?not!valid!?= Hello", MessageHeader::decodeWord("=?UTF-8?B?not!valid!?= =?UTF-8?B?SGVsbG8=?="));
	assertEqual ("Hello =?UTF-8?B?not!valid!?=", MessageHeader::decodeWord("=?UTF-8?B?SGVsbG8=?= =?UTF-8?B?not!valid!?="));
	assertEqual ("=?UTF-8?Q?bad=ZZ?= Hello", MessageHeader::decodeWord("=?UTF-8?Q?bad=ZZ?= =?UTF-8?B?SGVsbG8=?="));

	std::istringstream in("X-Subject: =?UTF-8?B?SGVsbG8=?=\r\n =?UTF-8?B?V29ybGQ=?=\r\n\r\n");
	MessageHeader header;
	header.read(in);
	assertEqual ("HelloWorld", header.get("X-Subject"));
}


void MessageHeaderTest::testDecodeRFC2047()
{
	std::string out;
	MessageHeader::decodeRFC2047("=?UTF-8?B?SGVsbG8=?=", out);
	assertEqual ("Hello", out);
	MessageHeader::decodeRFC2047("UTF-8?B?SGVsbG8=", out);
	assertEqual ("Hello", out);
	MessageHeader::decodeRFC2047("=?ISO-8859-1?Q?=E4?=", out, "UTF-8");
	assertEqual ("\xC3\xA4", out);
	MessageHeader::decodeRFC2047("=?x?y?z?=", out);
	assertEqual ("=?x?y?z?=", out);
	MessageHeader::decodeRFC2047("x?y?z", out);
	assertEqual ("x?y?z", out);
	MessageHeader::decodeRFC2047("=?UTF-8?B?not!valid!?=", out);
	assertEqual ("=?UTF-8?B?not!valid!?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?B?SGVsbG8=A?=", out);
	assertEqual ("=?UTF-8?B?SGVsbG8=A?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?B?SGVsbG8=QQ==?=", out);
	assertEqual ("=?UTF-8?B?SGVsbG8=QQ==?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?B?SGVsbG8?=", out);
	assertEqual ("=?UTF-8?B?SGVsbG8?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?Q?bad=ZZ?=", out);
	assertEqual ("=?UTF-8?Q?bad=ZZ?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?Q?bad=2?=", out);
	assertEqual ("=?UTF-8?Q?bad=2?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?Q?" "?=", out);
	assertEqual ("=?UTF-8?Q?" "?=", out);
	MessageHeader::decodeRFC2047("=?UTF-8?B?" "?=", out);
	assertEqual ("=?UTF-8?B?" "?=", out);
	MessageHeader::decodeRFC2047("=?bad/charset?Q?abc?=", out);
	assertEqual ("=?bad/charset?Q?abc?=", out);

	std::string inOut("=?UTF-8?B?SGVsbG8=?=");
	MessageHeader::decodeRFC2047(inOut, inOut);
	assertEqual ("Hello", inOut);
}


void MessageHeaderTest::testDecodeWordLongValue()
{
	std::string value;
	for (int i = 0; i < 2730; ++i)
		value += "=?a";
	assertEqual (8190, value.size());
	assertEqual (value, MessageHeader::decodeWord(value));

	std::string fields;
	for (int i = 0; i < 99; ++i)
		fields += "X-Field" + std::to_string(i) + ": " + value + "\r\n";
	fields += "\r\n";
	std::istringstream in(fields);
	MessageHeader header;
	header.read(in);
	for (int i = 0; i < 99; ++i)
		assertEqual (value, header.get("X-Field" + std::to_string(i)));
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


void MessageHeaderTest::testAutoDecodeAddressFields()
{
	std::istringstream istr(
		"From: =?UTF-8?q?Doe=2C_J=C3=B6rg?= <j@x.example>\r\n"
		"To: a@x.example (=?UTF-8?q?J=C3=B6rg_=28x=29?=), =?UTF-8?q?Ann?= <ann@x.example>\r\n"
		"Cc: =?UTF-8?Q?c@x.example?=\r\n"
		"Reply-To: =?UTF-8?q?x=22y=5C?= <r@x.example>\r\n"
		"Resent-To: =?UTF-8?q?T=C3=A4m?=: t@x.example;\r\n"
		"Bcc: \"=?UTF-8?q?quoted?=\" <q@x.example>, John =?UTF-8?q?Q=2E?= Public <jqp@x.example>\r\n"
		"Sender: =?UTF-8?q?Jane?= (=?UTF-8?q?a?= =?UTF-8?q?b?=) <s@x.example>\r\n"
		"X-To: =?UTF-8?q?Doe=2C_J?= <j@x.example>\r\n"
		"Resent-Cc: Mr. Smith <m@x.example>, (a\\(b) \"x\\\"\" <x@x.example>\r\n"
		"Disposition-Notification-To: =?UTF-8?Q?evil@x.example=2C?= <a@x.example>\r\n"
		"Delivered-To: d@[(=?UTF-8?Q?y=2C?=)]\r\n"
		"\r\n");
	MessageHeader mh;
	mh.read(istr);
	assertEqual ("\"Doe, J\xC3\xB6rg\" <j@x.example>", mh.get("From"));
	assertEqual ("a@x.example (J\xC3\xB6rg \\(x\\)), Ann <ann@x.example>", mh.get("To"));
	assertEqual ("=?UTF-8?Q?c@x.example?=", mh.get("Cc"));
	assertEqual ("\"x\\\"y\\\\\" <r@x.example>", mh.get("Reply-To"));
	assertEqual ("T\xC3\xA4m: t@x.example;", mh.get("Resent-To"));
	assertEqual ("\"=?UTF-8?q?quoted?=\" <q@x.example>, \"John Q. Public\" <jqp@x.example>", mh.get("Bcc"));
	assertEqual ("Jane (ab) <s@x.example>", mh.get("Sender"));
	assertEqual ("Doe, J <j@x.example>", mh.get("X-To"));
	assertEqual ("Mr. Smith <m@x.example>, (a\\(b) \"x\\\"\" <x@x.example>", mh.get("Resent-Cc"));
	assertEqual ("\"evil@x.example,\" <a@x.example>", mh.get("Disposition-Notification-To"));
	assertEqual ("d@[(=?UTF-8?Q?y=2C?=)]", mh.get("Delivered-To"));

	for (const char* name: {"Disposition-Notification-To", "Return-Receipt-To", "Mail-Followup-To", "Mail-Reply-To",
		"Errors-To", "Apparently-To", "Delivered-To", "X-Original-To"})
	{
		std::istringstream fieldStr(std::string(name) + ": =?UTF-8?q?Doe=2C_J?= <j@x.example>\r\n\r\n");
		MessageHeader field;
		field.read(fieldStr);
		assertEqual ("\"Doe, J\" <j@x.example>", field.get(name));
	}
}


void MessageHeaderTest::testAutoDecodeDomainLiteral()
{
	std::istringstream istr(
		"To: [a<b] =?UTF-8?Q?x?= <c@d.example>\r\n"
		"Cc: <a@[x>, =?UTF-8?Q?B?= <b@c.example>\r\n"
		"Bcc: a@[x, =?UTF-8?Q?B?= <b@c.example>\r\n"
		"Reply-To: a@[(=?UTF-8?Q?y?=), =?UTF-8?Q?B?= <b@c.example>\r\n"
		"Resent-To: a@[x[=?UTF-8?Q?y?=], =?UTF-8?Q?B?= <b@c.example>\r\n"
		"\r\n");
	MessageHeader mh;
	mh.read(istr);
	assertEqual ("[a<b] x <c@d.example>", mh.get("To"));
	assertEqual ("<a@[x>, B <b@c.example>", mh.get("Cc"));
	assertEqual ("a@[x, B <b@c.example>", mh.get("Bcc"));
	assertEqual ("a@[(=?UTF-8?Q?y?=), B <b@c.example>", mh.get("Reply-To"));
	assertEqual ("a@[x[=?UTF-8?Q?y?=], B <b@c.example>", mh.get("Resent-To"));
}


void MessageHeaderTest::testAutoDecodeQuotedLocalPart()
{
	std::istringstream istr(
		"To: \"=?UTF-8?Q?a?=\"@example.com\r\n"
		"Cc: =?UTF-8?Q?A?= <\"a,b\"@example.com>, =?UTF-8?Q?B?= <x@y.example>\r\n"
		"Bcc: =?UTF-8?Q?A?= <\"=?UTF-8?Q?a?=\"@example.com>\r\n"
		"Reply-To: \"x<y\" =?UTF-8?Q?A?= <a@b.example>\r\n"
		"\r\n");
	MessageHeader mh;
	mh.read(istr);
	assertEqual ("\"=?UTF-8?Q?a?=\"@example.com", mh.get("To"));
	assertEqual ("A <\"a,b\"@example.com>, B <x@y.example>", mh.get("Cc"));
	assertEqual ("A <\"=?UTF-8?Q?a?=\"@example.com>", mh.get("Bcc"));
	assertEqual ("\"x<y\" A <a@b.example>", mh.get("Reply-To"));
}


void MessageHeaderTest::testNoDecodeStructuredFields()
{
	const std::string fields(
		"Content-Type: text/plain; name=\"=?UTF-8?Q?a?=\"\r\n"
		"Content-Disposition: attachment; filename=\"=?UTF-8?Q?a?=\"\r\n"
		"Date: =?UTF-8?Q?a?=\r\n"
		"Message-ID: <=?UTF-8?Q?a?=@x.example>\r\n"
		"Received: from =?UTF-8?Q?a?= by x.example\r\n"
		"Content-Description: =?UTF-8?Q?d?=\r\n"
		"Comments: =?UTF-8?Q?c?=\r\n"
		"Subject: =?UTF-8?Q?s?=\r\n"
		"From: =?UTF-8?q?Doe=2C_J?= <j@x.example>\r\n"
		"\r\n");
	const std::pair<std::string, std::string> expected[] = {
		{"Content-Type", "text/plain; name=\"=?UTF-8?Q?a?=\""},
		{"Content-Disposition", "attachment; filename=\"=?UTF-8?Q?a?=\""},
		{"Date", "=?UTF-8?Q?a?="},
		{"Message-ID", "<=?UTF-8?Q?a?=@x.example>"},
		{"Received", "from =?UTF-8?Q?a?= by x.example"},
		{"Content-Description", "d"},
		{"Comments", "c"},
		{"Subject", "s"},
		{"From", "\"Doe, J\" <j@x.example>"}
	};

	std::istringstream istr(fields);
	MessageHeader mh;
	mh.read(istr);
	std::istringstream istrUndecoded(fields);
	MessageHeader mhUndecoded;
	mhUndecoded.setAutoDecode(false);
	mhUndecoded.read(istrUndecoded);
	for (const auto& [name, value]: expected)
	{
		assertEqual (value, mh.get(name));
		assertEqual (value, mhUndecoded.getDecoded(name));
	}
}


void MessageHeaderTest::testWriteReceivedForm()
{
	const std::string fields(
		"Subject: =?UTF-8?Q?a=C3=A4?=\r\n"
		"To: =?UTF-8?q?Doe=2C_J?= <j@x.example>\r\n"
		"X-Raw: J\xC3\xB6rg\r\n"
		"X-Plain: plain\r\n");
	std::istringstream istr(fields + "\r\n");
	MessageHeader mh;
	mh.read(istr);
	assertEqual ("a\xC3\xA4", mh.get("Subject"));
	assertEqual ("\"Doe, J\" <j@x.example>", mh.get("To"));

	std::ostringstream ostr;
	mh.write(ostr);
	assertEqual (fields, ostr.str());

	MessageHeader copy(mh);
	std::ostringstream copyStr;
	copy.write(copyStr);
	assertEqual (fields, copyStr.str());

	MessageHeader assigned;
	assigned = mh;
	std::ostringstream assignedStr;
	assigned.write(assignedStr);
	assertEqual (fields, assignedStr.str());

	mh.set("Subject", "b");
	mh.erase("To");
	mh.add("To", "k@x.example");
	std::ostringstream changedStr;
	mh.write(changedStr);
	assertEqual ("Subject: b\r\nX-Raw: J\xC3\xB6rg\r\nX-Plain: plain\r\nTo: k@x.example\r\n", changedStr.str());
}


void MessageHeaderTest::testEraseDropsReceivedForm()
{
	const std::string fields("Subject: =?UTF-8?Q?a=C3=A4?=\r\nX-A: =?UTF-8?Q?b=C3=A4?=\r\n\r\n");
	std::istringstream istr(fields);
	MessageHeader mh;
	mh.read(istr);
	mh.erase("subject");
	mh.add("Subject", "a\xC3\xA4");
	std::ostringstream ostr;
	mh.write(ostr);
	assertEqual ("X-A: =?UTF-8?Q?b=C3=A4?=\r\nSubject: a\xC3\xA4\r\n", ostr.str());

	mh.clear();
	mh.add("X-A", "b\xC3\xA4");
	std::ostringstream clearedStr;
	mh.write(clearedStr);
	assertEqual ("X-A: b\xC3\xA4\r\n", clearedStr.str());
}


void MessageHeaderTest::testSplitParametersDecodesNames()
{
	std::string value;
	NameValueCollection params;
	MessageHeader::splitParameters("attachment; filename=\"=?UTF-8?B?w6TDtsO8LnR4dA==?=\"; name==?UTF-8?Q?a?= =?UTF-8?Q?b?=; title=\"=?UTF-8?Q?t?=\"", value, params);
	assertEqual ("attachment", value);
	assertEqual ("\xC3\xA4\xC3\xB6\xC3\xBC.txt", params["filename"]);
	assertEqual ("ab", params["name"]);
	assertEqual ("=?UTF-8?Q?t?=", params["title"]);

	MessageHeader::splitParameters("form-data; name=\"a=?b.txt\"; filename=\"=?UTF-8?Q?a=0Ab?=\"", value, params);
	assertEqual ("a=?b.txt", params["name"]);
	assertEqual ("=?UTF-8?Q?a=0Ab?=", params["filename"]);

	MessageHeader::splitParameters("form-data; name=\"field\"; filename=\"x.txt\"", value, params);
	assertEqual ("field", params["name"]);
	assertEqual ("x.txt", params["filename"]);

	MessageHeader::splitParameters("attachment; FileName=\"=?UTF-8?Q?a?=\"; NAME==?UTF-8?Q?b?=", value, params);
	assertEqual ("a", params["filename"]);
	assertEqual ("b", params["name"]);

	// some mail clients encode only the words with non-ASCII characters
	MessageHeader::splitParameters("attachment; filename=\"Rechnung =?UTF-8?Q?f=C3=BCr?= Mai.pdf\"", value, params);
	assertEqual ("Rechnung f\xC3\xBCr Mai.pdf", params["filename"]);

	const std::string parameters("; FileName=\"=?UTF-8?Q?a?=\"");
	NameValueCollection iteratorParams;
	MessageHeader::splitParameters(parameters.begin(), parameters.end(), iteratorParams);
	assertEqual ("=?UTF-8?Q?a?=", iteratorParams["filename"]);
}


void MessageHeaderTest::testSwapAndSecureClear()
{
	const std::string fields("Subject: =?UTF-8?Q?a=C3=A4?=\r\n");
	std::istringstream istr(fields + "\r\n");
	MessageHeader received;
	received.setFieldLimit(5);
	received.setNameLengthLimit(7);
	received.setValueLengthLimit(9000);
	received.read(istr);
	MessageHeader other;
	other.setAutoDecode(false);
	other.setStrictParsing(true);
	other.set("X-A", "=?UTF-8?Q?b?=");

	received.swap(other);
	assertTrue (!received.getAutoDecode());
	assertTrue (received.getStrictParsing());
	assertTrue (!other.getStrictParsing());
	assertEqual (5, other.getFieldLimit());
	assertEqual (7, other.getNameLengthLimit());
	assertEqual (9000, other.getValueLengthLimit());
	assertEqual (100, received.getFieldLimit());
	assertEqual (256, received.getNameLengthLimit());
	assertEqual (8192, received.getValueLengthLimit());
	// The value of X-A was not decoded by read(), so getDecoded() decodes it.
	assertEqual ("b", received.getDecoded("X-A"));
	std::ostringstream swappedStr;
	other.write(swappedStr);
	assertEqual (fields, swappedStr.str());

	swap(received, other);
	std::ostringstream swappedBackStr;
	received.write(swappedBackStr);
	assertEqual (fields, swappedBackStr.str());

	received.secureErase("Subject");
	received.add("Subject", "a\xC3\xA4");
	std::ostringstream erasedStr;
	received.write(erasedStr);
	assertEqual ("Subject: a\xC3\xA4\r\n", erasedStr.str());

	std::istringstream istrAgain(fields + "\r\n");
	MessageHeader cleared;
	cleared.read(istrAgain);
	cleared.secureClear();
	cleared.add("Subject", "a\xC3\xA4");
	std::ostringstream clearedStr;
	cleared.write(clearedStr);
	assertEqual ("Subject: a\xC3\xA4\r\n", clearedStr.str());
}


void MessageHeaderTest::testSecureEraseIterator()
{
	std::istringstream istr("Subject: =?UTF-8?Q?a=C3=A4?=\r\n\r\n");
	MessageHeader mh;
	mh.read(istr);
	mh.secureErase(mh.find("Subject"));
	mh.add("Subject", "a\xC3\xA4");
	std::ostringstream ostr;
	mh.write(ostr);
	assertEqual ("Subject: a\xC3\xA4\r\n", ostr.str());

	std::istringstream twoStr("X-A: =?UTF-8?Q?a=C3=A4?=\r\nX-A: =?UTF-8?Q?b=C3=A4?=\r\n\r\n");
	MessageHeader two;
	two.read(twoStr);
	two.secureErase(two.find("X-A"));
	two.add("X-A", "a\xC3\xA4");
	std::ostringstream twoOstr;
	two.write(twoOstr);
	assertEqual ("X-A: =?UTF-8?Q?b=C3=A4?=\r\nX-A: a\xC3\xA4\r\n", twoOstr.str());
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
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWordNotEncoded);
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWordMalformed);
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWordAdjacent);
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeRFC2047);
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWordLongValue);
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
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecodeAddressFields);
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecodeDomainLiteral);
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecodeQuotedLocalPart);
	CppUnit_addTest(pSuite, MessageHeaderTest, testNoDecodeStructuredFields);
	CppUnit_addTest(pSuite, MessageHeaderTest, testWriteReceivedForm);
	CppUnit_addTest(pSuite, MessageHeaderTest, testEraseDropsReceivedForm);
	CppUnit_addTest(pSuite, MessageHeaderTest, testSplitParametersDecodesNames);
	CppUnit_addTest(pSuite, MessageHeaderTest, testSwapAndSecureClear);
	CppUnit_addTest(pSuite, MessageHeaderTest, testSecureEraseIterator);

	return pSuite;
}
