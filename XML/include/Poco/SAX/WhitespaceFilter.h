//
// WhitespaceFilter.h
//
// Library: XML
// Package: SAX
// Module:  WhitespaceFilter
//
// Definition of the WhitespaceFilter class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SAX_WhitespaceFilter_INCLUDED
#define SAX_WhitespaceFilter_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/XMLFilterImpl.h"
#include "Poco/SAX/LexicalHandler.h"


namespace Poco {
namespace XML {


class XML_API WhitespaceFilter: public XMLFilterImpl, public LexicalHandler
	/// This implementation of the SAX2 XMLFilter interface
	/// filters all whitespace-only character data element
	/// content.
{
public:
	WhitespaceFilter();
		/// Creates the WhitespaceFilter, with no parent.

	WhitespaceFilter(XMLReader* pReader);
		/// Creates the WhitespaceFilter with the specified parent.

	~WhitespaceFilter() override;
		/// Destroys the WhitespaceFilter.

	// XMLReader
	void setProperty(const XMLString& propertyId, const XMLString& value) override;
	void setProperty(const XMLString& propertyId, void* value) override;
	void* getProperty(const XMLString& propertyId) const override;

	// ContentHandler
	void startDocument() override;
	void endDocument() override;
	void startElement(const XMLString& uri, const XMLString& localName, const XMLString& qname, const Attributes& attrList) override;
	void endElement(const XMLString& uri, const XMLString& localName, const XMLString& qname) override;
	void characters(const XMLChar ch[], int start, int length) override;
	void ignorableWhitespace(const XMLChar ch[], int start, int length) override;
	void processingInstruction(const XMLString& target, const XMLString& data) override;

	// LexicalHandler
	void startDTD(const XMLString& name, const XMLString& publicId, const XMLString& systemId) override;
	void endDTD() override;
	void startEntity(const XMLString& name) override;
	void endEntity(const XMLString& name) override;
	void startCDATA() override;
	void endCDATA() override;
	void comment(const XMLChar ch[], int start, int length) override;

protected:
	void setupParse() override;

private:
	LexicalHandler* _pLexicalHandler;
	XMLString       _data;
	bool            _filter;
};


} } // namespace Poco::XML


#endif // SAX_WhitespaceFilter_INCLUDED
