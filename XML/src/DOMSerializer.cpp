//
// DOMSerializer.cpp
//
// Library: XML
// Package: DOM
// Module:  DOMSerializer
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/DOMSerializer.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/CDATASection.h"
#include "Poco/DOM/Comment.h"
#include "Poco/DOM/ProcessingInstruction.h"
#include "Poco/DOM/Entity.h"
#include "Poco/DOM/Notation.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/SAX/EntityResolver.h"
#include "Poco/SAX/SAXHandler.h"
#include "Poco/SAX/DeclHandler.h"
#include "Poco/SAX/DTDHandler.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/LexicalHandler.h"
#include "Poco/SAX/AttributesImpl.h"
#include "Poco/SAX/ErrorHandler.h"
#include "Poco/SAX/SAXException.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/RefPtr.h"


namespace Poco {
namespace XML {


const XMLString DOMSerializer::CDATA = toXMLString("CDATA");


DOMSerializer::DOMSerializer():
	_pEntityResolver(0),
	_pDTDHandler(0),
	_pContentHandler(0),
	_pErrorHandler(0),
	_pDeclHandler(0),
	_pLexicalHandler(0)
{
}


DOMSerializer::~DOMSerializer()
{
}


void DOMSerializer::setEntityResolver(EntityResolver::Ptr pEntityResolver)
{
	_pEntityResolver = pEntityResolver;
}


EntityResolver::Ptr DOMSerializer::getEntityResolver() const
{
	return _pEntityResolver;
}


void DOMSerializer::setDTDHandler(DTDHandler::Ptr pDTDHandler)
{
	_pDTDHandler = pDTDHandler;
}


DTDHandler::Ptr DOMSerializer::getDTDHandler() const
{
	return _pDTDHandler;
}


void DOMSerializer::setContentHandler(ContentHandler::Ptr pContentHandler)
{
	_pContentHandler = pContentHandler;
}


ContentHandler::Ptr DOMSerializer::getContentHandler() const
{
	return _pContentHandler;
}


void DOMSerializer::setErrorHandler(ErrorHandler::Ptr pErrorHandler)
{
	_pErrorHandler = pErrorHandler;
}


ErrorHandler::Ptr DOMSerializer::getErrorHandler() const
{
	return _pErrorHandler;
}


void DOMSerializer::setFeature(const XMLString& featureId, bool state)
{
	if (featureId == XMLReader::FEATURE_NAMESPACES)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_NAMESPACES));
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_NAMESPACE_PREFIXES));
	else
		throw SAXNotRecognizedException(fromXMLString(featureId));
}


bool DOMSerializer::getFeature(const XMLString& featureId) const
{
	if (featureId == XMLReader::FEATURE_NAMESPACES)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_NAMESPACES));
	else if (featureId == XMLReader::FEATURE_NAMESPACE_PREFIXES)
		throw SAXNotSupportedException(fromXMLString(XMLReader::FEATURE_NAMESPACE_PREFIXES));
	else
		throw SAXNotRecognizedException(fromXMLString(featureId));
}


void DOMSerializer::setProperty(const XMLString& propertyId, const XMLString& value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER || propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		throw SAXNotSupportedException(std::string("property does not take a string value: ") + fromXMLString(propertyId));
	else
		throw SAXNotRecognizedException(fromXMLString(propertyId));
}


void DOMSerializer::setProperty(const XMLString& propertyId, SAXHandler::Ptr value)
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		_pDeclHandler = value.cast<DeclHandler>();
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		_pLexicalHandler = value.cast<LexicalHandler>();
	else throw SAXNotRecognizedException(fromXMLString(propertyId));
}


SAXHandler::Ptr DOMSerializer::getProperty(const XMLString& propertyId) const
{
	if (propertyId == XMLReader::PROPERTY_DECLARATION_HANDLER)
		return _pDeclHandler.cast<SAXHandler>();
	else if (propertyId == XMLReader::PROPERTY_LEXICAL_HANDLER)
		return _pLexicalHandler.cast<SAXHandler>();
	else throw SAXNotSupportedException(fromXMLString(propertyId));
}


void DOMSerializer::serialize(const Node::Ptr pNode)
{
	poco_check_ptr (pNode);

	handleNode(pNode);
}


void DOMSerializer::parse(InputSource::Ptr pSource)
{
	throw XMLException("The DOMSerializer cannot parse an InputSource");
}


void DOMSerializer::parse(const XMLString& systemId)
{
	throw XMLException("The DOMSerializer cannot parse from a system identifier");
}


void DOMSerializer::parseMemoryNP(const char* xml, std::size_t size)
{
	throw XMLException("The DOMSerializer cannot parse from memory");
}


void DOMSerializer::iterate(Node::Ptr pNode) const
{
	while (pNode)
	{
		handleNode(pNode);
		pNode = pNode->nextSibling();
	}
}


void DOMSerializer::handleNode(Node::Ptr pNode) const
{
	switch (pNode->nodeType())
	{
	case Node::ELEMENT_NODE:
		handleElement(pNode.unsafeCast<Element>());
		break;
	case Node::TEXT_NODE:
		handleCharacterData(pNode.unsafeCast<Text>());
		break;
	case Node::CDATA_SECTION_NODE:
		handleCDATASection(pNode.unsafeCast<CDATASection>());
		break;
	case Node::ENTITY_NODE:
		handleEntity(pNode.unsafeCast<Entity>());
		break;
	case Node::PROCESSING_INSTRUCTION_NODE:
		handlePI(pNode.unsafeCast<ProcessingInstruction>());
		break;
	case Node::COMMENT_NODE:
		handleComment(pNode.unsafeCast<Comment>());
		break;
	case Node::DOCUMENT_NODE:
		handleDocument(pNode.unsafeCast<Document>());
		break;
	case Node::DOCUMENT_TYPE_NODE:
		handleDocumentType(pNode.unsafeCast<DocumentType>());
		break;
	case Node::DOCUMENT_FRAGMENT_NODE:
		handleFragment(pNode.unsafeCast<DocumentFragment>());
		break;
	case Node::NOTATION_NODE:
		handleNotation(pNode.unsafeCast<Notation>());
		break;
	}
}


void DOMSerializer::handleElement(Element::Ptr pElement) const
{
	if (_pContentHandler)
	{
		RefPtr<NamedNodeMap> pAttrs = pElement->attributes();
		AttributesImpl saxAttrs;
		for (unsigned long i = 0; i < pAttrs->length(); ++i)
		{
			Attr::Ptr pAttr = pAttrs->item(i).unsafeCast<Attr>();
			saxAttrs.addAttribute(pAttr->namespaceURI(), pAttr->localName(), pAttr->nodeName(), CDATA, pAttr->value(), pAttr->specified());
		}
		_pContentHandler->startElement(pElement->namespaceURI(), pElement->localName(), pElement->tagName(), saxAttrs);
	}
	iterate(pElement->firstChild());
	if (_pContentHandler)
		_pContentHandler->endElement(pElement->namespaceURI(), pElement->localName(), pElement->tagName());
}


void DOMSerializer::handleCharacterData(Text::Ptr pText) const
{
	if (_pContentHandler)
	{
		const XMLString& data = pText->data();
		_pContentHandler->characters(data.c_str(), 0, (int) data.length());
	}
}


void DOMSerializer::handleComment(Comment::Ptr pComment) const
{
	if (_pLexicalHandler)
	{
		const XMLString& data = pComment->data();
		_pLexicalHandler->comment(data.c_str(), 0, (int) data.length());
	}
}


void DOMSerializer::handlePI(ProcessingInstruction::Ptr pPI) const
{
	if (_pContentHandler) _pContentHandler->processingInstruction(pPI->target(), pPI->data());
}


void DOMSerializer::handleCDATASection(CDATASection::Ptr pCDATA) const
{
	if (_pLexicalHandler) _pLexicalHandler->startCDATA();
	handleCharacterData(pCDATA);
	if (_pLexicalHandler) _pLexicalHandler->endCDATA();
}


void DOMSerializer::handleDocument(Document::Ptr pDocument) const
{
	if (_pContentHandler) _pContentHandler->startDocument();
	DocumentType::Ptr pDoctype = pDocument->doctype();
	if (pDoctype) handleDocumentType(pDoctype);
	iterate(pDocument->firstChild());
	if (_pContentHandler) _pContentHandler->endDocument();
}


void DOMSerializer::handleDocumentType(DocumentType::Ptr pDocumentType) const
{
	if (_pLexicalHandler) _pLexicalHandler->startDTD(pDocumentType->name(), pDocumentType->publicId(), pDocumentType->systemId());
	iterate(pDocumentType->firstChild());
	if (_pLexicalHandler) _pLexicalHandler->endDTD();
}


void DOMSerializer::handleFragment(DocumentFragment::Ptr pFragment) const
{
	iterate(pFragment->firstChild());
}


void DOMSerializer::handleNotation(Notation::Ptr pNotation) const
{
	if (_pDTDHandler) _pDTDHandler->notationDecl(pNotation->nodeName(), &pNotation->publicId(), &pNotation->systemId());
}


void DOMSerializer::handleEntity(Entity::Ptr pEntity) const
{
	if (_pDTDHandler) _pDTDHandler->unparsedEntityDecl(pEntity->nodeName(), &pEntity->publicId(), pEntity->systemId(), pEntity->notationName());
}


} } // namespace Poco::XML
