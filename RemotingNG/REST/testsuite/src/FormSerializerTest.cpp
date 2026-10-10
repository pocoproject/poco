//
// FormSerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FormSerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/FormSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


FormSerializerTest::FormSerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


FormSerializerTest::~FormSerializerTest()
{
}


void FormSerializerTest::testGetRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/"s);
	FormSerializer serializer(request);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (request.getURI() == "/?id=1234&childId=foo");
}


void FormSerializerTest::testPostRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	std::ostringstream ostr;
	FormSerializer serializer(request, ostr);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (request.getURI() == "/");
	assert (ostr.str() == "id=1234&childId=foo");
}


void FormSerializerTest::testPostMultipartRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	request.setContentType("multipart/form-data"s);
	std::ostringstream ostr;
	FormSerializer serializer(request, ostr);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (request.getURI() == "/");
	std::string str = ostr.str();
	assert (str.substr(0, 16) == "--MIME_boundary_");
	assert (str.find("Content-Disposition: form-data; name=\"id\"\r\n") != std::string::npos);
	assert (str.find("1234\r\n") != std::string::npos);
	assert (str.find("Content-Disposition: form-data; name=\"childId\"\r\n") != std::string::npos);
	assert (str.find("foo\r\n") != std::string::npos);
}


void FormSerializerTest::testPostMultipartCharVectorRequest()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	request.setContentType("multipart/form-data"s);
	std::ostringstream ostr;
	FormSerializer serializer(request, ostr);
	serializer.serialize("id"s, 1234);
	std::vector<char> data;
	std::string s("testdata");
	data.assign(s.begin(), s.end());
	serializer.serialize("data"s, data);
	serializer.close();
	assert (request.getURI() == "/");
	std::string str = ostr.str();
	assert (str.substr(0, 16) == "--MIME_boundary_");
	assert (str.find("Content-Disposition: form-data; name=\"id\"\r\n") != std::string::npos);
	assert (str.find("1234\r\n") != std::string::npos);
	assert (str.find("Content-Disposition: form-data; name=\"data\"; filename=\"data\"\r\n") != std::string::npos);
	assert (str.find("Content-Type: application/octet-stream\r\n") != std::string::npos);
	assert (str.find("testdata\r\n") != std::string::npos);
}


void FormSerializerTest::testPostMultipartCharVectorRequestWithContentType()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	request.setContentType("multipart/form-data"s);
	std::ostringstream ostr;
	FormSerializer serializer(request, ostr);
	serializer.serialize("id"s, 1234);
	std::vector<char> data;
	std::string s("testdata");
	data.assign(s.begin(), s.end());
	serializer.pushProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, "image/jpeg"s);
	serializer.serialize("data"s, data);
	serializer.popProperty(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE);
	serializer.close();
	assert (request.getURI() == "/");
	std::string str = ostr.str();
	assert (str.substr(0, 16) == "--MIME_boundary_");
	assert (str.find("Content-Disposition: form-data; name=\"id\"\r\n") != std::string::npos);
	assert (str.find("1234\r\n") != std::string::npos);
	assert (str.find("Content-Disposition: form-data; name=\"data\"; filename=\"data\"\r\n") != std::string::npos);
	assert (str.find("Content-Type: image/jpeg\r\n") != std::string::npos);
	assert (str.find("testdata\r\n") != std::string::npos);
}


void FormSerializerTest::testResponse()
{
	Poco::Net::HTTPResponse response;
	std::ostringstream ostr;
	FormSerializer serializer(response, ostr);
	serializer.serialize("id"s, 1234);
	serializer.serialize("childId"s, "foo"s);
	serializer.close();
	assert (ostr.str() == "id=1234&childId=foo");
}


void FormSerializerTest::testTypes()
{
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/");
	std::ostringstream ostr;
	FormSerializer serializer(request, ostr);
	serializer.serialize("bool"s, false);
	serializer.serialize("double"s, 3.5);
	serializer.serialize("int"s, static_cast<Poco::Int32>(42));
	serializer.serialize("string"s, "foo"s);
	serializer.close();
	assert (ostr.str() == "bool=false&double=3.5&int=42&string=foo");
}


void FormSerializerTest::setUp()
{
}


void FormSerializerTest::tearDown()
{
}


CppUnit::Test* FormSerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FormSerializerTest");

	CppUnit_addTest(pSuite, FormSerializerTest, testGetRequest);
	CppUnit_addTest(pSuite, FormSerializerTest, testPostRequest);
	CppUnit_addTest(pSuite, FormSerializerTest, testPostMultipartRequest);
	CppUnit_addTest(pSuite, FormSerializerTest, testPostMultipartCharVectorRequest);
	CppUnit_addTest(pSuite, FormSerializerTest, testPostMultipartCharVectorRequestWithContentType);
	CppUnit_addTest(pSuite, FormSerializerTest, testResponse);
	CppUnit_addTest(pSuite, FormSerializerTest, testTypes);

	return pSuite;
}
