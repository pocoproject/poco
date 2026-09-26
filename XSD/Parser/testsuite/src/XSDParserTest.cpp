//
// XSDParserTest.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "XSDParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/SAX/InputSource.h"
#include <sstream>


using namespace Poco::XSD::Parser;
using namespace Poco::XSD::Types;
using namespace Poco::XML;


XSDParserTest::XSDParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


XSDParserTest::~XSDParserTest()
{
}


void XSDParserTest::testAnnotation()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:annotation>"
										"<xs:documentation xml:lang=\"en\" "
											"source=\"http://appinf.com/prod.html#product\">\r\n"
												"<ns1:description>This element represents a product."
												"</ns1:description>"
										"</xs:documentation>"
									"</xs:annotation>"
							"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testAnnotation");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	assert (schema.getAnnotations().size() == 1);
	const Annotation& ann = schema.getAnnotations()[0];
	assert (ann.annotationContent().size() == 1);
	assert (ann.annotationContent()[0]->source() == "http://appinf.com/prod.html#product");
	assert (ann.annotationContent()[0]->getData().empty());
}


void XSDParserTest::testAnnotation2()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">\r\n"
									"<xs:annotation>\r\n"
										"<xs:documentation xml:lang=\"en\" "
											"source=\"http://appinf.com/prod.html#product\">"
												"Some simple string"
										"</xs:documentation>"
									"</xs:annotation>"
							"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testAnnotation2");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	assert (schema.getAnnotations().size() == 1);
	const Annotation& ann = schema.getAnnotations()[0];
	assert (ann.annotationContent().size() == 1);
	assert (ann.annotationContent()[0]->getData() == "Some simple string");
}


void XSDParserTest::testAttribute()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:attribute name=\"age\" type=\"xs:positiveInteger\" use=\"required\"/>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testAttribute");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const AbstractAttribute* pAttr = schema.getAttribute("age");
	poco_check_ptr (pAttr);
	poco_assert (pAttr->usage() == AbstractAttribute::USE_REQUIRED);
	poco_assert (pAttr->name() == "age");
	poco_assert (pAttr->defaultValue().empty());
	poco_assert (pAttr->fixedValue().empty());
	const SimpleType* pType = pAttr->type();
	poco_check_ptr (pType);
	poco_assert (pType->name() == "positiveInteger");
	poco_check_ptr (pType->getSchema());
}


void XSDParserTest::testAttribute2()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:attribute default=\"0\" name=\"age\" type=\"xs:positiveInteger\" use=\"required\"/>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testAttribute");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const AbstractAttribute* pAttr = schema.getAttribute("age");
	poco_check_ptr (pAttr);
	poco_assert (pAttr->usage() == AbstractAttribute::USE_REQUIRED);
	poco_assert (pAttr->name() == "age");
	poco_assert (pAttr->defaultValue() == "0");
	poco_assert (pAttr->fixedValue().empty());
	const SimpleType* pType = pAttr->type();
	poco_check_ptr (pType);
	poco_assert (pType->name() == "positiveInteger");
	poco_check_ptr (pType->getSchema());
}


void XSDParserTest::testAttribute3()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:attribute default=\"0\" fixed=\"18\" name=\"age\" type=\"xs:positiveInteger\" use=\"required\"/>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testAttribute");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	try
	{
		parser.parse(&in);
		fail("illegal xsd");
	}
	catch (Poco::XSD::Types::XSDException&)
	{
	}
}


void XSDParserTest::testElement()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:element name=\"name\" type=\"xs:string\"/>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testElement");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const Element* pElem = schema.getElement("name");
	poco_check_ptr (pElem);
	poco_assert (pElem->name() == "name");
	poco_assert (pElem->getMaxOccurs() == 1);
	poco_assert (pElem->getMinOccurs() == 1);
	const Poco::XSD::Types::Type& aType = pElem->type();
	poco_assert (aType.name() == "string");
}


void XSDParserTest::testElementInline()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:element name=\"name\">"
										"<xs:complexType>"
											"<xs:sequence>"
												"<xs:element name=\"birthday\" type=\"xs:date\"/>"
											"</xs:sequence>"
										"</xs:complexType>"
									"</xs:element>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testElement");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const Element* pElem = schema.getElement("name");
	poco_check_ptr (pElem);
	poco_assert (pElem->name() == "name");
	poco_assert (pElem->getMaxOccurs() == 1);
	poco_assert (pElem->getMinOccurs() == 1);
	const Poco::XSD::Types::Type& aType = pElem->type();
	poco_assert (aType.name() == "#name");
}


void XSDParserTest::testComplexType()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"http://www.appinf.com\" "
									"xmlns=\"http://www.appinf.com\" "
									"xmlns:ns1=\"someotheruri\" "
									"elementFormDefault=\"qualified\">"
									"<xs:complexType name=\"Person\">"
										"<xs:sequence>"
											"<xs:element name=\"birthday\" type=\"xs:date\"/>"
											"<xs:element name=\"firstName\" type=\"xs:string\"/>"
											"<xs:element name=\"lastName\" type=\"xs:string\"/>"
											"<xs:element maxOccurs=\"3\" ref=\"address\"/>"
										"</xs:sequence>"
									"</xs:complexType>"
									"<xs:complexType name=\"Address\">"
										"<xs:all>"
											"<xs:element name=\"street\" type=\"xs:string\"/>"
											"<xs:element name=\"houseNr\" type=\"xs:unsignedInt\"/>"
											"<xs:element name=\"country\" type=\"xs:string\"/>"
										"</xs:all>"
									"</xs:complexType>"
									"<xs:element name=\"address\" type=\"Address\"/>"
									"<xs:element name=\"person\" type=\"Person\"/>"
								"</xs:schema>");

	TypesManager::instance().eraseSchema("http://www.appinf.com");
	std::istringstream iss(xml);
	Poco::XML::InputSource in(iss);
	Poco::URI loc("mem://testComplexType");
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	XSDContentHandler xsd(loc, schemaMap);
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(&xsd);
	parser.parse(&in);
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const Element* pElem = schema.getElement("person");
	poco_check_ptr (pElem);
	poco_assert (pElem->name() == "person");
	poco_assert (pElem->getMaxOccurs() == 1);
	poco_assert (pElem->getMinOccurs() == 1);
	const Poco::XSD::Types::Type& aType = pElem->type();
	poco_assert (aType.name() == "Person");
	poco_assert (aType.getSchema() != 0);
}


void XSDParserTest::setUp()
{
}


void XSDParserTest::tearDown()
{
}


CppUnit::Test* XSDParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDParserTest");

	CppUnit_addTest(pSuite, XSDParserTest, testAnnotation);
	CppUnit_addTest(pSuite, XSDParserTest, testAnnotation2);
	CppUnit_addTest(pSuite, XSDParserTest, testAttribute);
	CppUnit_addTest(pSuite, XSDParserTest, testAttribute2);
	CppUnit_addTest(pSuite, XSDParserTest, testAttribute3);
	CppUnit_addTest(pSuite, XSDParserTest, testElement);
	CppUnit_addTest(pSuite, XSDParserTest, testElementInline);
	CppUnit_addTest(pSuite, XSDParserTest, testComplexType);

	return pSuite;
}
