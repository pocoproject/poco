//
// XMLFilterImpl.h
//
// Library: XML
// Package: SAX
// Module:  SAXFilters
//
// SAX2 XMLFilterImpl class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SAX_XMLFilterImpl_INCLUDED
#define SAX_XMLFilterImpl_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/XMLFilter.h"
#include "Poco/SAX/EntityResolver.h"
#include "Poco/SAX/DTDHandler.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/ErrorHandler.h"


namespace Poco {
namespace XML {


class XML_API XMLFilterImpl: public XMLFilter, public EntityResolver, public DTDHandler, public ContentHandler, public ErrorHandler
	/// Base class for deriving an XML filter.
	///
	/// This class is designed to sit between an XMLReader and the client application's event
	/// handlers. By default, it does nothing but pass requests up to the reader and events on to
	/// the handlers unmodified, but subclasses can override specific methods to modify the event
	/// stream or the configuration requests as they pass through.
{
public:
	XMLFilterImpl();
		/// Construct an empty XML filter, with no parent.
		///
		/// This filter will have no parent: you must assign a parent before you start a parse or do any
		/// configuration with setFeature or setProperty, unless you use this as a pure event consumer rather
		/// than as an XMLReader.

	XMLFilterImpl(XMLReader* pParent);
		/// Construct an XML filter with the specified parent.

	~XMLFilterImpl() override;
		/// Destroys the XMLFilterImpl.

	// XMLFilter
	XMLReader* getParent() const override;
	void setParent(XMLReader* pParent) override;

	// XMLReader
	void setEntityResolver(EntityResolver* pResolver) override;
	EntityResolver* getEntityResolver() const override;
	void setDTDHandler(DTDHandler* pDTDHandler) override;
	DTDHandler* getDTDHandler() const override;
	void setContentHandler(ContentHandler* pContentHandler) override;
	ContentHandler* getContentHandler() const override;
	void setErrorHandler(ErrorHandler* pErrorHandler) override;
	ErrorHandler* getErrorHandler() const override;
	void setFeature(const XMLString& featureId, bool state) override;
	bool getFeature(const XMLString& featureId) const override;
	void setProperty(const XMLString& propertyId, const XMLString& value) override;
	void setProperty(const XMLString& propertyId, void* value) override;
	void* getProperty(const XMLString& propertyId) const override;
	void parse(InputSource* pSource) override;
	void parse(const XMLString& systemId) override;
	void parseMemoryNP(const char* xml, std::size_t size) override;

	// EntityResolver
	InputSource* resolveEntity(const XMLString* publicId, const XMLString& systemId) override;
	void releaseInputSource(InputSource* pSource) override;

	// DTDHandler
	void notationDecl(const XMLString& name, const XMLString* publicId, const XMLString* systemId) override;
	void unparsedEntityDecl(const XMLString& name, const XMLString* publicId, const XMLString& systemId, const XMLString& notationName) override;

	// ContentHandler
	void setDocumentLocator(const Locator* loc) override;
	void startDocument() override;
	void endDocument() override;
	void startElement(const XMLString& uri, const XMLString& localName, const XMLString& qname, const Attributes& attrList) override;
	void endElement(const XMLString& uri, const XMLString& localName, const XMLString& qname) override;
	void characters(const XMLChar ch[], int start, int length) override;
	void ignorableWhitespace(const XMLChar ch[], int start, int length) override;
	void processingInstruction(const XMLString& target, const XMLString& data) override;
	void startPrefixMapping(const XMLString& prefix, const XMLString& uri) override;
	void endPrefixMapping(const XMLString& prefix) override;
	void skippedEntity(const XMLString& prefix) override;
	void warning(const SAXException& e) override;
	void error(const SAXException& e) override;
	void fatalError(const SAXException& e) override;

protected:
	XMLReader* parent() const;
		/// Return a pointer to the parent reader.
		/// Subclasses can use this method instead of
		/// getParent() for better performance - this method
		/// is non-virtual and implemented as inline.

	virtual void setupParse();
		/// Setup the event handlers in the parent reader.

private:
	XMLReader*      _pParent;
	EntityResolver* _pEntityResolver;
	DTDHandler*     _pDTDHandler;
	ContentHandler* _pContentHandler;
	ErrorHandler*   _pErrorHandler;
};


//
// inlines
//
inline XMLReader* XMLFilterImpl::parent() const
{
	return _pParent;
}


} } // namespace Poco::XML


#endif // SAX_XMLFilterImpl_INCLUDED
