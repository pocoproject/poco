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
#include "Poco/Format.h"
#include "Poco/String.h"
#include <libxml/parser.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlversion.h>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>


namespace Poco::XSD::Validator {


namespace {


struct LibXmlInitializer
	/// Initializes libxml2 and its schema type registry once, before the first use from any thread;
	/// libxml2 before 2.12 initializes the registry lazily without a lock.
{
	LibXmlInitializer()
	{
		xmlInitParser();
		xmlSchemaInitTypes();
	}
};


template <auto Free>
struct LibXmlDeleter
	/// Deleter for a std::unique_ptr that owns a libxml2 object.
{
	template <typename T>
	void operator () (T* pObject) const
	{
		Free(pObject);
	}
};


using SchemaParserCtxtPtr = std::unique_ptr<xmlSchemaParserCtxt, LibXmlDeleter<xmlSchemaFreeParserCtxt>>;
using SchemaPtr = std::unique_ptr<xmlSchema, LibXmlDeleter<xmlSchemaFree>>;
using ParserCtxtPtr = std::unique_ptr<xmlParserCtxt, LibXmlDeleter<xmlFreeParserCtxt>>;
using DocPtr = std::unique_ptr<xmlDoc, LibXmlDeleter<xmlFreeDoc>>;
using SchemaValidCtxtPtr = std::unique_ptr<xmlSchemaValidCtxt, LibXmlDeleter<xmlSchemaFreeValidCtxt>>;


// From libxml2 2.12, xmlStructuredErrorFunc takes the error as a pointer to const.
#if LIBXML_VERSION >= 21200
using ErrorPtr = const xmlError*;
#else
using ErrorPtr = xmlErrorPtr;
#endif


// The schema is trusted and its internal entities are expanded, as libxml2's own schema
// loader does; external entities stay off where libxml2 allows it.
#if LIBXML_VERSION >= 21300
constexpr int SCHEMA_PARSE_OPTIONS = XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE;
#else
constexpr int SCHEMA_PARSE_OPTIONS = XML_PARSE_NONET | XML_PARSE_NOENT;
#endif

// Entities are not expanded; documents with a document type declaration are rejected by validate().
constexpr int DOCUMENT_PARSE_OPTIONS = XML_PARSE_NONET;

constexpr const char* OUT_OF_MEMORY_MESSAGE = "XML: libxml2 could not allocate memory";


class ErrorCollector
	/// Collects the errors libxml2 reports for one stage of validate(); keeps the first
	/// MAX_MESSAGES messages in the order reported and counts the rest.
{
public:
	void add(ErrorPtr pError) noexcept
		/// Records an error; warnings are ignored.
		/// libxml2 calls this function from C code, so no exception may leave it.
	{
		if (pError == nullptr) return;
		if (pError->code == XML_ERR_NO_MEMORY)
		{
			_outOfMemory = true;
			return;
		}
		if (pError->level <= XML_ERR_WARNING) return;
		if (_messages.size() >= MAX_MESSAGES)
		{
			++_omitted;
			return;
		}
		try
		{
			std::string text(pError->message != nullptr ? pError->message : "unknown error");
			Poco::trimRightInPlace(text);
			// A message may span several lines; the exception message stays on one.
			Poco::translateInPlace(text, "\r\n", "  ");
			if (pError->line > 0)
				_messages.push_back(Poco::format("line %d: %s", pError->line, text));
			else
				_messages.push_back(text);
		}
		catch (...)
		{
			// Only an allocation can fail here.
			_outOfMemory = true;
		}
	}

	[[nodiscard]] bool outOfMemory() const
		/// Returns true if libxml2 or the collector ran out of memory.
	{
		return _outOfMemory;
	}

	[[nodiscard]] std::string message() const
		/// Returns the collected messages joined with "; ".
	{
		if (_messages.empty()) return "unknown error";
		std::string result = Poco::cat(std::string("; "), _messages.begin(), _messages.end());
		if (_omitted == 1)
			result += "; ... (1 further error)";
		else if (_omitted > 1)
			result += Poco::format("; ... (%z further errors)", _omitted);
		return result;
	}

private:
	static constexpr std::size_t MAX_MESSAGES = 10;

	std::vector<std::string> _messages;
	std::size_t _omitted = 0;
	bool _outOfMemory = false;
};


void XMLCALL onError(void* userData, ErrorPtr pError)
	/// Forwards a libxml2 error to the ErrorCollector given as userData.
{
	static_cast<ErrorCollector*>(userData)->add(pError);
}


#if LIBXML_VERSION < 21300
void XMLCALL onParserError(void* ctx, ErrorPtr pError)
	/// Forwards a parser error to the ErrorCollector in the parser context's _private;
	/// libxml2 before 2.13 passes the parser context itself as ctx.
{
	static_cast<ErrorCollector*>(static_cast<xmlParserCtxtPtr>(ctx)->_private)->add(pError);
}
#endif


DocPtr parseXML(const std::string& text, int options, ErrorCollector& errors)
	/// Parses text with a parser context of its own and reports its errors to errors.
	/// Returns a null pointer if text is not well-formed.
{
	if (text.size() > static_cast<std::string::size_type>(std::numeric_limits<int>::max()))
		throw Poco::InvalidArgumentException("XML: text exceeds the libxml2 size limit");
	ParserCtxtPtr pCtxt(xmlNewParserCtxt());
	if (pCtxt == nullptr)
		throw Poco::RuntimeException(OUT_OF_MEMORY_MESSAGE);
#if LIBXML_VERSION >= 21300
	xmlCtxtSetErrorHandler(pCtxt.get(), onError, &errors);
#else
	pCtxt->sax->serror = onParserError;
	pCtxt->_private = &errors;
#endif
	return DocPtr(xmlCtxtReadMemory(pCtxt.get(), text.data(), static_cast<int>(text.size()), nullptr, nullptr, options));
}


[[noreturn]] void throwParseError(const std::string& prefix, const ErrorCollector& errors)
	/// Throws the exception for a failed stage, with the collected messages.
{
	if (errors.outOfMemory())
		throw Poco::RuntimeException(OUT_OF_MEMORY_MESSAGE);
	throw Poco::DataFormatException(prefix + errors.message());
}


} // namespace


void Validator::validate(const std::string& xml, const std::string& xsdContent)
{
	static const LibXmlInitializer libXmlInitializer;

	if (xsdContent.empty())
		throw Poco::IllegalStateException("XSD schema content is empty");

	// Each stage (schema parse, document parse, validation) reports its own
	// diagnostics, so the exception message contains only what failed.
	ErrorCollector schemaErrors;
	ErrorCollector documentErrors;
	ErrorCollector validationErrors;

	// The schema document must outlive the schema compiled from it.
	DocPtr pSchemaDoc = parseXML(xsdContent, SCHEMA_PARSE_OPTIONS, schemaErrors);
	if (pSchemaDoc == nullptr)
		throwParseError("XSD: malformed schema document: ", schemaErrors);
	SchemaParserCtxtPtr pParserCtxt(xmlSchemaNewDocParserCtxt(pSchemaDoc.get()));
	if (pParserCtxt == nullptr)
		throw Poco::RuntimeException(OUT_OF_MEMORY_MESSAGE);
	xmlSchemaSetParserStructuredErrors(pParserCtxt.get(), onError, &schemaErrors);

	SchemaPtr pSchema(xmlSchemaParse(pParserCtxt.get()));
	pParserCtxt.reset();
	if (pSchema == nullptr)
		throwParseError("XSD: invalid schema: ", schemaErrors);

	DocPtr pDoc = parseXML(xml, DOCUMENT_PARSE_OPTIONS, documentErrors);
	if (pDoc == nullptr)
		throwParseError("XML: malformed document: ", documentErrors);

	// A document type declaration can add default attributes and entities that the
	// application's own XML parser applies but schema validation would not see.
	if (pDoc->intSubset != nullptr || pDoc->extSubset != nullptr)
		throw Poco::DataFormatException("XML: document type declarations are not supported");

	SchemaValidCtxtPtr pValidCtxt(xmlSchemaNewValidCtxt(pSchema.get()));
	if (pValidCtxt == nullptr)
		throw Poco::RuntimeException(OUT_OF_MEMORY_MESSAGE);
	xmlSchemaSetValidStructuredErrors(pValidCtxt.get(), onError, &validationErrors);

	const int rc = xmlSchemaValidateDoc(pValidCtxt.get(), pDoc.get());
	if (rc != 0 && validationErrors.outOfMemory())
		throw Poco::RuntimeException(OUT_OF_MEMORY_MESSAGE);
	if (rc > 0)
		throw Poco::DataFormatException("XML: schema validation failed: " + validationErrors.message());
	if (rc < 0)
		throw Poco::RuntimeException("XSD: internal schema validator error: " + validationErrors.message());
}


} // namespace Poco::XSD::Validator
