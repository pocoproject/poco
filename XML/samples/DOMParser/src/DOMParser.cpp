//
// DOMParser.cpp
//
// This sample demonstrates the DOMParser, RefPtr and
// NodeIterator classes.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/RefPtr.h"
#include "Poco/Exception.h"
#include <iostream>


using Poco::XML::DOMParser;
using Poco::XML::InputSource;
using Poco::XML::Document;
using Poco::XML::NodeIterator;
using Poco::XML::NodeFilter;
using Poco::XML::Node;
using Poco::RefPtr;
using Poco::Exception;


int main(int argc, char** argv)
{
	// Parse an XML document from standard input
	// and use a NodeIterator to print out all nodes.

	InputSource::Ptr src = new InputSource(std::cin);
	try
	{
		DOMParser parser;
		RefPtr<Document> pDoc = parser.parse(src);
		
		NodeIterator it(pDoc, NodeFilter::SHOW_ALL);
		RefPtr<Node> pNode = it.nextNode();
		while (pNode)
		{
			std::cout << pNode->nodeName() << ":" << pNode->nodeValue() << std::endl;
			pNode = it.nextNode();
		}
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
	}

	return 0;
}
