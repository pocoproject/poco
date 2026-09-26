//
// Constants.h
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  Constants
//
// Definition of the Constants class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDParser_Constants_INCLUDED
#define XSDParser_Constants_INCLUDED


#include "Poco/XSD/Parser/XSDParser.h"


namespace Poco {
namespace XSD {
namespace Parser {


class XSDParser_API	Constants
	/// Constants used by the parser.
{
public:
	static const std::string XSD_NAMESPACE_URI;
	static const std::string XSD_ALL;
	static const std::string XSD_ANNOTATION;
	static const std::string XSD_ANY;
	static const std::string XSD_ANYATTRIBUTE;
	static const std::string XSD_APPINFO;
	static const std::string XSD_ATTRIBUTE;
	static const std::string XSD_ATTRIBUTEGROUP;
	static const std::string XSD_CHOICE;
	static const std::string XSD_COMPLEXCONTENT;
	static const std::string XSD_COMPLEXTYPE;
	static const std::string XSD_DOCUMENTATION;
	static const std::string XSD_ELEMENT;
	static const std::string XSD_EXTENSION;
	static const std::string XSD_FIELD;
	static const std::string XSD_GROUP;
	static const std::string XSD_IMPORT;
	static const std::string XSD_INCLUDE;
	static const std::string XSD_KEY;
	static const std::string XSD_KEYREF;
	static const std::string XSD_LIST;
	static const std::string XSD_METAANY;
	static const std::string XSD_NOTATION;
	static const std::string XSD_REDEFINE;
	static const std::string XSD_RESTRICTION;
	static const std::string XSD_SCHEMA;
	static const std::string XSD_SELECTOR;
	static const std::string XSD_SEQUENCE;
	static const std::string XSD_SIMPLECONTENT;
	static const std::string XSD_SIMPLETYPE;
	static const std::string XSD_UNION;
	static const std::string XSD_UNIQUE;
	static const std::string XSD_ENUMERATION;
	static const std::string XSD_FRACTIONDIGITS;
	static const std::string XSD_LENGTH;
	static const std::string XSD_MAXEXCLUSIVE;
	static const std::string XSD_MAXINCLUSIVE;
	static const std::string XSD_MAXLENGTH;
	static const std::string XSD_MINEXCLUSIVE;
	static const std::string XSD_MININCLUSIVE;
	static const std::string XSD_MINLENGTH;
	static const std::string XSD_PATTERN;
	static const std::string XSD_TOTALDIGITS;
	static const std::string XSD_WHITESPACE;
	
	// attributes of schema entries
	static const std::string XSD_REF;
	static const std::string XSD_ATTRIBUTEFORMDEFAULT;
	static const std::string XSD_BLOCKDEFAULT;
	static const std::string XSD_ELEMENTFORMDEFAULT;
	static const std::string XSD_FINALDEFAULT;
	static const std::string XSD_ID;
	static const std::string XSD_TARGETNAMESPACE;
	static const std::string XSD_VERSION;
	static const std::string XSD_ABSTRACT;
	static const std::string XSD_BLOCK;
	static const std::string XSD_FINAL;
	static const std::string XSD_FIXED;
	static const std::string XSD_FORM;
	static const std::string XSD_MAXOCCURS;
	static const std::string XSD_MINOCCURS;
	static const std::string XSD_NAME;
	static const std::string XSD_NILLABLE;
	static const std::string XSD_DEFAULT;
	static const std::string XSD_SUBSTITUTIONGROUP;
	static const std::string XSD_TYPE;
	static const std::string XSD_SOURCE;
	static const std::string XSD_LANG;
	static const std::string XSD_NAMESPACE;
	static const std::string XSD_PROCESSCONTENTS;
	static const std::string XSD_USE;
	static const std::string XSD_MIXED;
	static const std::string XSD_BASE;
	static const std::string XSD_SCHEMALOCATION;
	static const std::string XSD_ITEMTYPE;
	static const std::string XSD_PUBLIC;
	static const std::string XSD_SYSTEM;
	static const std::string XSD_MEMBERTYPES;
	
	// Attr Values
	static const std::string XSD_QUALIFIED;
	static const std::string XSD_UNQUALIFIED;
	static const std::string XSD_SUBSTITUTION;
	static const std::string XSD_HASHALL;
	static const std::string XSD_TRUE;
	static const std::string XSD_FALSE;
	static const std::string XSD_UNBOUNDED;
	static const std::string XSD_STRICT;
	static const std::string XSD_LAX;
	static const std::string XSD_SKIP;
	static const std::string XSD_REQUIRED;
	static const std::string XSD_OPTIONAL;
	static const std::string XSD_PROHIBITED;

	enum
	{
		VAL_UNBOUNDED = 0xffffffff
	};

	// Other
	static const std::string XSD_COLON;
	static const char        XSD_COLONCHAR;
	static const std::string XSD_EMPTY_STRING;
	static const std::string XSD_DOUBLEHASH_ANY;
	static const std::string XSD_SIMPLEEXTENSION;
	static const std::string XSD_COMPLEXEXTENSION;
	static const std::string XSD_SIMPLETYPERESTRICTION;
	static const std::string XSD_SIMPLECONTENTRESTRICTION;
	static const std::string XSD_COMPLEXRESTRICTION;
	
	// WSDL
	static const std::string WSDL_NAMESPACE_URI;
	static const std::string WSDL_DEFINITIONS;
	static const std::string WSDL_IMPORT;
	static const std::string WSDL_TYPES;
	static const std::string WSDL_MESSAGE;
	static const std::string WSDL_PART;
	static const std::string WSDL_PORTTYPE;
	static const std::string WSDL_OPERATION;
	static const std::string WSDL_DOCUMENTATION;
	static const std::string WSDL_INPUT;
	static const std::string WSDL_OUTPUT;
	static const std::string WSDL_FAULT;
	static const std::string WSDL_BINDING;
	static const std::string WSDL_SERVICE;
	static const std::string WSDL_PORT;
	static const std::string WSDL_PARAMETER_ORDER;
	static const std::string WSDL_NAMESPACE;
	static const std::string WSDL_LOCATION;
	
	// SOAP
	static const std::string SOAP_NAMESPACE_URI;
	static const std::string SOAP12_NAMESPACE_URI;
	static const std::string SOAP_BINDING;
	static const std::string SOAP_OPERATION;
	static const std::string SOAP_HEADER;
	static const std::string SOAP_HEADERFAULT;
	static const std::string SOAP_BODY;
	static const std::string SOAP_FAULT;
	static const std::string SOAP_ADDRESS;
	static const std::string SOAP_STYLE;
	static const std::string SOAP_TRANSPORT;
	static const std::string SOAP_ACTION;
	static const std::string SOAP_PART;
	static const std::string SOAP_PARTS;
	static const std::string SOAP_USE;
	static const std::string SOAP_ENCODINGSTYLE;
	static const std::string SOAP_NAMESPACE;
	static const std::string SOAP_MESSAGE;

	// WS-Addressing
	static const std::string WSA_NAMESPACE_URI;
	static const std::string WSA_ACTION;

private:
	Constants();
	~Constants();
	Constants(const Constants&);
	Constants& operator=(const Constants&);
};


} } } // namespace Poco::XSD::Parser


#endif // XSDParser_Constants_INCLUDED
