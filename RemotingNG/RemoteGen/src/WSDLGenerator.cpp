//
// WSDLGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "WSDLGenerator.h"
#include "XSDGenerator.h"
#include "GenUtility.h"
#include "ProxyGenerator.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SAX/AttributesImpl.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/LayeredConfiguration.h"
#include <cctype>


using Poco::CodeGeneration::Utility;
using namespace Poco::XML;


const Poco::XML::XMLString WSDLGenerator::DEFINITIONS("definitions");
const Poco::XML::XMLString WSDLGenerator::SOAP_NS("http://schemas.xmlsoap.org/wsdl/soap/");
const Poco::XML::XMLString WSDLGenerator::SOAP12_NS("http://schemas.xmlsoap.org/wsdl/soap12/");
const Poco::XML::XMLString WSDLGenerator::SOAPHTTP_NS("http://schemas.xmlsoap.org/soap/http");
const Poco::XML::XMLString WSDLGenerator::SOAPENC_NS("http://schemas.xmlsoap.org/soap/encoding/");
const Poco::XML::XMLString WSDLGenerator::WSDL_NS("http://schemas.xmlsoap.org/wsdl/");
const Poco::XML::XMLString WSDLGenerator::SOAP_PREFIX("soap");
const Poco::XML::XMLString WSDLGenerator::SOAP12_PREFIX("soap12");
const Poco::XML::XMLString WSDLGenerator::SOAPENC_PREFIX("soapenc");
const Poco::XML::XMLString WSDLGenerator::WSDL_PREFIX("wsdl");
const Poco::XML::XMLString WSDLGenerator::TYPE_PREFIX("ts");
const Poco::XML::XMLString WSDLGenerator::WSDLTYPE_PREFIX("wts");
const Poco::XML::XMLString WSDLGenerator::MESSAGE("message");
const Poco::XML::XMLString WSDLGenerator::PART("part");
const Poco::XML::XMLString WSDLGenerator::PARTS("parts");
const Poco::XML::XMLString WSDLGenerator::PORTTYPE("portType");
const Poco::XML::XMLString WSDLGenerator::PORT("port");
const Poco::XML::XMLString WSDLGenerator::INPUT("input");
const Poco::XML::XMLString WSDLGenerator::OUTPUT("output");
const Poco::XML::XMLString WSDLGenerator::FAULT("fault");
const Poco::XML::XMLString WSDLGenerator::OPERATION("operation");
const Poco::XML::XMLString WSDLGenerator::BINDING("binding");
const Poco::XML::XMLString WSDLGenerator::STYLE("style");
const Poco::XML::XMLString WSDLGenerator::TRANSPORT("transport");
const Poco::XML::XMLString WSDLGenerator::BODY("body");
const Poco::XML::XMLString WSDLGenerator::PARAMETERS("parameters");
const Poco::XML::XMLString WSDLGenerator::SOAPACTION("soapAction");
const Poco::XML::XMLString WSDLGenerator::SERVICE("service");
const Poco::XML::XMLString WSDLGenerator::ADDRESS("address");
const Poco::XML::XMLString WSDLGenerator::LOCATION("location");
const Poco::XML::XMLString WSDLGenerator::TYPES("types");
const Poco::XML::XMLString WSDLGenerator::MSG("Msg");
const Poco::XML::XMLString WSDLGenerator::SOAPHEADER("header");
const Poco::XML::XMLString WSDLGenerator::REMOTING_NS("http://www.appinf.com/remoting");
const Poco::XML::XMLString WSDLGenerator::REMOTING_PREFIX("remoting");


WSDLGenerator::WSDLGenerator(Poco::CodeGeneration::CppGenerator& cppGen, const std::string& targetNamespace, Poco::XML::ContentHandler& wsdlOut):
	AbstractGenerator(cppGen),
	_wsdlOut(wsdlOut),
	_targetNamespace(targetNamespace)
{
}


WSDLGenerator::~WSDLGenerator()
{
}


std::string WSDLGenerator::generateFileName(const Poco::CppParser::Struct* pStruct)
{
	return generateFileName(pStruct->name());
}


std::string WSDLGenerator::generateFileName(const std::string& structName)
{
	return structName + ".wsdl";
}


void WSDLGenerator::writeRemotingSchema()
{
	AttributesImpl xsdAttr;
	xsdAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TARGETNAMESPACE, XSDGenerator::TARGETNAMESPACE, XSDGenerator::EMPTYSTRING, REMOTING_NS);
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::SCHEMA, XSDGenerator::EMPTYSTRING, xsdAttr);

	AttributesImpl elemAttr;
	elemAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Exception");
	elemAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TYPE, XSDGenerator::TYPE, XSDGenerator::EMPTYSTRING, "remoting:Exception");
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, elemAttr);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING);

	AttributesImpl complexTypeAttr;
	complexTypeAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Exception");
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::COMPLEXTYPE, XSDGenerator::EMPTYSTRING, complexTypeAttr);

	AttributesImpl emptyAttr;
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::SEQUENCE, XSDGenerator::EMPTYSTRING, emptyAttr);

	AttributesImpl typeAttr;
	typeAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Type");
	typeAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TYPE, XSDGenerator::TYPE, XSDGenerator::EMPTYSTRING, "xsd:string");
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, typeAttr);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING);

	AttributesImpl msgAttr;
	msgAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Message");
	msgAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TYPE, XSDGenerator::TYPE, XSDGenerator::EMPTYSTRING, "xsd:string");
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, msgAttr);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING);

	AttributesImpl codeAttr;
	codeAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Code");
	codeAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TYPE, XSDGenerator::TYPE, XSDGenerator::EMPTYSTRING, "xsd:int");
	_wsdlOut.startElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, codeAttr);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING);

	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::SEQUENCE, XSDGenerator::EMPTYSTRING);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::COMPLEXTYPE, XSDGenerator::EMPTYSTRING);
	_wsdlOut.endElement(XSDGenerator::SCHEMA_NS, XSDGenerator::SCHEMA, XSDGenerator::EMPTYSTRING);
}


void WSDLGenerator::writeExceptionMessage()
{
	AttributesImpl messageAttr;
	messageAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "ExceptionFaultMsg");
	_wsdlOut.startElement(WSDL_NS, MESSAGE, XSDGenerator::EMPTYSTRING, messageAttr);

	AttributesImpl partAttr;
	partAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "fault");
	partAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::ELEMENT, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, "remoting:Exception");
	_wsdlOut.startElement(WSDL_NS, PART, XSDGenerator::EMPTYSTRING, partAttr);
	_wsdlOut.endElement(WSDL_NS, PART, XSDGenerator::EMPTYSTRING);

	_wsdlOut.endElement(WSDL_NS, MESSAGE, XSDGenerator::EMPTYSTRING);
}


void WSDLGenerator::structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties)
{
	_pStructIn = pStruct;
	_functions.clear();
	_wsdlOut.startDocument();
	/*
	<definitions name="StockQuotes"
	             targetNamespace="http://www.appinf.com/ns/StockQuotes"
	             xmlns="http://schemas.xmlsoap.org/wsdl/"
	             xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
	             xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
	             xmlns:soap12="http://schemas.xmlsoap.org/wsdl/soap12/"
	             xmlns:ts="http://www.appinf.com/ns/StockQuotes" xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"
	             xmlns:wts="http://www.appinf.com/ns/StockQuotes" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
	*/
	_wsdlOut.startPrefixMapping(XSDGenerator::SCHEMA_PREFIX, XSDGenerator::SCHEMA_NS);
	_wsdlOut.startPrefixMapping(SOAP_PREFIX, SOAP_NS);
	_wsdlOut.startPrefixMapping(SOAP12_PREFIX, SOAP12_NS);
	_wsdlOut.startPrefixMapping(SOAPENC_PREFIX, SOAPENC_NS);
	_wsdlOut.startPrefixMapping(WSDL_PREFIX, WSDL_NS);
	_wsdlOut.startPrefixMapping(XSDGenerator::EMPTYSTRING, WSDL_NS);
	_wsdlOut.startPrefixMapping(TYPE_PREFIX, _targetNamespace);
	_wsdlOut.startPrefixMapping(WSDLTYPE_PREFIX, _targetNamespace);
	_wsdlOut.startPrefixMapping(REMOTING_PREFIX, REMOTING_NS);
	AttributesImpl attr;
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TARGETNAMESPACE, XSDGenerator::TARGETNAMESPACE, XSDGenerator::EMPTYSTRING, _targetNamespace);
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, pStruct->name());
	_wsdlOut.startElement(WSDL_NS, DEFINITIONS, DEFINITIONS, attr);
	AttributesImpl empty;
	_wsdlOut.startElement(WSDL_NS, TYPES, TYPES, empty);
	writeRemotingSchema();
	XSDGenerator xsd(_cppGen, _targetNamespace, _wsdlOut, true);
	Poco::CodeGeneration::GeneratorEngine e;
	e.generate(pStruct, xsd);
	_wsdlOut.endElement(WSDL_NS, TYPES, TYPES);
	writeExceptionMessage();
}


void WSDLGenerator::structEnd()
{
	// include all methods of all superclasses, include them in structEnd so that the mehtods of the subclass
	// take precedence over the parents class
	handleParentFunctions(_pStructIn);
	// write PortTypes
	AttributesImpl attr;
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, _pStructIn->name());
	_wsdlOut.startElement(WSDL_NS, PORTTYPE, PORTTYPE, attr);
	PortTypeVec::const_iterator itPT = _portTypes.begin();
	PortTypeVec::const_iterator itPTEnd = _portTypes.end();
	for (; itPT != itPTEnd; ++itPT)
	{
		writePortType(*itPT);
	}
	_wsdlOut.endElement(WSDL_NS, PORTTYPE, PORTTYPE);

	writeBinding(SOAP_NS, "SoapBinding");
	writeBinding(SOAP12_NS, "Soap12Binding");

	writeServiceLocations(SOAP_NS, _pStructIn->name(), "", "SoapBinding");
	writeServiceLocations(SOAP12_NS, _pStructIn->name(), "12", "Soap12Binding");

	_wsdlOut.endElement(WSDL_NS, DEFINITIONS, DEFINITIONS);
	_wsdlOut.endDocument();
}


void WSDLGenerator::writeBinding(const std::string& ns, const std::string& bindingSuffix)
{
	/*
	<binding name="StockQuotesSoapBinding" type="wts:StockQuotes">
		<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
	*/
	AttributesImpl attrB;
	attrB.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, _pStructIn->name()+bindingSuffix);
	attrB.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::TYPE, XSDGenerator::TYPE, XSDGenerator::EMPTYSTRING, WSDLTYPE_PREFIX+":"+_pStructIn->name());
	_wsdlOut.startElement(WSDL_NS, BINDING, BINDING, attrB);
	AttributesImpl attrS;
	attrS.addAttribute(XSDGenerator::EMPTYSTRING, STYLE, STYLE, XSDGenerator::EMPTYSTRING, "document");
	attrS.addAttribute(XSDGenerator::EMPTYSTRING, TRANSPORT, TRANSPORT, XSDGenerator::EMPTYSTRING, SOAPHTTP_NS);
	_wsdlOut.startElement(ns, BINDING, BINDING, attrS);
	_wsdlOut.endElement(ns, BINDING, BINDING);
	BindingInfoVec::const_iterator itBI = _bindingInfos.begin();
	BindingInfoVec::const_iterator itBIEnd = _bindingInfos.end();
	for (; itBI != itBIEnd; ++itBI)
	{
		writeBindingInfo(ns, *itBI);
	}
	_wsdlOut.endElement(WSDL_NS, BINDING, BINDING);
}


void WSDLGenerator::methodStart(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
	// due to the grouping of input/output params in the schema, the message part will be very simple for each method
	// we have at most two messages: one for request, one for reply
	/*
	<message name="quoteForRequestMsg">
	     <part element="ts:quoteFor" name="parameters"/>
	</message>
	*/

	Poco::CodeGeneration::CodeGenerator::Properties funcProps(properties);
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFuncOld, funcProps);
	if (!GenUtility::isRemoteMethod(pFuncOld, funcProps)) return;

	std::string methodName = GenUtility::getMethodName(pFuncOld);
	if (_functions.find(methodName) != _functions.end())
		return;

	_functions.insert(methodName);

	std::string reqName;
	std::string repName;
	std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*> soapReqParams = getSoapHeader(pFuncOld, true);
	std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*> soapRespParams = getSoapHeader(pFuncOld, false);
	std::multimap<Poco::UInt32, HeaderInfo> soapReqHeaders;
	std::multimap<Poco::UInt32, HeaderInfo> soapRespHeaders;

	{
		reqName = methodName;
		reqName.append(XSDGenerator::REQUEST);
		AttributesImpl attr;
		attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, reqName+MSG);
		_wsdlOut.startElement(WSDL_NS, MESSAGE, MESSAGE, attr);
		attr.setValue(0, PARAMETERS);
		attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::ELEMENT, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, TYPE_PREFIX + ":" + methodName);
		// we always write first the header parts, then the body!
		std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*>::const_iterator it = soapReqParams.begin();
		std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*>::const_iterator itEnd = soapReqParams.end();
		for (; it != itEnd; ++it)
		{
			std::string partName = it->second->name()+SOAPHEADER;
			std::string paramName = GenUtility::getParameterName(pFuncOld, it->second);
			std::string partElement = TYPE_PREFIX + ":" + paramName;
			AttributesImpl attrHeader;
			attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, partName);
			attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::ELEMENT, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, partElement);
			_wsdlOut.startElement(WSDL_NS, PART, PART, attrHeader);
			_wsdlOut.endElement(WSDL_NS, PART, PART);
			soapReqHeaders.insert(std::make_pair(it->first, HeaderInfo(WSDLTYPE_PREFIX+":"+reqName+MSG, partName, "literal")));
		}

		_wsdlOut.startElement(WSDL_NS, PART, PART, attr);
		_wsdlOut.endElement(WSDL_NS, PART, PART);
		_wsdlOut.endElement(WSDL_NS, MESSAGE, MESSAGE);
	}

	bool isOneWay = false;
	Poco::CodeGeneration::GeneratorEngine::getBoolProperty(funcProps, Poco::CodeGeneration::Utility::ONEWAY, isOneWay);
	// now check for Reply
	if (!isOneWay)
	{
		repName = GenUtility::getReplyMethodName(pFuncOld);
		AttributesImpl attr;
		attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, repName+MSG);
		_wsdlOut.startElement(WSDL_NS, MESSAGE, MESSAGE, attr);
		attr.setValue(0, PARAMETERS);
		attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::ELEMENT, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, TYPE_PREFIX + ":" + repName);

		std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*>::const_iterator it = soapRespParams.begin();
		std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*>::const_iterator itEnd = soapRespParams.end();
		for (; it != itEnd; ++it)
		{
			std::string partName = it->second->name()+SOAPHEADER;
			std::string paramName = GenUtility::getParameterName(pFuncOld, it->second);
			std::string partElement = TYPE_PREFIX + ":" + paramName;
			AttributesImpl attrHeader;
			attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, partName);
			attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::ELEMENT, XSDGenerator::ELEMENT, XSDGenerator::EMPTYSTRING, partElement);
			_wsdlOut.startElement(WSDL_NS, PART, PART, attrHeader);
			_wsdlOut.endElement(WSDL_NS, PART, PART);

			soapRespHeaders.insert(std::make_pair(it->first, HeaderInfo(WSDLTYPE_PREFIX+":"+repName+MSG, partName, "literal")));
		}

		_wsdlOut.startElement(WSDL_NS, PART, PART, attr);
		_wsdlOut.endElement(WSDL_NS, PART, PART);
		_wsdlOut.endElement(WSDL_NS, MESSAGE, MESSAGE);
	}

	PortType port(methodName, reqName, repName, soapReqHeaders, soapRespHeaders);
	_portTypes.push_back(port);

	// create BindingInfo
	std::string soapAction(_targetNamespace);
	if (_targetNamespace[_targetNamespace.size()-1] != '/')
		soapAction.append("/");
	soapAction.append(methodName);

	BindingInfo nfo(soapAction, port, "document", "literal");
	_bindingInfos.push_back(nfo);
}


void WSDLGenerator::writePortType(const PortType& pt)
{
	/*
	<portType name="StockQuotes">
		<operation name="quoteFor">
			<input message="wts:quoteForRequestMsg"/>
			<output message="wts:quoteForReplyMsg"/>
			<fault message="wts:quoteForFaultMsg"/>
		</operation>
	</portType>
	*/

	AttributesImpl attr;
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, pt.operationName);
	_wsdlOut.startElement(WSDL_NS, OPERATION, OPERATION, attr);

	if (!pt.input.empty())
	{
		AttributesImpl attrIn;
		attrIn.addAttribute(XSDGenerator::EMPTYSTRING, MESSAGE, MESSAGE, XSDGenerator::EMPTYSTRING, WSDLTYPE_PREFIX + ":" + pt.input+MSG);
		_wsdlOut.startElement(XSDGenerator::EMPTYSTRING, INPUT, INPUT, attrIn);
		_wsdlOut.endElement(WSDL_NS, INPUT, INPUT);
	}
	if (!pt.output.empty())
	{
		AttributesImpl attrOut;
		attrOut.addAttribute(XSDGenerator::EMPTYSTRING, MESSAGE, MESSAGE, XSDGenerator::EMPTYSTRING, WSDLTYPE_PREFIX + ":" + pt.output+MSG);
		_wsdlOut.startElement(WSDL_NS, OUTPUT, OUTPUT, attrOut);
		_wsdlOut.endElement(WSDL_NS, OUTPUT, OUTPUT);
	}
	if (!pt.output.empty())
	{
		AttributesImpl attrOut;
		attrOut.addAttribute(XSDGenerator::EMPTYSTRING, MESSAGE, MESSAGE, XSDGenerator::EMPTYSTRING, WSDLTYPE_PREFIX + ":ExceptionFaultMsg");
		attrOut.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Exception");
		_wsdlOut.startElement(WSDL_NS, FAULT, FAULT, attrOut);
		_wsdlOut.endElement(WSDL_NS, FAULT, FAULT);
	}
	_wsdlOut.endElement(WSDL_NS, OPERATION, OPERATION);
}


void WSDLGenerator::writeBindingInfo(const std::string& ns, const BindingInfo& bi)
{
	/*
	<operation name="quoteFor">
		<soap:operation soapAction="http://www.appinf.com/ns/StockQuotes/quoteFor" style="document"/>
		<input>
			<soap:body parts="parameters" use="literal"/>
		</input>
		<output>
			<soap:body parts="parameters" use="literal"/>
		</output>
	</operation>
	*/
	AttributesImpl attr;
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, bi.operationInfo.operationName);
	_wsdlOut.startElement(WSDL_NS, OPERATION, OPERATION, attr);
	AttributesImpl attrOP;
	attrOP.addAttribute(XSDGenerator::EMPTYSTRING, SOAPACTION, SOAPACTION, XSDGenerator::EMPTYSTRING, bi.soapAction);
	attrOP.addAttribute(XSDGenerator::EMPTYSTRING, STYLE, STYLE, XSDGenerator::EMPTYSTRING, bi.bindingStyle);
	_wsdlOut.startElement(ns, OPERATION, OPERATION, attrOP);
	_wsdlOut.endElement(ns, OPERATION, OPERATION);

	AttributesImpl attrBody;
	attrBody.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::USE, XSDGenerator::USE, XSDGenerator::EMPTYSTRING, bi.bodyUse);
	attrBody.addAttribute(XSDGenerator::EMPTYSTRING, PARTS, PARTS, XSDGenerator::EMPTYSTRING, PARAMETERS);
	if (!bi.operationInfo.input.empty())
	{
		AttributesImpl inp;
		_wsdlOut.startElement(WSDL_NS, INPUT, INPUT, inp);

		writeSoapHeaders(bi.operationInfo.inputHeaders);

		_wsdlOut.startElement(ns, BODY, BODY, attrBody);
		_wsdlOut.endElement(ns, BODY, BODY);

		_wsdlOut.endElement(WSDL_NS, INPUT, INPUT);
	}
	if (!bi.operationInfo.output.empty())
	{
		AttributesImpl out;
		_wsdlOut.startElement(WSDL_NS, OUTPUT, OUTPUT, out);
		writeSoapHeaders(bi.operationInfo.outputHeaders);
		_wsdlOut.startElement(ns, BODY, BODY, attrBody);
		_wsdlOut.endElement(ns, BODY, BODY);
		_wsdlOut.endElement(WSDL_NS, OUTPUT, OUTPUT);
	}
	if (!bi.operationInfo.output.empty())
	{
		AttributesImpl out;
		out.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Exception");
		_wsdlOut.startElement(WSDL_NS, FAULT, FAULT, out);
		AttributesImpl faultAttr;
		faultAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::USE, XSDGenerator::USE, XSDGenerator::EMPTYSTRING, bi.bodyUse);
		faultAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, "Exception");
		_wsdlOut.startElement(ns, FAULT, FAULT, faultAttr);
		_wsdlOut.endElement(ns, FAULT, FAULT);
		_wsdlOut.endElement(WSDL_NS, FAULT, FAULT);
	}
	_wsdlOut.endElement(WSDL_NS, OPERATION, OPERATION);
}


void WSDLGenerator::writeSoapHeaders(const std::multimap<Poco::UInt32, HeaderInfo>& headers)
{
	std::multimap<Poco::UInt32, HeaderInfo>::const_iterator it = headers.begin();
	std::multimap<Poco::UInt32, HeaderInfo>::const_iterator itEnd = headers.end();
	for (; it != itEnd; ++it)
	{
		const HeaderInfo& hdr = it->second;
		AttributesImpl attrHeader;
		attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::USE, XSDGenerator::USE, XSDGenerator::EMPTYSTRING, hdr.bodyUse);
		attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, PART, PART, XSDGenerator::EMPTYSTRING, hdr.part);
		attrHeader.addAttribute(XSDGenerator::EMPTYSTRING, MESSAGE, MESSAGE, XSDGenerator::EMPTYSTRING, hdr.message);
		_wsdlOut.startElement(SOAP_NS, SOAPHEADER, SOAPHEADER, attrHeader);
		_wsdlOut.endElement(SOAP_NS, SOAPHEADER, SOAPHEADER);
	}
}


void WSDLGenerator::writeServiceLocations(const std::string& ns, const std::string& className, const std::string& serviceSuffix, const std::string& bindingSuffix)
{
	/*
	<service name="StockQuotesService">
		<port binding="wts:StockQuotesSoapBinding" name="StockQuotes">
			<soap:address location="http://localhost:8080/soap/StockQuotes/StockQuotes"/>
		</port>
	</service>
	*/
	AttributesImpl attr;
	attr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, className+serviceSuffix);
	_wsdlOut.startElement(WSDL_NS, SERVICE, SERVICE, attr);
	Poco::Util::LayeredConfiguration& cfg = Poco::Util::Application::instance().config();
	std::string endPoint = cfg.getString("RemoteGen.schema."+className+".serviceLocation");
	AttributesImpl portAttr;
	portAttr.addAttribute(XSDGenerator::EMPTYSTRING, XSDGenerator::NAME, XSDGenerator::NAME, XSDGenerator::EMPTYSTRING, className+serviceSuffix);
	portAttr.addAttribute(XSDGenerator::EMPTYSTRING, BINDING, BINDING, XSDGenerator::EMPTYSTRING, WSDLTYPE_PREFIX+":"+className+bindingSuffix);
	_wsdlOut.startElement(WSDL_NS, PORT, PORT, portAttr);
	AttributesImpl addr;
	addr.addAttribute(XSDGenerator::EMPTYSTRING, LOCATION, LOCATION,XSDGenerator::EMPTYSTRING, endPoint);
	_wsdlOut.startElement(ns, ADDRESS, ADDRESS, addr);
	_wsdlOut.endElement(ns, ADDRESS, ADDRESS);
	_wsdlOut.endElement(WSDL_NS, PORT, PORT);

	_wsdlOut.endElement(WSDL_NS, SERVICE, SERVICE);
}


void WSDLGenerator::methodEnd(const Poco::CppParser::Function* pFuncOld, const CodeGenerator::Properties& properties)
{
}


void WSDLGenerator::registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e)
{
}


std::vector<std::string> WSDLGenerator::newBaseClasses(const Poco::CppParser::Struct* pStruct)
{
	std::vector<std::string> bases;

	return bases;
}


bool WSDLGenerator::hasSoapHeader(const Poco::CppParser::Function* pFunc, bool request)
{
	bool hasHeader = !getSoapHeader(pFunc, request).empty();
	return hasHeader;
}


std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*> WSDLGenerator::getSoapHeader(const Poco::CppParser::Function* pFunc, bool request)
{
	Poco::UInt32 order = 0xffff0000u;
	std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*> ret;
	Poco::CodeGeneration::CodeGenerator::Properties funcProps;
	Poco::CodeGeneration::GeneratorEngine::parseProperties(pFunc, funcProps);

	Poco::CppParser::Function::Iterator it = pFunc->begin();
	Poco::CppParser::Function::Iterator itEnd = pFunc->end();
	for (; it != itEnd; ++it)
	{
		const Poco::CppParser::Parameter* pParam = *it;
		bool outParam = XSDGenerator::isOutParameter(pParam);
		if (outParam ^ request)
		{
			// either outParam && response or !outParam && request

			// check if this param has soapHeader set
			Poco::CodeGeneration::CodeGenerator::Properties props;
			std::string elemPropStr;
			bool elemPropsFound = Poco::CodeGeneration::GeneratorEngine::getStringProperty(funcProps, "$" + pParam->name(), elemPropStr);
			if (elemPropsFound)
			{
				Poco::CodeGeneration::CodeGenerator::Properties elemProps;
				Poco::CodeGeneration::GeneratorEngine::parseElementProperties(elemPropStr, elemProps);
				bool hasHeader = false;
				Poco::CodeGeneration::GeneratorEngine::getBoolProperty(elemProps, GenUtility::ATTR_HEADER, hasHeader);
				if (hasHeader)
				{
					Poco::UInt32 ord = order++;
					ord = GenUtility::getParameterOrder(pFunc, *it, ord);
					ret.insert(std::make_pair(ord, *it));
				}
			}
		}
	}
	return ret;
}

