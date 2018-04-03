//
// NodeIteratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NodeIteratorTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/RefPtr.h"


using Poco::XML::NodeIterator;
using Poco::XML::NodeFilter;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::Text;
using Poco::XML::Node;
using Poco::XML::XMLString;
using Poco::RefPtr;


namespace
{
	class TestNodeFilter: public NodeFilter
	{
		short acceptNode(Node::Ptr node) const
		{
			if (node->innerText() == "text1")
				return NodeFilter::FILTER_ACCEPT;
			else
				return NodeFilter::FILTER_REJECT;
		}
	};
}


NodeIteratorTest::NodeIteratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


NodeIteratorTest::~NodeIteratorTest()
{
}


void NodeIteratorTest::testShowAll()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Text> pText1 = pDoc->createTextNode("text1");
	RefPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	NodeIterator it(pRoot, NodeFilter::SHOW_ALL);

	assertTrue (it.nextNode().cast<Element>() == pRoot);
	assertTrue (it.nextNode().cast<Element>() == pElem1);
	assertTrue (it.nextNode().cast<Text>() == pText1);
	assertTrue (it.nextNode().cast<Element>() == pElem2);
	assertTrue (it.nextNode().cast<Text>() == pText2);
	assertTrue (it.nextNode().isNull());

	assertTrue (it.previousNode().cast<Text>() == pText2);
	assertTrue (it.previousNode().cast<Element>() == pElem2);
	assertTrue (it.previousNode().cast<Text>() == pText1);
	assertTrue (it.previousNode().cast<Element>() == pElem1);
	assertTrue (it.previousNode().cast<Element>() == pRoot);
	assertTrue (it.previousNode().isNull());
}


void NodeIteratorTest::testShowElements()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Text> pText1 = pDoc->createTextNode("text1");
	RefPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);
	
	NodeIterator it(pRoot, NodeFilter::SHOW_ELEMENT);

	assertTrue (it.nextNode().cast<Element>() == pRoot);
	assertTrue (it.nextNode().cast<Element>() == pElem1);
	assertTrue (it.nextNode().cast<Element>() == pElem2);
	assertTrue (it.nextNode().isNull());

	assertTrue (it.previousNode().cast<Element>() == pElem2);
	assertTrue (it.previousNode().cast<Element>() == pElem1);
	assertTrue (it.previousNode().cast<Element>() == pRoot);
	assertTrue (it.previousNode().isNull());
}


void NodeIteratorTest::testFilter()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Text> pText1 = pDoc->createTextNode("text1");
	RefPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	TestNodeFilter::Ptr filter = new TestNodeFilter;
	NodeIterator it(pRoot, NodeFilter::SHOW_ELEMENT, filter);

	assertTrue (it.nextNode().cast<Element>() == pElem1);
	assertTrue (it.nextNode().isNull());

	assertTrue (it.previousNode().cast<Element>() == pElem1);
	assertTrue (it.previousNode().isNull());
}


void NodeIteratorTest::testShowNothing()
{
	RefPtr<Document> pDoc = new Document;
	RefPtr<Element> pRoot = pDoc->createElement("root");
	RefPtr<Element> pElem1 = pDoc->createElement("elem");
	RefPtr<Element> pElem2 = pDoc->createElement("elem");
	RefPtr<Text> pText1 = pDoc->createTextNode("text1");
	RefPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	NodeIterator it(pRoot, 0);
	assertTrue (it.nextNode().isNull());
	assertTrue (it.previousNode().isNull());
}


void NodeIteratorTest::setUp()
{
}


void NodeIteratorTest::tearDown()
{
}


CppUnit::Test* NodeIteratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NodeIteratorTest");

	CppUnit_addTest(pSuite, NodeIteratorTest, testShowAll);
	CppUnit_addTest(pSuite, NodeIteratorTest, testShowElements);
	CppUnit_addTest(pSuite, NodeIteratorTest, testFilter);
	CppUnit_addTest(pSuite, NodeIteratorTest, testShowNothing);

	return pSuite;
}
