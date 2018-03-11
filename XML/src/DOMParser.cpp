//
// DOMParser.cpp
//
// Library: XML
// Package: DOM
// Module:  DOMParser
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/DOMBuilder.h"
#include "Poco/SAX/WhitespaceFilter.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/XML/NamePool.h"
#include <sstream>


namespace Poco {
namespace XML {


const XMLString DOMParser::FEATURE_FILTER_WHITESPACE = toXMLString("http://www.appinf.com/features/no-whitespace-in-element-content");


DOMParser::DOMParser(NamePool::Ptr pNamePool):
	_pSAXParser(new SAXParser),
	_pNamePool(pNamePool),
	_filterWhitespace(false)
{
	_pSAXParser->setFeature(XMLReader::FEATURE_NAMESPACES, true);
	_pSAXParser->setFeature(XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
}


DOMParser::DOMParser(unsigned long namePoolSize):
	_pSAXParser(new SAXParser),
	_pNamePool(new NamePool(namePoolSize)),
	_filterWhitespace(false)
{
	_pSAXParser->setFeature(XMLReader::FEATURE_NAMESPACES, true);
	_pSAXParser->setFeature(XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
}


DOMParser::~DOMParser()
{
}


void DOMParser::setEncoding(const XMLString& encoding)
{
	_pSAXParser->setEncoding(encoding);
}


const XMLString& DOMParser::getEncoding() const
{
	return _pSAXParser->getEncoding();
}


void DOMParser::addEncoding(const XMLString& name, Poco::TextEncoding* pEncoding)
{
	_pSAXParser->addEncoding(name, pEncoding);
}


void DOMParser::setFeature(const XMLString& name, bool state)
{
	if (name == FEATURE_FILTER_WHITESPACE)
		_filterWhitespace = state;
	else
		_pSAXParser->setFeature(name, state);
}


bool DOMParser::getFeature(const XMLString& name) const
{
	if (name == FEATURE_FILTER_WHITESPACE)
		return _filterWhitespace;
	else
		return _pSAXParser->getFeature(name);
}


Document::Ptr DOMParser::parse(const XMLString& uri)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter::Ptr filter = new WhitespaceFilter(_pSAXParser);
		DOMBuilder::Ptr builder = new DOMBuilder(filter, _pNamePool);
		return builder->parse(uri);
	}
	else
	{
		DOMBuilder::Ptr builder = new DOMBuilder(_pSAXParser, _pNamePool);
		return builder->parse(uri);
	}
}


Document::Ptr DOMParser::parse(InputSource::Ptr pInputSource)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter::Ptr filter = new WhitespaceFilter(_pSAXParser);
		DOMBuilder::Ptr builder = new DOMBuilder(filter, _pNamePool);
		return builder->parse(pInputSource);
	}
	else
	{
		DOMBuilder::Ptr builder = new DOMBuilder(_pSAXParser, _pNamePool);
		return builder->parse(pInputSource);
	}
}


Document::Ptr DOMParser::parseString(const std::string& xml)
{
	return parseMemory(xml.data(), xml.size());
}


Document::Ptr DOMParser::parseMemory(const char* xml, std::size_t size)
{
	if (_filterWhitespace)
	{
		WhitespaceFilter::Ptr filter = new WhitespaceFilter(_pSAXParser);
		DOMBuilder::Ptr builder = new DOMBuilder(filter, _pNamePool);
		return builder->parseMemoryNP(xml, size);
	}
	else
	{
		DOMBuilder::Ptr builder = new DOMBuilder(_pSAXParser, _pNamePool);
		return builder->parseMemoryNP(xml, size);
	}
}


EntityResolver::Ptr DOMParser::getEntityResolver() const
{
	return _pSAXParser->getEntityResolver();
}


void DOMParser::setEntityResolver(EntityResolver::Ptr pEntityResolver)
{
	_pSAXParser->setEntityResolver(pEntityResolver);
}


} } // namespace Poco::XML
