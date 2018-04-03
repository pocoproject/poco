//
// SAXParser.cpp
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/SAXException.h"
#include "Poco/SAX/EntityResolverImpl.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/SAX/SAXHandler.h"
#include "Poco/SAX/DTDHandler.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/ErrorHandler.h"
#include "Poco/SAX/DeclHandler.h"
#include "Poco/SAX/LexicalHandler.h"
#include "Poco/XML/NamespaceStrategy.h"
#include <sstream>


namespace Poco {
namespace XML {


const XMLString SAXParser::FEATURE_PARTIAL_READS = toXMLString("http://www.appinf.com/features/enable-partial-reads");


SAXParser::SAXParser():
	_pEngine(new ParserEngine),
	_namespaces(true),
	_namespacePrefixes(false)
{
}


SAXParser::SAXParser(const XMLString& encoding):
	_pEngine(new ParserEngine(encoding)),
	_namespaces(true),
	_namespacePrefixes(false)
{
}


SAXParser::~SAXParser()
{
}


void SAXParser::setEncoding(const XMLString& encoding)
{
	_pEngine->setEncoding(encoding);
}


const XMLString& SAXParser::getEncoding() const
{
	return _pEngine->getEncoding();
}


void SAXParser::addEncoding(const XMLString& name, Poco::TextEncoding* pEncoding)
{
	_pEngine->addEncoding(name, pEncoding);
}


void SAXParser::setEntityResolver(EntityResolver::Ptr pResolver)
{
	_pEngine->setEntityResolver(pResolver);
}


EntityResolver::Ptr SAXParser::getEntityResolver() const
{
	return _pEngine->getEntityResolver();
}


void SAXParser::setDTDHandler(DTDHandler::Ptr pDTDHandler)
{
	_pEngine->setDTDHandler(pDTDHandler);
}


DTDHandler::Ptr SAXParser::getDTDHandler() const
{
	return _pEngine->getDTDHandler();
}


void SAXParser::setContentHandler(ContentHandler::Ptr pContentHandler)
{
	_pEngine->setContentHandler(pContentHandler);
}


ContentHandler::Ptr SAXParser::getContentHandler() const
{
	return _pEngine->getContentHandler();
}


void SAXParser::setErrorHandler(ErrorHandler::Ptr pErrorHandler)
{
	_pEngine->setErrorHandler(pErrorHandler);
}


ErrorHandler::Ptr SAXParser::getErrorHandler() const
{
	return _pEngine->getErrorHandler();
}


void SAXParser::setFeature(const XMLString& featureId, bool state)
{
	if (featureId == XMLReader::FEATURE_VALIDATION || featureId == XMLReader::FEATURE_STRING_INTERNING)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_VALIDATION));
	else if (featureId == XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES)
		_pEngine->setExternalGeneralEntities(state);
	else if (featureId == XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES)
		_pEngine->setExternalParameterEntities(state);
	else if (featureId == XMLReader::FEATURE_NAMESPACES)
		_namespaces = state;
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		_namespacePrefixes = state;
	else if (featureId == FEATURE_PARTIAL_READS)
		_pEngine->setEnablePartialReads(state);
	else throw SAXNotRecognizedException(fromXMLString(featureId));
}


bool SAXParser::getFeature(const XMLString& featureId) const
{
	if (featureId == XMLReader::FEATURE_VALIDATION || featureId == XMLReader::FEATURE_STRING_INTERNING)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_VALIDATION));
	else if (featureId == XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES)
		return _pEngine->getExternalGeneralEntities();
	else if (featureId == XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES)
		return _pEngine->getExternalParameterEntities();
	else if (featureId == XMLReader::FEATURE_NAMESPACES)
		return _namespaces;
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		return _namespacePrefixes;
	else if (featureId == FEATURE_PARTIAL_READS)
		return _pEngine->getEnablePartialReads();
	else throw SAXNotRecognizedException(fromXMLString(featureId));
}


void SAXParser::setProperty(const XMLString& propertyId, const XMLString& value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER || propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		throw SAXNotSupportedException(std::string("property does not take a string value: ") + fromXMLString(propertyId));
	else
		throw SAXNotRecognizedException(fromXMLString(propertyId));
}


void SAXParser::setProperty(const XMLString& propertyId, SAXHandler::Ptr value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		_pEngine->setDeclHandler(value.cast<DeclHandler>());
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		_pEngine->setLexicalHandler(value.cast<LexicalHandler>());
	else throw SAXNotRecognizedException(fromXMLString(propertyId));
}


SAXHandler::Ptr SAXParser::getProperty(const XMLString& propertyId) const
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		return _pEngine->getDeclHandler();
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		return _pEngine->getLexicalHandler();
	else throw SAXNotSupportedException(fromXMLString(propertyId));
}


void SAXParser::parse(InputSource::Ptr pInputSource)
{
	if (pInputSource->getByteStream() || pInputSource->getCharacterStream())
	{
		setupParse();
		_pEngine->parse(pInputSource);
	}
	else parse(pInputSource->getSystemId());
}


void SAXParser::parse(const XMLString& systemId)
{
	setupParse();
	EntityResolverImpl::Ptr entityResolver = new EntityResolverImpl;
	InputSource::Ptr pInputSource = entityResolver->resolveEntity(0, systemId);
	if (pInputSource)
	{
		try
		{
			_pEngine->parse(pInputSource);
		}
		catch (...)
		{
			pInputSource = 0;
			throw;
		}
		pInputSource = 0;
	}
	else throw XMLException("Cannot resolve system identifier", fromXMLString(systemId));
}


void SAXParser::parseString(const std::string& xml)
{
	parseMemoryNP(xml.data(), xml.size());
}


void SAXParser::parseMemoryNP(const char* xml, std::size_t size)
{
	setupParse();
	_pEngine->parse(xml, size);
}


void SAXParser::setupParse()
{
	if (_namespaces && !_namespacePrefixes)
		_pEngine->setNamespaceStrategy(new NoNamespacePrefixesStrategy);
	else if (_namespaces && _namespacePrefixes)
		_pEngine->setNamespaceStrategy(new NamespacePrefixesStrategy);
	else
		_pEngine->setNamespaceStrategy(new NoNamespacesStrategy);
}


} } // namespace Poco::XML
