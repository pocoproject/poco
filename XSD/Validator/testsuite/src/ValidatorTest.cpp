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
#include "Poco/Ascii.h"
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/URI.h"
#include <type_traits>


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


// libxml2 parses the entity text without the namespace declarations of the
// place where it is referenced, so the declaration declares the prefix itself.
const char* const ELEMENT_B_DECLARATION = R"(<xs:element xmlns:xs="http://www.w3.org/2001/XMLSchema" name="b" type="xs:int"/>)";


std::string writeFile(const Poco::TemporaryFile& file, const std::string& content)
	/// Writes content to file and returns the absolute path of file.
{
	{
		Poco::FileOutputStream out(file.path());
		out << content;
	}
	return Poco::Path(file.path()).absolute().toString();
}


std::string fileURI(const std::string& path)
	/// Returns the file URI of path.
{
	return Poco::URI(Poco::Path(path)).toString();
}


} // namespace


ValidatorTest::ValidatorTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ValidatorTest::~ValidatorTest() = default;


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
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("XML: malformed document: line ") != std::string::npos);
	}
}


void ValidatorTest::testValidateRejectsInvalidSchema()
{
	try
	{
		Validator::validate("<root/>", "<not-a-schema/>");
		fail("invalid XSD must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("XSD: invalid schema: ") != std::string::npos);
	}
}


void ValidatorTest::testValidateRejectsMalformedSchema()
{
	const std::string schema(SAMPLE_XSD);
	try
	{
		Validator::validate("<root/>", schema.substr(0, schema.size()/2));
		fail("truncated XSD must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("XSD: malformed schema document: line ") != std::string::npos);
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
	static_assert(!std::is_default_constructible_v<Validator>,
		"Poco::XSD::Validator::Validator must remain static-only (= delete)");
}


void ValidatorTest::testSchemaIncludeNamedSchemaXsd()
{
	// The schema text has no location of its own, so an include of a file named
	// schema.xsd must not be taken for the schema including itself.
	try
	{
		Validator::validate("<a>1</a>", R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation="schema.xsd"/>
  <xs:element name="a" type="xs:string"/>
</xs:schema>)");
		fail("an include of a missing schema must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("itself") == std::string::npos);
	}
}


void ValidatorTest::testSchemaInternalEntities()
{
	// The entity declares the xs prefix itself; see ELEMENT_B_DECLARATION.
	const std::string schema = R"(<!DOCTYPE xs:schema [
  <!ENTITY b '<xs:element xmlns:xs="http://www.w3.org/2001/XMLSchema" name="b" type="xs:int"/>'>
]>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="a">
    <xs:complexType>
      <xs:sequence>&b;</xs:sequence>
    </xs:complexType>
  </xs:element>
</xs:schema>)";

	Validator::validate("<a><b>1</b></a>", schema);
	try
	{
		Validator::validate("<a><b>x</b></a>", schema);
		fail("a value that is not an xs:int must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("XML: schema validation failed: ") != std::string::npos);
	}
}


void ValidatorTest::testMalformedDocumentFirstError()
{
	try
	{
		Validator::validate("<a><b></a>", SAMPLE_XSD);
		fail("malformed XML must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("Opening and ending tag mismatch") != std::string::npos);
	}
}


void ValidatorTest::testValidationMessageFormat()
{
	try
	{
		Validator::validate("<root>\n<item direction=\"sideways\"/>\n</root>", SAMPLE_XSD);
		fail("an item without a name and with an invalid direction must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		// Both errors are on line 2; libxml2 ends each message with a newline,
		// which must not appear before the separator.
		const std::string& message = exc.message();
		assertTrue (message.find("XML: schema validation failed: line 2: ") != std::string::npos);
		const std::string::size_type pos = message.find("; line 2: ");
		assertTrue (pos != std::string::npos);
		assertTrue (!Poco::Ascii::isSpace(message[pos - 1]));
	}
}


void ValidatorTest::testDocumentTypeDeclarationRejected()
{
	try
	{
		Validator::validate("<!DOCTYPE root [<!ATTLIST root role CDATA 'admin'>]><root/>", SAMPLE_XSD);
		fail("a document with a default attribute declaration must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("document type declarations are not supported") != std::string::npos);
	}

	const std::string intSchema = R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="a" type="xs:int"/>
</xs:schema>)";
	try
	{
		Validator::validate("<!DOCTYPE a [<!ENTITY n \"12\">]><a>&n;</a>", intSchema);
		fail("a document with an entity reference must throw");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("document type declarations are not supported") != std::string::npos);
	}
}


void ValidatorTest::testErrorMessageBounded()
{
	std::string xml("<root>");
	for (int i = 0; i < 200; ++i)
		xml += "<item name=\"X\" direction=\"sideways\"/>";
	xml += "</root>";
	try
	{
		Validator::validate(xml, SAMPLE_XSD);
		fail("invalid direction values must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		// A stack trace, if enabled, starts on a new line and is not counted.
		const std::string message = exc.message().substr(0, exc.message().find('\n'));
		assertTrue (message.find("further errors") != std::string::npos);
		assertTrue (message.size() < 4096);
	}

	// Ten errors are listed; the eleventh is counted.
	xml = "<root>";
	for (int i = 0; i < 11; ++i)
		xml += "<item name=\"X\" direction=\"sideways\"/>";
	xml += "</root>";
	try
	{
		Validator::validate(xml, SAMPLE_XSD);
		fail("invalid direction values must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("(1 further error)") != std::string::npos);
	}
}


void ValidatorTest::testValidationMessageSingleLine()
{
	const std::string schema = R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="b">
    <xs:simpleType>
      <xs:restriction base="xs:string">
        <xs:enumeration value="a"/>
      </xs:restriction>
    </xs:simpleType>
  </xs:element>
</xs:schema>)";
	try
	{
		Validator::validate("<b>x&#13;&#10;y</b>", schema);
		fail("a value outside the enumeration must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		// libxml2 quotes the value with its line break; a stack trace, if enabled,
		// starts on a new line after the message.
		const std::string& message = exc.message();
		const std::string firstLine = message.substr(0, message.find('\n'));
		assertTrue (firstLine.find("XML: schema validation failed: ") != std::string::npos);
		assertTrue (firstLine.find("x  y") != std::string::npos);
		assertTrue (message.find('\r') == std::string::npos);
	}
}


void ValidatorTest::testSchemaExternalEntityRejected()
{
	Poco::TemporaryFile declarationFile;
	const std::string declarationURI = fileURI(writeFile(declarationFile, ELEMENT_B_DECLARATION));
	Poco::TemporaryFile dtdFile;
	const std::string dtdURI = fileURI(writeFile(dtdFile, std::string("<!ENTITY b '") + ELEMENT_B_DECLARATION + "'>"));
	const std::string schemaElement = R"(
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="a">
    <xs:complexType>
      <xs:sequence>&b;</xs:sequence>
    </xs:complexType>
  </xs:element>
</xs:schema>)";
	// Some libxml2 versions fail the parse on a reference to an entity declared only in an
	// unread external parameter entity, before the declarations are checked.
	const std::string schemaElementWithoutReference = R"(
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="a" type="xs:int"/>
</xs:schema>)";
	for (const std::string& schema: {
		"<!DOCTYPE xs:schema [<!ENTITY b SYSTEM \"" + declarationURI + "\">]>" + schemaElement,
		"<!DOCTYPE xs:schema [<!ENTITY % d SYSTEM \"" + dtdURI + "\"> %d;]>" + schemaElementWithoutReference,
		"<!DOCTYPE xs:schema SYSTEM \"" + dtdURI + "\">" + schemaElement})
	{
		try
		{
			Validator::validate("<a><b>1</b></a>", schema);
			fail("a schema with an external DTD or an external entity must throw");
		}
		catch (const Poco::DataFormatException& exc)
		{
			assertTrue (exc.message().find("XSD: external DTDs and external entities are not supported in the schema") != std::string::npos);
		}
	}
}


void ValidatorTest::testReferencedLocalSchemaLoaded()
{
	Poco::TemporaryFile includedFile;
	const std::string includedPath = writeFile(includedFile, R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="b" type="xs:int"/>
</xs:schema>)");
	// "file://localhost" is followed by the absolute path, which starts with a slash in a file URI.
	for (const std::string& location: {includedPath, fileURI(includedPath), "file://localhost" + fileURI(includedPath).substr(7)})
	{
		const std::string schema = R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + location + R"("/>
</xs:schema>)";
		Validator::validate("<b>1</b>", schema);
		try
		{
			Validator::validate("<b>x</b>", schema);
			fail("a value that is not an xs:int must fail schema validation");
		}
		catch (const Poco::DataFormatException& exc)
		{
			assertTrue (exc.message().find("XML: schema validation failed: ") != std::string::npos);
		}
	}
}


void ValidatorTest::testNestedReferencedLocalSchemasLoaded()
{
	// The last schema is named relative to the schema that includes it; all temporary files
	// are in the same directory.
	Poco::TemporaryFile lastFile;
	const std::string lastPath = writeFile(lastFile, R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="b" type="xs:int"/>
</xs:schema>)");
	Poco::TemporaryFile includedFile;
	const std::string includedPath = writeFile(includedFile, R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + Poco::Path(lastPath).getFileName() + R"("/>
</xs:schema>)");
	const std::string schema = R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + includedPath + R"("/>
</xs:schema>)";
	Validator::validate("<b>1</b>", schema);
	try
	{
		Validator::validate("<b>x</b>", schema);
		fail("a value that is not an xs:int must fail schema validation");
	}
	catch (const Poco::DataFormatException& exc)
	{
		assertTrue (exc.message().find("XML: schema validation failed: ") != std::string::npos);
	}
}


void ValidatorTest::testReferencedSchemaCycle()
{
	// File URIs, because libxml2 on Windows cannot resolve a drive-letter path named in an included schema.
	Poco::TemporaryFile firstFile;
	Poco::TemporaryFile secondFile;
	const std::string firstURI = fileURI(Poco::Path(firstFile.path()).absolute().toString());
	const std::string secondURI = fileURI(writeFile(secondFile, R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + firstURI + R"("/>
</xs:schema>)"));
	writeFile(firstFile, R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + secondURI + R"("/>
  <xs:element name="b" type="xs:int"/>
</xs:schema>)");
	Validator::validate("<b>1</b>", R"(<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:include schemaLocation=")" + firstURI + R"("/>
</xs:schema>)");
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
	CppUnit_addTest(pSuite, ValidatorTest, testValidateRejectsMalformedSchema);
	CppUnit_addTest(pSuite, ValidatorTest, testValidateThrowsOnEmptySchema);
	CppUnit_addTest(pSuite, ValidatorTest, testValidatorCannotBeInstantiated);
	CppUnit_addTest(pSuite, ValidatorTest, testSchemaIncludeNamedSchemaXsd);
	CppUnit_addTest(pSuite, ValidatorTest, testSchemaInternalEntities);
	CppUnit_addTest(pSuite, ValidatorTest, testMalformedDocumentFirstError);
	CppUnit_addTest(pSuite, ValidatorTest, testValidationMessageFormat);
	CppUnit_addTest(pSuite, ValidatorTest, testDocumentTypeDeclarationRejected);
	CppUnit_addTest(pSuite, ValidatorTest, testErrorMessageBounded);
	CppUnit_addTest(pSuite, ValidatorTest, testValidationMessageSingleLine);
	CppUnit_addTest(pSuite, ValidatorTest, testSchemaExternalEntityRejected);
	CppUnit_addTest(pSuite, ValidatorTest, testReferencedLocalSchemaLoaded);
	CppUnit_addTest(pSuite, ValidatorTest, testNestedReferencedLocalSchemasLoaded);
	CppUnit_addTest(pSuite, ValidatorTest, testReferencedSchemaCycle);

	return pSuite;
}
