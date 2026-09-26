//
// Validator.cpp
//
// Library: XSD/Validator
// Package: XSDValidator
// Module:  Validator
//
// Copyright (c) 2021-2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Validator/Validator.h"
#include "Poco/Exception.h"
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <cstdarg>
#include <cstdio>


namespace Poco {
namespace XSD {
namespace Validator {


namespace {


void XMLCDECL collectLibXmlError(void* userData, const char* msg, ...)
{
	if (!userData || !msg) return;

	char buffer[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg, args);
	va_end(args);

	std::string& out = *static_cast<std::string*>(userData);
	out += buffer;
}


} // namespace


void Validator::validate(const std::string& xml, const std::string& xsdContent)
{
	if (xsdContent.empty())
		throw Poco::IllegalStateException("XSD schema content is empty");

	// Each phase (schema parse, XML parse, validate) gets its own error
	// buffer so the exception message contains only the diagnostics relevant
	// to the stage that actually failed — not warnings carried over from
	// earlier stages.
	std::string schemaErrors;
	std::string xmlErrors;
	std::string validateErrors;

	xmlSchemaParserCtxtPtr pParserCtxt =
		xmlSchemaNewMemParserCtxt(xsdContent.data(), static_cast<int>(xsdContent.size()));
	if (!pParserCtxt)
		throw Poco::RuntimeException("XSD: cannot create schema parser context");
	xmlSchemaSetParserErrors(pParserCtxt, collectLibXmlError, collectLibXmlError, &schemaErrors);

	xmlSchemaPtr pSchema = xmlSchemaParse(pParserCtxt);
	xmlSchemaFreeParserCtxt(pParserCtxt);
	if (!pSchema)
		throw Poco::DataFormatException("XSD: invalid schema: " + schemaErrors);

	// Capture XML parse errors via libxml2's generic error handler so the
	// thrown message reflects the actual parse failure (xmlReadMemory does
	// not route through the schema parser/validator callbacks).
	xmlSetGenericErrorFunc(&xmlErrors, collectLibXmlError);
	xmlDocPtr pDoc = xmlReadMemory(xml.data(), static_cast<int>(xml.size()),
		"document.xml", nullptr, XML_PARSE_NONET);
	xmlSetGenericErrorFunc(nullptr, nullptr);
	if (!pDoc)
	{
		xmlSchemaFree(pSchema);
		throw Poco::DataFormatException("XML: malformed document: " + xmlErrors);
	}

	xmlSchemaValidCtxtPtr pValidCtxt = xmlSchemaNewValidCtxt(pSchema);
	if (!pValidCtxt)
	{
		xmlFreeDoc(pDoc);
		xmlSchemaFree(pSchema);
		throw Poco::RuntimeException("XSD: cannot create schema validation context");
	}
	xmlSchemaSetValidErrors(pValidCtxt, collectLibXmlError, collectLibXmlError, &validateErrors);

	const int rc = xmlSchemaValidateDoc(pValidCtxt, pDoc);

	xmlSchemaFreeValidCtxt(pValidCtxt);
	xmlFreeDoc(pDoc);
	xmlSchemaFree(pSchema);

	// xmlSchemaValidateDoc:
	//   0  : XML is valid against the schema
	//   >0 : XML violates the schema (validation failure)
	//   <0 : internal libxml2 error (e.g. out-of-memory, broken schema state)
	// Distinguishing these helps callers diagnose configuration vs library issues.
	if (rc > 0)
		throw Poco::DataFormatException("XML: schema validation failed: " + validateErrors);
	if (rc < 0)
		throw Poco::RuntimeException("XSD: internal schema validator error: " + validateErrors);
}


} } } // namespace Poco::XSD::Validator
