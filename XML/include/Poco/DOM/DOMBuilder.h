//
// DOMBuilder.h
//
// Library: XML
// Package: DOM
// Module:  DOMBuilder
//
// Definition of the DOMBuilder class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_DOMBuilder_INCLUDED
#define DOM_DOMBuilder_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/LexicalHandler.h"
#include "Poco/SAX/DTDHandler.h"
#include "Poco/XML/XMLString.h"


namespace Poco {
namespace XML {


class XMLReader;
class Document;
class InputSource;
class AbstractNode;
class AbstractContainerNode;
class NamePool;


class XML_API DOMBuilder: protected DTDHandler, protected ContentHandler, protected LexicalHandler
	/// This class builds a tree representation of an
	/// XML document, according to the W3C Document Object Model, Level 1 and 2
	/// specifications.
	///
	/// The actual XML parsing is done by an XMLReader, which
	/// must be supplied to the DOMBuilder.
{
public:
	DOMBuilder(XMLReader& xmlReader, NamePool* pNamePool = nullptr);
		/// Creates a DOMBuilder using the given XMLReader.
		/// If a NamePool is given, it becomes the Document's NamePool.

	~DOMBuilder() override;
		/// Destroys the DOMBuilder.

	virtual Document* parse(const XMLString& uri);
		/// Parse an XML document from a location identified by an URI.

	virtual Document* parse(InputSource* pInputSource);
		/// Parse an XML document from a location identified by an InputSource.

	virtual Document* parseMemoryNP(const char* xml, std::size_t size);
		/// Parses an XML document from memory.

protected:
	// DTDHandler
	void notationDecl(const XMLString& name, const XMLString* publicId, const XMLString* systemId) override;
	void unparsedEntityDecl(const XMLString& name, const XMLString* publicId, const XMLString& systemId, const XMLString& notationName) override;

	// ContentHandler
	void setDocumentLocator(const Locator* loc) override;
	void startDocument() override;
	void endDocument() override;
	void startElement(const XMLString& uri, const XMLString& localName, const XMLString& qname, const Attributes& attributes) override;
	void endElement(const XMLString& uri, const XMLString& localName, const XMLString& qname) override;
	void characters(const XMLChar ch[], int start, int length) override;
	void ignorableWhitespace(const XMLChar ch[], int start, int length) override;
	void processingInstruction(const XMLString& target, const XMLString& data) override;
	void startPrefixMapping(const XMLString& prefix, const XMLString& uri) override;
	void endPrefixMapping(const XMLString& prefix) override;
	void skippedEntity(const XMLString& name) override;

	// LexicalHandler
	void startDTD(const XMLString& name, const XMLString& publicId, const XMLString& systemId) override;
	void endDTD() override;
	void startEntity(const XMLString& name) override;
	void endEntity(const XMLString& name) override;
	void startCDATA() override;
	void endCDATA() override;
	void comment(const XMLChar ch[], int start, int length) override;

	void appendNode(AbstractNode* pNode);

	void setupParse();

private:
	static const XMLString EMPTY_STRING;

	XMLReader&             _xmlReader;
	NamePool*              _pNamePool;
	Document*              _pDocument;
	AbstractContainerNode* _pParent;
	AbstractNode*          _pPrevious;
	bool                   _inCDATA;
	bool                   _namespaces;
};


} } // namespace Poco::XML


#endif // DOM_DOMBuilder_INCLUDED
