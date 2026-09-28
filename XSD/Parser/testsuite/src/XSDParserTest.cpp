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
#include "Poco/XSD/Types/Definitions.h"
#include "Poco/XSD/Types/Element.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/SAXException.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include <algorithm>
#include <sstream>


using namespace Poco::XSD::Parser;
using namespace Poco::XSD::Types;
using namespace Poco::XML;


namespace
{
	void parseDocument(const std::string& xml, const std::string& location)
	{
		std::istringstream iss(xml);
		Poco::XML::InputSource in(iss);
		XSDContentHandler::SchemaNSToLocationMap schemaMap;
		XSDContentHandler xsd(Poco::URI(location), schemaMap);
		Poco::XML::SAXParser parser;
		parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
		parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
		parser.setContentHandler(&xsd);
		parser.parse(&in);
	}


	Poco::URI writeSchemaFile(const Poco::TemporaryFile& file, const std::string& xml)
	{
		{
			Poco::FileOutputStream out(file.path());
			out << xml;
		}
		return Poco::URI(Poco::Path(file.path()).absolute());
	}
}


XSDParserTest::XSDParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


XSDParserTest::~XSDParserTest() = default;


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
	parseDocument(xml, "mem://testAnnotation");
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
	parseDocument(xml, "mem://testAnnotation2");
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
	parseDocument(xml, "mem://testAttribute");
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
	parseDocument(xml, "mem://testAttribute");
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
	try
	{
		parseDocument(xml, "mem://testAttribute");
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
	parseDocument(xml, "mem://testElement");
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
	parseDocument(xml, "mem://testElement");
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
	parseDocument(xml, "mem://testComplexType");
	Schema& schema = TypesManager::instance().getSchema("http://www.appinf.com");
	schema.fixup();
	const Element* pElem = schema.getElement("person");
	poco_check_ptr (pElem);
	poco_assert (pElem->name() == "person");
	poco_assert (pElem->getMaxOccurs() == 1);
	poco_assert (pElem->getMinOccurs() == 1);
	const Poco::XSD::Types::Type& aType = pElem->type();
	poco_assert (aType.name() == "Person");
	poco_assert (aType.getSchema() != nullptr);
}


void XSDParserTest::testElementWithTypeAndInlineType()
{
	static const std::string complexXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:typeAndInlineComplex\">"
										"<xs:element name=\"e\" type=\"xs:string\">"
											"<xs:complexType/>"
										"</xs:element>"
									"</xs:schema>");
	static const std::string simpleXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:typeAndInlineSimple\">"
										"<xs:element name=\"e\" type=\"xs:string\">"
											"<xs:simpleType><xs:restriction base=\"xs:string\"/></xs:simpleType>"
										"</xs:element>"
									"</xs:schema>");

	try
	{
		parseDocument(complexXml, "mem://testElementWithTypeAndInlineType/complex");
		failmsg("an element with a type attribute and an inline complexType must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema("urn:XSDParserTest:typeAndInlineComplex");

	try
	{
		parseDocument(simpleXml, "mem://testElementWithTypeAndInlineType/simple");
		failmsg("an element with a type attribute and an inline simpleType must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema("urn:XSDParserTest:typeAndInlineSimple");
}


void XSDParserTest::testRestrictionWithoutBase()
{
	static const std::string ns("urn:XSDParserTest:restrictionWithoutBase");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:restrictionWithoutBase\">"
									"<xs:simpleType name=\"t\"><xs:restriction/></xs:simpleType>"
								"</xs:schema>");

	try
	{
		parseDocument(xml, "mem://testRestrictionWithoutBase");
		TypesManager::instance().findSchema(ns)->fixup();
		failmsg("a restriction without base attribute and inline simpleType must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema(ns);
}


void XSDParserTest::testTopLevelElementRef()
{
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:topLevelRef\" "
									"xmlns:tns=\"urn:XSDParserTest:topLevelRef\">"
									"<xs:element name=\"e\" type=\"xs:string\"/>"
									"<xs:element ref=\"tns:e\"/>"
								"</xs:schema>");

	try
	{
		parseDocument(xml, "mem://testTopLevelElementRef");
		failmsg("a top-level element declaration with a ref attribute must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema("urn:XSDParserTest:topLevelRef");
}


void XSDParserTest::testSoapHeaderFault()
{
	static const std::string ns("urn:XSDParserTest:soapHeaderFault");
	static const std::string xml("<definitions xmlns=\"http://schemas.xmlsoap.org/wsdl/\" "
									"xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" "
									"xmlns:tns=\"urn:XSDParserTest:soapHeaderFault\" "
									"targetNamespace=\"urn:XSDParserTest:soapHeaderFault\">"
									"<message name=\"Request\"><part name=\"body\" element=\"tns:Request\"/></message>"
									"<message name=\"Header\"><part name=\"h\" element=\"tns:Header\"/></message>"
									"<message name=\"HeaderFault\"><part name=\"f\" element=\"tns:HeaderFault\"/></message>"
									"<portType name=\"PT\">"
										"<operation name=\"op\"><input message=\"tns:Request\"/></operation>"
									"</portType>"
									"<binding name=\"B\" type=\"tns:PT\">"
										"<soap:binding style=\"document\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>"
										"<operation name=\"op\">"
											"<soap:operation soapAction=\"\"/>"
											"<input>"
												"<soap:body use=\"literal\"/>"
												"<soap:header message=\"tns:Header\" part=\"h\" use=\"literal\">"
													"<soap:headerfault message=\"tns:HeaderFault\" part=\"f\" use=\"literal\"/>"
												"</soap:header>"
											"</input>"
										"</operation>"
									"</binding>"
								"</definitions>");

	parseDocument(xml, "mem://testSoapHeaderFault");
	TypesManager& tm = TypesManager::instance();
	const Definitions& defs = tm.getDefinitions(ns);
	const auto it = defs.portTypes().find("PT");
	assertTrue (it != defs.portTypes().end());
	const Operation::Ptr pOperation = it->second->findOperation("op");
	assertTrue (!pOperation.isNull());
	const BindingProperties& props = pOperation->inputBindingProperties();
	assertEqual ("tns:Header", props.get("soap.header[0].message"));
	assertEqual ("tns:HeaderFault", props.get("soap.header[0].headerfault.message"));
	assertEqual ("HeaderFault", props.get("soap.header[0].headerfault.message.localName"));
	assertEqual (ns, props.get("soap.header[0].headerfault.message.namespaceURI"));
	tm.removeDefinitions(tm.findDefinitions(ns));
}


void XSDParserTest::testSameNamespaceDocuments()
{
	static const std::string ns("urn:XSDParserTest:sameNamespace");
	static const std::string firstXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:sameNamespace\" "
										"xmlns:tns=\"urn:XSDParserTest:sameNamespace\">"
										"<xs:complexType name=\"A\"><xs:sequence/></xs:complexType>"
										"<xs:element name=\"e\" type=\"tns:B\"/>"
									"</xs:schema>");
	static const std::string secondXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:sameNamespace\">"
										"<xs:complexType name=\"B\"><xs:sequence/></xs:complexType>"
									"</xs:schema>");

	TypesManager& tm = TypesManager::instance();
	parseDocument(firstXml, "mem://testSameNamespaceDocuments/first");
	parseDocument(secondXml, "mem://testSameNamespaceDocuments/second");
	tm.findSchema(ns)->fixup();
	assertTrue (tm.getType(QName("A", ns)) != nullptr);
	assertTrue (tm.getType(QName("B", ns)) != nullptr);
	const Element* pElem = tm.getElement(QName("e", ns));
	assertTrue (pElem != nullptr);
	assertEqual ("B", pElem->type().name());
	tm.eraseSchema(ns);
}


void XSDParserTest::testFailedLoadIsRevoked()
{
	static const std::string ns("urn:XSDParserTest:failedLoad");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:failedLoad\">"
									"<xs:element name=\"e\" type=\"xs:string\">"
										"<xs:complexType/>"
									"</xs:element>"
								"</xs:schema>");

	const Poco::TemporaryFile file;
	const Poco::URI uri = writeSchemaFile(file, xml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	for (int attempt = 0; attempt < 2; ++attempt)
	{
		try
		{
			XSDContentHandler::loadXSD(uri, schemaMap);
			failmsg("loading an invalid schema must fail");
		}
		catch (SchemaException&)
		{
		}
		assertTrue (!tm.hasSchemaLocation(uri));
		assertTrue (!tm.hasSchema(ns));
	}
}


void XSDParserTest::testEraseSchemaRemovesLocation()
{
	static const std::string ns("urn:XSDParserTest:eraseLocation");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:eraseLocation\">"
									"<xs:element name=\"e\" type=\"xs:string\"/>"
								"</xs:schema>");

	const Poco::URI location("mem://testEraseSchemaRemovesLocation");
	parseDocument(xml, location.toString());
	TypesManager& tm = TypesManager::instance();
	assertTrue (tm.hasSchemaLocation(location));
	assertTrue (tm.eraseSchema(ns));
	assertTrue (!tm.hasSchemaLocation(location));
	assertTrue (!tm.hasSchema(ns));
}


void XSDParserTest::testAttributeWithTypeAndInlineType()
{
	static const std::string ns("urn:XSDParserTest:attributeTypeAndInline");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:attributeTypeAndInline\">"
									"<xs:attribute name=\"a\" type=\"xs:string\">"
										"<xs:simpleType><xs:restriction base=\"xs:string\"/></xs:simpleType>"
									"</xs:attribute>"
								"</xs:schema>");

	try
	{
		parseDocument(xml, "mem://testAttributeWithTypeAndInlineType");
		failmsg("an attribute with a type attribute and an inline simpleType must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema(ns);
}


void XSDParserTest::testRestrictionWithBaseAndInlineType()
{
	static const std::string ns("urn:XSDParserTest:restrictionBaseAndInline");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:restrictionBaseAndInline\">"
									"<xs:simpleType name=\"t\">"
										"<xs:restriction base=\"xs:string\">"
											"<xs:simpleType><xs:restriction base=\"xs:string\"/></xs:simpleType>"
										"</xs:restriction>"
									"</xs:simpleType>"
								"</xs:schema>");

	try
	{
		parseDocument(xml, "mem://testRestrictionWithBaseAndInlineType");
		failmsg("a restriction with a base attribute and an inline simpleType must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema(ns);
}


void XSDParserTest::testRestrictionWithTwoInlineTypes()
{
	static const std::string ns("urn:XSDParserTest:restrictionTwoInline");
	static const std::string xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:restrictionTwoInline\">"
									"<xs:simpleType name=\"t\">"
										"<xs:restriction>"
											"<xs:simpleType><xs:restriction base=\"xs:string\"/></xs:simpleType>"
											"<xs:simpleType><xs:restriction base=\"xs:int\"/></xs:simpleType>"
										"</xs:restriction>"
									"</xs:simpleType>"
								"</xs:schema>");

	try
	{
		parseDocument(xml, "mem://testRestrictionWithTwoInlineTypes");
		failmsg("a restriction with two inline simpleTypes must be rejected");
	}
	catch (SchemaException&)
	{
	}
	TypesManager::instance().eraseSchema(ns);
}


void XSDParserTest::testContentAfterSchemaEndMergesNothing()
{
	static const std::string ns("urn:XSDParserTest:contentAfterEnd");
	static const std::string firstXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:contentAfterEnd\">"
										"<xs:complexType name=\"A\"><xs:sequence/></xs:complexType>"
									"</xs:schema>");
	static const std::string secondXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:contentAfterEnd\">"
										"<xs:complexType name=\"B\"><xs:sequence/></xs:complexType>"
									"</xs:schema>"
									"<junk/>");

	const Poco::TemporaryFile firstFile;
	const Poco::TemporaryFile secondFile;
	const Poco::URI firstUri = writeSchemaFile(firstFile, firstXml);
	const Poco::URI secondUri = writeSchemaFile(secondFile, secondXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	XSDContentHandler::loadXSD(firstUri, schemaMap);
	try
	{
		XSDContentHandler::loadXSD(secondUri, schemaMap);
		failmsg("loading a schema document with content after its root element must fail");
	}
	catch (SAXParseException&)
	{
	}
	const Schema::Ptr pSchema = tm.findSchema(ns);
	assertTrue (!pSchema.isNull());
	assertTrue (pSchema->getType("A") != nullptr);
	assertTrue (pSchema->getType("B") == nullptr);
	assertTrue (!tm.hasSchemaLocation(secondUri));
	tm.eraseSchema(ns);
}


void XSDParserTest::testEraseSchemaRemovesMergedLocations()
{
	static const std::string ns("urn:XSDParserTest:eraseMergedLocations");
	static const std::string firstXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:eraseMergedLocations\">"
										"<xs:complexType name=\"A\"><xs:sequence/></xs:complexType>"
									"</xs:schema>");
	static const std::string secondXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
										"targetNamespace=\"urn:XSDParserTest:eraseMergedLocations\">"
										"<xs:complexType name=\"B\"><xs:sequence/></xs:complexType>"
									"</xs:schema>");

	const Poco::TemporaryFile firstFile;
	const Poco::TemporaryFile secondFile;
	const Poco::URI firstUri = writeSchemaFile(firstFile, firstXml);
	const Poco::URI secondUri = writeSchemaFile(secondFile, secondXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	XSDContentHandler::loadXSD(firstUri, schemaMap);
	const Schema::Ptr pSchema = XSDContentHandler::loadXSD(secondUri, schemaMap);
	assertTrue (pSchema == tm.findSchema(ns));
	assertTrue (XSDContentHandler::loadXSD(secondUri, schemaMap) == pSchema);
	assertTrue (tm.eraseSchema(ns));
	assertTrue (!tm.hasSchemaLocation(firstUri));
	assertTrue (!tm.hasSchemaLocation(secondUri));
}


void XSDParserTest::testIncludeAfterImport()
{
	static const std::string ns("urn:XSDParserTest:includeAfterImport");
	static const std::string importedNs("urn:XSDParserTest:includeAfterImportM");
	static const std::string includedImportNs("urn:XSDParserTest:includeAfterImportC");
	static const std::string mXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:includeAfterImportM\">"
									"<xs:complexType name=\"TM\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");
	static const std::string cXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:includeAfterImportC\" "
									"xmlns:c=\"urn:XSDParserTest:includeAfterImportC\">"
									"<xs:complexType name=\"TC\"><xs:sequence/></xs:complexType>"
									"<xs:element name=\"ec\" type=\"c:TC\"/>"
								"</xs:schema>");

	const Poco::TemporaryFile mFile;
	const Poco::TemporaryFile cFile;
	const Poco::TemporaryFile bFile;
	const Poco::TemporaryFile aFile;
	const Poco::URI mUri = writeSchemaFile(mFile, mXml);
	const Poco::URI cUri = writeSchemaFile(cFile, cXml);
	const std::string bXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:includeAfterImport\" "
								"xmlns:c=\"urn:XSDParserTest:includeAfterImportC\">"
								"<xs:import namespace=\"urn:XSDParserTest:includeAfterImportC\" "
									"schemaLocation=\"" + cUri.toString() + "\"/>"
								"<xs:complexType name=\"TB\"><xs:sequence>"
									"<xs:element name=\"c\" type=\"c:TC\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI bUri = writeSchemaFile(bFile, bXml);
	const std::string aXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:includeAfterImport\" "
								"xmlns:n=\"urn:XSDParserTest:includeAfterImport\" "
								"xmlns:m=\"urn:XSDParserTest:includeAfterImportM\">"
								"<xs:import namespace=\"urn:XSDParserTest:includeAfterImportM\" "
									"schemaLocation=\"" + mUri.toString() + "\"/>"
								"<xs:include schemaLocation=\"" + bUri.toString() + "\"/>"
								"<xs:complexType name=\"TA\"><xs:sequence>"
									"<xs:element name=\"a\" type=\"m:TM\"/>"
									"<xs:element name=\"b\" type=\"n:TB\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI aUri = writeSchemaFile(aFile, aXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	// fixing up the schema of A must reach C, which only the included B imports
	XSDContentHandler::loadXSD(aUri, schemaMap)->fixup();
	assertTrue (tm.getType(QName("TA", ns)) != nullptr);
	assertTrue (tm.getType(QName("TB", ns)) != nullptr);
	assertTrue (tm.getType(QName("TC", includedImportNs)) != nullptr);
	assertTrue (tm.getType(QName("TM", importedNs)) != nullptr);
	const Element* pElem = tm.getElement(QName("ec", includedImportNs));
	assertTrue (pElem != nullptr);
	assertEqual ("TC", pElem->type().name());
	// fixupSchemas(), as XSDGen calls it, also fixes up C, so it runs after the checks above
	tm.fixupSchemas();
	tm.eraseSchema(ns);
	tm.eraseSchema(importedNs);
	tm.eraseSchema(includedImportNs);
}


void XSDParserTest::testIncludeTwice()
{
	static const std::string ns("urn:XSDParserTest:includeTwice");
	static const std::string importedNs("urn:XSDParserTest:includeTwiceM");
	static const std::string mXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:includeTwiceM\">"
									"<xs:complexType name=\"TM\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");
	static const std::string bXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:includeTwice\">"
									"<xs:complexType name=\"TB\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");

	const Poco::TemporaryFile mFile;
	const Poco::TemporaryFile bFile;
	const Poco::TemporaryFile aFile;
	const Poco::URI mUri = writeSchemaFile(mFile, mXml);
	const Poco::URI bUri = writeSchemaFile(bFile, bXml);
	const std::string aXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:includeTwice\" "
								"xmlns:n=\"urn:XSDParserTest:includeTwice\" "
								"xmlns:m=\"urn:XSDParserTest:includeTwiceM\">"
								"<xs:import namespace=\"urn:XSDParserTest:includeTwiceM\" "
									"schemaLocation=\"" + mUri.toString() + "\"/>"
								"<xs:include schemaLocation=\"" + bUri.toString() + "\"/>"
								"<xs:include schemaLocation=\"" + bUri.toString() + "\"/>"
								"<xs:complexType name=\"TA\"><xs:sequence>"
									"<xs:element name=\"a\" type=\"m:TM\"/>"
									"<xs:element name=\"b\" type=\"n:TB\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI aUri = writeSchemaFile(aFile, aXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	const Schema::Ptr pSchema = XSDContentHandler::loadXSD(aUri, schemaMap);
	tm.fixupSchemas();
	assertTrue (tm.getType(QName("TB", ns)) != nullptr);
	assertTrue (tm.findSchema(ns) == pSchema);
	tm.eraseSchema(ns);
	tm.eraseSchema(importedNs);
}


void XSDParserTest::testImportSecondDocument()
{
	static const std::string ns("urn:XSDParserTest:importSecondDocument");
	static const std::string secondImportNs("urn:XSDParserTest:importSecondDocumentC");
	static const std::string importingNs("urn:XSDParserTest:importSecondDocumentT");
	static const std::string a1Xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:importSecondDocument\">"
									"<xs:complexType name=\"T1\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");
	static const std::string cXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
									"targetNamespace=\"urn:XSDParserTest:importSecondDocumentC\" "
									"xmlns:c=\"urn:XSDParserTest:importSecondDocumentC\">"
									"<xs:complexType name=\"TC\"><xs:sequence/></xs:complexType>"
									"<xs:element name=\"ec\" type=\"c:TC\"/>"
								"</xs:schema>");

	const Poco::TemporaryFile a1File;
	const Poco::TemporaryFile cFile;
	const Poco::TemporaryFile a2File;
	const Poco::TemporaryFile tFile;
	const Poco::URI a1Uri = writeSchemaFile(a1File, a1Xml);
	const Poco::URI cUri = writeSchemaFile(cFile, cXml);
	const std::string a2Xml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:importSecondDocument\" "
								"xmlns:c=\"urn:XSDParserTest:importSecondDocumentC\">"
								"<xs:import namespace=\"urn:XSDParserTest:importSecondDocumentC\" "
									"schemaLocation=\"" + cUri.toString() + "\"/>"
								"<xs:complexType name=\"T2\"><xs:sequence>"
									"<xs:element name=\"c\" type=\"c:TC\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI a2Uri = writeSchemaFile(a2File, a2Xml);
	const std::string tXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:importSecondDocumentT\" "
								"xmlns:n=\"urn:XSDParserTest:importSecondDocument\">"
								"<xs:import namespace=\"urn:XSDParserTest:importSecondDocument\" "
									"schemaLocation=\"" + a1Uri.toString() + "\"/>"
								"<xs:import namespace=\"urn:XSDParserTest:importSecondDocument\" "
									"schemaLocation=\"" + a2Uri.toString() + "\"/>"
								"<xs:complexType name=\"TT\"><xs:sequence>"
									"<xs:element name=\"b\" type=\"n:T2\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI tUri = writeSchemaFile(tFile, tXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	Schema::Ptr pSchema = XSDContentHandler::loadXSD(tUri, schemaMap);
	// A2 is merged into the registered schema of A1, which must take over the import of C
	const Schema::Ptr pSecond = XSDContentHandler::loadXSD(a2Uri, schemaMap);
	assertTrue (pSecond == tm.findSchema(ns));
	const Schema::Schemas& imported = pSecond->importedSchemas();
	assertTrue (std::find(imported.begin(), imported.end(), tm.findSchema(secondImportNs)) != imported.end());
	pSchema->fixup();
	const Element* pElem = tm.getElement(QName("ec", secondImportNs));
	assertTrue (pElem != nullptr);
	assertEqual ("TC", pElem->type().name());
	tm.fixupSchemas();
	tm.eraseSchema(ns);
	tm.eraseSchema(secondImportNs);
	tm.eraseSchema(importingNs);
}


void XSDParserTest::testIncludeChameleon()
{
	static const std::string ns("urn:XSDParserTest:includeChameleon");
	static const std::string xXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">"
									"<xs:complexType name=\"TX\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");
	static const std::string bXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">"
									"<xs:complexType name=\"TB\"><xs:sequence/></xs:complexType>"
								"</xs:schema>");

	const Poco::TemporaryFile xFile;
	const Poco::TemporaryFile bFile;
	const Poco::TemporaryFile aFile;
	const Poco::URI xUri = writeSchemaFile(xFile, xXml);
	const Poco::URI bUri = writeSchemaFile(bFile, bXml);
	const std::string aXml("<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
								"targetNamespace=\"urn:XSDParserTest:includeChameleon\" "
								"xmlns:n=\"urn:XSDParserTest:includeChameleon\">"
								"<xs:include schemaLocation=\"" + bUri.toString() + "\"/>"
								"<xs:complexType name=\"TA\"><xs:sequence>"
									"<xs:element name=\"b\" type=\"n:TB\"/>"
								"</xs:sequence></xs:complexType>"
							"</xs:schema>");
	const Poco::URI aUri = writeSchemaFile(aFile, aXml);
	XSDContentHandler::SchemaNSToLocationMap schemaMap;
	TypesManager& tm = TypesManager::instance();
	// B has no target namespace and is merged into the registered schema of X;
	// the include takes over the declarations of B only
	XSDContentHandler::loadXSD(xUri, schemaMap);
	XSDContentHandler::loadXSD(aUri, schemaMap);
	assertTrue (tm.getType(QName("TB", ns)) != nullptr);
	assertTrue (tm.getType(QName("TX", ns)) == nullptr);
	tm.eraseSchema(ns);
	tm.eraseSchema("");
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
	CppUnit_addTest(pSuite, XSDParserTest, testElementWithTypeAndInlineType);
	CppUnit_addTest(pSuite, XSDParserTest, testRestrictionWithoutBase);
	CppUnit_addTest(pSuite, XSDParserTest, testTopLevelElementRef);
	CppUnit_addTest(pSuite, XSDParserTest, testSoapHeaderFault);
	CppUnit_addTest(pSuite, XSDParserTest, testSameNamespaceDocuments);
	CppUnit_addTest(pSuite, XSDParserTest, testFailedLoadIsRevoked);
	CppUnit_addTest(pSuite, XSDParserTest, testEraseSchemaRemovesLocation);
	CppUnit_addTest(pSuite, XSDParserTest, testAttributeWithTypeAndInlineType);
	CppUnit_addTest(pSuite, XSDParserTest, testRestrictionWithBaseAndInlineType);
	CppUnit_addTest(pSuite, XSDParserTest, testRestrictionWithTwoInlineTypes);
	CppUnit_addTest(pSuite, XSDParserTest, testContentAfterSchemaEndMergesNothing);
	CppUnit_addTest(pSuite, XSDParserTest, testEraseSchemaRemovesMergedLocations);
	CppUnit_addTest(pSuite, XSDParserTest, testIncludeAfterImport);
	CppUnit_addTest(pSuite, XSDParserTest, testIncludeTwice);
	CppUnit_addTest(pSuite, XSDParserTest, testImportSecondDocument);
	CppUnit_addTest(pSuite, XSDParserTest, testIncludeChameleon);

	return pSuite;
}
