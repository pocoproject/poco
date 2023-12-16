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
	Node* getNamedItem(const XMLString& name) const override;
	Node* setNamedItem(Node* arg) override;
	Node* removeNamedItem(const XMLString& name) override;
	Node* item(unsigned long index) const override;
	unsigned long length() const override;

	Node* getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const override;
	Node* setNamedItemNS(Node* arg) override;
	Node* removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) override;

	void autoRelease() override;

protected:
	DTDMap(const DocumentType* pDocumentType, unsigned short type);
	~DTDMap() override;

private:
	DTDMap();

	const DocumentType* _pDocumentType;
	unsigned short _type;

	friend class DocumentType;
};


} } // namespace Poco::XML


#endif // DOM_DTDMap_INCLUDED
