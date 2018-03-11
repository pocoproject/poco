//
// DOMSerializer.h
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


#ifndef DOM_DOMSerializer_INCLUDED
#define DOM_DOMSerializer_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace XML {


class Node;
class Element;
class Text;
class Comment;
class ProcessingInstruction;
class Entity;
class CDATASection;
class Notation;
class Document;
class DocumentType;
class DocumentFragment;
class SAXHandler;
class DeclHandler;
class LexicalHandler;


class XML_API DOMSerializer: public XMLReader
	/// The DOMSerializer serializes a DOM document
	/// into a sequence of SAX events which are
	/// reported to the registered SAX event
	/// handlers.
	///
	/// The DOMWriter uses a DOMSerializer with an
	/// XMLWriter to serialize a DOM document into
	/// textual XML.
{
public:
	typedef AutoPtr<DOMSerializer> Ptr;

	DOMSerializer();
		/// Creates the DOMSerializer.

	void serialize(AutoPtr<Node> pNode);
		/// Serializes a DOM node and its children
		/// into a sequence of SAX events, which are
		/// reported to the registered SAX event
		/// handlers.

	// XMLReader
	void setEntityResolver(AutoPtr<EntityResolver> pResolver);
	AutoPtr<EntityResolver> getEntityResolver() const;
	void setDTDHandler(AutoPtr<DTDHandler> pDTDHandler);
	AutoPtr<DTDHandler> getDTDHandler() const;
	void setContentHandler(AutoPtr<ContentHandler> pContentHandler);
	AutoPtr<ContentHandler> getContentHandler() const;
	void setErrorHandler(AutoPtr<ErrorHandler> pErrorHandler);
	AutoPtr<ErrorHandler> getErrorHandler() const;

	void setFeature(const XMLString& featureId, bool state);
	bool getFeature(const XMLString& featureId) const;
	void setProperty(const XMLString& propertyId, const XMLString& value);
	void setProperty(const XMLString& propertyId, AutoPtr<SAXHandler> value);
	AutoPtr<SAXHandler> getProperty(const XMLString& propertyId) const;

protected:
	~DOMSerializer();
	/// Destroys the DOMSerializer.

	void parse(AutoPtr<InputSource> pSource);
		/// The DOMSerializer cannot parse an InputSource,
		/// so this method simply throws an XMLException when invoked.
		
	void parse(const XMLString& systemId);
		/// The DOMSerializer cannot parse from a system identifier,
		/// so this method simply throws an XMLException when invoked.

	void parseMemoryNP(const char* xml, std::size_t size);
		/// The DOMSerializer cannot parse from a system identifier,
		/// so this method simply throws an XMLException when invoked.

	void iterate(AutoPtr<Node> pNode) const;
	void handleNode(AutoPtr<Node> pNode) const;
	void handleElement(AutoPtr<Element> pElement) const;
	void handleCharacterData(AutoPtr<Text> pText) const;
	void handleComment(AutoPtr<Comment> pComment) const;
	void handlePI(AutoPtr<ProcessingInstruction> pPI) const;
	void handleCDATASection(AutoPtr<CDATASection> pCDATA) const;
	void handleDocument(AutoPtr<Document> pDocument) const;
	void handleDocumentType(AutoPtr<DocumentType> pDocumentType) const;
	void handleFragment(AutoPtr<DocumentFragment> pFragment) const;
	void handleNotation(AutoPtr<Notation> pNotation) const;
	void handleEntity(AutoPtr<Entity> pEntity) const;

private:
	AutoPtr<EntityResolver> _pEntityResolver;
	mutable AutoPtr<DTDHandler>     _pDTDHandler;
	mutable AutoPtr<ContentHandler> _pContentHandler;
	mutable AutoPtr<ErrorHandler>   _pErrorHandler;
	mutable AutoPtr<DeclHandler>    _pDeclHandler;
	mutable AutoPtr<LexicalHandler> _pLexicalHandler;

	static const XMLString CDATA;
};


} } // namespace Poco::XML


#endif // DOM_DOMSerializer_INCLUDED
