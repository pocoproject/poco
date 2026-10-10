//
// Validator.h
//
// Library: XSD/Validator
// Package: XSDValidator
// Module:  Validator
//
// Definition of the Validator class.
//
// Copyright (c) 2021-2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Poco_XSD_Validator_Validator_INCLUDED
#define Poco_XSD_Validator_Validator_INCLUDED


#include "Poco/Foundation.h"
#include <string>


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the XSDValidator_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(XSDValidator_EXPORTS)
		#define XSDValidator_API __declspec(dllexport)
	#else
		#define XSDValidator_API __declspec(dllimport)
	#endif
#endif


#if !defined(XSDValidator_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define XSDValidator_API __attribute__ ((visibility ("default")))
	#else
		#define XSDValidator_API
	#endif
#endif


//
// Automatically link XSDValidator library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XSDValidator_EXPORTS)
		#pragma comment(lib, "PocoXSDValidator" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco::XSD::Validator {


class XSDValidator_API Validator
	/// Validates XML documents against an XSD schema.
	///
	/// Static-only: this class has no instance state and cannot be instantiated.
	/// All operations are exposed as static methods so callers don't need to
	/// manage object lifetime.
	///
	/// Implementation is backed by libxml2's xmlSchemaValidateDoc. The schema
	/// is parsed and compiled from xsdContent on every call (the current API
	/// does not retain a compiled schema between calls), so high-frequency
	/// validation paths should batch where possible or, in the future, use a
	/// stateful overload that retains a compiled xmlSchemaPtr.
	///
	/// validate() may be called concurrently from several threads.
	///
	/// The document may come from an untrusted source: it is parsed without
	/// network access and without entity expansion, and a document with a
	/// document type declaration is rejected.
	///
	/// The schema is parsed without network access and with its internal
	/// entities expanded; a schema that names an external DTD or declares
	/// external entities is rejected. Schemas referenced by xs:include,
	/// xs:import and xs:redefine are loaded by libxml2's default resource
	/// loader with the privileges of the process; depending on the libxml2
	/// version and build, it can read network locations and expands the
	/// external entities of those schemas, so schemas must come from a trusted
	/// source. With libxml2 before 2.13, diagnostics about them may be written
	/// to standard error.
	///
	/// The document is parsed into memory as a whole, so callers should limit
	/// the size of the documents they validate.
{
public:
	Validator() = delete;
		/// Not instantiable; see the class documentation.

	static void validate(const std::string& xml, const std::string& xsdContent);
		/// Validates the given XML string against the schema in xsdContent.
		///
		/// Throws Poco::DataFormatException if:
		///   - xsdContent is not well-formed XML
		///     ("XSD: malformed schema document: line N: ..."),
		///   - xsdContent names an external DTD or declares external entities
		///     ("XSD: external DTDs and external entities are not supported in the schema"),
		///   - xsdContent is not a valid XML Schema ("XSD: invalid schema: ..."),
		///   - xml is not well-formed XML ("XML: malformed document: line N: ..."),
		///   - xml contains a document type declaration
		///     ("XML: document type declarations are not supported"),
		///   - xml does not conform to the schema
		///     ("XML: schema validation failed: ...").
		/// The exception message lists at most the first ten libxml2 errors,
		/// separated by "; ".
		///
		/// Throws Poco::InvalidArgumentException if xml or xsdContent exceeds
		/// the libxml2 size limit (INT_MAX bytes).
		///
		/// Throws Poco::RuntimeException if libxml2 reports an internal error
		/// ("XSD: internal schema validator error: ...") or cannot allocate
		/// memory ("XML: libxml2 could not allocate memory").
		///
		/// Throws Poco::IllegalStateException if xsdContent is empty.
};


} // namespace Poco::XSD::Validator


#endif // Poco_XSD_Validator_Validator_INCLUDED
