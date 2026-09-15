//
// SerializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SerializerTest.h"
#include "ObjectSerializer.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;
using namespace std::string_literals;


SerializerTest::SerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


SerializerTest::~SerializerTest()
{
}


void SerializerTest::testGet()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request;
	ser.setRequest(request);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("get"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "query"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("options"s, "test"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.serializeMessageEnd("get"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234?options=test");
}


void SerializerTest::testPostForm()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	ser.setRequest(request);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "form"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("string", "foo bar"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string form = stream.str();
	assert (form == "string=foo%20bar");
}


void SerializerTest::testPostFormMultiPart()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	request.setContentType("multipart/form-data"s);
	ser.setRequest(request);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "form"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("string"s, "foo bar"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string form = stream.str();
	assert (form.substr(0, 16) == "--MIME_boundary_");
	assert (form.find("Content-Disposition: form-data; name=\"string\"\r\n\r\n") != std::string::npos);
	assert (form.find("foo bar\r\n") != std::string::npos);
}


void SerializerTest::testPostJSONObject()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	ser.setRequest(request);

	Object object;
	object.data = "foo\036bar";

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeSerializer<Object >::serialize("object"s, object, ser);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string json = stream.str();
	assert (json ==
		"{\n"
		"\t\"data\": \"foo\\u001Ebar\"\n"
		"}");
}


void SerializerTest::testPostJSONArray()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	ser.setRequest(request);

	std::vector<std::string> array;
	array.push_back("foo"s);
	array.push_back("bar"s);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id", 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeSerializer<std::vector<std::string> >::serialize("array"s, array, ser);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string json = stream.str();
	assert (json ==
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]");
}


void SerializerTest::testPostRaw()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	ser.setRequest(request);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "body"s);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, "raw"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("string"s, "foo bar"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string form = stream.str();
	assert (form == "foo bar");
}


void SerializerTest::testPostRawHeader()
{
	Serializer ser;
	std::stringstream stream;
	ser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/"s);
	ser.setRequest(request);

	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}"s);
	ser.serializeMessageBegin("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize("id"s, 1234, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "body"s);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, "raw"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("string"s, "foo bar"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "header"s);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize("X-Header"s, "Foo Bar"s, ser);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	ser.serializeMessageEnd("post"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	std::string uri = request.getURI();
	assert (uri == "/tester/1234");

	std::string form = stream.str();
	assert (form == "foo bar");

	std::string header = request.get("X-Header");
	assert (header == "Foo Bar");
}


void SerializerTest::setUp()
{
}


void SerializerTest::tearDown()
{
}


CppUnit::Test* SerializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerializerTest");

	CppUnit_addTest(pSuite, SerializerTest, testGet);
	CppUnit_addTest(pSuite, SerializerTest, testPostForm);
	CppUnit_addTest(pSuite, SerializerTest, testPostFormMultiPart);
	CppUnit_addTest(pSuite, SerializerTest, testPostJSONObject);
	CppUnit_addTest(pSuite, SerializerTest, testPostJSONArray);
	CppUnit_addTest(pSuite, SerializerTest, testPostRaw);
	CppUnit_addTest(pSuite, SerializerTest, testPostRawHeader);

	return pSuite;
}
