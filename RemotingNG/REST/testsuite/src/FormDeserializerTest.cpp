//
// FormDeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FormDeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/FormDeserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


FormDeserializerTest::FormDeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


FormDeserializerTest::~FormDeserializerTest()
{
}


void FormDeserializerTest::testGetRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/?id=1234&childId=foo");
	FormDeserializer deserializer(request);

	int id;
	bool ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);

	std::string childId;
	ok = deserializer.deserialize("childId", true, childId);
	assert (ok);
	assert (childId == "foo");
}


void FormDeserializerTest::testPostRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/");
	std::stringstream str;
	str << "id=1234&childId=foo";
	FormDeserializer deserializer(request, str);

	int id;
	bool ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);

	std::string childId;
	ok = deserializer.deserialize("childId", true, childId);
	assert (ok);
	assert (childId == "foo");
}


void FormDeserializerTest::testPostRequestMultiPart()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/");
	request.setContentType(Poco::Net::HTMLForm::ENCODING_MULTIPART + "; boundary=MIME_boundary_0123456789");

	std::istringstream str(
		"\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"first\"\r\n"
		"\r\n"
		"value\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"second\"\r\n"
		"\r\n"
		"42\r\n"
		"--MIME_boundary_0123456789\r\n"
		"Content-Disposition: form-data; name=\"third\"; filename=\"att1.txt\"\r\n"
		"Content-Type: text/plain\r\n"
		"\r\n"
		"This is an attachment\r\n"
		"--MIME_boundary_0123456789--\r\n"
	);

	FormDeserializer deserializer(request, str);

	std::string first;
	bool ok = deserializer.deserialize("first", true, first);
	assert (ok);
	assert (first == "value");

	int second;
	ok = deserializer.deserialize("second", true, second);
	assert (ok);
	assert (second == 42);

	std::vector<char> third;
	ok = deserializer.deserialize("third", true, third);
	assert (ok);
	std::string expected("This is an attachment");
	assert (third == std::vector<char>(expected.begin(), expected.end()));
}


void FormDeserializerTest::testResponse()
{
	Poco::Net::HTTPResponse response;
	response.setContentType(Poco::Net::HTMLForm::ENCODING_URL);
	std::stringstream str;
	str << "id=1234&childId=foo";
	FormDeserializer deserializer(response, str);

	int id;
	bool ok = deserializer.deserialize("id", true, id);
	assert (ok);
	assert (id == 1234);

	std::string childId;
	ok = deserializer.deserialize("childId", true, childId);
	assert (ok);
	assert (childId == "foo");
}


void FormDeserializerTest::testTypes()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/?bool=false&double=3.5&int=42&string=foo");
	FormDeserializer deserializer(request);

	std::string stringValue;
	bool ok = deserializer.deserialize("string", true, stringValue);
	assert (ok);
	assert (stringValue == "foo");

	int intValue;
	ok = deserializer.deserialize("int", true, intValue);
	assert (ok);
	assert (intValue == 42);

	bool boolValue;
	ok = deserializer.deserialize("bool", true, boolValue);
	assert (ok);
	assert (boolValue == false);

	double doubleValue;
	ok = deserializer.deserialize("double", true, doubleValue);
	assert (ok);
	assert (doubleValue == 3.5);
}


void FormDeserializerTest::setUp()
{
}


void FormDeserializerTest::tearDown()
{
}


CppUnit::Test* FormDeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FormDeserializerTest");

	CppUnit_addTest(pSuite, FormDeserializerTest, testGetRequest);
	CppUnit_addTest(pSuite, FormDeserializerTest, testPostRequest);
	CppUnit_addTest(pSuite, FormDeserializerTest, testPostRequestMultiPart);
	CppUnit_addTest(pSuite, FormDeserializerTest, testResponse);
	CppUnit_addTest(pSuite, FormDeserializerTest, testTypes);

	return pSuite;
}
