//
// DocumentTypeTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DocumentTypeTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Notation.h"
#include "Poco/DOM/Entity.h"
#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/RefPtr.h"


using Poco::XML::DocumentType;
using Poco::XML::Document;
using Poco::XML::Entity;
using Poco::XML::Notation;
using Poco::XML::DOMImplementation;
using Poco::XML::NamedNodeMap;
using Poco::RefPtr;


DocumentTypeTest::DocumentTypeTest(const std::string& name): CppUnit::TestCase(name)
{
}


DocumentTypeTest::~DocumentTypeTest()
{
}


void DocumentTypeTest::testDocumentType()
{
	RefPtr<DocumentType> pDoctype = DOMImplementation::instance().createDocumentType("test", "public", "system");

	assertTrue (pDoctype->ownerDocument().isNull());
	assertTrue (pDoctype->name() == "test");
	assertTrue (pDoctype->publicId() == "public");
	assertTrue (pDoctype->systemId() == "system");

	RefPtr<Document> pDoc = new Document(pDoctype);
	assertTrue (pDoc->doctype() == pDoctype);
	assertTrue (pDoctype->ownerDocument() == pDoc);

	RefPtr<NamedNodeMap> pEntities = pDoctype->entities();
	RefPtr<NamedNodeMap> pNotations = pDoctype->notations();

	assertTrue (pEntities->length() == 0);
	assertTrue (pNotations->length() == 0);

	RefPtr<Entity> pEntity1 = pDoc->createEntity("entity1", "public1", "system1", "");
	pDoctype->appendChild(pEntity1);

	assertTrue (pEntities->length() == 1);
	assertTrue (pNotations->length() == 0);
	assertTrue (pEntities->item(0).cast<Entity>() == pEntity1);
	assertTrue (pEntities->getNamedItem("entity1").cast<Entity>() == pEntity1);

	RefPtr<Entity> pEntity2 = pDoc->createEntity("entity2", "public2", "system2", "");
	pDoctype->appendChild(pEntity2);
	assertTrue (pEntities->length() == 2);
	assertTrue (pNotations->length() == 0);
	assertTrue (pEntities->item(0).cast<Entity>() == pEntity1);
	assertTrue (pEntities->item(1).cast<Entity>() == pEntity2);
	assertTrue (pEntities->getNamedItem("entity1").cast<Entity>() == pEntity1);
	assertTrue (pEntities->getNamedItem("entity2").cast<Entity>() == pEntity2);

	RefPtr<Notation> pNotation = pDoc->createNotation("notation", "public", "system");
	pDoctype->appendChild(pNotation);
	assertTrue (pEntities->length() == 2);
	assertTrue (pNotations->length() == 1);
	assertTrue (pEntities->item(0).cast<Entity>() == pEntity1);
	assertTrue (pEntities->item(1).cast<Entity>() == pEntity2);
	assertTrue (pNotations->item(0).cast<Notation>() == pNotation);
	assertTrue (pEntities->getNamedItem("entity1").cast<Entity>() == pEntity1);
	assertTrue (pEntities->getNamedItem("entity2").cast<Entity>() == pEntity2);
	assertTrue (pNotations->getNamedItem("notation").cast<Notation>() == pNotation);
}


void DocumentTypeTest::setUp()
{
}


void DocumentTypeTest::tearDown()
{
}


CppUnit::Test* DocumentTypeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DocumentTypeTest");

	CppUnit_addTest(pSuite, DocumentTypeTest, testDocumentType);

	return pSuite;
}
