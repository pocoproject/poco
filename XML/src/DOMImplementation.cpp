//
// DOMImplementation.cpp
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/String.h"


namespace Poco::XML {


const XMLString DOMImplementation::FEATURE_XML            = toXMLString("xml");
const XMLString DOMImplementation::FEATURE_CORE           = toXMLString("core");
const XMLString DOMImplementation::FEATURE_EVENTS         = toXMLString("events");
const XMLString DOMImplementation::FEATURE_MUTATIONEVENTS = toXMLString("mutationevents");
const XMLString DOMImplementation::FEATURE_TRAVERSAL      = toXMLString("traversal");
const XMLString DOMImplementation::VERSION_1_0            = toXMLString("1.0");
const XMLString DOMImplementation::VERSION_2_0            = toXMLString("2.0");


DOMImplementation::DOMImplementation()
{
}


DOMImplementation::~DOMImplementation()
{
}


bool DOMImplementation::hasFeature(const XMLString& feature, const XMLString& version) const
{
	XMLString lcFeature = Poco::toLower(feature);
	bool isXML = (lcFeature == FEATURE_XML && version == VERSION_1_0);
	bool isV2Feature = (version == VERSION_2_0) &&
		(lcFeature == FEATURE_CORE ||
		 lcFeature == FEATURE_EVENTS ||
		 lcFeature == FEATURE_MUTATIONEVENTS ||
		 lcFeature == FEATURE_TRAVERSAL);
	return isXML || isV2Feature;
}


DocumentType* DOMImplementation::createDocumentType(const XMLString& name, const XMLString& publicId, const XMLString& systemId) const
{
	return new DocumentType(nullptr, name, publicId, systemId);
}


Document* DOMImplementation::createDocument(const XMLString& namespaceURI, const XMLString& qualifiedName, DocumentType* doctype) const
{
	Document* pDoc = new Document(doctype);
	if (namespaceURI.empty())
		pDoc->appendChild(pDoc->createElement(qualifiedName))->release();
	else
		pDoc->appendChild(pDoc->createElementNS(namespaceURI, qualifiedName))->release();
	return pDoc;
}


const DOMImplementation& DOMImplementation::instance()
{
	static DOMImplementation di;
	return di;
}


} // namespace Poco::XML
