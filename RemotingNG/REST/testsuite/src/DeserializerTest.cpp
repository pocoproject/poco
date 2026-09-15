//
// DeserializerTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DeserializerTest.h"
#include "ObjectDeserializer.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


DeserializerTest::DeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


DeserializerTest::~DeserializerTest()
{
}


void DeserializerTest::testGet()
{
	Deserializer deser;
	std::stringstream stream;
	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/tester/1234?options=test");
	deser.setRequest(request);

	std::string method;
	assert (deser.findMessage(method) == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (method == "get");

	int id;
	std::string options("none");
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("get", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "query");
	Poco::RemotingNG::TypeDeserializer<std::string >::deserialize("options", false, deser, options);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.deserializeMessageEnd("get", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (options == "test");
}


void DeserializerTest::testPostForm()
{
	Deserializer deser;
	std::stringstream stream;
	stream << "text=foo%20bar";

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	deser.setRequest(request);

	int id;
	std::string text;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "form");
	Poco::RemotingNG::TypeDeserializer<std::string >::deserialize("text", true, deser, text);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.deserializeMessageEnd("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (text == "foo bar");
}


void DeserializerTest::testPostJSONObject()
{
	Deserializer deser;
	std::stringstream stream;
	stream <<
		"{\n"
		"\t\"data\": \"foo\\u001Ebar\"\n"
		"}";

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	deser.setRequest(request);

	int id;
	Object object;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeDeserializer<Object >::deserialize("object", true, deser, object);
	deser.deserializeMessageEnd("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (object.data == "foo\036bar");
}


void DeserializerTest::testPostJSONArray()
{
	Deserializer deser;
	std::stringstream stream;
	stream <<
		"[\n"
		"\t\"foo\",\n"
		"\t\"bar\"\n"
		"]";

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	deser.setRequest(request);

	int id;
	std::vector<std::string> array;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeDeserializer<std::vector<std::string> >::deserialize("array", true, deser, array);
	deser.deserializeMessageEnd("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (array.size() == 2);
	assert (array[0] == "foo");
	assert (array[1] == "bar");
}


void DeserializerTest::testPostRaw()
{
	Deserializer deser;
	std::stringstream stream;
	stream << "foo bar";

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	deser.setRequest(request);

	std::string method;
	assert (deser.findMessage(method) == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (method == "post");

	int id;
	std::string text;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "body");
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, "raw");
	Poco::RemotingNG::TypeDeserializer<std::string >::deserialize("text", true, deser, text);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.deserializeMessageEnd("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (text == "foo bar");
}


void DeserializerTest::testPostRawHeader()
{
	Deserializer deser;
	std::stringstream stream;
	stream << "foo bar";

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	request.set("X-Header", "Foo Bar");
	deser.setRequest(request);

	std::string method;
	assert (deser.findMessage(method) == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (method == "post");

	int id;
	std::string text;
	std::string header;

	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/tester/{id}");
	deser.deserializeMessageBegin("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path");
	Poco::RemotingNG::TypeDeserializer<int >::deserialize("id", true, deser, id);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "body");
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT, "raw");
	Poco::RemotingNG::TypeDeserializer<std::string >::deserialize("text", true, deser, text);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_FORMAT);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "header");
	Poco::RemotingNG::TypeDeserializer<std::string >::deserialize("X-Header", true, deser, header);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	deser.deserializeMessageEnd("post", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);

	assert (id == 1234);
	assert (text == "foo bar");
	assert (header == "Foo Bar");
}


void DeserializerTest::testMethodOverride()
{
	Deserializer deser;
	std::stringstream stream;

	deser.setup(stream);
	Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/tester/1234");
	request.set("X-HTTP-Method-Override", "DELETE");
	deser.setRequest(request);

	std::string method;
	assert (deser.findMessage(method) == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (method == "delete_");
}


void DeserializerTest::setUp()
{
}


void DeserializerTest::tearDown()
{
}


CppUnit::Test* DeserializerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DeserializerTest");

	CppUnit_addTest(pSuite, DeserializerTest, testGet);
	CppUnit_addTest(pSuite, DeserializerTest, testPostForm);
	CppUnit_addTest(pSuite, DeserializerTest, testPostJSONObject);
	CppUnit_addTest(pSuite, DeserializerTest, testPostJSONArray);
	CppUnit_addTest(pSuite, DeserializerTest, testPostRaw);
	CppUnit_addTest(pSuite, DeserializerTest, testPostRawHeader);
	CppUnit_addTest(pSuite, DeserializerTest, testMethodOverride);

	return pSuite;
}
