//
// DeserializerTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DeserializerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/SOAP/Deserializer.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include <sstream>


using namespace Poco::RemotingNG::SOAP;
using namespace std::string_literals;


DeserializerTest::DeserializerTest(const std::string& name): CppUnit::TestCase(name)
{
}


DeserializerTest::~DeserializerTest()
{
}


void DeserializerTest::testRequestNoParams11()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	assert (deser.soapVersion() == Serializer::SOAP_1_1);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testReplyNoParams11()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	deser.deserializeMessageBegin("methodResponse", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	deser.deserializeMessageEnd("methodResponse", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testRequestTwoParams11()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">\n"
		"\t\t\t<arg1>value1</arg1>\n"
		"\t\t\t<arg2>42</arg2>\n"
		"\t\t</ns1:method>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testReplyTwoParams11()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">\n"
		"\t\t\t<arg1>value1</arg1>\n"
		"\t\t\t<arg2>42</arg2>\n"
		"\t\t</ns1:methodResponse>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name = "methodResponse";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testFault11()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<s:Fault>\n"
		"\t\t\t<faultcode>s:Client</faultcode>\n"
		"\t\t\t<faultstring>Some Error</faultstring>\n"
		"\t\t</s:Fault>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name = "Dummy";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	try
	{
		deser.deserializeMessageBegin(name, mt);
		deser.deserializeMessageEnd(name, mt);
		fail("deserializing fault - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.displayText() == "SOAP Fault: Some Error");
	}
	catch (CppUnit::CppUnitException&)
	{
		throw;
	}
	catch (...)
	{
		fail("expected a Poco::RemotingNG::RemoteException");
	}
}


void DeserializerTest::testRequestNoParams12()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);

	assert (deser.soapVersion() == Serializer::SOAP_1_2);

	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testReplyNoParams12()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	deser.deserializeMessageBegin("methodResponse", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	deser.deserializeMessageEnd("methodResponse", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testRequestTwoParams12()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">\n"
		"\t\t\t<arg1>value1</arg1>\n"
		"\t\t\t<arg2>42</arg2>\n"
		"\t\t</ns1:method>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testReplyTwoParams12()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"\t<s:Body>\n"
		"\t\t<ns1:methodResponse xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">\n"
		"\t\t\t<arg1>value1</arg1>\n"
		"\t\t\t<arg2>42</arg2>\n"
		"\t\t</ns1:methodResponse>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name = "methodResponse";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testFault12()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\n"
		"\t<s:Body>\n"
		"\t\t<s:Fault>\n"
		"\t\t\t<s:Code><s:Value>s:Client</s:Value></s:Code>\n"
		"\t\t\t<s:Reason><s:Text>Some Error</s:Text></s:Reason>\n"
		"\t\t</s:Fault>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name = "Dummy";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	try
	{
		deser.deserializeMessageBegin(name, mt);
		deser.deserializeMessageEnd(name, mt);
		fail("deserializing fault - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.displayText() == "SOAP Fault: Some Error");
	}
	catch (CppUnit::CppUnitException&)
	{
		throw;
	}
	catch (...)
	{
		fail("expected a Poco::RemotingNG::RemoteException");
	}
}


void DeserializerTest::testRequestNoParamsDL()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body/>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	assert (deser.soapVersion() == Serializer::SOAP_1_1);
	
	try
	{
		deser.deserializeMessageBegin("#", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		deser.deserializeMessageEnd("#", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	}
	catch (Poco::Exception& exc)
	{
		std::cout << exc.displayText() << std::endl;
	}
}


void DeserializerTest::testReplyNoParamsDL()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body/>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	deser.deserializeMessageBegin("#", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	deser.deserializeMessageEnd("#", Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
}


void DeserializerTest::testRequestTwoParamsDL()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<arg1>value1</arg1>\n"
		"\t\t<arg2>42</arg2>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);

	std::string name = "#";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testReplyTwoParamsDL()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Body>\n"
		"\t\t<arg1>value1</arg1>\n"
		"\t\t<arg2>42</arg2>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name = "#";
	Poco::RemotingNG::SerializerBase::MessageType mt = Poco::RemotingNG::SerializerBase::MESSAGE_REPLY;
	
	deser.deserializeMessageBegin(name, mt);
	std::string arg1;
	deser.deserialize("arg1", true, arg1);
	assert (arg1 == "value1");
	int arg2;
	deser.deserialize("arg2", true, arg2);
	assert (arg2 == 42);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testRequestTwoAttrs()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string attr1;
	int attr2;
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (attr1 == "value1");
	assert (attr2 == 2);
}


void DeserializerTest::testRequestWithAttrs()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:success>false</ns1:success>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string attr1;
	int attr2;
	bool success = true;
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.deserialize("success", true, success);
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (attr1 == "value1");
	assert (attr2 == 2);
	assert (success == false);
}


void DeserializerTest::testComplexRequest()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" attr2=\"2\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:status>"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string attr1;
	int attr2;
	bool failure = true;
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.deserializeStructBegin("status", true);
	deser.deserialize("failure", true, failure);
	deser.deserializeStructEnd("status");
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (attr1 == "value1");
	assert (attr2 == 2);
	assert (failure == false);
}


void DeserializerTest::testRequestWithAttrsInc()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	try
	{
		deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
		fail("must fail");
	}
	catch(Poco::Exception&)
	{
	}
}


void DeserializerTest::testRequestWithAttrsOptional()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<ns1:method attr1=\"value1\" xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:status>"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", false);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	std::string attr1;
	int attr2 = 0;
	bool failure = true;
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", false, attr2);
	deser.deserializeStructBegin("status", true);
	deser.deserialize("failure", true, failure);
	deser.deserializeStructEnd("status");
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (attr1 == "value1");
	assert (attr2 == 0);
	assert (failure == false);
}


void DeserializerTest::testVector()
{
	const std::string input(
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

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	std::vector<int> vec;
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeDeserializer<std::vector<int> >::deserialize("vec", true, deser, vec);
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (vec.size() == 3);
	assert (vec[0] == 0);
	assert (vec[1] == 1);
	assert (vec[2] == 2);
}


void DeserializerTest::testHeader()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
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

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	bool failure = true;
	std::string attr1;
	int attr2 = 0;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, "status");
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.deserializeStructBegin("status", true);
	deser.deserialize("failure", true, failure);
	deser.deserializeStructEnd("status");
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (attr1 == "value1");
	assert (attr2 == 2);
	assert (failure == false);
}


void DeserializerTest::testHeaderAttr()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
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

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	bool failure = true;
	std::string attr1;
	int attr2 = 0;
	int attr3 = 0;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, "status");
	deser.pushAttribute("", "attr1", true);
	deser.pushAttribute("", "attr2", true);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.pushAttribute("", "attr3", true);
	deser.deserializeStructBegin("status", true);
	deser.deserialize("attr3", true, attr3);
	deser.deserialize("failure", true, failure);
	deser.deserializeStructEnd("status");
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);

	assert (attr1 == "value1");
	assert (attr2 == 2);
	assert (attr3 == 3);
	assert (failure == false);
}


void DeserializerTest::testHeaderAttrOptional()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Header>"
		"<ns1:status xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:failure>false</ns1:failure>"
		"</ns1:status>"
		"</soap:Header>"
		"<soap:Body>"
		"<ns2:method xmlns:ns2=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	bool failure = true;
	std::string attr1;
	int attr2 = 0;
	int attr3 = 0;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER, "status");
	deser.pushAttribute("", "attr1", false);
	deser.pushAttribute("", "attr2", false);
	deser.deserializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	deser.deserialize("attr1", false, attr1);
	deser.deserialize("attr2", false, attr2);
	deser.pushAttribute("", "attr3", false);
	deser.deserializeStructBegin("status", true);
	deser.deserialize("attr3", false, attr3);
	deser.deserialize("failure", true, failure);
	deser.deserializeStructEnd("status");
	deser.deserializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);

	assert (attr1.empty());
	assert (attr2 == 0);
	assert (attr3 == 0);
	assert (failure == false);
}



void DeserializerTest::testCharVector()
{
	const std::string input(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	std::vector<char> arg1;
	deser.deserialize("arg1", true, arg1);
	std::string tmp(arg1.begin(), arg1.end());
	assert (tmp == "Hello, world!");
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testCharVectorNil1()
{
	const std::string input(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1 xsi:nil=\"true\"/>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	bool isNull = false;
	deser.deserializeNullableBegin("arg1", true, isNull);
	assert (isNull);
	deser.deserializeNullableEnd("arg1");
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testCharVectorNil2()
{
	const std::string input(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1 xsi:nil=\"1\"/>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	bool isNull = false;
	deser.deserializeNullableBegin("arg1", true, isNull);
	assert (isNull);
	deser.deserializeNullableEnd("arg1");
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testCharVectorNotNil()
{
	const std::string input(
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	std::vector<char> arg1;
	bool isNil = false;
	deser.deserializeNullableBegin("arg1", true, isNil);
	assert (!isNil);
	deser.deserialize("arg1", true, arg1);
	std::string tmp(arg1.begin(), arg1.end());
	assert (tmp == "Hello, world!");
	deser.deserializeNullableEnd("arg1");
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testMTOM()
{
	const std::string input(
		"--MIME_boundary_54B45DB81FC04D12\r\n"
		"Content-ID: <3ebdaaf0-9250-48e1-b8cb-eed56bb9b42b@appinf.com>\r\n"
		"Content-Transfer-Encoding: 8bit\r\n"
		"Content-Type: application/xop+xml; charset=\"utf-8\"; type=\"application/soap+xml\"\r\n"
		"\r\n"
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:arg1>SGVsbG8sIHdvcmxkIQ==</ns1:arg1>"
		"<ns1:arg2><ns2:Include href=\"cid:2f1e4de4-00d9-4046-80b7-59f3878ab488@appinf.com\" xmlns:ns2=\"http://www.w3.org/2004/08/xop/include\"/></ns1:arg2>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>\r\n"
		"--MIME_boundary_54B45DB81FC04D12\r\n"
		"Content-ID: <2f1e4de4-00d9-4046-80b7-59f3878ab488@appinf.com>\r\n"
		"Content-Transfer-Encoding: binary\r\n"
		"Content-Type: application/binary\r\n"
		"\r\n"
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis porta, augue non sodales eleifend, arcu neque accumsan nibh, sit amet auctor risus purus et lacus.\r\n"
		"--MIME_boundary_54B45DB81FC04D12--\r\n"
		"\r\n"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.enableMTOM(true);
	deser.parseContentType("Multipart/Related; boundary=\"MIME_boundary_54B45DB81FC04D12\"; "
		"type=\"application/xop+xml\";"
		"start=\"<3ebdaaf0-9250-48e1-b8cb-eed56bb9b42b@appinf.com>\"; "
		"start-info=\"application/soap+xml\""
	);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	std::vector<char> arg1;
	deser.deserialize("arg1", true, arg1);
	std::string tmp1(arg1.begin(), arg1.end());
	assert (tmp1 == "Hello, world!");

	std::vector<char> arg2;
	deser.deserialize("arg2", true, arg2);
	std::string tmp2(arg2.begin(), arg2.end());
	assert (tmp2 == "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis porta, augue non sodales eleifend, arcu neque accumsan nibh, sit amet auctor risus purus et lacus.");
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testStructAsRoot()
{
	const std::string input(
		"<ns1:object attr1=\"value1\" xmlns:ns1=\"urn:schemas-appinf-com:service:MySchema:1.0\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<ns1:attr2>value2</ns1:attr2>"
		"<ns1:subobject ns2:attr3=\"value3\" xmlns:ns2=\"urn:schemas-appinf-com:service:MyOtherSchema:1.0\">"
		"<ns2:attr4>value4</ns2:attr4>"
		"</ns1:subobject>"
		"</ns1:object>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);

	deser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MySchema:1.0");
	deser.pushAttribute("", "attr1", true);
	deser.deserializeStructBegin("object", true);
	std::string attr1;
	std::string attr2;
	std::string attr3;
	std::string attr4;
	deser.deserialize("attr1", true, attr1);
	deser.deserialize("attr2", true, attr2);
	deser.pushAttribute("urn:schemas-appinf-com:service:MyOtherSchema:1.0", "attr3", true);
	deser.deserializeStructBegin("subobject", true);
	deser.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, "urn:schemas-appinf-com:service:MyOtherSchema:1.0");
	deser.deserialize("attr3", true, attr3);
	deser.deserialize("attr4", true, attr4);
	deser.popProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE);
	deser.deserializeStructEnd("subobject");
	deser.deserializeStructEnd("object");

	assert (attr1 == "value1");
	assert (attr2 == "value2");
	assert (attr3 == "value3");
	assert (attr4 == "value4");
}


void DeserializerTest::testDateTime()
{
	const std::string input(
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

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	Poco::DateTime date;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	deser.deserialize("date", true, date);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	assert (date.year() == 2024 && date.month() == 2 && date.day() == 1);
	Poco::DateTime time;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "time"s);
	deser.deserialize("time", true, time);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	assert (time.hour() == 10 && time.minute() == 11 && time.second() == 12);
	Poco::DateTime dateTime;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "dateTime"s);
	deser.deserialize("dateTime", true, dateTime);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	assert (dateTime.year() == 2024 && dateTime.month() == 2 && dateTime.day() == 1 && dateTime.hour() == 10 && dateTime.minute() == 11 && dateTime.second() == 12);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testLocalDateTime()
{
	const std::string input(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">"
		"<ns1:date>2024-02-01</ns1:date>"
		"<ns1:time>10:11:12.000000+01:00</ns1:time>"
		"<ns1:dateTime>2024-02-01T10:11:12.000000+01:00</ns1:dateTime>"
		"</ns1:method>"
		"</soap:Body>"
		"</soap:Envelope>"
	);

	Deserializer deser;
	std::istringstream istr(input);
	deser.setup(istr);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");
	
	deser.deserializeMessageBegin(name, mt);
	Poco::LocalDateTime date;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	deser.deserialize("date", true, date);
	assert (date.year() == 2024 && date.month() == 2 && date.day() == 1);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	Poco::LocalDateTime time;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "time"s);
	deser.deserialize("time", true, time);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	assert (time.hour() == 10 && time.minute() == 11 && time.second() == 12 && time.tzd() == 3600);
	Poco::LocalDateTime dateTime;
	deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "dateTime"s);
	deser.deserialize("dateTime", true, dateTime);
	deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	assert (dateTime.year() == 2024 && dateTime.month() == 2 && dateTime.day() == 1 && dateTime.hour() == 10 && dateTime.minute() == 11 && dateTime.second() == 12 && dateTime.tzd() == 3600);
	deser.deserializeMessageEnd(name, mt);
}


void DeserializerTest::testInterceptor()
{
	const std::string input(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<s:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
		"\t<s:Header>\n"
		"\t<ns1:simple xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\">value</ns1:simple>\n"
		"\t</s:Header>\n"
		"\t<s:Body>\n"
		"\t\t<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>\n"
		"\t</s:Body>\n"
		"</s:Envelope>\n"
	);

	class TestDeserInterceptor: public Deserializer::HeaderInterceptor
	{
	public:
		void interceptHeader(const XMLElement& header)
		{
			if (header.children().size() > 0)
			{
				const XMLElement* pSimple = header.findChild("simple"s, "urn:schemas-appinf-com:service:MyService:1.0"s);
				if (pSimple)
				{
					_value = pSimple->value();
				}
			}
		}

		const std::string& value() const
		{
			return _value;
		}

	private:
		std::string _value;
	};

	Deserializer deser;
	Poco::SharedPtr<TestDeserInterceptor> pInterceptor = new TestDeserInterceptor;
	deser.addHeaderInterceptor(pInterceptor);
	std::istringstream istr(input);
	deser.setup(istr);
	
	assert (deser.soapVersion() == Serializer::SOAP_1_1);
	
	std::string name;
	Poco::RemotingNG::SerializerBase::MessageType mt = deser.findMessage(name);
	assert (mt == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assert (name == "method");

	deser.deserializeMessageBegin(name, mt);

	assert (pInterceptor->value() == "value");

	deser.deserializeMessageEnd(name, mt);	
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

	CppUnit_addTest(pSuite, DeserializerTest, testRequestNoParams11);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyNoParams11);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestTwoParams11);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyTwoParams11);
	CppUnit_addTest(pSuite, DeserializerTest, testFault11);

	CppUnit_addTest(pSuite, DeserializerTest, testRequestNoParams12);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyNoParams12);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestTwoParams12);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyTwoParams12);
	CppUnit_addTest(pSuite, DeserializerTest, testFault12);

	CppUnit_addTest(pSuite, DeserializerTest, testRequestNoParamsDL);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyNoParamsDL);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestTwoParamsDL);
	CppUnit_addTest(pSuite, DeserializerTest, testReplyTwoParamsDL);

	CppUnit_addTest(pSuite, DeserializerTest, testRequestTwoAttrs);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestWithAttrs);
	CppUnit_addTest(pSuite, DeserializerTest, testComplexRequest);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestWithAttrsInc);
	CppUnit_addTest(pSuite, DeserializerTest, testRequestWithAttrsOptional);
	CppUnit_addTest(pSuite, DeserializerTest, testVector);
	CppUnit_addTest(pSuite, DeserializerTest, testHeader);
	CppUnit_addTest(pSuite, DeserializerTest, testHeaderAttr);
	CppUnit_addTest(pSuite, DeserializerTest, testHeaderAttrOptional);

	CppUnit_addTest(pSuite, DeserializerTest, testCharVector);
	CppUnit_addTest(pSuite, DeserializerTest, testCharVectorNil1);
	CppUnit_addTest(pSuite, DeserializerTest, testCharVectorNil2);
	CppUnit_addTest(pSuite, DeserializerTest, testCharVectorNotNil);

	CppUnit_addTest(pSuite, DeserializerTest, testMTOM);

	CppUnit_addTest(pSuite, DeserializerTest, testStructAsRoot);

	CppUnit_addTest(pSuite, DeserializerTest, testDateTime);
	CppUnit_addTest(pSuite, DeserializerTest, testLocalDateTime);

	CppUnit_addTest(pSuite, DeserializerTest, testInterceptor);

	return pSuite;
}
