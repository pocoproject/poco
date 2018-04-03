//
// TreeWalkerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TreeWalkerTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DOM/TreeWalker.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/RefPtr.h"


using Poco::XML::TreeWalker;
using Poco::XML::NodeFilter;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::Text;
using Poco::XML::Node;
using Poco::RefPtr;
using Poco::XML::XMLString;


namespace
{
	class RejectNodeFilter: public NodeFilter
	{
		short acceptNode(Node::Ptr node) const
		{
			if (node->nodeType() != Node::ELEMENT_NODE || node->innerText() == "text1" || node->nodeName() == "root")
				return NodeFilter::FILTER_ACCEPT;
			else
				return NodeFilter::FILTER_REJECT;
		}

		~RejectNodeFilter() {}
	};

	class SkipNodeFilter: public NodeFilter
	{
		short acceptNode(Node::Ptr node) const
		{
			if (node->nodeType() != Node::ELEMENT_NODE || node->innerText() == "text1")
				return NodeFilter::FILTER_ACCEPT;
			else
				return NodeFilter::FILTER_SKIP;
		}

		~SkipNodeFilter() {}
	};
}


TreeWalkerTest::TreeWalkerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TreeWalkerTest::~TreeWalkerTest()
{
}


void TreeWalkerTest::testShowAll()
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

	TreeWalker it(pRoot, NodeFilter::SHOW_ALL);

	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.nextNode().cast<Element>() == pElem1);
	assertTrue (it.nextNode().cast<Text>() == pText1);
	assertTrue (it.nextNode().cast<Element>() == pElem2);
	assertTrue (it.nextNode().cast<Text>() == pText2);
	assertTrue (it.nextNode().isNull());

	assertTrue (it.currentNode().cast<Text>() == pText2);
	assertTrue (it.previousNode().cast<Element>() == pElem2);
	assertTrue (it.previousNode().cast<Text>() == pText1);
	assertTrue (it.previousNode().cast<Element>() == pElem1);
	assertTrue (it.previousNode().cast<Element>() == pRoot);
	assertTrue (it.previousNode().isNull());

	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.parentNode().isNull());
	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.firstChild().cast<Element>() == pElem1);
	assertTrue (it.parentNode().cast<Element>() == pRoot);
	assertTrue (it.lastChild().cast<Element>() == pElem2);
	assertTrue (it.previousSibling().cast<Element>() == pElem1);
	assertTrue (it.previousSibling().isNull());
	assertTrue (it.currentNode().cast<Element>() == pElem1);
	assertTrue (it.nextSibling().cast<Element>() == pElem2);
	assertTrue (it.nextSibling().isNull());
	assertTrue (it.currentNode().cast<Element>() == pElem2);
	assertTrue (it.firstChild().cast<Text>() == pText2);
	assertTrue (it.nextSibling().isNull());
	assertTrue (it.previousSibling().isNull());
	assertTrue (it.parentNode().cast<Element>() == pElem2);
	assertTrue (it.lastChild().cast<Text>() == pText2);
}


void TreeWalkerTest::testShowElements()
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

	TreeWalker it(pRoot, NodeFilter::SHOW_ELEMENT);

	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.nextNode().cast<Element>() == pElem1);
	assertTrue (it.nextNode().cast<Element>() == pElem2);
	assertTrue (it.nextNode().isNull());

	assertTrue (it.currentNode().cast<Element>() == pElem2);
	assertTrue (it.previousNode().cast<Element>() == pElem1);
	assertTrue (it.previousNode().cast<Element>() == pRoot);
	assertTrue (it.previousNode().isNull());

	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.parentNode().isNull());
	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.firstChild().cast<Element>() == pElem1);
	assertTrue (it.parentNode().cast<Element>() == pRoot);
	assertTrue (it.lastChild().cast<Element>() == pElem2);
	assertTrue (it.firstChild().isNull());
	assertTrue (it.currentNode().cast<Element>() == pElem2);
	assertTrue (it.lastChild().isNull());
	assertTrue (it.currentNode().cast<Element>() == pElem2);
	assertTrue (it.previousSibling().cast<Element>() == pElem1);
	assertTrue (it.firstChild().isNull());
	assertTrue (it.lastChild().isNull());
	assertTrue (it.parentNode().cast<Element>() == pRoot);
}


void TreeWalkerTest::testFilter()
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

	SkipNodeFilter::Ptr skipFilter = new SkipNodeFilter;
	TreeWalker it1(pRoot, NodeFilter::SHOW_ELEMENT, skipFilter);

	assertTrue (it1.nextNode().cast<Element>() == pElem1);
	assertTrue (it1.nextNode().isNull());

	assertTrue (it1.currentNode().cast<Element>() == pElem1);
	assertTrue (it1.previousNode().isNull());

	assertTrue (it1.parentNode().isNull());
	assertTrue (it1.firstChild().isNull());
	assertTrue (it1.lastChild().isNull());
	assertTrue (it1.nextSibling().isNull());
	assertTrue (it1.previousSibling().isNull());

	TreeWalker it2(pRoot, NodeFilter::SHOW_ALL, skipFilter);

	assertTrue (it2.nextNode().cast<Element>() == pElem1);
	assertTrue (it2.nextNode().cast<Text>() == pText1);
	assertTrue (it2.nextNode().cast<Text>() == pText2);
	assertTrue (it2.nextNode().isNull());

	assertTrue (it2.currentNode().cast<Text>() == pText2);
	assertTrue (it2.previousNode().cast<Text>() == pText1);
	assertTrue (it2.previousNode().cast<Element>() == pElem1);
	assertTrue (it2.previousNode().isNull());

	assertTrue (it2.currentNode().cast<Element>() == pElem1);
	assertTrue (it2.parentNode().isNull());
	assertTrue (it2.nextSibling().isNull());
	assertTrue (it2.previousSibling().isNull());
	assertTrue (it2.firstChild().cast<Text>() == pText1);
	assertTrue (it2.nextSibling().isNull());
	assertTrue (it2.previousSibling().isNull());
	assertTrue (it2.parentNode().cast<Element>() == pElem1);

	RejectNodeFilter::Ptr rejectFilter = new RejectNodeFilter;
	TreeWalker it3(pRoot, NodeFilter::SHOW_ELEMENT, rejectFilter);

	assertTrue (it3.nextNode().cast<Element>() == pElem1);
	assertTrue (it3.nextNode().isNull());

	assertTrue (it3.currentNode().cast<Element>() == pElem1);
	assertTrue (it3.previousNode().cast<Element>() == pRoot);
	assertTrue (it3.previousNode().isNull());

	assertTrue (it3.currentNode().cast<Element>() == pRoot);
	assertTrue (it3.parentNode().isNull());
	assertTrue (it3.firstChild().cast<Element>() == pElem1);
	assertTrue (it3.nextSibling().isNull());
	assertTrue (it3.previousSibling().isNull());
	assertTrue (it3.parentNode().cast<Element>() == pRoot);
	assertTrue (it3.lastChild().cast<Element>() == pElem1);

	TreeWalker it4(pRoot, NodeFilter::SHOW_ALL, rejectFilter);

	assertTrue (it4.nextNode().cast<Element>() == pElem1);
	assertTrue (it4.nextNode().cast<Text>() == pText1);
	assertTrue (it4.nextNode().isNull());

	assertTrue (it4.currentNode().cast<Text>() == pText1);
	assertTrue (it4.previousNode().cast<Element>() == pElem1);
	assertTrue (it4.previousNode().cast<Element>() == pRoot);
	assertTrue (it4.previousNode().isNull());

	assertTrue (it4.currentNode().cast<Element>() == pRoot);
	assertTrue (it4.parentNode().isNull());
	assertTrue (it4.firstChild().cast<Element>() == pElem1);
	assertTrue (it4.firstChild().cast<Text>() == pText1);
	assertTrue (it4.nextSibling().isNull());
	assertTrue (it4.previousSibling().isNull());
	assertTrue (it4.parentNode().cast<Element>() == pElem1);
	assertTrue (it4.lastChild().cast<Text>() == pText1);
	assertTrue (it4.parentNode().cast<Element>() == pElem1);
	assertTrue (it4.nextSibling().isNull());
	assertTrue (it4.previousSibling().isNull());
	assertTrue (it4.parentNode().cast<Element>() == pRoot);
}


void TreeWalkerTest::testShowNothing()
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

	TreeWalker it(pRoot, 0);

	assertTrue (it.nextNode().isNull());

	assertTrue (it.previousNode().isNull());

	assertTrue (it.currentNode().cast<Element>() == pRoot);
	assertTrue (it.firstChild().isNull());
	assertTrue (it.lastChild().isNull());
	assertTrue (it.nextSibling().isNull());
	assertTrue (it.previousSibling().isNull());
}


void TreeWalkerTest::setUp()
{
}


void TreeWalkerTest::tearDown()
{
}


CppUnit::Test* TreeWalkerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TreeWalkerTest");

	CppUnit_addTest(pSuite, TreeWalkerTest, testShowAll);
	CppUnit_addTest(pSuite, TreeWalkerTest, testShowElements);
	CppUnit_addTest(pSuite, TreeWalkerTest, testFilter);
	CppUnit_addTest(pSuite, TreeWalkerTest, testShowNothing);

	return pSuite;
}
