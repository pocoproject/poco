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
#include "Poco/RefPtr.h"


using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::DocumentFragment;
using Poco::XML::Node;
using Poco::RefPtr;


NodeTest::NodeTest(const std::string& name): CppUnit::TestCase(name)
{
}


NodeTest::~NodeTest()
{
}


void NodeTest::testInsert()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");

	assertTrue (!pRoot->hasChildNodes());
	assertTrue (pRoot->firstChild().isNull());
	assertTrue (pRoot->lastChild().isNull());

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->insertBefore(pChild1, 0);

	assertTrue (pRoot->hasChildNodes());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild1);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->insertBefore(pChild3, 0);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().isNull());

	RefPtr<Element> pChild0 = pDoc->createElement("child0");
	pRoot->insertBefore(pChild0, pChild1);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().isNull());

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->insertBefore(pChild2, pChild3);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());
}


void NodeTest::testAppend()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild1);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild2);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());
}


void NodeTest::testRemove()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	pRoot->removeChild(pChild2);
	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().isNull());

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	pRoot->removeChild(pChild4);
	assertTrue (pChild4->previousSibling().isNull());
	assertTrue (pChild4->nextSibling().isNull());

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().isNull());

	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	pRoot->removeChild(pChild1);
	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());
	
	assertTrue (pRoot->firstChild().cast<Element>() == pChild3);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);
	
	pRoot->removeChild(pChild3);
	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().isNull());
	assertTrue (pRoot->firstChild().isNull());
	assertTrue (pRoot->lastChild().isNull());
}


void NodeTest::testReplace()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	RefPtr<Element> pChild11 = pDoc->createElement("child11");
	pRoot->replaceChild(pChild11, pChild1);
	
	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild22 = pDoc->createElement("child22");
	pRoot->replaceChild(pChild22, pChild2);

	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild33 = pDoc->createElement("child33");
	pRoot->replaceChild(pChild33, pChild3);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild44 = pDoc->createElement("child44");
	pRoot->replaceChild(pChild44, pChild4);

	assertTrue (pChild4->previousSibling().isNull());
	assertTrue (pChild4->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild44);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild44);
	assertTrue (pChild44->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild44->nextSibling().isNull());
}


void NodeTest::testInsertFragment1()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assertTrue (!pRoot->hasChildNodes());
	assertTrue (pRoot->firstChild().isNull());
	assertTrue (pRoot->lastChild().isNull());

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild1);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild1);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild3);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().isNull());

	RefPtr<Element> pChild0 = pDoc->createElement("child0");
	pFrag->appendChild(pChild0);
	pRoot->insertBefore(pFrag, pChild1);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild3->nextSibling().isNull());
	
	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild2);
	pRoot->insertBefore(pFrag, pChild3);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());
}


void NodeTest::testInsertFragment2()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assertTrue (!pRoot->hasChildNodes());
	assertTrue (pRoot->firstChild().isNull());
	assertTrue (pRoot->lastChild().isNull());

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild2);
	pFrag->appendChild(pChild3);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild2);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);
	
	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());
	
	RefPtr<Element> pChild6 = pDoc->createElement("child6");
	RefPtr<Element> pChild7 = pDoc->createElement("child7");
	pFrag->appendChild(pChild6);
	pFrag->appendChild(pChild7);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild2);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild7);

	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild6->nextSibling().cast<Element>() == pChild7);
	assertTrue (pChild7->previousSibling().cast<Element>() == pChild6);
	assertTrue (pChild7->nextSibling().isNull());

	RefPtr<Element> pChild0 = pDoc->createElement("child0");
	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild0);
	pFrag->appendChild(pChild1);
	pRoot->insertBefore(pFrag, pChild2);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild7);
	
	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild6->nextSibling().cast<Element>() == pChild7);
	assertTrue (pChild7->previousSibling().cast<Element>() == pChild6);
	assertTrue (pChild7->nextSibling().isNull());
	
	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	RefPtr<Element> pChild5 = pDoc->createElement("child5");
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pRoot->insertBefore(pFrag, pChild6);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild7);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild6->nextSibling().cast<Element>() == pChild7);
	assertTrue (pChild7->previousSibling().cast<Element>() == pChild6);
	assertTrue (pChild7->nextSibling().isNull());
}


void NodeTest::testInsertFragment3()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	assertTrue (!pRoot->hasChildNodes());
	assertTrue (pRoot->firstChild().isNull());
	assertTrue (pRoot->lastChild().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	RefPtr<Element> pChild5 = pDoc->createElement("child5");
	pFrag->appendChild(pChild3);
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild3);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild5);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>().isNull());

	RefPtr<Element> pChild9  = pDoc->createElement("child9");
	RefPtr<Element> pChild10 = pDoc->createElement("child10");
	RefPtr<Element> pChild11 = pDoc->createElement("child11");
	pFrag->appendChild(pChild9);
	pFrag->appendChild(pChild10);
	pFrag->appendChild(pChild11);
	pRoot->insertBefore(pFrag, 0);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild3);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild11);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild9);
	assertTrue (pChild9->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild9->nextSibling().cast<Element>() == pChild10);
	assertTrue (pChild10->previousSibling().cast<Element>() == pChild9);
	assertTrue (pChild10->nextSibling().cast<Element>() == pChild11);
	assertTrue (pChild11->previousSibling().cast<Element>() == pChild10);
	assertTrue (pChild11->nextSibling().isNull());

	RefPtr<Element> pChild0 = pDoc->createElement("child0");
	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild0);
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pRoot->insertBefore(pFrag, pChild3);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild11);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild9);
	assertTrue (pChild9->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild9->nextSibling().cast<Element>() == pChild10);
	assertTrue (pChild10->previousSibling().cast<Element>() == pChild9);
	assertTrue (pChild10->nextSibling().cast<Element>() == pChild11);
	assertTrue (pChild11->previousSibling().cast<Element>() == pChild10);
	assertTrue (pChild11->nextSibling().isNull());

	RefPtr<Element> pChild6 = pDoc->createElement("child6");
	RefPtr<Element> pChild7 = pDoc->createElement("child7");
	RefPtr<Element> pChild8 = pDoc->createElement("child8");
	pFrag->appendChild(pChild6);
	pFrag->appendChild(pChild7);
	pFrag->appendChild(pChild8);
	pRoot->insertBefore(pFrag, pChild9);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild0);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild11);

	assertTrue (pChild0->previousSibling().isNull());
	assertTrue (pChild0->nextSibling().cast<Element>() == pChild1);
	assertTrue (pChild1->previousSibling().cast<Element>() == pChild0);
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild6->nextSibling().cast<Element>() == pChild7);
	assertTrue (pChild7->previousSibling().cast<Element>() == pChild6);
	assertTrue (pChild7->nextSibling().cast<Element>() == pChild8);
	assertTrue (pChild8->previousSibling().cast<Element>() == pChild7);
	assertTrue (pChild8->nextSibling().cast<Element>() == pChild9);
	assertTrue (pChild9->previousSibling().cast<Element>() == pChild8);
	assertTrue (pChild9->nextSibling().cast<Element>() == pChild10);
	assertTrue (pChild10->previousSibling().cast<Element>() == pChild9);
	assertTrue (pChild10->nextSibling().cast<Element>() == pChild11);
	assertTrue (pChild11->previousSibling().cast<Element>() == pChild10);
	assertTrue (pChild11->nextSibling().isNull());
}


void NodeTest::testAppendFragment1()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pFrag->appendChild(pChild1);
	pRoot->appendChild(pFrag);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild1);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild2);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild2);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild3);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());
}


void NodeTest::testAppendFragment2()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pRoot->appendChild(pFrag);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild2);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().isNull());

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pFrag->appendChild(pChild3);
	pFrag->appendChild(pChild4);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild5 = pDoc->createElement("child5");
	RefPtr<Element> pChild6 = pDoc->createElement("child6");
	pFrag->appendChild(pChild5);
	pFrag->appendChild(pChild6);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild6);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild6->nextSibling().isNull());
}


void NodeTest::testAppendFragment3()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pFrag->appendChild(pChild1);
	pFrag->appendChild(pChild2);
	pFrag->appendChild(pChild3);
	pRoot->appendChild(pFrag);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild3);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().isNull());

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	RefPtr<Element> pChild5 = pDoc->createElement("child5");
	RefPtr<Element> pChild6 = pDoc->createElement("child6");
	pFrag->appendChild(pChild4);
	pFrag->appendChild(pChild5);
	pFrag->appendChild(pChild6);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild6);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild6->nextSibling().isNull());

	RefPtr<Element> pChild7 = pDoc->createElement("child7");
	RefPtr<Element> pChild8 = pDoc->createElement("child8");
	RefPtr<Element> pChild9 = pDoc->createElement("child9");
	pFrag->appendChild(pChild7);
	pFrag->appendChild(pChild8);
	pFrag->appendChild(pChild9);
	pRoot->appendChild(pFrag);
	assertTrue (pRoot->firstChild().cast<Element>() == pChild1);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild9);

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild1);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().cast<Element>() == pChild5);
	assertTrue (pChild5->previousSibling().cast<Element>() == pChild4);
	assertTrue (pChild5->nextSibling().cast<Element>() == pChild6);
	assertTrue (pChild6->previousSibling().cast<Element>() == pChild5);
	assertTrue (pChild6->nextSibling().cast<Element>() == pChild7);
	assertTrue (pChild7->previousSibling().cast<Element>() == pChild6);
	assertTrue (pChild7->nextSibling().cast<Element>() == pChild8);
	assertTrue (pChild8->previousSibling().cast<Element>() == pChild7);
	assertTrue (pChild8->nextSibling().cast<Element>() == pChild9);
	assertTrue (pChild9->previousSibling().cast<Element>() == pChild8);
	assertTrue (pChild9->nextSibling().isNull());
}


void NodeTest::testReplaceFragment1()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	RefPtr<Element> pChild11 = pDoc->createElement("child11");
	pFrag->appendChild(pChild11);
	pRoot->replaceChild(pFrag, pChild1);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild22 = pDoc->createElement("child22");
	pFrag->appendChild(pChild22);
	pRoot->replaceChild(pFrag, pChild2);

	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild33 = pDoc->createElement("child33");
	pFrag->appendChild(pChild33);
	pRoot->replaceChild(pFrag, pChild3);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild44 = pDoc->createElement("child44");
	pFrag->appendChild(pChild44);
	pRoot->replaceChild(pFrag, pChild4);

	assertTrue (pChild4->previousSibling().isNull());
	assertTrue (pChild4->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild44);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild44);
	assertTrue (pChild44->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild44->nextSibling().isNull());
}


void NodeTest::testReplaceFragment2()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	RefPtr<Element> pChild11 = pDoc->createElement("child11");
	RefPtr<Element> pChild12 = pDoc->createElement("child12");
	pFrag->appendChild(pChild11);
	pFrag->appendChild(pChild12);
	pRoot->replaceChild(pFrag, pChild1);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild21 = pDoc->createElement("child21");
	RefPtr<Element> pChild22 = pDoc->createElement("child22");
	pFrag->appendChild(pChild21);
	pFrag->appendChild(pChild22);
	pRoot->replaceChild(pFrag, pChild2);

	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild31 = pDoc->createElement("child31");
	RefPtr<Element> pChild32 = pDoc->createElement("child32");
	pFrag->appendChild(pChild31);
	pFrag->appendChild(pChild32);
	pRoot->replaceChild(pFrag, pChild3);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild31);
	assertTrue (pChild31->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild31->nextSibling().cast<Element>() == pChild32);
	assertTrue (pChild32->previousSibling().cast<Element>() == pChild31);
	assertTrue (pChild32->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild32);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild41 = pDoc->createElement("child41");
	RefPtr<Element> pChild42 = pDoc->createElement("child42");
	pFrag->appendChild(pChild41);
	pFrag->appendChild(pChild42);
	pRoot->replaceChild(pFrag, pChild4);

	assertTrue (pChild4->previousSibling().isNull());
	assertTrue (pChild4->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild42);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild31);
	assertTrue (pChild31->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild31->nextSibling().cast<Element>() == pChild32);
	assertTrue (pChild32->previousSibling().cast<Element>() == pChild31);
	assertTrue (pChild32->nextSibling().cast<Element>() == pChild41);
	assertTrue (pChild41->previousSibling().cast<Element>() == pChild32);
	assertTrue (pChild41->nextSibling().cast<Element>() == pChild42);
	assertTrue (pChild42->previousSibling().cast<Element>() == pChild41);
	assertTrue (pChild42->nextSibling().isNull());
}


void NodeTest::testReplaceFragment3()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<DocumentFragment> pFrag = pDoc->createDocumentFragment();

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	pRoot->appendChild(pChild2);

	RefPtr<Element> pChild3 = pDoc->createElement("child3");
	pRoot->appendChild(pChild3);

	RefPtr<Element> pChild4 = pDoc->createElement("child4");
	pRoot->appendChild(pChild4);

	RefPtr<Element> pChild11 = pDoc->createElement("child11");
	RefPtr<Element> pChild12 = pDoc->createElement("child12");
	RefPtr<Element> pChild13 = pDoc->createElement("child13");
	pFrag->appendChild(pChild11);
	pFrag->appendChild(pChild12);
	pFrag->appendChild(pChild13);
	pRoot->replaceChild(pFrag, pChild1);
	assertTrue (pFrag->firstChild().isNull());
	assertTrue (pFrag->lastChild().isNull());

	assertTrue (pChild1->previousSibling().isNull());
	assertTrue (pChild1->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild13);
	assertTrue (pChild13->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild13->nextSibling().cast<Element>() == pChild2);
	assertTrue (pChild2->previousSibling().cast<Element>() == pChild13);
	assertTrue (pChild2->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild2);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild21 = pDoc->createElement("child21");
	RefPtr<Element> pChild22 = pDoc->createElement("child22");
	RefPtr<Element> pChild23 = pDoc->createElement("child23");
	pFrag->appendChild(pChild21);
	pFrag->appendChild(pChild22);
	pFrag->appendChild(pChild23);
	pRoot->replaceChild(pFrag, pChild2);

	assertTrue (pChild2->previousSibling().isNull());
	assertTrue (pChild2->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild13);
	assertTrue (pChild13->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild13->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild13);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild23);
	assertTrue (pChild23->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild23->nextSibling().cast<Element>() == pChild3);
	assertTrue (pChild3->previousSibling().cast<Element>() == pChild23);
	assertTrue (pChild3->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild3);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild31 = pDoc->createElement("child31");
	RefPtr<Element> pChild32 = pDoc->createElement("child32");
	RefPtr<Element> pChild33 = pDoc->createElement("child33");
	pFrag->appendChild(pChild31);
	pFrag->appendChild(pChild32);
	pFrag->appendChild(pChild33);
	pRoot->replaceChild(pFrag, pChild3);

	assertTrue (pChild3->previousSibling().isNull());
	assertTrue (pChild3->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild4);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild13);
	assertTrue (pChild13->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild13->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild13);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild23);
	assertTrue (pChild23->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild23->nextSibling().cast<Element>() == pChild31);
	assertTrue (pChild31->previousSibling().cast<Element>() == pChild23);
	assertTrue (pChild31->nextSibling().cast<Element>() == pChild32);
	assertTrue (pChild32->previousSibling().cast<Element>() == pChild31);
	assertTrue (pChild32->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild32);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild4);
	assertTrue (pChild4->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild4->nextSibling().isNull());

	RefPtr<Element> pChild41 = pDoc->createElement("child41");
	RefPtr<Element> pChild42 = pDoc->createElement("child42");
	RefPtr<Element> pChild43 = pDoc->createElement("child43");
	pFrag->appendChild(pChild41);
	pFrag->appendChild(pChild42);
	pFrag->appendChild(pChild43);
	pRoot->replaceChild(pFrag, pChild4);

	assertTrue (pChild4->previousSibling().isNull());
	assertTrue (pChild4->nextSibling().isNull());
	assertTrue (pRoot->firstChild().cast<Element>() == pChild11);
	assertTrue (pRoot->lastChild().cast<Element>() == pChild43);
	assertTrue (pChild11->previousSibling().isNull());
	assertTrue (pChild11->nextSibling().cast<Element>() == pChild12);
	assertTrue (pChild12->previousSibling().cast<Element>() == pChild11);
	assertTrue (pChild12->nextSibling().cast<Element>() == pChild13);
	assertTrue (pChild13->previousSibling().cast<Element>() == pChild12);
	assertTrue (pChild13->nextSibling().cast<Element>() == pChild21);
	assertTrue (pChild21->previousSibling().cast<Element>() == pChild13);
	assertTrue (pChild21->nextSibling().cast<Element>() == pChild22);
	assertTrue (pChild22->previousSibling().cast<Element>() == pChild21);
	assertTrue (pChild22->nextSibling().cast<Element>() == pChild23);
	assertTrue (pChild23->previousSibling().cast<Element>() == pChild22);
	assertTrue (pChild23->nextSibling().cast<Element>() == pChild31);
	assertTrue (pChild31->previousSibling().cast<Element>() == pChild23);
	assertTrue (pChild31->nextSibling().cast<Element>() == pChild32);
	assertTrue (pChild32->previousSibling().cast<Element>() == pChild31);
	assertTrue (pChild32->nextSibling().cast<Element>() == pChild33);
	assertTrue (pChild33->previousSibling().cast<Element>() == pChild32);
	assertTrue (pChild33->nextSibling().cast<Element>() == pChild41);
	assertTrue (pChild41->previousSibling().cast<Element>() == pChild33);
	assertTrue (pChild41->nextSibling().cast<Element>() == pChild42);
	assertTrue (pChild42->previousSibling().cast<Element>() == pChild41);
	assertTrue (pChild42->nextSibling().cast<Element>() == pChild43);
	assertTrue (pChild43->previousSibling().cast<Element>() == pChild42);
	assertTrue (pChild43->nextSibling().isNull());
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
