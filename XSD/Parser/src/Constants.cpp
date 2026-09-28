//
// Constants.cpp
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  Constants
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Parser/Constants.h"


namespace Poco::XSD::Parser {


const std::string Constants::XSD_NAMESPACE_URI("http://www.w3.org/2001/XMLSchema");
const std::string Constants::XSD_ALL("all");
const std::string Constants::XSD_ANNOTATION("annotation");
const std::string Constants::XSD_ANY("any");
const std::string Constants::XSD_ANYATTRIBUTE("anyAttribute");
const std::string Constants::XSD_APPINFO("appinfo");
const std::string Constants::XSD_ATTRIBUTE("attribute");
const std::string Constants::XSD_ATTRIBUTEGROUP("attributeGroup");
const std::string Constants::XSD_CHOICE("choice");
const std::string Constants::XSD_COMPLEXCONTENT("complexContent");
const std::string Constants::XSD_COMPLEXTYPE("complexType");
const std::string Constants::XSD_DOCUMENTATION("documentation");
const std::string Constants::XSD_ELEMENT("element");
const std::string Constants::XSD_EXTENSION("extension");
const std::string Constants::XSD_FIELD("field");
const std::string Constants::XSD_GROUP("group");
const std::string Constants::XSD_IMPORT("import");
const std::string Constants::XSD_INCLUDE("include");
const std::string Constants::XSD_KEY("key");
const std::string Constants::XSD_KEYREF("keyref");
const std::string Constants::XSD_LIST("list");
const std::string Constants::XSD_METAANY("##any");
const std::string Constants::XSD_NOTATION("notation");
const std::string Constants::XSD_REDEFINE("redefine");
const std::string Constants::XSD_RESTRICTION("restriction");
const std::string Constants::XSD_SCHEMA("schema");
const std::string Constants::XSD_SELECTOR("selector");
const std::string Constants::XSD_SEQUENCE("sequence");
const std::string Constants::XSD_SIMPLECONTENT("simpleContent");
const std::string Constants::XSD_SIMPLETYPE("simpleType");
const std::string Constants::XSD_UNION("union");
const std::string Constants::XSD_UNIQUE("unique");
const std::string Constants::XSD_ENUMERATION("enumeration");
const std::string Constants::XSD_FRACTIONDIGITS("fractionDigits");
const std::string Constants::XSD_LENGTH("length");
const std::string Constants::XSD_MAXEXCLUSIVE("maxExclusive");
const std::string Constants::XSD_MAXINCLUSIVE("maxInclusive");
const std::string Constants::XSD_MAXLENGTH("maxLength");
const std::string Constants::XSD_MINEXCLUSIVE("minExclusive");
const std::string Constants::XSD_MININCLUSIVE("minInclusive");
const std::string Constants::XSD_MINLENGTH("minLength");
const std::string Constants::XSD_PATTERN("pattern");
const std::string Constants::XSD_TOTALDIGITS("totalDigits");
const std::string Constants::XSD_WHITESPACE("whiteSpace");
const std::string Constants::XSD_NAMESPACE("namespace");
const std::string Constants::XSD_PROCESSCONTENTS("processContents");

// Attributes
const std::string Constants::XSD_REF("ref");
const std::string Constants::XSD_ATTRIBUTEFORMDEFAULT("attributeFormDefault");
const std::string Constants::XSD_BLOCKDEFAULT("blockDefault");
const std::string Constants::XSD_ELEMENTFORMDEFAULT("elementFormDefault");
const std::string Constants::XSD_FINALDEFAULT("finalDefault");
const std::string Constants::XSD_ID("id");
const std::string Constants::XSD_TARGETNAMESPACE("targetNamespace");
const std::string Constants::XSD_VERSION("version");
const std::string Constants::XSD_ABSTRACT("abstract");
const std::string Constants::XSD_BLOCK("block");
const std::string Constants::XSD_FINAL("final");
const std::string Constants::XSD_FIXED("fixed");
const std::string Constants::XSD_FORM("form");
const std::string Constants::XSD_MAXOCCURS("maxOccurs");
const std::string Constants::XSD_MINOCCURS("minOccurs");
const std::string Constants::XSD_NAME("name");
const std::string Constants::XSD_NILLABLE("nillable");
const std::string Constants::XSD_DEFAULT("default");
const std::string Constants::XSD_SUBSTITUTIONGROUP("substitutionGroup");
const std::string Constants::XSD_TYPE("type");
const std::string Constants::XSD_SOURCE("source");
const std::string Constants::XSD_LANG("lang");
const std::string Constants::XSD_USE("use");
const std::string Constants::XSD_MIXED("mixed");
const std::string Constants::XSD_BASE("base");
const std::string Constants::XSD_SCHEMALOCATION("schemaLocation");
const std::string Constants::XSD_ITEMTYPE("itemType");
const std::string Constants::XSD_PUBLIC("public");
const std::string Constants::XSD_SYSTEM("system");
const std::string Constants::XSD_MEMBERTYPES("memberTypes");

// Attr Values
const std::string Constants::XSD_QUALIFIED("qualified");
const std::string Constants::XSD_UNQUALIFIED("unqualified");
const std::string Constants::XSD_SUBSTITUTION("substitution");
const std::string Constants::XSD_HASHALL("#all");
const std::string Constants::XSD_TRUE("true");
const std::string Constants::XSD_FALSE("false");
const std::string Constants::XSD_UNBOUNDED("unbounded");
const std::string Constants::XSD_STRICT("strict");
const std::string Constants::XSD_LAX("lax");
const std::string Constants::XSD_SKIP("skip");
const std::string Constants::XSD_REQUIRED("required");
const std::string Constants::XSD_OPTIONAL("optional");
const std::string Constants::XSD_PROHIBITED("prohibited");

// Other
const std::string Constants::XSD_COLON(":");
const std::string Constants::XSD_EMPTY_STRING;
const std::string Constants::XSD_DOUBLEHASH_ANY("##any");
const std::string Constants::XSD_SIMPLEEXTENSION("sx");
const std::string Constants::XSD_COMPLEXEXTENSION("cx");
const std::string Constants::XSD_SIMPLETYPERESTRICTION("str");
const std::string Constants::XSD_SIMPLECONTENTRESTRICTION("scr");
const std::string Constants::XSD_COMPLEXRESTRICTION("cr");

// WSDL
const std::string Constants::WSDL_NAMESPACE_URI("http://schemas.xmlsoap.org/wsdl/");
const std::string Constants::WSDL_DEFINITIONS("definitions");
const std::string Constants::WSDL_IMPORT("import");
const std::string Constants::WSDL_TYPES("types");
const std::string Constants::WSDL_MESSAGE("message");
const std::string Constants::WSDL_PART("part");
const std::string Constants::WSDL_PORTTYPE("portType");
const std::string Constants::WSDL_OPERATION("operation");
const std::string Constants::WSDL_DOCUMENTATION("documentation");
const std::string Constants::WSDL_INPUT("input");
const std::string Constants::WSDL_OUTPUT("output");
const std::string Constants::WSDL_FAULT("fault");
const std::string Constants::WSDL_BINDING("binding");
const std::string Constants::WSDL_SERVICE("service");
const std::string Constants::WSDL_PORT("port");
const std::string Constants::WSDL_PARAMETER_ORDER("parameterOrder");
const std::string Constants::WSDL_NAMESPACE("namespace");
const std::string Constants::WSDL_LOCATION("location");

// SOAP
const std::string Constants::SOAP_NAMESPACE_URI("http://schemas.xmlsoap.org/wsdl/soap/");
const std::string Constants::SOAP12_NAMESPACE_URI("http://schemas.xmlsoap.org/wsdl/soap12/");
const std::string Constants::SOAP_BINDING("binding");
const std::string Constants::SOAP_OPERATION("operation");
const std::string Constants::SOAP_HEADER("header");
const std::string Constants::SOAP_HEADERFAULT("headerfault");
const std::string Constants::SOAP_BODY("body");
const std::string Constants::SOAP_FAULT("fault");
const std::string Constants::SOAP_ADDRESS("address");
const std::string Constants::SOAP_STYLE("style");
const std::string Constants::SOAP_TRANSPORT("transport");
const std::string Constants::SOAP_ACTION("soapAction");
const std::string Constants::SOAP_PART("part");
const std::string Constants::SOAP_PARTS("parts");
const std::string Constants::SOAP_USE("use");
const std::string Constants::SOAP_ENCODINGSTYLE("encodingStyle");
const std::string Constants::SOAP_NAMESPACE("namespace");
const std::string Constants::SOAP_MESSAGE("message");

// WS-Addressing
const std::string Constants::WSA_NAMESPACE_URI("http://schemas.xmlsoap.org/ws/2004/08/addressing");
const std::string Constants::WSA_ACTION("Action");


} // namespace Poco::XSD::Parser
