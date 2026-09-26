//
// ValidatorTest.cpp
//
// Copyright (c) 2021-2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ValidatorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XSD/Validator/Validator.h"
#include "Poco/Exception.h"


using Poco::XSD::Validator::Validator;


namespace {


// A small representative XSD covering the patterns we care about:
// required attributes, an enumerated simple type, and <xs:anyAttribute>
// (so the test exercises the same shape consumers like DevsLoad use).
const char* const SAMPLE_XSD = R"(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
           elementFormDefault="qualified"
           attributeFormDefault="unqualified">
  <xs:simpleType name="directionType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="in"/>
      <xs:enumeration value="out"/>
      <xs:enumeration value="io"/>
    </xs:restriction>
  </xs:simpleType>
  <xs:element name="root">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="item" minOccurs="0" maxOccurs="unbounded"/>
      </xs:sequence>
    </xs:complexType>
  </xs:element>
  <xs:element name="item">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="child" minOccurs="0" maxOccurs="unbounded"/>
      </xs:sequence>
      <xs:attribute name="name" type="xs:string" use="required"/>
      <xs:attribute name="direction" type="directionType" use="optional"/>
      <xs:anyAttribute processContents="lax"/>
    </xs:complexType>
  </xs:element>
  <xs:element name="child">
    <xs:complexType>
      <xs:attribute name="id" type="xs:string" use="required"/>
      <xs:anyAttribute processContents="lax"/>
    </xs:complexType>
  </xs:element>
</xs:schema>)";


} // namespace


ValidatorTest::ValidatorTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ValidatorTest::~ValidatorTest()
{
}


void ValidatorTest::setUp()
{
}


void ValidatorTest::tearDown()
{
}


void ValidatorTest::testValidateAcceptsValidXml()
{
	Validator::validate(R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
  <item name="A" direction="in"/>
  <item name="B" direction="io"/>
</root>)", SAMPLE_XSD);
}


void ValidatorTest::testValidateAcceptsNestedStructure()
{
	Validator::validate(R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
  <item name="parent" direction="out">
    <child id="c1"/>
    <child id="c2"/>
  </item>
</root>)", SAMPLE_XSD);
}


void ValidatorTest::testValidateAllowsExtraAttributesViaAnyAttribute()
{
	// extraAttr1/extraAttr2 are not declared in the XSD but pass via
	// <xs:anyAttribute processContents="lax"/>.
	Validator::validate(R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
  <item name="X" direction="io" extraAttr1="hello" extraAttr2="42"/>
</root>)", SAMPLE_XSD);
}


void ValidatorTest::testValidateRejectsInvalidEnumValue()
{
	try
	{
		Validator::validate(R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
  <item name="X" direction="sideways"/>
</root>)", SAMPLE_XSD);
		fail("invalid direction value must fail schema validation");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void ValidatorTest::testValidateRejectsMissingRequiredAttribute()
{
	try
	{
		Validator::validate(R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
  <item direction="in"/>
</root>)", SAMPLE_XSD);
		fail("missing required attribute 'name' must fail schema validation");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void ValidatorTest::testValidateRejectsMalformedXml()
{
	try
	{
		Validator::validate("<root><item name=\"A\">", SAMPLE_XSD);
		fail("malformed XML must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void ValidatorTest::testValidateRejectsInvalidSchema()
{
	try
	{
		Validator::validate("<root/>", "<not-a-schema/>");
		fail("invalid XSD must throw");
	}
	catch (const Poco::DataFormatException&)
	{
	}
}


void ValidatorTest::testValidateThrowsOnEmptySchema()
{
	try
	{
		Validator::validate("<root/>", "");
		fail("empty xsdContent must throw IllegalStateException");
	}
	catch (const Poco::IllegalStateException&)
	{
	}
}


void ValidatorTest::testValidatorCannotBeInstantiated()
{
	// Compile-time check via std::is_default_constructible: the deleted
	// default constructor on Validator makes the trait false. If someone
	// accidentally removes `= delete`, this assertion catches it.
	static_assert(!std::is_default_constructible<Validator>::value,
		"Poco::XSD::Validator::Validator must remain static-only (= delete)");
}


CppUnit::Test* ValidatorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ValidatorTest");

	CppUnit_addTest(pSuite, ValidatorTest, testValidateAcceptsValidXml);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateAcceptsNestedStructure);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateAllowsExtraAttributesViaAnyAttribute);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateRejectsInvalidEnumValue);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateRejectsMissingRequiredAttribute);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateRejectsMalformedXml);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateRejectsInvalidSchema);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateThrowsOnEmptySchema);
	CppUnit_addTest(pSuite, ValidatorTest, testValidatorCannotBeInstantiated);

	return pSuite;
}
