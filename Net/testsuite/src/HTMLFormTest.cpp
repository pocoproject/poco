//
// HTMLFormTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTMLFormTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/NetException.h"
#include <sstream>


using Poco::Net::HTMLForm;
using Poco::Net::PartSource;
using Poco::Net::StringPartSource;
using Poco::Net::PartHandler;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPMessage;
using Poco::Net::MessageHeader;


namespace
{
	class StringPartHandler: public PartHandler
	{
	public:
		StringPartHandler()
		{
		}

		void handlePart(const MessageHeader& header, std::istream& stream)
		{
			_disp = header["Content-Disposition"];
			_type = header["Content-Type"];
			int ch = stream.get();
			while (ch > 0)
			{
				_data += (char) ch;
				ch = stream.get();
			}
		}

		const std::string& data() const
		{
			return _data;
		}

		const std::string& disp() const
		{
			return _disp;
		}

		const std::string& type() const
		{
			return _type;
		}

	private:
		std::string _data;
		std::string _disp;
		std::string _type;
	};
}


HTMLFormTest::HTMLFormTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTMLFormTest::~HTMLFormTest()
{
}


void HTMLFormTest::testWriteUrl()
{
	HTMLForm form;
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");
	form.set("field5", "value+5");

	std::ostringstream ostr;
	form.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "field1=value1&field2=value%202&field3=value%3D3&field4=value%264&field5=value%2B5");
}


void HTMLFormTest::testWriteMultipart()
{
	HTMLForm form(HTMLForm::ENCODING_MULTIPART);
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");

	form.addPart("attachment1", new StringPartSource("This is an attachment"));
	StringPartSource* pSPS = new StringPartSource("This is another attachment", "text/plain", "att2.txt");
	pSPS->headers().set("Content-ID", "1234abcd");
	form.addPart("attachment2", pSPS);

	std::ostringstream ostr;
	form.write(ostr, "MIME_boundary_0123456789");
	std::string s = ostr.str();
	assertTrue (s ==
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field1\"\r\n"
		"\r\n"
		"value1\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field2\"\r\n"
		"\r\n"
		"value 2\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field3\"\r\n"
		"\r\n"
		"value=3\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field4\"\r\n"
		"\r\n"
		"value&4\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"attachment1\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"This is an attachment\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-ID: 1234abcd\r\n"
		"Content-Disposition: form-data; name=\"attachment2\"; filename=\"att2.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"This is another attachment\r\n"
		"--MIME_boundary_0123456789--\r\n"
	);
	assertTrue (s.length() == form.calculateContentLength());
}


void HTMLFormTest::testReadUrlGET()
{
	HTTPRequest req("GET", "/form.cgi?field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
	HTMLForm form(req);
	assertTrue (form.size() == 4);
	assertTrue (form["field1"] == "value1");
	assertTrue (form["field2"] == "value 2");
	assertTrue (form["field3"] == "value=3");
	assertTrue (form["field4"] == "value&4");
}


void HTMLFormTest::testReadUrlGETMultiple()
{
	HTTPRequest req("GET", "/form.cgi?field1=value1&field1=value%202&field1=value%3D3&field1=value%264");
	HTMLForm form(req);
	assertTrue (form.size() == 4);

	HTMLForm::ConstIterator it = form.find("field1");
	assertTrue (it != form.end());
	assertTrue (it->first == "field1" && it->second == "value1");
	++it;
	assertTrue (it != form.end());
	assertTrue (it->first == "field1" && it->second == "value 2");
	++it;
	assertTrue (it != form.end());
	assertTrue (it->first == "field1" && it->second == "value=3");
	++it;
	assertTrue (it != form.end());
	assertTrue (it->first == "field1" && it->second == "value&4");
	++it;
	assertTrue (it == form.end());
}


void HTMLFormTest::testReadUrlPOST()
{
	HTTPRequest req("POST", "/form.cgi?field0=value0");
	std::istringstream istr("field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
	HTMLForm form(req, istr);
	assertTrue (form.size() == 5);
	assertTrue (form["field0"] == "value0");
	assertTrue (form["field1"] == "value1");
	assertTrue (form["field2"] == "value 2");
	assertTrue (form["field3"] == "value=3");
	assertTrue (form["field4"] == "value&4");
}


void HTMLFormTest::testReadUrlPUT()
{
	HTTPRequest req("PUT", "/form.cgi?field0=value0");
	std::istringstream istr("field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
	HTMLForm form(req, istr);
	assertTrue (form.size() == 5);
	assertTrue (form["field0"] == "value0");
	assertTrue (form["field1"] == "value1");
	assertTrue (form["field2"] == "value 2");
	assertTrue (form["field3"] == "value=3");
	assertTrue (form["field4"] == "value&4");
}


void HTMLFormTest::testReadUrlBOM()
{
	HTTPRequest req("PUT", "/form.cgi?field0=value0");
	std::istringstream istr("\357\273\277field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
	HTMLForm form(req, istr);
	assertTrue (form.size() == 5);
	assertTrue (form["field0"] == "value0");
	assertTrue (form["field1"] == "value1");
	assertTrue (form["field2"] == "value 2");
	assertTrue (form["field3"] == "value=3");
	assertTrue (form["field4"] == "value&4");
}


void HTMLFormTest::testReadMultipart()
{
	std::istringstream istr(
		"\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field1\"\r\n"
		"\r\n"
		"value1\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field2\"\r\n"
		"\r\n"
		"value 2\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field3\"\r\n"
		"\r\n"
		"value=3\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field4\"\r\n"
		"\r\n"
		"value&4\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: file; name=\"attachment1\"; filename=\"att1.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"This is an attachment\r\n"
		"--MIME_boundary_0123456789--\r\n"
	);
	HTTPRequest req("POST", "/form.cgi");
	req.setContentType(HTMLForm::ENCODING_MULTIPART + "; boundary=\"MIME_boundary_0123456789\"");
	StringPartHandler sah;
	HTMLForm form(req, istr, sah);
	assertTrue (form.size() == 4);
	assertTrue (form["field1"] == "value1");
	assertTrue (form["field2"] == "value 2");
	assertTrue (form["field3"] == "value=3");
	assertTrue (form["field4"] == "value&4");

	assertTrue (sah.type() == "text/plain");
	assertTrue (sah.disp() == "file; name=\"attachment1\"; filename=\"att1.txt\"");
	assertTrue (sah.data() == "This is an attachment");
}


void HTMLFormTest::testSubmit1()
{
	HTMLForm form;
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");

	HTTPRequest req("GET", "/form.cgi");
	form.prepareSubmit(req);
	assertTrue (req.getURI() == "/form.cgi?field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
}


void HTMLFormTest::testSubmit2()
{
	HTMLForm form;
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");

	HTTPRequest req("POST", "/form.cgi");
	form.prepareSubmit(req);
	assertTrue (req.getContentType() == HTMLForm::ENCODING_URL);
	assertTrue (req.getContentLength() == 64);
}


void HTMLFormTest::testSubmit3()
{
	HTMLForm form(HTMLForm::ENCODING_MULTIPART);
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");

	HTTPRequest req("POST", "/form.cgi", HTTPMessage::HTTP_1_1);
	form.prepareSubmit(req);
	std::string expCT(HTMLForm::ENCODING_MULTIPART);
	expCT.append("; boundary=\"");
	expCT.append(form.boundary());
	expCT.append("\"");
	assertTrue (req.getContentType() == expCT);
	assertTrue (req.getChunkedTransferEncoding());
}


void HTMLFormTest::testSubmit4()
{
	HTMLForm form;
	form.add("field1", "value1");
	form.add("field1", "value 2");
	form.add("field1", "value=3");
	form.add("field1", "value&4");

	HTTPRequest req("GET", "/form.cgi");
	form.prepareSubmit(req);

	assertTrue (req.getURI() == "/form.cgi?field1=value1&field1=value%202&field1=value%3D3&field1=value%264");
}


void HTMLFormTest::testSubmit5()
{
	HTMLForm form(HTMLForm::ENCODING_MULTIPART);
	form.set("field1", "value1");
	form.set("field2", "value 2");
	form.set("field3", "value=3");
	form.set("field4", "value&4");

	HTTPRequest req("POST", "/form.cgi", HTTPMessage::HTTP_1_1);
	form.prepareSubmit(req, HTMLForm::OPT_USE_CONTENT_LENGTH);
	std::string expCT(HTMLForm::ENCODING_MULTIPART);
	expCT.append("; boundary=\"");
	expCT.append(form.boundary());
	expCT.append("\"");
	assertTrue (req.getContentType() == expCT);
	assertTrue (req.getContentLength() == 403);
}


void HTMLFormTest::testFieldLimitUrl()
{
	HTTPRequest req("GET", "/form.cgi?field1=value1&field2=value%202&field3=value%3D3&field4=value%264");
	HTMLForm form;
	form.setFieldLimit(3);
	try
	{
		form.load(req);
		fail("field limit violated - must throw");
	}
	catch (Poco::Net::HTMLFormException&)
	{
	}
}


void HTMLFormTest::testFieldLimitMultipart()
{
	std::istringstream istr(
		"\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field1\"\r\n"
		"\r\n"
		"value1\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field2\"\r\n"
		"\r\n"
		"value 2\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field3\"\r\n"
		"\r\n"
		"value=3\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"field4\"\r\n"
		"\r\n"
		"value&4\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: file; name=\"attachment1\"; filename=\"att1.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"This is an attachment\r\n"
		"--MIME_boundary_0123456789--\r\n"
	);
	HTTPRequest req("POST", "/form.cgi");
	req.setContentType(HTMLForm::ENCODING_MULTIPART + "; boundary=\"MIME_boundary_0123456789\"");
	StringPartHandler sah;
	HTMLForm form;
	form.setFieldLimit(3);
	try
	{
		form.load(req, istr, sah);
		fail("field limit violated - must throw");
	}
	catch (Poco::Net::HTMLFormException&)
	{
	}
}


void HTMLFormTest::setUp()
{
}


void HTMLFormTest::tearDown()
{
}


CppUnit::Test* HTMLFormTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTMLFormTest");

	CppUnit_addTest(pSuite, HTMLFormTest, testWriteUrl);
	CppUnit_addTest(pSuite, HTMLFormTest, testWriteMultipart);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadUrlGET);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadUrlGETMultiple);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadUrlPOST);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadUrlPUT);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadUrlBOM);
	CppUnit_addTest(pSuite, HTMLFormTest, testReadMultipart);
	CppUnit_addTest(pSuite, HTMLFormTest, testSubmit1);
	CppUnit_addTest(pSuite, HTMLFormTest, testSubmit2);
	CppUnit_addTest(pSuite, HTMLFormTest, testSubmit3);
	CppUnit_addTest(pSuite, HTMLFormTest, testSubmit4);
	CppUnit_addTest(pSuite, HTMLFormTest, testSubmit5);
	CppUnit_addTest(pSuite, HTMLFormTest, testFieldLimitUrl);
	CppUnit_addTest(pSuite, HTMLFormTest, testFieldLimitMultipart);

	return pSuite;
}
