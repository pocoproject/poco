//
// Serializer.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Serializer
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/SAX/NamespaceSupport.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Base64Encoder.h"
#include "Poco/UUIDGenerator.h"
#include <algorithm>
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


const std::string Serializer::SOAPPREFIX("soap");
const std::string Serializer::SOAPENCPREFIX("soap-enc");
const std::string Serializer::SOAP11NAMESPACE("http://schemas.xmlsoap.org/soap/envelope/");
const std::string Serializer::SOAP12NAMESPACE("http://www.w3.org/2003/05/soap-envelope");
const std::string Serializer::REMOTINGPREFIX("remoting");
const std::string Serializer::REMOTINGNAMESPACE("http://www.appinf.com/remoting");
const std::string Serializer::XSIPREFIX("xsi");
const std::string Serializer::XSINAMESPACE("http://www.w3.org/2001/XMLSchema-instance");
const std::string Serializer::XSINIL("nil");
const std::string Serializer::XOPNAMESPACE("http://www.w3.org/2004/08/xop/include");
const std::string Serializer::XOPINCLUDE("Include");
const std::string Serializer::XOPHREF("href");
const std::string Serializer::SOAPBODY("Body");
const std::string Serializer::SOAPHEADER("Header");
const std::string Serializer::SOAPENCODINGNAMESPACE("http://schemas.xmlsoap.org/soap/encoding/");
const std::string Serializer::EMPTYSTRING;
const XML::AttributesImpl Serializer::EMPTYATTRIBUTES;
const std::string Serializer::VAL_TRUE("true");
const std::string Serializer::VAL_FALSE("false");
const std::string Serializer::ENVELOPE("Envelope");
const std::string Serializer::FAULT("Fault");
const std::string Serializer::ENCODINGSTYLE("encodingStyle");
const std::string Serializer::FAULTCODE("faultcode");
const std::string Serializer::FAULTCODESERVER("soap:Server");
const std::string Serializer::FAULTSTRING("faultstring");
const std::string Serializer::DETAIL("detail");
const std::string Serializer::CODE("Code");
const std::string Serializer::CODE_VALUE("Value");
const std::string Serializer::REASON("Reason");
const std::string Serializer::REASON_TEXT("Text");
const std::string Serializer::DETAIL12("Detail");
const std::string Serializer::DETAIL_EXCEPTION("Exception");
const std::string Serializer::DETAIL_TYPE("Type");
const std::string Serializer::DETAIL_MESSAGE("Message");
const std::string Serializer::DETAIL_CODE("Code");
const std::string Serializer::LANG("lang");
const std::string Serializer::CODEVALUERECEIVER("soap:Receiver");
const std::string Serializer::REPLY("Response");
const std::string Serializer::WSSEPREFIX("wsse");
const std::string Serializer::WSSENAMESPACE("http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd");
const std::string Serializer::WSUPREFIX("wsu");
const std::string Serializer::WSUNAMESPACE("http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd");


Serializer::Serializer(int options):
	_soapVersion(SOAP_1_1),
	_mtom(false),
	_options(options),
	_pSOAPNamespace(&SOAP11NAMESPACE),
	_pXMLWriter(),
	handleStartElement(&Serializer::handleStartElementImpl),
	handleStartElementComplex(&Serializer::handleStartElementComplexImpl),
	handleEndElementComplex(&Serializer::handleEndElementComplexImpl),
	handleStartElementNil(&Serializer::handleStartElementNilImpl),
	handleStartElementInclude(&Serializer::handleStartElementIncludeImpl),
	_documentStarted(false),
	_pStream(0),
	_pMultipartWriter(0),
	_declareEnvelopeNamespace(false)
{
}


Serializer::Serializer(SOAPVersion soapVersion, int options):
	_soapVersion(soapVersion),
	_mtom(false),
	_options(options),
	_pSOAPNamespace(soapVersion == SOAP_1_1 ? &SOAP11NAMESPACE : &SOAP12NAMESPACE),
	_pXMLWriter(),
	handleStartElement(&Serializer::handleStartElementImpl),
	handleStartElementComplex(&Serializer::handleStartElementComplexImpl),
	handleEndElementComplex(&Serializer::handleEndElementComplexImpl),
	handleStartElementNil(&Serializer::handleStartElementNilImpl),
	handleStartElementInclude(&Serializer::handleStartElementIncludeImpl),
	_documentStarted(false),
	_pStream(0),
	_pMultipartWriter(0),
	_declareEnvelopeNamespace(false)
{
}


Serializer::~Serializer()
{
}


void Serializer::addHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor)
{
	_headerInterceptors.push_back(pInterceptor);
}


void Serializer::removeHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor)
{
	_headerInterceptors.erase(
		std::remove(_headerInterceptors.begin(), _headerInterceptors.end(), pInterceptor), 
		_headerInterceptors.end()
	);
}


void Serializer::setSOAPVersion(SOAPVersion version)
{
	_soapVersion = version;
	_pSOAPNamespace = (version == SOAP_1_1 ? &SOAP11NAMESPACE : &SOAP12NAMESPACE);
}


void Serializer::enableMTOM(bool enable)
{
	_mtom = enable;
}


bool Serializer::isMTOMEnabled() const
{
	return _mtom;
}


void Serializer::setOptions(int options)
{
	_options = options;
}


std::string Serializer::createMTOMContentType(const std::string& action)
{
	_boundary = Poco::Net::MultipartWriter::createBoundary();
	_startContentID = createContentID();
	std::string contentType("Multipart/Related; boundary=\"");
	contentType.append(_boundary);
	contentType.append("\"; type=\"application/xop+xml\"; start=\"<");
	contentType.append(_startContentID);
	contentType.append(">\"; start-info=\"");
	if (_soapVersion == SOAP_1_1)
	{
		contentType.append("text/xml");
	}
	else
	{
		contentType.append("application/soap+xml");
		if (!action.empty())
		{
			contentType.append("\"; action=\"");
			contentType.append(action);
		}
	}
	contentType.append("\"");
	return contentType;
}


void Serializer::serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pXMLWriter.get());

	if (type == Poco::RemotingNG::SerializerBase::MESSAGE_EVENT)
	{
		poco_bugcheck();
	}
	else if (type == Poco::RemotingNG::SerializerBase::MESSAGE_EVENT_REPLY)
	{
		poco_bugcheck();
	}
	else
	{
		_declareEnvelopeNamespace = name == "#";
		createEnvelope();
		DelayedElement* pElem = _curParent.top();
		if (!_headerElements.empty() || !_headerInterceptors.empty())
		{
			pElem->children.push_back(DelayedElement(SOAPHEADER, *_pSOAPNamespace, EMPTYSTRING, false));
			if (!_headerInterceptors.empty())
			{
				interceptHeaders(pElem->children.back());
			}
		}
		pElem->children.push_back(DelayedElement(SOAPBODY, *_pSOAPNamespace, EMPTYSTRING, false));
		pElem->curChild = 1;
		_curParent.push(&(pElem->children[pElem->children.size()-1])); // guarantees that the method is written to the body
		if (_headerElements.empty())
			writeRoot();
		serializeStructBegin(name);
	}
}


void Serializer::serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pXMLWriter.get());

	if (type == Poco::RemotingNG::SerializerBase::MESSAGE_EVENT)
	{
		poco_bugcheck();
	}
	else if (type == Poco::RemotingNG::SerializerBase::MESSAGE_EVENT_REPLY)
	{
		poco_bugcheck();
	}
	else
	{
		if (!_documentStarted)
		{
			writeRoot();
		}
		serializeStructEnd(name);

		closeXML();
		if (_pMultipartWriter)
		{
			writeParts();
			_pMultipartWriter->close();
			delete _pMultipartWriter;
			_pMultipartWriter = 0;
			_parts.clear();
		}
		_pStream->flush();
		_pStream = 0;
		_pXMLWriter.reset(0);
	}
}


void Serializer::serializeStructBegin(const std::string& name)
{
	if (!_pRoot && !_documentStarted)
	{
		_pXMLWriter->startDocument();
		_pXMLWriter->startPrefixMapping(XSIPREFIX, XSINAMESPACE);
		handleStartElement = &Serializer::handleStartElementXml;
		handleStartElementComplex = &Serializer::handleStartElementComplexXml;
		handleEndElementComplex = &Serializer::handleEndElementComplexXml;
		handleStartElementNil = &Serializer::handleStartElementNilXml;
		handleStartElementInclude = &Serializer::handleStartElementIncludeXml;
		_documentStarted = true;
	}
	(this->*handleStartElementComplex)(name);
}


void Serializer::serializeStructEnd(const std::string& name)
{
	(this->*handleEndElementComplex)(name);
	if (_pXMLWriter->depth() == 0)
	{
		_pXMLWriter->endPrefixMapping(XSIPREFIX);
		_pXMLWriter->endDocument();
	}
}


void Serializer::serializeFaultMessage(const std::string& /*name*/, const Poco::Exception& exc)
{
	_pXMLWriter->startDocument();
	XML::AttributesImpl envelopeAttrs;
	if (_soapVersion == SOAP_1_1 && (_options & OPT_NO_ENCODINGSTYLE_ATTRIBUTE) == 0)
	{
		envelopeAttrs.addAttribute(*_pSOAPNamespace, EMPTYSTRING, ENCODINGSTYLE, EMPTYSTRING, SOAPENCODINGNAMESPACE);
	}
	_pXMLWriter->startPrefixMapping(SOAPPREFIX, *_pSOAPNamespace);
	_pXMLWriter->startPrefixMapping(REMOTINGPREFIX, REMOTINGNAMESPACE);
	_pXMLWriter->startElement(*_pSOAPNamespace, ENVELOPE, EMPTYSTRING, envelopeAttrs);
	XML::AttributesImpl bodyAttrs;
	_pXMLWriter->startElement(*_pSOAPNamespace, SOAPBODY, EMPTYSTRING, bodyAttrs);
	_pXMLWriter->startElement(*_pSOAPNamespace, FAULT, EMPTYSTRING, EMPTYATTRIBUTES);
	if (_soapVersion == SOAP_1_1)
	{
		_pXMLWriter->dataElement(EMPTYSTRING, EMPTYSTRING, FAULTCODE, FAULTCODESERVER);
		_pXMLWriter->dataElement(EMPTYSTRING, EMPTYSTRING, FAULTSTRING, exc.displayText());
		_pXMLWriter->startElement(EMPTYSTRING, EMPTYSTRING, DETAIL, EMPTYATTRIBUTES);
		XML::AttributesImpl excAttrs;
		_pXMLWriter->startElement(REMOTINGNAMESPACE, DETAIL_EXCEPTION, EMPTYSTRING, excAttrs);
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_TYPE, EMPTYSTRING, exc.name());
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_MESSAGE, EMPTYSTRING, exc.message());
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_CODE, EMPTYSTRING, Poco::NumberFormatter::format(exc.code()));
		_pXMLWriter->endElement(REMOTINGNAMESPACE, DETAIL_EXCEPTION, EMPTYSTRING);
		_pXMLWriter->endElement(EMPTYSTRING, EMPTYSTRING, DETAIL);
	}
	else
	{
		_pXMLWriter->startElement(*_pSOAPNamespace, CODE, EMPTYSTRING, EMPTYATTRIBUTES);
		_pXMLWriter->dataElement(*_pSOAPNamespace, CODE_VALUE, EMPTYSTRING, CODEVALUERECEIVER);
		_pXMLWriter->endElement(*_pSOAPNamespace, CODE, EMPTYSTRING);
		_pXMLWriter->startElement(*_pSOAPNamespace, REASON, EMPTYSTRING, EMPTYATTRIBUTES);
		XML::AttributesImpl textAttrs;
		textAttrs.addAttribute(XML::NamespaceSupport::XML_NAMESPACE, LANG, EMPTYSTRING, EMPTYSTRING, "en");
		_pXMLWriter->startElement(*_pSOAPNamespace, REASON_TEXT, EMPTYSTRING, textAttrs);
		std::string msg(exc.displayText());
		_pXMLWriter->characters(XML::toXMLString(msg));
		_pXMLWriter->endElement(*_pSOAPNamespace, REASON_TEXT, EMPTYSTRING);
		_pXMLWriter->endElement(*_pSOAPNamespace, REASON, EMPTYSTRING);
		_pXMLWriter->startElement(*_pSOAPNamespace, DETAIL12, EMPTYSTRING, EMPTYATTRIBUTES);
		XML::AttributesImpl excAttrs;
		_pXMLWriter->startElement(REMOTINGNAMESPACE, DETAIL_EXCEPTION, EMPTYSTRING, excAttrs);
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_TYPE, EMPTYSTRING, exc.name());
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_MESSAGE, EMPTYSTRING, exc.message());
		_pXMLWriter->dataElement(REMOTINGNAMESPACE, DETAIL_CODE, EMPTYSTRING, Poco::NumberFormatter::format(exc.code()));
		_pXMLWriter->endElement(REMOTINGNAMESPACE, DETAIL_EXCEPTION, EMPTYSTRING);
		_pXMLWriter->endElement(*_pSOAPNamespace, DETAIL12, EMPTYSTRING);
	}
	_pXMLWriter->endElement(*_pSOAPNamespace, FAULT, EMPTYSTRING);
	_pXMLWriter->endElement(*_pSOAPNamespace, SOAPBODY, EMPTYSTRING);
	_pXMLWriter->endElement(*_pSOAPNamespace, ENVELOPE, ENVELOPE);
	_pXMLWriter->endPrefixMapping(SOAPPREFIX);
	_pXMLWriter->endPrefixMapping(REMOTINGPREFIX);
	_pXMLWriter->endDocument();
	_pXMLWriter.reset(0);
}


void Serializer::serializeNullableBegin(const std::string& name, bool isNull)
{
	if (isNull)
	{
		(this->*handleStartElementNil)(name);
	}
}


void Serializer::serializeNullableEnd(const std::string&)
{
}


void Serializer::serializeOptionalBegin(const std::string&, bool)
{
}


void Serializer::serializeOptionalEnd(const std::string&)
{
}


void Serializer::serialize(const std::string& name, Poco::Int8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::UInt8 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::Int16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::UInt16 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::Int32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::UInt32 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, unsigned long val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


#ifndef POCO_INT64_IS_LONG
void Serializer::serialize(const std::string& name, Poco::Int64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, Poco::UInt64 val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}
#endif


void Serializer::serialize(const std::string& name, float val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, double val)
{
	serializeData(name, Poco::NumberFormatter::format(val));
}


void Serializer::serialize(const std::string& name, bool val)
{
	serializeData(name, val ? VAL_TRUE : VAL_FALSE);
}


void Serializer::serialize(const std::string& name, const std::string& val)
{
	serializeData(name, val);
}


void Serializer::serialize(const std::string& name, const Poco::DateTime& val)
{
	const std::string xsdType = getProperty(SerializerBase::PROP_XSDTYPE, ""s);
	if (xsdType == "date"s)
		serializeData(name, Poco::DateTimeFormatter::format(val, "%Y-%m-%d"s));
	else if (xsdType == "time"s)
		serializeData(name, Poco::DateTimeFormatter::format(val, "%H:%M:%s"s));
	else
		serializeData(name, Poco::DateTimeFormatter::format(val, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
}


void Serializer::serialize(const std::string& name, const Poco::LocalDateTime& val)
{
	const std::string xsdType = getProperty(SerializerBase::PROP_XSDTYPE, ""s);
	if (xsdType == "date"s)
		serializeData(name, Poco::DateTimeFormatter::format(val, "%Y-%m-%d"s));
	else if (xsdType == "time"s)
		serializeData(name, Poco::DateTimeFormatter::format(val, "%H:%M:%s%z"s));
	else
		serializeData(name, Poco::DateTimeFormatter::format(val, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
}


void Serializer::serializeSequenceBegin(const std::string& /*name*/, Poco::UInt32 /*length*/)
{
}


void Serializer::serializeSequenceEnd(const std::string& /*name*/)
{
}


void Serializer::serializeData(const std::string& name, const std::string& val)
{
	(this->*handleStartElement)(name, val);
}


void Serializer::serialize(const std::string& name, char val)
{
	poco_assert_dbg(_pXMLWriter.get());
	std::string tmp(&val, 1);
	serializeData(name, tmp);
}


void Serializer::serialize(const std::string& name, const std::vector<char>& val)
{
	if (_mtom && val.size() >= MIN_MTOM_SIZE)
	{
		std::string cid(createContentID());
		_parts[cid] = &val;
		(this->*handleStartElementComplex)(name);
		(this->*handleStartElementInclude)(name, cid);
		(this->*handleEndElementComplex)(name);
	}
	else
	{
		std::stringstream base64;
		if (!val.empty())
		{
			Poco::Base64Encoder encoder(base64);
			encoder.write(&val[0], static_cast<std::streamsize>(val.size()));
			encoder.close();
		}
		serializeData(name, base64.str());
	}
}


void Serializer::handleStartElementImpl(const std::string& name, const std::string& value)
{
	poco_assert_dbg(_pXMLWriter.get());

	// we either add a simple element or an attribute
	DelayedElement* pElem = _curParent.top();
	std::map<std::string, std::string>::iterator it = pElem->openAttr.find(name);

	if (it == pElem->openAttr.end())
	{
		// a simple element
		bool switched = checkHeadersStart(name);
		if (switched)
		{
			handleStartElementXml(name, value);
			return;
		}
		if (hasProperty(Serializer::PROP_NAMESPACE))
			pElem->children.push_back(DelayedElement(name, getProperty(Serializer::PROP_NAMESPACE), value, true));
		else
			pElem->children.push_back(DelayedElement(name, EMPTYSTRING, value, true));
		pElem->curChild++;
	}
	else
	{
		// an attribute
		pElem->attr.addAttribute(it->second, name, EMPTYSTRING, EMPTYSTRING, value);
		pElem->openAttr.erase(name);
	}
}


void Serializer::handleStartElementXml(const std::string& name, const std::string& value)
{
	poco_assert_dbg(_pXMLWriter.get());

	// we either add a simple element or an attribute
	bool isElem = !_pDelayed.isNull();
	std::map<std::string, std::string>::iterator it;
	if (isElem)
	{
		it = _pDelayed->openAttr.find(name);
		isElem = !(it != _pDelayed->openAttr.end());
		if (isElem)
		{
			if (_pDelayed->simpleElem)
			{
				if (!_pDelayed->name.empty())
					_pXMLWriter->startElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING, _pDelayed->attr);
				if (!_pDelayed->value.empty())
					_pXMLWriter->characters(_pDelayed->value);
				if (!_pDelayed->name.empty())
					_pXMLWriter->endElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING);
			}
			else
			{
				if (!_pDelayed->name.empty() && _pDelayed->name != "#")
					_pXMLWriter->startElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING, _pDelayed->attr);
				if (!_pDelayed->value.empty())
					_pXMLWriter->characters(_pDelayed->value);
			}
			_pDelayed = 0;
		}
	}
	else isElem = true;

	if (isElem)
	{
		if (_attr.empty())
		{
			if (hasProperty(Serializer::PROP_NAMESPACE))
			{
				const std::string& ns = getProperty(Serializer::PROP_NAMESPACE);
				if (!name.empty() && name != "#")
					_pXMLWriter->startElement(ns, name, EMPTYSTRING, EMPTYATTRIBUTES);
				if (!value.empty())
					_pXMLWriter->characters(value);
				if (!name.empty() && name != "#")
					_pXMLWriter->endElement(ns, name, EMPTYSTRING);
			}
			else
			{
				if (!name.empty() && name != "#")
					_pXMLWriter->startElement(EMPTYSTRING, name, EMPTYSTRING, EMPTYATTRIBUTES);
				if (!value.empty())
					_pXMLWriter->characters(value);
				if (!name.empty() && name != "#")
					_pXMLWriter->endElement(EMPTYSTRING, name, EMPTYSTRING);
			}
		}
		else
		{
			if (hasProperty(Serializer::PROP_NAMESPACE))
				_pDelayed = new DelayedElement(name, getProperty(Serializer::PROP_NAMESPACE), value, true, _attr);
			else
				_pDelayed = new DelayedElement(name, EMPTYSTRING, value, true, _attr);
			_attr.clear();
		}
	}
	else
	{
		// an attribute
		_pDelayed->attr.addAttribute(it->second, name, EMPTYSTRING, EMPTYSTRING, value);
		_pDelayed->openAttr.erase(name);
		if (_pDelayed->openAttr.empty())
		{
			_pXMLWriter->startElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING, _pDelayed->attr);
			if (!_pDelayed->value.empty())
				_pXMLWriter->characters(_pDelayed->value);
			if (_pDelayed->simpleElem)
				_pXMLWriter->endElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING);
			_pDelayed = 0;
		}
	}
}


void Serializer::handleStartElementComplexImpl(const std::string& name)
{
	// we get the root and add another complex type
	bool switched = checkHeadersStart(name);
	if (switched)
	{
		handleStartElementComplexXml(name);
		return;
	}
	DelayedElement* pElem = _curParent.top();
	++_depth;
	if (hasProperty(Serializer::PROP_NAMESPACE))
	{
		const std::string& ns = getProperty(Serializer::PROP_NAMESPACE);
		pElem->children.push_back(DelayedElement(name, ns, EMPTYSTRING, false, _attr));
	}
	else
	{
		pElem->children.push_back(DelayedElement(name, EMPTYSTRING, EMPTYSTRING, false, _attr));
	}
	_attr.clear();
	pElem->curChild++;
	_curParent.push(&pElem->children[pElem->curChild]);
}


void Serializer::handleStartElementComplexXml(const std::string& name)
{
	if (_attr.empty())
	{
		if (name != "#")
		{
			if (hasProperty(Serializer::PROP_NAMESPACE))
				_pXMLWriter->startElement(getProperty(Serializer::PROP_NAMESPACE), name, EMPTYSTRING, EMPTYATTRIBUTES);
			else
				_pXMLWriter->startElement(EMPTYSTRING, name, EMPTYSTRING, EMPTYATTRIBUTES);
		}
	}
	else
	{
		poco_assert_dbg (_pDelayed.isNull());
		if (hasProperty(Serializer::PROP_NAMESPACE))
			_pDelayed = new DelayedElement(name, getProperty(Serializer::PROP_NAMESPACE), EMPTYSTRING, false, _attr);
		else
			_pDelayed = new DelayedElement(name, EMPTYSTRING, EMPTYSTRING, false, _attr);
		_attr.clear();
	}
}


void Serializer::handleEndElementComplexImpl(const std::string& name)
{
	DelayedElement* pElem = _curParent.top();
	poco_assert_dbg(name == pElem->name);
	pElem->curChild++;
	_curParent.pop();
	--_depth;
	checkHeadersEnd(name);
}


void Serializer::handleEndElementComplexXml(const std::string& name)
{
	poco_assert_dbg (_attr.empty());
	if (_pDelayed.isNull())
	{
		if (name != "#")
		{
			if (hasProperty(Serializer::PROP_NAMESPACE))
				_pXMLWriter->endElement(getProperty(Serializer::PROP_NAMESPACE), name, EMPTYSTRING);
			else
				_pXMLWriter->endElement(EMPTYSTRING, name, EMPTYSTRING);
		}
	}
	else
	{
		if (!_pDelayed->name.empty() && _pDelayed->name != "#")
		{
			_pXMLWriter->startElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING, _pDelayed->attr);
			_pXMLWriter->endElement(_pDelayed->nameSpace, _pDelayed->name, EMPTYSTRING);
		}
		_pDelayed = 0;
	}
}


void Serializer::handleStartElementNilImpl(const std::string& name)
{
	bool switched = checkHeadersStart(name);
	if (switched)
	{
		handleStartElementNilXml(name);
		return;
	}
	DelayedElement* pElem = _curParent.top();
	if (hasProperty(Serializer::PROP_NAMESPACE))
	{
		const std::string& ns = getProperty(Serializer::PROP_NAMESPACE);
		pElem->children.push_back(DelayedElement(name, ns));
	}
	else
	{
		pElem->children.push_back(DelayedElement(name, EMPTYSTRING));
	}
	pElem->curChild++;
}


void Serializer::handleStartElementNilXml(const std::string& name)
{
	poco_assert_dbg (_attr.empty());

	XML::AttributesImpl attrs;
	attrs.addAttribute(XSINAMESPACE, XSINIL, EMPTYSTRING, EMPTYSTRING, VAL_TRUE);
	if (hasProperty(Serializer::PROP_NAMESPACE))
		_pXMLWriter->emptyElement(getProperty(Serializer::PROP_NAMESPACE), name, EMPTYSTRING, attrs);
	else
		_pXMLWriter->emptyElement(EMPTYSTRING, name, EMPTYSTRING, attrs);
}


void Serializer::handleStartElementIncludeImpl(const std::string& name, const std::string& cid)
{
	bool switched = checkHeadersStart(name);
	if (switched)
	{
		handleStartElementIncludeXml(name, cid);
		return;
	}
	DelayedElement* pElem = _curParent.top();
	pElem->children.push_back(DelayedElement(XOPINCLUDE, XOPNAMESPACE));
	pElem->children.back().attr.addAttribute(EMPTYSTRING, XOPHREF, EMPTYSTRING, EMPTYSTRING, "cid:" + cid);
	pElem->curChild++;
}


void Serializer::handleStartElementIncludeXml(const std::string& /*name*/, const std::string& cid)
{
	poco_assert_dbg (_attr.empty());

	XML::AttributesImpl attrs;
	attrs.addAttribute(EMPTYSTRING, XOPHREF, EMPTYSTRING, EMPTYSTRING, "cid:" + cid);
	_pXMLWriter->emptyElement(XOPNAMESPACE, XOPINCLUDE, EMPTYSTRING, attrs);
}


bool Serializer::checkHeadersStart(const std::string& name)
{
	if (_depth == 1)
	{
		bool hadHeaders = !_headerElements.empty();
		bool found = (_headerElements.find(name) != _headerElements.end());
		if (hadHeaders)
		{
			if (!found)
			{
				_curParent.pop();
				DelayedElement* pElem = _curParent.top();
				pElem->curChild++; // move to body
				writeRoot();
				return true;
			}
			else
			{
				// if we are at body move to the header
				// Envelope:Body:Method
				if (_curParent.size() == 3)
				{
					_curParent.push(&_pRoot->children[0]);
				}
			}
		}
	}
	return false;
}


void Serializer::checkHeadersEnd(const std::string& name)
{
	if (_depth == 1)
	{
		bool hadHeaders = !_headerElements.empty();
		_headerElements.erase(name);
		if (hadHeaders && _headerElements.empty())
		{
			//Envelope:Body:method:Header
			_curParent.pop(); // remove the header
			writeRoot();
		}
	}
}


void Serializer::setupImpl(std::ostream& out)
{
	if (_mtom)
	{
		_pMultipartWriter = new Poco::Net::MultipartWriter(out, _boundary);
		Poco::Net::MessageHeader header;
		std::string contentType("application/xop+xml; charset=\"utf-8\"; type=\"");
		contentType.append(_soapVersion == SOAP_1_1 ? "text/xml" : "application/soap+xml");
		contentType.append("\"");
		header.set("Content-Type"s, contentType);
		header.set("Content-Transfer-Encoding"s, "8bit"s);
		header.set("Content-ID"s, "<" + _startContentID + ">");
		_pMultipartWriter->nextPart(header);
		_pXMLWriter.reset(new XML::XMLWriter(_pMultipartWriter->stream(), (_options & OPT_PRETTY_PRINT) ? XML::XMLWriter::PRETTY_PRINT : 0));
	}
	else
	{
		_pXMLWriter.reset(new XML::XMLWriter(out, (_options & OPT_PRETTY_PRINT) ? XML::XMLWriter::PRETTY_PRINT : 0));
	}
	_pStream = &out;
}


void Serializer::resetImpl()
{
	delete _pMultipartWriter;
	_pMultipartWriter = 0;
	_pXMLWriter.reset(0);
	_attr.clear();
	_headerElements.clear();
	_depth = 0;
	_pRoot = 0;
	handleStartElement = &Serializer::handleStartElementImpl;
	handleStartElementComplex = &Serializer::handleStartElementComplexImpl;
	handleEndElementComplex = &Serializer::handleEndElementComplexImpl;
	handleStartElementNil = &Serializer::handleStartElementNilImpl;
	handleStartElementInclude = &Serializer::handleStartElementIncludeImpl;
	_documentStarted = false;
	while (!_curParent.empty())
	{
		_curParent.pop();
	}
}


void Serializer::createEnvelope()
{
	//<soap:Envelope
	//	xmlns:soap="http://www.w3.org/2001/12/soap-envelope"
	//	soap:encodingStyle="http://www.w3.org/2001/12/soap-encoding">
	XML::AttributesImpl attrs;
	if (_soapVersion == SOAP_1_1 && (_options & OPT_NO_ENCODINGSTYLE_ATTRIBUTE) == 0)
	{
		attrs.addAttribute(*_pSOAPNamespace, EMPTYSTRING, ENCODINGSTYLE, EMPTYSTRING, SOAPENCODINGNAMESPACE);
	}
	_pRoot = new DelayedElement(ENVELOPE, *_pSOAPNamespace, EMPTYSTRING, false);
	_pRoot->attr = attrs;
	_curParent.push(_pRoot.get());

	// the properties are stored in reverse order: the top most element is the last one we should receive
	// -> reverse the order
	while (hasProperty(Serializer::PROP_HEADER))
	{
		_headerElements.insert(getProperty(Serializer::PROP_HEADER));
		popProperty(Serializer::PROP_HEADER);
	}
}


Serializer::DelayedElement::DelayedElement():
	name(),
	nameSpace(),
	value(),
	simpleElem(false),
	attr(),
	openAttr()
{
}


Serializer::DelayedElement::DelayedElement(const std::string& n, const std::string& ns):
	name(n),
	nameSpace(ns),
	value(),
	simpleElem(true),
	attr(),
	openAttr(),
	children(),
	curChild(-1)
{
	this->attr.addAttribute(XSINAMESPACE, XSINIL, EMPTYSTRING, EMPTYSTRING, VAL_TRUE);
}


Serializer::DelayedElement::DelayedElement(const std::string& n, const std::string& ns, const std::string& val, bool simpleE, const std::map<std::string, std::string>& opAt):
	name(n),
	nameSpace(ns),
	value(val),
	simpleElem(simpleE),
	attr(),
	openAttr(opAt),
	children(),
	curChild(-1)
{
}


Serializer::DelayedElement::DelayedElement(const std::string& n, const std::string& ns, const std::string& val, bool simpleE):
	name(n),
	nameSpace(ns),
	value(val),
	simpleElem(simpleE),
	attr(),
	openAttr(),
	children(),
	curChild(-1)
{
}


Serializer::DelayedElement::DelayedElement(const std::string& val):
	name(),
	nameSpace(),
	value(val),
	simpleElem(true),
	attr(),
	openAttr(),
	children(),
	curChild(-1)
{
}


bool Serializer::DelayedElement::valid() const
{
	return !name.empty() || (simpleElem && !value.empty());
}


void Serializer::DelayedElement::clear()
{
	name.clear();
	attr.clear();
	nameSpace.clear();
	value.clear();
	openAttr.clear();
	children.clear();
	curChild = -1;
}


void Serializer::writeRoot()
{
	_pXMLWriter->startDocument();
	_pXMLWriter->startPrefixMapping(SOAPPREFIX, *_pSOAPNamespace);
	_pXMLWriter->startPrefixMapping(XSIPREFIX, XSINAMESPACE);
	if (_options & OPT_DECLARE_SOAPENC_NAMESPACE)
	{
		_pXMLWriter->startPrefixMapping(SOAPENCPREFIX, SOAPENCODINGNAMESPACE);
	}
	if (_options & OPT_DECLARE_WSSE_NAMESPACE)
	{
		_pXMLWriter->startPrefixMapping(WSSEPREFIX, WSSENAMESPACE);
		_pXMLWriter->startPrefixMapping(WSUPREFIX, WSUNAMESPACE);
	}

	if (_declareEnvelopeNamespace && hasProperty(Serializer::PROP_NAMESPACE))
	{
		const std::string& ns = getProperty(Serializer::PROP_NAMESPACE);
		_pXMLWriter->startPrefixMapping(_pXMLWriter->uniquePrefix(), ns);
	}

	if (!_pRoot.isNull())
	{
		_pXMLWriter->startElement(_pRoot->nameSpace, _pRoot->name, _pRoot->name, _pRoot->attr);

		// now write children
		for (std::size_t i = 0; i < _pRoot->children.size() - 1; ++i)
		{
			write(_pRoot->children[i], true);
		}
		// the last element is the body, where we are only allowed to write opening tags!
		write(_pRoot->children[_pRoot->children.size()-1], false);
	}

	// now update the funct pointers to point to the simple implementation
	// that writes XML directly
	handleStartElement = &Serializer::handleStartElementXml;
	handleStartElementComplex = &Serializer::handleStartElementComplexXml;
	handleEndElementComplex = &Serializer::handleEndElementComplexXml;
	handleStartElementNil = &Serializer::handleStartElementNilXml;
	handleStartElementInclude = &Serializer::handleStartElementIncludeXml;
	_documentStarted = true;
}


void Serializer::closeXML()
{
	_pXMLWriter->endElement(*_pSOAPNamespace, SOAPBODY, SOAPBODY);
	_pXMLWriter->endElement(*_pSOAPNamespace, ENVELOPE, ENVELOPE);
	_pXMLWriter->endPrefixMapping(SOAPPREFIX);
	_pXMLWriter->endPrefixMapping(XSIPREFIX);
	if (_options & OPT_DECLARE_SOAPENC_NAMESPACE)
	{
		_pXMLWriter->endPrefixMapping(SOAPENCPREFIX);
	}
	if (_options & OPT_DECLARE_WSSE_NAMESPACE)
	{
		_pXMLWriter->endPrefixMapping(WSSEPREFIX);
		_pXMLWriter->endPrefixMapping(WSUPREFIX);
	}
	_pXMLWriter->endDocument();
}


void Serializer::write(const DelayedElement& elem, bool closeTags)
{
	if (elem.name.empty())
	{
		poco_assert_dbg (elem.children.empty());
		_pXMLWriter->characters(elem.value);
	}
	else
	{
		if (elem.name != "#")
			_pXMLWriter->startElement(elem.nameSpace, elem.name, EMPTYSTRING, elem.attr);
		if (!elem.value.empty())
			_pXMLWriter->characters(elem.value);
			// now write children
		std::vector<DelayedElement>::const_iterator it = elem.children.begin();
		std::vector<DelayedElement>::const_iterator itEnd = elem.children.end();
		for (; it != itEnd; ++it)
		{
			write(*it, closeTags);
		}
		if (closeTags && elem.name != "#")
			_pXMLWriter->endElement(elem.nameSpace, elem.name, EMPTYSTRING);
	}
}


void Serializer::pushAttribute(const std::string& ns, const std::string& attrName)
{
	_attr.insert(std::make_pair(attrName, ns));
}


void Serializer::registerNamespace(const std::string& ns)
{
	if (!_pXMLWriter->isNamespaceMapped(ns))
	{
		_pXMLWriter->startPrefixMapping(_pXMLWriter->uniquePrefix(), ns);
	}
}


std::string Serializer::createContentID()
{
	Poco::UUIDGenerator uuidGenerator;
	std::string cid(uuidGenerator.createRandom().toString());
	cid.append("@appinf.com");
	return cid;
}


void Serializer::writeParts()
{
	for (std::map<std::string, const std::vector<char>*>::const_iterator it = _parts.begin(); it != _parts.end(); ++it)
	{
		Poco::Net::MessageHeader header;
		header.set("Content-Type"s, "application/binary"s);
		header.set("Content-Transfer-Encoding"s, "binary"s);
		header.set("Content-ID"s, "<" + it->first + ">");
		_pMultipartWriter->nextPart(header);
		if (it->second->size() > 0)
		{
			_pMultipartWriter->stream().write(&(*it->second)[0], it->second->size());
		}
	}
}


void Serializer::interceptHeaders(DelayedElement& delayedHeader)
{
	XMLElement headerElem(delayedHeader.name, delayedHeader.nameSpace);
	for (auto& pInterceptor: _headerInterceptors)
	{
		pInterceptor->interceptHeader(headerElem);
	}
	for (const auto& c: headerElem._children)
	{
		delayedHeader.children.push_back(delayElement(c));
	}
}


Serializer::DelayedElement Serializer::delayElement(const XMLElement& element)
{
	DelayedElement delayed(element._name, element._uri, element._value, element._children.empty());
	delayed.attr = element._attributes;
	for (const auto& c: element._children)
	{
		delayed.children.push_back(delayElement(c));
	}
	return delayed;
}


} } } // namespace Poco::RemotingNG::SOAP
