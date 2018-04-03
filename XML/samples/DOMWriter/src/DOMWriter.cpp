//
// DOMWriter.cpp
//
// This sample demonstrates the DOMWriter class and how to
// build DOM documents in memory.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/RefPtr.h"
#include <iostream>


using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::DOMWriter;
using Poco::XML::XMLWriter;
using Poco::RefPtr;


int main(int argc, char** argv)
{
	// build a DOM document and write it to standard output.

	RefPtr<Document> pDoc = new Document;
	
	RefPtr<Element> pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);

	RefPtr<Element> pChild1 = pDoc->createElement("child1");
	RefPtr<Text> pText1 = pDoc->createTextNode("text1");
	pChild1->appendChild(pText1);
	pRoot->appendChild(pChild1);

	RefPtr<Element> pChild2 = pDoc->createElement("child2");
	RefPtr<Text> pText2 = pDoc->createTextNode("text2");
	pChild2->appendChild(pText2);
	pRoot->appendChild(pChild2);
	
	DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(XMLWriter::PRETTY_PRINT);
	writer.writeNode(std::cout, pDoc);
	
	return 0;
}
