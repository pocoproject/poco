//
// SOAPContentHandler.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPContentHandler
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/SOAPContentHandler.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


SOAPContentHandler::SOAPContentHandler()
{
}


SOAPContentHandler::~SOAPContentHandler()
{
}


void SOAPContentHandler::setDocumentLocator(const XML::Locator* /*loc*/)
{
}


void SOAPContentHandler::startDocument()
{
	_content = Node(); // reset data
	_nodeStack.push(&_content);
}


void SOAPContentHandler::endDocument()
{
	_nodeStack.pop();
}


void SOAPContentHandler::startElement(const std::string& uri, const std::string& localName, const std::string& qname, const XML::Attributes& attrList)
{
	Node* pNode = _nodeStack.top();
	pNode->_children.push_back(Node());
	pNode = &pNode->_children.back();
	_nodeStack.push(pNode);

	pNode->_name = localName.empty() ? qname : localName;
	poco_assert_dbg (!pNode->_name.empty());
	pNode->_attributes = attrList;
	pNode->_uri = uri;
}


void SOAPContentHandler::endElement(const std::string& /*uri*/, const std::string& /*localName*/, const std::string& /*qname*/)
{
	_nodeStack.pop();
}


void SOAPContentHandler::characters(const XML::XMLChar ch[], int start, int length)
{
	_nodeStack.top()->_value.append(ch + start, length);
}


void SOAPContentHandler::ignorableWhitespace(const XML::XMLChar[], int /*start*/, int /*length*/)
{
}


void SOAPContentHandler::processingInstruction(const std::string& /*target*/, const std::string& /*data*/)
{
}


void SOAPContentHandler::startPrefixMapping(const std::string& /*prefix*/, const std::string& /*uri*/)
{
}


void SOAPContentHandler::endPrefixMapping(const std::string& /*prefix*/)
{
}


void SOAPContentHandler::skippedEntity(const std::string& /*name*/)
{
}


} } } // namespace Poco::RemotingNG::SOAP
