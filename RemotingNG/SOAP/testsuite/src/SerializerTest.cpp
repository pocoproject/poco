//
// SerializerTest.cpp
//
// Copyright (c) 2018-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "SerializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include <sstream>


using namespace Poco::RemotingNG::SOAP;
using namespace std::string_literals;


SerializerTest::SerializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


SerializerTest::~SerializerTest()
{
}


void SerializerTest::testRequestNoParams11()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestNoParams11DeclareEncNS()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:soap-enc=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::OPT_DECLARE_SOAPENC_NAMESPACE);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestNoParams11NoEncodingStyle()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::OPT_NO_ENCODINGSTYLE_ATTRIBUTE);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyNoParams11()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serializeMessageEnd("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestTwoParams11()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("arg1"s, "value1"s);
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyTwoParams11()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</ns1:methodResponse>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serialize("arg1"s, std::string("value1"));
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testFault11()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:remoting=\"http://www.appinf.com/remoting\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<soap:Fault>"
		"<faultcode>soap:Server</faultcode>"
		"<faultstring>Exception: Some exception: some arg</faultstring>"
		"<detail>"
		"<remoting:Exception>"
		"<remoting:Type>Exception</remoting:Type>"
		"<remoting:Message>Some exception: some arg</remoting:Message>"
		"<remoting:Code>0</remoting:Code>"
		"</remoting:Exception>"
		"</detail>"
		"</soap:Fault>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	Poco::Exception e("Some exception"s, "some arg"s);
	ser.serializeFaultMessage(""s, e);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestNoParams12()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyNoParams12()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serializeMessageEnd("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestTwoParams12()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("arg1"s, "value1"s);
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyTwoParams12()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</ns1:methodResponse>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("methodResponse"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serialize("arg1"s, "value1"s);
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("methodResponse", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testFault12()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:remoting=\"http://www.appinf.com/remoting\" xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\">"
		"<soap:Body>"
		"<soap:Fault>"
		"<soap:Code><soap:Value>soap:Receiver</soap:Value></soap:Code>"
		"<soap:Reason><soap:Text xml:lang=\"en\">Exception: Some exception: some arg</soap:Text></soap:Reason>"
		"<soap:Detail>"
		"<remoting:Exception>"
		"<remoting:Type>Exception</remoting:Type>"
		"<remoting:Message>Some exception: some arg</remoting:Message>"
		"<remoting:Code>0</remoting:Code>"
		"</remoting:Exception>"
		"</soap:Detail>"
		"</soap:Fault>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	Poco::Exception e("Some exception"s, "some arg"s);
	ser.serializeFaultMessage("", e);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestNoParamsDL()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body/>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyNoParamsDL()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body/>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serializeMessageEnd("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestTwoParamsDL()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("arg1"s, "value1"s);
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("#", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testReplyTwoParamsDL()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:arg1>value1</ns1:arg1>"
		"<ns1:arg2>42</ns1:arg2>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	ser.serialize("arg1"s, "value1"s);
	ser.serialize("arg2"s, 42);
	ser.serializeMessageEnd("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestTwoAttrs()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testRequestWithAttrs()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:value>false</ns1:value>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.serialize("value"s, false);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testComplexRequest()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:status>"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.serializeStructBegin("status"s);
	ser.serialize("failure"s, false);
	ser.serializeStructEnd("status"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testComplexRequestNS()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:status xmlns:ns2=\"urn:schemas-appinf-com:schemas:MySchema:1.0\">"
		"<ns2:failure>false</ns2:failure>"
		"</ns1:status>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.registerNamespace("urn:schemas-appinf-com:schemas:MySchema:1.0"s);
	ser.serializeStructBegin("status"s);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:schemas:MySchema:1.0"s);
	ser.serialize("failure"s, false);
	ser.popProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE);
	ser.serializeStructEnd("status"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testVector()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns2:vec xmlns:ns2=\"urn:schemas-appinf-com:service:MySchema:1.0\">0</ns2:vec>"
		"<ns3:vec xmlns:ns3=\"urn:schemas-appinf-com:service:MySchema:1.0\">1</ns3:vec>"
		"<ns4:vec xmlns:ns4=\"urn:schemas-appinf-com:service:MySchema:1.0\">2</ns4:vec>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	std::vector<int> vec;
	vec.push_back(0);
	vec.push_back(1);
	vec.push_back(2);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MySchema:1.0"s);
	Poco::RemotingNG::TypeSerializer<std::vector<int> >::serialize("vec", vec, ser);
	ser.popProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testSubElemAttrs()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:status ns2:attr3=\"3\" xmlns:ns2=\"urn:schemas-appinf-com:service:MySchema:1.0\">"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0");
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.pushAttribute("urn:schemas-appinf-com:service:MySchema:1.0"s, "attr3"s);
	ser.serializeStructBegin("status"s);
	ser.serialize("attr3"s, 3);
	ser.serialize("failure"s, false);
	ser.serializeStructEnd("status"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}



void SerializerTest::testHeader()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Header>"
		"<ns1:status xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</soap:Header>"
		"<soap:Body>"
		"<ns2:method attr1=\"value1\" attr2=\"2\" xmlns:ns2=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_HEADER, "status"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.serializeStructBegin("status"s);
	ser.serialize("failure"s, false);
	ser.serializeStructEnd("status"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testHeaderAttr()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Header>"
		"<ns1:status attr3=\"3\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</soap:Header>"
		"<soap:Body>"
		"<ns2:method attr1=\"value1\" attr2=\"2\" xmlns:ns2=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_HEADER, "status"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.pushAttribute(""s, "attr2"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, 2);
	ser.pushAttribute(""s, "attr3"s);
	ser.serializeStructBegin("status"s);
	ser.serialize("attr3"s, 3);
	ser.serialize("failure"s, false);
	ser.serializeStructEnd("status"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}



void SerializerTest::testCharVector()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::vector<char> vec;
	std::string data("Hello, world!"s);
	vec.insert(vec.end(), data.begin(), data.end());
	ser.serialize("arg1"s, vec);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testCharVectorNil()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1 xsi:nil=\"true\"/>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeNullableBegin("arg1"s, true);
	ser.serializeNullableEnd("arg1"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testCharVectorNotNil()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::vector<char> vec;
	std::string data("Hello, world!");
	vec.insert(vec.end(), data.begin(), data.end());
	ser.serializeNullableBegin("arg1"s, false);
	ser.serialize("arg1"s, vec);
	ser.serializeNullableEnd("arg1"s);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testNilNS()
{
	const std::string expectedResult(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns2:arg1 xmlns:ns2=\"urn:schemas-appinf-com:service:MySchema:1.0\" xsi:nil=\"true\"/>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MySchema:1.0"s);
	ser.serializeNullableBegin("arg1"s, true);
	ser.serializeNullableEnd("arg1"s);
	ser.popProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testMTOM()
{
	const std::string expectedResult(
		"--MIME_boundary_****************\r\n"
		"Content-Type: application/xop+xml; charset=\"utf-8\"; type=\"application/soap+xml\"\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-ID: <********-****-****-****-************@appinf.com>\r\n"
		"\r\n"
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"<ns1:arg2><ns2:Include href=\"cid:********-****-****-****-************@appinf.com\" xmlns:ns2=\"http://www.w3.org/2004/08/xop/include\"/></ns1:arg2>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>\r\n"
		"--MIME_boundary_****************\r\n"
		"Content-Type: application/binary\r\n"
		"Content-Transfer-Encoding: binary\r\n"
		"Content-ID: <********-****-****-****-************@appinf.com>\r\n"
		"\r\n"
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis porta, augue non sodales eleifend, arcu neque accumsan nibh, sit amet auctor risus purus et lacus.\r\n"
		"--MIME_boundary_****************--\r\n"
	);

	Serializer ser(Serializer::SOAP_1_2);
	std::ostringstream ostr;
	ser.enableMTOM(true);
	std::string ct = ser.createMTOMContentType();
	assert (!ct.empty());
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::vector<char> arg1;
	std::string data1("Hello, world!");
	arg1.insert(arg1.end(), data1.begin(), data1.end());
	ser.serialize("arg1"s, arg1);
	std::vector<char> arg2;
	std::string data2("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis porta, augue non sodales eleifend, arcu neque accumsan nibh, sit amet auctor risus purus et lacus.");
	arg2.insert(arg2.end(), data2.begin(), data2.end());
	ser.serialize("arg2"s, arg2);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result.size() == expectedResult.size());

	for (int i = 0; i < result.size(); i++)
	{
		assert (expectedResult[i] == '*' || result[i] == expectedResult[i]);
	}
}


void SerializerTest::testStructAsRoot()
{
	const std::string expectedResult(
		"<ns1:object attr1=\"value1\" xmlns:ns1=\"urn:schemas-appinf-com:service:MySchema:1.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<ns1:attr2>value2</ns1:attr2>"
		"<ns1:subobject ns2:attr3=\"value3\" xmlns:ns2=\"urn:schemas-appinf-com:service:MyOtherSchema:1.0\">"
		"<ns2:attr4>value4</ns2:attr4>"
		"</ns1:subobject>"
		"</ns1:object>"
	);

	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MySchema:1.0"s);
	ser.pushAttribute(""s, "attr1"s);
	ser.serializeStructBegin("object"s);
	ser.serialize("attr1"s, "value1"s);
	ser.serialize("attr2"s, "value2"s);
	ser.registerNamespace("urn:schemas-appinf-com:service:MyOtherSchema:1.0"s);
	ser.pushAttribute("urn:schemas-appinf-com:service:MyOtherSchema:1.0"s, "attr3"s);
	ser.serializeStructBegin("subobject");
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyOtherSchema:1.0"s);
	ser.serialize("attr3"s, "value3"s);
	ser.serialize("attr4"s, "value4"s);
	ser.popProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE);
	ser.serializeStructEnd("subobject"s);
	ser.serializeStructEnd("object"s);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testDateTime()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:date>2024-02-01</ns1:date>"
		"<ns1:time>10:11:12.000000</ns1:time>"
		"<ns1:dateTime>2024-02-01T10:11:12.000000Z</ns1:dateTime>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Poco::DateTime dt(2024,2,1,10,11,12);
	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	ser.serialize("date"s, dt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "time"s);
	ser.serialize("time"s, dt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "dateTime"s);
	ser.serialize("dateTime"s, dt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testLocalDateTime()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:date>2024-02-01</ns1:date>"
		"<ns1:time>11:11:12.000000+01:00</ns1:time>"
		"<ns1:dateTime>2024-02-01T11:11:12.000000+01:00</ns1:dateTime>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Poco::DateTime dt(2024,2,1,10,11,12);
	Poco::LocalDateTime ldt(3600, dt);
	Serializer ser;
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	ser.serialize("date"s, ldt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "time"s);
	ser.serialize("time"s, ldt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "dateTime"s);
	ser.serialize("dateTime"s, ldt);
	ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	ser.serializeMessageEnd("method"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert (result == expectedResult);
}


void SerializerTest::testInterceptor()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Header>"
		"<ns1:simple>"
		"value"
		"</ns1:simple>"
		"</soap:Header>"
		"<soap:Body/>"
		"</soap:Envelope>"
	);

	class TestSerInterceptor: public Serializer::HeaderInterceptor
	{
	public:
		void interceptHeader(XMLElement& header)
		{
			header.addChild(XMLElement("simple"s, "urn:schemas-appinf-com:service:MyService:1.0"s, "value"s));
		}
	};

	Serializer ser;
	ser.addHeaderInterceptor(new TestSerInterceptor);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert(result == expectedResult);
}


void SerializerTest::testInterceptorComplex()
{
	const std::string expectedResult(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Header>"
		"<ns1:complex ns1:attr=\"true\">"
		"<ns1:simple>value</ns1:simple>"
		"</ns1:complex>"
		"</soap:Header>"
		"<soap:Body/>"
		"</soap:Envelope>"
	);

	class TestCmplSerInterceptor: public Serializer::HeaderInterceptor
	{
	public:
		void interceptHeader(XMLElement& header)
		{
			XMLElement complex("complex"s, "urn:schemas-appinf-com:service:MyService:1.0"s);
			complex.setAttribute("attr"s, "urn:schemas-appinf-com:service:MyService:1.0"s, "true"s);
			complex.addChild(XMLElement("simple"s, "urn:schemas-appinf-com:service:MyService:1.0"s, "value"s));
			header.addChild(std::move(complex));
		}
	};

	Serializer ser;
	ser.addHeaderInterceptor(new TestCmplSerInterceptor);
	std::ostringstream ostr;
	ser.setup(ostr);
	ser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyService:1.0"s);
	ser.serializeMessageBegin("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	ser.serializeMessageEnd("#"s, Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string result = ostr.str();
	assert(result == expectedResult);
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

	CppUnit_addTest(pSuite, SerializerTest, testRequestNoParams11);
	CppUnit_addTest(pSuite, SerializerTest, testRequestNoParams11DeclareEncNS);
	CppUnit_addTest(pSuite, SerializerTest, testRequestNoParams11NoEncodingStyle);
	CppUnit_addTest(pSuite, SerializerTest, testReplyNoParams11);
	CppUnit_addTest(pSuite, SerializerTest, testRequestTwoParams11);
	CppUnit_addTest(pSuite, SerializerTest, testReplyTwoParams11);
	CppUnit_addTest(pSuite, SerializerTest, testFault11);

	CppUnit_addTest(pSuite, SerializerTest, testRequestNoParams12);
	CppUnit_addTest(pSuite, SerializerTest, testReplyNoParams12);
	CppUnit_addTest(pSuite, SerializerTest, testRequestTwoParams12);
	CppUnit_addTest(pSuite, SerializerTest, testReplyTwoParams12);
	CppUnit_addTest(pSuite, SerializerTest, testFault12);

	CppUnit_addTest(pSuite, SerializerTest, testRequestNoParamsDL);
	CppUnit_addTest(pSuite, SerializerTest, testReplyNoParamsDL);
	CppUnit_addTest(pSuite, SerializerTest, testRequestTwoParamsDL);
	CppUnit_addTest(pSuite, SerializerTest, testReplyTwoParamsDL);

	CppUnit_addTest(pSuite, SerializerTest, testRequestTwoAttrs);
	CppUnit_addTest(pSuite, SerializerTest, testRequestWithAttrs);
	CppUnit_addTest(pSuite, SerializerTest, testComplexRequest);
	CppUnit_addTest(pSuite, SerializerTest, testComplexRequestNS);
	CppUnit_addTest(pSuite, SerializerTest, testVector);
	CppUnit_addTest(pSuite, SerializerTest, testSubElemAttrs);
	CppUnit_addTest(pSuite, SerializerTest, testHeader);
	CppUnit_addTest(pSuite, SerializerTest, testHeaderAttr);

	CppUnit_addTest(pSuite, SerializerTest, testCharVector);
	CppUnit_addTest(pSuite, SerializerTest, testCharVectorNil);
	CppUnit_addTest(pSuite, SerializerTest, testCharVectorNotNil);
	CppUnit_addTest(pSuite, SerializerTest, testNilNS);
	CppUnit_addTest(pSuite, SerializerTest, testMTOM);

	CppUnit_addTest(pSuite, SerializerTest, testStructAsRoot);

	CppUnit_addTest(pSuite, SerializerTest, testDateTime);
	CppUnit_addTest(pSuite, SerializerTest, testLocalDateTime);

	CppUnit_addTest(pSuite, SerializerTest, testInterceptor);
	CppUnit_addTest(pSuite, SerializerTest, testInterceptorComplex);

	return pSuite;
}
