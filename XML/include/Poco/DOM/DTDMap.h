//
// DTDMap.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Definition of the DTDMap class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_DTDMap_INCLUDED
#define DOM_DTDMap_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/DOM/NamedNodeMap.h"


namespace Poco {
namespace XML {


class DocumentType;


class XML_API DTDMap: public NamedNodeMap
	/// This implementation of NamedNodeMap
	/// is returned by DocumentType::entities()
	/// and DocumentType::notations().
{
public:
	Node::Ptr getNamedItem(const XMLString& name) const;
	Node::Ptr setNamedItem(Node::Ptr arg);
	Node::Ptr removeNamedItem(const XMLString& name);
	Node::Ptr item(unsigned long index) const;
	unsigned long length() const;

	Node::Ptr getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const;
	Node::Ptr setNamedItemNS(Node::Ptr arg);
	Node::Ptr removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName);

protected:
	DTDMap(const RefPtr<DocumentType> pDocumentType, unsigned short type);
	~DTDMap();

private:
	DTDMap();

	const RefPtr<DocumentType> _pDocumentType;
	unsigned short _type;

	friend class DocumentType;
};


} } // namespace Poco::XML


#endif // DOM_DTDMap_INCLUDED
