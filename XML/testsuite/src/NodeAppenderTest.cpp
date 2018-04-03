//
// NodeAppenderTest.cpp
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NodeAppenderTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DOM/NodeAppender.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/RefPtr.h"


using Poco::XML::NodeAppender;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::DocumentFragment;
using Poco::XML::XMLString;
using Poco::RefPtr;


NodeAppenderTest::NodeAppenderTest(const std::string& name): CppUnit::TestCase(name)
{
}


NodeAppenderTest::~NodeAppenderTest()
{
}


void NodeAppenderTest::testAppendNode()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element>  pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);

	NodeAppender appender(pRoot);

	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Element> pElem3 = pDoc->createElement("elem");

	appender.appendChild(pElem1);
	appender.appendChild(pElem2);
	appender.appendChild(pElem3);

	assertTrue (pRoot->firstChild().cast<Element>() == pElem1);
	assertTrue (pRoot->lastChild().cast<Element>() == pElem3);

	assertTrue (pElem1->nextSibling().cast<Element>() == pElem2);
	assertTrue (pElem2->nextSibling().cast<Element>() == pElem3);
	assertTrue (pElem3->nextSibling().isNull());

	assertTrue (pElem1->previousSibling().isNull());
	assertTrue (pElem2->previousSibling().cast<Element>() == pElem1);
	assertTrue (pElem3->previousSibling().cast<Element>() == pElem2);

	assertTrue (pElem1->parentNode().cast<Element>() == pRoot);
	assertTrue (pElem2->parentNode().cast<Element>() == pRoot);
	assertTrue (pElem3->parentNode().cast<Element>() == pRoot);
}


void NodeAppenderTest::testAppendNodeList()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element>  pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);

	NodeAppender appender(pRoot);

	RefPtr<DocumentFragment> pFrag1 = pDoc->createDocumentFragment();
	RefPtr<DocumentFragment> pFrag2 = pDoc->createDocumentFragment();
	RefPtr<DocumentFragment> pFrag3 = pDoc->createDocumentFragment();

	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Element> pElem3 = pDoc->createElement("elem");
	RefPtr<Element> pElem4 = pDoc->createElement("elem");

	pFrag2->appendChild(pElem1);
	pFrag2->appendChild(pElem2);
	pFrag2->appendChild(pElem3);

	pFrag3->appendChild(pElem4);

	appender.appendChild(pFrag1);
	assertTrue (pRoot->firstChild().isNull());

	appender.appendChild(pFrag2);
	assertTrue (pRoot->firstChild().cast<Element>() == pElem1);
	assertTrue (pRoot->lastChild().cast<Element>() == pElem3);

	assertTrue (pElem1->nextSibling().cast<Element>() == pElem2);
	assertTrue (pElem2->nextSibling().cast<Element>() == pElem3);
	assertTrue (pElem3->nextSibling().isNull());

	assertTrue (pElem1->previousSibling().isNull());
	assertTrue (pElem2->previousSibling().cast<Element>() == pElem1);
	assertTrue (pElem3->previousSibling().cast<Element>() == pElem2);

	assertTrue (pElem1->parentNode().cast<Element>() == pRoot);
	assertTrue (pElem2->parentNode().cast<Element>() == pRoot);
	assertTrue (pElem3->parentNode().cast<Element>() == pRoot);

	appender.appendChild(pFrag3);
	assertTrue (pRoot->lastChild().cast<Element>() == pElem4);
	assertTrue (pElem4->parentNode().cast<Element>() == pRoot);
	assertTrue (pElem3->nextSibling().cast<Element>() == pElem4);
	assertTrue (pElem4->previousSibling().cast<Element>() == pElem3);
}


void NodeAppenderTest::setUp()
{
}


void NodeAppenderTest::tearDown()
{
}


CppUnit::Test* NodeAppenderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NodeAppenderTest");

	CppUnit_addTest(pSuite, NodeAppenderTest, testAppendNode);
	CppUnit_addTest(pSuite, NodeAppenderTest, testAppendNodeList);

	return pSuite;
}
