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
#include "Poco/RefPtr.h"


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
	typedef RefPtr<DOMSerializer> Ptr;

	DOMSerializer();
		/// Creates the DOMSerializer.

	void serialize(RefPtr<Node>&& pNode);
		/// Serializes a DOM node and its children
		/// into a sequence of SAX events, which are
		/// reported to the registered SAX event
		/// handlers.

	// XMLReader
	void setEntityResolver(RefPtr<EntityResolver> pResolver);
	RefPtr<EntityResolver> getEntityResolver() const;
	void setDTDHandler(RefPtr<DTDHandler> pDTDHandler);
	RefPtr<DTDHandler> getDTDHandler() const;
	void setContentHandler(RefPtr<ContentHandler> pContentHandler);
	RefPtr<ContentHandler> getContentHandler() const;
	void setErrorHandler(RefPtr<ErrorHandler> pErrorHandler);
	RefPtr<ErrorHandler> getErrorHandler() const;

	void setFeature(const XMLString& featureId, bool state);
	bool getFeature(const XMLString& featureId) const;
	void setProperty(const XMLString& propertyId, const XMLString& value);
	void setProperty(const XMLString& propertyId, RefPtr<SAXHandler> value);
	RefPtr<SAXHandler> getProperty(const XMLString& propertyId) const;

protected:
	~DOMSerializer();
	/// Destroys the DOMSerializer.

	void parse(RefPtr<InputSource> pSource);
		/// The DOMSerializer cannot parse an InputSource,
		/// so this method simply throws an XMLException when invoked.
		
	void parse(const XMLString& systemId);
		/// The DOMSerializer cannot parse from a system identifier,
		/// so this method simply throws an XMLException when invoked.

	void parseMemoryNP(const char* xml, std::size_t size);
		/// The DOMSerializer cannot parse from a system identifier,
		/// so this method simply throws an XMLException when invoked.

	void iterate(RefPtr<Node> pNode) const;
	void handleNode(RefPtr<Node>&& pNode) const;
	void handleElement(RefPtr<Element>&& pElement) const;
	void handleCharacterData(RefPtr<Text>&& pText) const;
	void handleComment(RefPtr<Comment>&& pComment) const;
	void handlePI(RefPtr<ProcessingInstruction>&& pPI) const;
	void handleCDATASection(RefPtr<CDATASection>&& pCDATA) const;
	void handleDocument(RefPtr<Document>&& pDocument) const;
	void handleDocumentType(RefPtr<DocumentType>&& pDocumentType) const;
	void handleFragment(RefPtr<DocumentFragment>&& pFragment) const;
	void handleNotation(RefPtr<Notation>&& pNotation) const;
	void handleEntity(RefPtr<Entity>&& pEntity) const;

private:
	RefPtr<EntityResolver> _pEntityResolver;
	mutable RefPtr<DTDHandler>     _pDTDHandler;
	mutable RefPtr<ContentHandler> _pContentHandler;
	mutable RefPtr<ErrorHandler>   _pErrorHandler;
	mutable RefPtr<DeclHandler>    _pDeclHandler;
	mutable RefPtr<LexicalHandler> _pLexicalHandler;

	static const XMLString CDATA;
};


} } // namespace Poco::XML


#endif // DOM_DOMSerializer_INCLUDED
