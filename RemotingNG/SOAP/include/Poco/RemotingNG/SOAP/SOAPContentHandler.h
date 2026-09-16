//
// SOAPContentHandler.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPContentHandler
//
// Definition of the SOAPContentHandler class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_SOAPContentHandler_INCLUDED
#define RemotingNG_SOAP_SOAPContentHandler_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/XMLElement.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/AttributesImpl.h"
#include <stack>
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API SOAPContentHandler: public Poco::XML::ContentHandler
	/// This ContentHandler builds a simplified in-memory representation of an XML
	/// infoset containing only elements, attributes and character data.
	///
	/// The in-memory representation is optimized for SOAP infosets and cannot
	/// represent arbitrary XML documents (e.g., those with mixed element content).
{
public:
	using Node = XMLElement;

	SOAPContentHandler();
		/// Creates the SOAPContentHandler.

	~SOAPContentHandler();
		/// Destroys the SOAPContentHandler.

	const Node& content() const;
		/// Returns the Node representing the document element.

	// ContentHandler
	void setDocumentLocator(const XML::Locator* loc);
	void startDocument();
	void endDocument();
	void startElement(const std::string& uri, const std::string& localName, const std::string& qname, const XML::Attributes& attrList);
	void endElement(const std::string& uri, const std::string& localName, const std::string& qname);
	void characters(const XML::XMLChar ch[], int start, int length);
	void ignorableWhitespace(const XML::XMLChar ch[], int start, int length);
	void processingInstruction(const std::string& target, const std::string& data);
	void startPrefixMapping(const std::string& prefix, const std::string& uri);
	void endPrefixMapping(const std::string& prefix);
	void skippedEntity(const std::string& name);

private:
	Node _content;
	std::stack<Node*> _nodeStack;
};


//
// inlines
//
inline const SOAPContentHandler::Node& SOAPContentHandler::content() const
{
	return _content;
}


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_SOAPContentHandler_INCLUDED
