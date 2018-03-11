//
// NodeTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NodeTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/AutoPtr.h"


using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::DocumentFragment;
using Poco::XML::Node;
using Poco::XML::AutoPtr;


NodeTest::NodeTest(const std::string& name): CppUnit::TestCase(name)
{
}


NodeTest::~NodeTest()
{
}


void NodeTest::testInsert()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");

	assert (!pRoot->hasChildNodes());
	assert (pRoot->firstChild().isNull());
	assert (pRoot->lastChild().isNull());

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->insertBefore(pChild1, 0);
	assert (pRoot->hasChildNodes());
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild1);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->insertBefore(pChild3, 0);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().isNull());

	AutoPtr<Element> pChild0 = pDoc->createElement("child0");
	pRoot->insertBefore(pChild0, pChild1);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().isNull());

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->insertBefore(pChild2, pChild3);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());
}


void NodeTest::testAppend()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild1);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild2);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());
}


void NodeTest::testRemove()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	pRoot->removeChild(pChild2);
	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().isNull());

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	pRoot->removeChild(pChild4);
	assert (pChild4->previousSibling().isNull());
	assert (pChild4->nextSibling().isNull());

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().isNull());

	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	pRoot->removeChild(pChild1);
	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());
	
	assert (pRoot->firstChild().cast<Element>() == pChild3);
	assert (pRoot->lastChild().cast<Element>() == pChild3);
	
	pRoot->removeChild(pChild3);
	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().isNull());
	assert (pRoot->firstChild().isNull());
	assert (pRoot->lastChild().isNull());
}


void NodeTest::testReplace()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	AutoPtr<Element> pChild11 = pDoc->createElement("child11");
	pRoot->replaceChild(pChild11, pChild1);
	
	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild11);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild22 = pDoc->createElement("child22");
	pRoot->replaceChild(pChild22, pChild2);

	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild22);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild33 = pDoc->createElement("child33");
	pRoot->replaceChild(pChild33, pChild3);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild22);
	assert (pChild33->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild33);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild44 = pDoc->createElement("child44");
	pRoot->replaceChild(pChild44, pChild4);

	assert (pChild4->previousSibling().isNull());
	assert (pChild4->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild44);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild22);
	assert (pChild33->nextSibling().cast<Element>() == pChild44);
	assert (pChild44->previousSibling().cast<Element>() == pChild33);
	assert (pChild44->nextSibling().isNull());
}


void NodeTest::testInsertFragment1()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assert (!pRoot->hasChildNodes());
	assert (pRoot->firstChild().isNull());
	assert (pRoot->lastChild().isNull());

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild1);
	pRoot->insertBefore(pFrag, 0);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild1);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild3);
	pRoot->insertBefore(pFrag, 0);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().isNull());

	AutoPtr<Element> pChild0 = pDoc->createElement("child0");
	pFrag->appendChild(pChild0);
	pRoot->insertBefore(pFrag, pChild1);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild1);
	assert (pChild3->nextSibling().isNull());
	
	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild2);
	pRoot->insertBefore(pFrag, pChild3);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());
}


void NodeTest::testInsertFragment2()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assert (!pRoot->hasChildNodes());
	assert (pRoot->firstChild().isNull());
	assert (pRoot->lastChild().isNull());

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild2);
	pFrag->appendChild(pChild3);
	pRoot->insertBefore(pFrag, 0);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild2);
	assert (pRoot->lastChild().cast<Element>() == pChild3);
	
	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());
	
	AutoPtr<Element> pChild6 = pDoc->createElement("child6");
	AutoPtr<Element> pChild7 = pDoc->createElement("child7");
	pFrag->appendChild(pChild6);
	pFrag->appendChild(pChild7);
	pRoot->insertBefore(pFrag, 0);
	assert (pRoot->firstChild().cast<Element>() == pChild2);
	assert (pRoot->lastChild().cast<Element>() == pChild7);

	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild3);
	assert (pChild6->nextSibling().cast<Element>() == pChild7);
	assert (pChild7->previousSibling().cast<Element>() == pChild6);
	assert (pChild7->nextSibling().isNull());

	AutoPtr<Element> pChild0 = pDoc->createElement("child0");
	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild0);
	pFrag->appendChild(pChild1);
	pRoot->insertBefore(pFrag, pChild2);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild7);
	
	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild3);
	assert (pChild6->nextSibling().cast<Element>() == pChild7);
	assert (pChild7->previousSibling().cast<Element>() == pChild6);
	assert (pChild7->nextSibling().isNull());
	
	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	AutoPtr<Element> pChild5 = pDoc->createElement("child5");
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pRoot->insertBefore(pFrag, pChild6);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild7);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild5);
	assert (pChild6->nextSibling().cast<Element>() == pChild7);
	assert (pChild7->previousSibling().cast<Element>() == pChild6);
	assert (pChild7->nextSibling().isNull());
}


void NodeTest::testInsertFragment3()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assert (!pRoot->hasChildNodes());
	assert (pRoot->firstChild().isNull());
	assert (pRoot->lastChild().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	AutoPtr<Element> pChild5 = pDoc->createElement("child5");
	pFrag->appendChild(pChild3);
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pRoot->insertBefore(pFrag, 0);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild3);
	assert (pRoot->lastChild().cast<Element>() == pChild5);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>().isNull());

	AutoPtr<Element> pChild9  = pDoc->createElement("child9");
	AutoPtr<Element> pChild10 = pDoc->createElement("child10");
	AutoPtr<Element> pChild11 = pDoc->createElement("child11");
	pFrag->appendChild(pChild9);
	pFrag->appendChild(pChild10);
	pFrag->appendChild(pChild11);
	pRoot->insertBefore(pFrag, 0);
	assert (pRoot->firstChild().cast<Element>() == pChild3);
	assert (pRoot->lastChild().cast<Element>() == pChild11);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild9);
	assert (pChild9->previousSibling().cast<Element>() == pChild5);
	assert (pChild9->nextSibling().cast<Element>() == pChild10);
	assert (pChild10->previousSibling().cast<Element>() == pChild9);
	assert (pChild10->nextSibling().cast<Element>() == pChild11);
	assert (pChild11->previousSibling().cast<Element>() == pChild10);
	assert (pChild11->nextSibling().isNull());

	AutoPtr<Element> pChild0 = pDoc->createElement("child0");
	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild0);
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pRoot->insertBefore(pFrag, pChild3);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild11);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild9);
	assert (pChild9->previousSibling().cast<Element>() == pChild5);
	assert (pChild9->nextSibling().cast<Element>() == pChild10);
	assert (pChild10->previousSibling().cast<Element>() == pChild9);
	assert (pChild10->nextSibling().cast<Element>() == pChild11);
	assert (pChild11->previousSibling().cast<Element>() == pChild10);
	assert (pChild11->nextSibling().isNull());

	AutoPtr<Element> pChild6 = pDoc->createElement("child6");
	AutoPtr<Element> pChild7 = pDoc->createElement("child7");
	AutoPtr<Element> pChild8 = pDoc->createElement("child8");
	pFrag->appendChild(pChild6);
	pFrag->appendChild(pChild7);
	pFrag->appendChild(pChild8);
	pRoot->insertBefore(pFrag, pChild9);
	assert (pRoot->firstChild().cast<Element>() == pChild0);
	assert (pRoot->lastChild().cast<Element>() == pChild11);

	assert (pChild0->previousSibling().isNull());
	assert (pChild0->nextSibling().cast<Element>() == pChild1);
	assert (pChild1->previousSibling().cast<Element>() == pChild0);
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild5);
	assert (pChild6->nextSibling().cast<Element>() == pChild7);
	assert (pChild7->previousSibling().cast<Element>() == pChild6);
	assert (pChild7->nextSibling().cast<Element>() == pChild8);
	assert (pChild8->previousSibling().cast<Element>() == pChild7);
	assert (pChild8->nextSibling().cast<Element>() == pChild9);
	assert (pChild9->previousSibling().cast<Element>() == pChild8);
	assert (pChild9->nextSibling().cast<Element>() == pChild10);
	assert (pChild10->previousSibling().cast<Element>() == pChild9);
	assert (pChild10->nextSibling().cast<Element>() == pChild11);
	assert (pChild11->previousSibling().cast<Element>() == pChild10);
	assert (pChild11->nextSibling().isNull());
}


void NodeTest::testAppendFragment1()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild1);
	pRoot->appendChild(pFrag);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild1);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild2);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild2);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild3);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());
}


void NodeTest::testAppendFragment2()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pRoot->appendChild(pFrag);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild2);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().isNull());

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pFrag->appendChild(pChild3);
	pFrag->appendChild(pChild4);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild4);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild5 = pDoc->createElement("child5");
	AutoPtr<Element> pChild6 = pDoc->createElement("child6");
	pFrag->appendChild(pChild5);
	pFrag->appendChild(pChild6);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild6);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild5);
	assert (pChild6->nextSibling().isNull());
}


void NodeTest::testAppendFragment3()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pFrag->appendChild(pChild3);
	pRoot->appendChild(pFrag);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild3);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().isNull());

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	AutoPtr<Element> pChild5 = pDoc->createElement("child5");
	AutoPtr<Element> pChild6 = pDoc->createElement("child6");
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pFrag->appendChild(pChild6);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild6);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild5);
	assert (pChild6->nextSibling().isNull());

	AutoPtr<Element> pChild7 = pDoc->createElement("child7");
	AutoPtr<Element> pChild8 = pDoc->createElement("child8");
	AutoPtr<Element> pChild9 = pDoc->createElement("child9");
	pFrag->appendChild(pChild7);
	pFrag->appendChild(pChild8);
	pFrag->appendChild(pChild9);
	pRoot->appendChild(pFrag);
	assert (pRoot->firstChild().cast<Element>() == pChild1);
	assert (pRoot->lastChild().cast<Element>() == pChild9);

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild1);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().cast<Element>() == pChild5);
	assert (pChild5->previousSibling().cast<Element>() == pChild4);
	assert (pChild5->nextSibling().cast<Element>() == pChild6);
	assert (pChild6->previousSibling().cast<Element>() == pChild5);
	assert (pChild6->nextSibling().cast<Element>() == pChild7);
	assert (pChild7->previousSibling().cast<Element>() == pChild6);
	assert (pChild7->nextSibling().cast<Element>() == pChild8);
	assert (pChild8->previousSibling().cast<Element>() == pChild7);
	assert (pChild8->nextSibling().cast<Element>() == pChild9);
	assert (pChild9->previousSibling().cast<Element>() == pChild8);
	assert (pChild9->nextSibling().isNull());
}


void NodeTest::testReplaceFragment1()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	AutoPtr<Element> pChild11 = pDoc->createElement("child11");
	pFrag->appendChild(pChild11);
	pRoot->replaceChild(pFrag, pChild1);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild11);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild22 = pDoc->createElement("child22");
	pFrag->appendChild(pChild22);
	pRoot->replaceChild(pFrag, pChild2);

	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild22);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild33 = pDoc->createElement("child33");
	pFrag->appendChild(pChild33);
	pRoot->replaceChild(pFrag, pChild3);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild22);
	assert (pChild33->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild33);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild44 = pDoc->createElement("child44");
	pFrag->appendChild(pChild44);
	pRoot->replaceChild(pFrag, pChild4);

	assert (pChild4->previousSibling().isNull());
	assert (pChild4->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild44);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild11);
	assert (pChild22->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild22);
	assert (pChild33->nextSibling().cast<Element>() == pChild44);
	assert (pChild44->previousSibling().cast<Element>() == pChild33);
	assert (pChild44->nextSibling().isNull());
}


void NodeTest::testReplaceFragment2()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();	

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	AutoPtr<Element> pChild11 = pDoc->createElement("child11");
	AutoPtr<Element> pChild12 = pDoc->createElement("child12");
	pFrag->appendChild(pChild11);
	pFrag->appendChild(pChild12);
	pRoot->replaceChild(pFrag, pChild1);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild12);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild21 = pDoc->createElement("child21");
	AutoPtr<Element> pChild22 = pDoc->createElement("child22");
	pFrag->appendChild(pChild21);
	pFrag->appendChild(pChild22);
	pRoot->replaceChild(pFrag, pChild2);

	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild12);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild22);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild31 = pDoc->createElement("child31");
	AutoPtr<Element> pChild32 = pDoc->createElement("child32");
	pFrag->appendChild(pChild31);
	pFrag->appendChild(pChild32);
	pRoot->replaceChild(pFrag, pChild3);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild12);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild31);
	assert (pChild31->previousSibling().cast<Element>() == pChild22);
	assert (pChild31->nextSibling().cast<Element>() == pChild32);
	assert (pChild32->previousSibling().cast<Element>() == pChild31);
	assert (pChild32->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild32);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild41 = pDoc->createElement("child41");
	AutoPtr<Element> pChild42 = pDoc->createElement("child42");
	pFrag->appendChild(pChild41);
	pFrag->appendChild(pChild42);
	pRoot->replaceChild(pFrag, pChild4);

	assert (pChild4->previousSibling().isNull());
	assert (pChild4->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild42);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild12);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild31);
	assert (pChild31->previousSibling().cast<Element>() == pChild22);
	assert (pChild31->nextSibling().cast<Element>() == pChild32);
	assert (pChild32->previousSibling().cast<Element>() == pChild31);
	assert (pChild32->nextSibling().cast<Element>() == pChild41);
	assert (pChild41->previousSibling().cast<Element>() == pChild32);
	assert (pChild41->nextSibling().cast<Element>() == pChild42);
	assert (pChild42->previousSibling().cast<Element>() == pChild41);
	assert (pChild42->nextSibling().isNull());
}


void NodeTest::testReplaceFragment3()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	AutoPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	AutoPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	AutoPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	AutoPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	AutoPtr<Element> pChild11 = pDoc->createElement("child11");
	AutoPtr<Element> pChild12 = pDoc->createElement("child12");
	AutoPtr<Element> pChild13 = pDoc->createElement("child13");
	pFrag->appendChild(pChild11);
	pFrag->appendChild(pChild12);
	pFrag->appendChild(pChild13);
	pRoot->replaceChild(pFrag, pChild1);
	assert (pFrag->firstChild().isNull());
	assert (pFrag->lastChild().isNull());

	assert (pChild1->previousSibling().isNull());
	assert (pChild1->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild13);
	assert (pChild13->previousSibling().cast<Element>() == pChild12);
	assert (pChild13->nextSibling().cast<Element>() == pChild2);
	assert (pChild2->previousSibling().cast<Element>() == pChild13);
	assert (pChild2->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild2);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild21 = pDoc->createElement("child21");
	AutoPtr<Element> pChild22 = pDoc->createElement("child22");
	AutoPtr<Element> pChild23 = pDoc->createElement("child23");
	pFrag->appendChild(pChild21);
	pFrag->appendChild(pChild22);
	pFrag->appendChild(pChild23);
	pRoot->replaceChild(pFrag, pChild2);

	assert (pChild2->previousSibling().isNull());
	assert (pChild2->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild13);
	assert (pChild13->previousSibling().cast<Element>() == pChild12);
	assert (pChild13->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild13);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild23);
	assert (pChild23->previousSibling().cast<Element>() == pChild22);
	assert (pChild23->nextSibling().cast<Element>() == pChild3);
	assert (pChild3->previousSibling().cast<Element>() == pChild23);
	assert (pChild3->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild3);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild31 = pDoc->createElement("child31");
	AutoPtr<Element> pChild32 = pDoc->createElement("child32");
	AutoPtr<Element> pChild33 = pDoc->createElement("child33");
	pFrag->appendChild(pChild31);
	pFrag->appendChild(pChild32);
	pFrag->appendChild(pChild33);
	pRoot->replaceChild(pFrag, pChild3);

	assert (pChild3->previousSibling().isNull());
	assert (pChild3->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild4);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild13);
	assert (pChild13->previousSibling().cast<Element>() == pChild12);
	assert (pChild13->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild13);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild23);
	assert (pChild23->previousSibling().cast<Element>() == pChild22);
	assert (pChild23->nextSibling().cast<Element>() == pChild31);
	assert (pChild31->previousSibling().cast<Element>() == pChild23);
	assert (pChild31->nextSibling().cast<Element>() == pChild32);
	assert (pChild32->previousSibling().cast<Element>() == pChild31);
	assert (pChild32->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild32);
	assert (pChild33->nextSibling().cast<Element>() == pChild4);
	assert (pChild4->previousSibling().cast<Element>() == pChild33);
	assert (pChild4->nextSibling().isNull());

	AutoPtr<Element> pChild41 = pDoc->createElement("child41");
	AutoPtr<Element> pChild42 = pDoc->createElement("child42");
	AutoPtr<Element> pChild43 = pDoc->createElement("child43");
	pFrag->appendChild(pChild41);
	pFrag->appendChild(pChild42);
	pFrag->appendChild(pChild43);
	pRoot->replaceChild(pFrag, pChild4);

	assert (pChild4->previousSibling().isNull());
	assert (pChild4->nextSibling().isNull());
	assert (pRoot->firstChild().cast<Element>() == pChild11);
	assert (pRoot->lastChild().cast<Element>() == pChild43);
	assert (pChild11->previousSibling().isNull());
	assert (pChild11->nextSibling().cast<Element>() == pChild12);
	assert (pChild12->previousSibling().cast<Element>() == pChild11);
	assert (pChild12->nextSibling().cast<Element>() == pChild13);
	assert (pChild13->previousSibling().cast<Element>() == pChild12);
	assert (pChild13->nextSibling().cast<Element>() == pChild21);
	assert (pChild21->previousSibling().cast<Element>() == pChild13);
	assert (pChild21->nextSibling().cast<Element>() == pChild22);
	assert (pChild22->previousSibling().cast<Element>() == pChild21);
	assert (pChild22->nextSibling().cast<Element>() == pChild23);
	assert (pChild23->previousSibling().cast<Element>() == pChild22);
	assert (pChild23->nextSibling().cast<Element>() == pChild31);
	assert (pChild31->previousSibling().cast<Element>() == pChild23);
	assert (pChild31->nextSibling().cast<Element>() == pChild32);
	assert (pChild32->previousSibling().cast<Element>() == pChild31);
	assert (pChild32->nextSibling().cast<Element>() == pChild33);
	assert (pChild33->previousSibling().cast<Element>() == pChild32);
	assert (pChild33->nextSibling().cast<Element>() == pChild41);
	assert (pChild41->previousSibling().cast<Element>() == pChild33);
	assert (pChild41->nextSibling().cast<Element>() == pChild42);
	assert (pChild42->previousSibling().cast<Element>() == pChild41);
	assert (pChild42->nextSibling().cast<Element>() == pChild43);
	assert (pChild43->previousSibling().cast<Element>() == pChild42);
	assert (pChild43->nextSibling().isNull());
}


void NodeTest::setUp()
{
}


void NodeTest::tearDown()
{
}


CppUnit::Test* NodeTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NodeTest");

	CppUnit_addTest(pSuite, NodeTest, testInsert);
	CppUnit_addTest(pSuite, NodeTest, testAppend);
	CppUnit_addTest(pSuite, NodeTest, testRemove);
	CppUnit_addTest(pSuite, NodeTest, testReplace);
	CppUnit_addTest(pSuite, NodeTest, testInsertFragment1);
	CppUnit_addTest(pSuite, NodeTest, testInsertFragment2);
	CppUnit_addTest(pSuite, NodeTest, testInsertFragment3);
	CppUnit_addTest(pSuite, NodeTest, testAppendFragment1);
	CppUnit_addTest(pSuite, NodeTest, testAppendFragment2);
	CppUnit_addTest(pSuite, NodeTest, testAppendFragment3);
	CppUnit_addTest(pSuite, NodeTest, testReplaceFragment1);
	CppUnit_addTest(pSuite, NodeTest, testReplaceFragment2);
	CppUnit_addTest(pSuite, NodeTest, testReplaceFragment3);

	return pSuite;
}
