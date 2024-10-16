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
	CppUnit_addTest(pSuite, MessageHeaderTest, testDecodeWord);
	CppUnit_addTest(pSuite, MessageHeaderTest, testAutoDecode);

	return pSuite;
}
