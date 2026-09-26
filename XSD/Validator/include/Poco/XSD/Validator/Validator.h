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
	#define XSDValidator_API
#endif


//
// Automatically link XSDValidator library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(XSDValidator_EXPORTS)
		#pragma comment(lib, "PocoXSDValidator" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {
namespace XSD {
namespace Validator {


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
{
public:
	Validator() = delete;
		/// No instantiation — see class documentation.

	static void validate(const std::string& xml, const std::string& xsdContent);
		/// Validates the given XML string against the schema in xsdContent.
		///
		/// Throws Poco::DataFormatException with the underlying libxml2 error
		/// messages if:
		///   - xsdContent does not parse as a valid XML Schema,
		///   - xml is not well-formed XML,
		///   - xml does not conform to the schema.
		///
		/// Throws Poco::RuntimeException if libxml2 cannot create the parser
		/// or validation context, or reports an internal error.
		///
		/// Throws Poco::IllegalStateException if xsdContent is empty.
};


} } } // namespace Poco::XSD::Validator


#endif // Poco_XSD_Validator_Validator_INCLUDED
