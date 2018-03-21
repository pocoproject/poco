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
#include "Poco/DOM/AutoPtr.h"


using Poco::XML::TreeWalker;
using Poco::XML::NodeFilter;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::Text;
using Poco::XML::Node;
using Poco::XML::AutoPtr;
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
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
	AutoPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	TreeWalker it(pRoot, NodeFilter::SHOW_ALL);

	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.nextNode().cast<Element>() == pElem1);
	assert (it.nextNode().cast<Text>() == pText1);
	assert (it.nextNode().cast<Element>() == pElem2);
	assert (it.nextNode().cast<Text>() == pText2);
	assert (it.nextNode().isNull());

	assert (it.currentNode().cast<Text>() == pText2);
	assert (it.previousNode().cast<Element>() == pElem2);
	assert (it.previousNode().cast<Text>() == pText1);
	assert (it.previousNode().cast<Element>() == pElem1);
	assert (it.previousNode().cast<Element>() == pRoot);
	assert (it.previousNode().isNull());

	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.parentNode().isNull());
	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.firstChild().cast<Element>() == pElem1);
	assert (it.parentNode().cast<Element>() == pRoot);
	assert (it.lastChild().cast<Element>() == pElem2);
	assert (it.previousSibling().cast<Element>() == pElem1);
	assert (it.previousSibling().isNull());
	assert (it.currentNode().cast<Element>() == pElem1);
	assert (it.nextSibling().cast<Element>() == pElem2);
	assert (it.nextSibling().isNull());
	assert (it.currentNode().cast<Element>() == pElem2);
	assert (it.firstChild().cast<Text>() == pText2);
	assert (it.nextSibling().isNull());
	assert (it.previousSibling().isNull());
	assert (it.parentNode().cast<Element>() == pElem2);
	assert (it.lastChild().cast<Text>() == pText2);
}


void TreeWalkerTest::testShowElements()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
	AutoPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	TreeWalker it(pRoot, NodeFilter::SHOW_ELEMENT);

	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.nextNode().cast<Element>() == pElem1);
	assert (it.nextNode().cast<Element>() == pElem2);
	assert (it.nextNode().isNull());

	assert (it.currentNode().cast<Element>() == pElem2);
	assert (it.previousNode().cast<Element>() == pElem1);
	assert (it.previousNode().cast<Element>() == pRoot);
	assert (it.previousNode().isNull());

	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.parentNode().isNull());
	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.firstChild().cast<Element>() == pElem1);
	assert (it.parentNode().cast<Element>() == pRoot);
	assert (it.lastChild().cast<Element>() == pElem2);
	assert (it.firstChild().isNull());
	assert (it.currentNode().cast<Element>() == pElem2);
	assert (it.lastChild().isNull());
	assert (it.currentNode().cast<Element>() == pElem2);
	assert (it.previousSibling().cast<Element>() == pElem1);
	assert (it.firstChild().isNull());
	assert (it.lastChild().isNull());
	assert (it.parentNode().cast<Element>() == pRoot);
}


void TreeWalkerTest::testFilter()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
	AutoPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	SkipNodeFilter::Ptr skipFilter = new SkipNodeFilter;
	TreeWalker it1(pRoot, NodeFilter::SHOW_ELEMENT, skipFilter);

	assert (it1.nextNode().cast<Element>() == pElem1);
	assert (it1.nextNode().isNull());

	assert (it1.currentNode().cast<Element>() == pElem1);
	assert (it1.previousNode().isNull());

	assert (it1.parentNode().isNull());
	assert (it1.firstChild().isNull());
	assert (it1.lastChild().isNull());
	assert (it1.nextSibling().isNull());
	assert (it1.previousSibling().isNull());

	TreeWalker it2(pRoot, NodeFilter::SHOW_ALL, skipFilter);

	assert (it2.nextNode().cast<Element>() == pElem1);
	assert (it2.nextNode().cast<Text>() == pText1);
	assert (it2.nextNode().cast<Text>() == pText2);
	assert (it2.nextNode().isNull());

	assert (it2.currentNode().cast<Text>() == pText2);
	assert (it2.previousNode().cast<Text>() == pText1);
	assert (it2.previousNode().cast<Element>() == pElem1);
	assert (it2.previousNode().isNull());

	assert (it2.currentNode().cast<Element>() == pElem1);
	assert (it2.parentNode().isNull());
	assert (it2.nextSibling().isNull());
	assert (it2.previousSibling().isNull());
	assert (it2.firstChild().cast<Text>() == pText1);
	assert (it2.nextSibling().isNull());
	assert (it2.previousSibling().isNull());
	assert (it2.parentNode().cast<Element>() == pElem1);

	RejectNodeFilter::Ptr rejectFilter = new RejectNodeFilter;
	TreeWalker it3(pRoot, NodeFilter::SHOW_ELEMENT, rejectFilter);

	assert (it3.nextNode().cast<Element>() == pElem1);
	assert (it3.nextNode().isNull());

	assert (it3.currentNode().cast<Element>() == pElem1);
	assert (it3.previousNode().cast<Element>() == pRoot);
	assert (it3.previousNode().isNull());

	assert (it3.currentNode().cast<Element>() == pRoot);
	assert (it3.parentNode().isNull());
	assert (it3.firstChild().cast<Element>() == pElem1);
	assert (it3.nextSibling().isNull());
	assert (it3.previousSibling().isNull());
	assert (it3.parentNode().cast<Element>() == pRoot);
	assert (it3.lastChild().cast<Element>() == pElem1);

	TreeWalker it4(pRoot, NodeFilter::SHOW_ALL, rejectFilter);

	assert (it4.nextNode().cast<Element>() == pElem1);
	assert (it4.nextNode().cast<Text>() == pText1);
	assert (it4.nextNode().isNull());

	assert (it4.currentNode().cast<Text>() == pText1);
	assert (it4.previousNode().cast<Element>() == pElem1);
	assert (it4.previousNode().cast<Element>() == pRoot);
	assert (it4.previousNode().isNull());

	assert (it4.currentNode().cast<Element>() == pRoot);
	assert (it4.parentNode().isNull());
	assert (it4.firstChild().cast<Element>() == pElem1);
	assert (it4.firstChild().cast<Text>() == pText1);
	assert (it4.nextSibling().isNull());
	assert (it4.previousSibling().isNull());
	assert (it4.parentNode().cast<Element>() == pElem1);
	assert (it4.lastChild().cast<Text>() == pText1);
	assert (it4.parentNode().cast<Element>() == pElem1);
	assert (it4.nextSibling().isNull());
	assert (it4.previousSibling().isNull());
	assert (it4.parentNode().cast<Element>() == pRoot);
}


void TreeWalkerTest::testShowNothing()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pRoot = pDoc->createElement("root");
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
	AutoPtr<Text> pText2 = pDoc->createTextNode("text2");

	pElem1->appendChild(pText1);
	pElem2->appendChild(pText2);
	pRoot->appendChild(pElem1);
	pRoot->appendChild(pElem2);
	pDoc->appendChild(pRoot);

	TreeWalker it(pRoot, 0);

	assert (it.nextNode().isNull());

	assert (it.previousNode().isNull());

	assert (it.currentNode().cast<Element>() == pRoot);
	assert (it.firstChild().isNull());
	assert (it.lastChild().isNull());
	assert (it.nextSibling().isNull());
	assert (it.previousSibling().isNull());
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
