//
// XMLStreamParserTestSuite.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/XMLStreamParserTestSuite.cpp#4 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "XMLStreamParserTestSuite.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XML/XMLStreamParser.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace Poco::XML;
using namespace std;

XMLStreamParserTestSuite::XMLStreamParserTestSuite(const std::string& name)
		: CppUnit::TestCase(name)
{
}

XMLStreamParserTestSuite::~XMLStreamParserTestSuite()
{
}

void XMLStreamParserTestSuite::testParser()
{
	// Test error handling.
	//
	try
	{
		istringstream is("<root><nested>X</nasted></root>");
		XMLStreamParser p(is, "test");

		poco_assert(p.next() == XMLStreamParser::StartElement);
		poco_assert(p.next() == XMLStreamParser::StartElement);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == "X");
		p.next();
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		istringstream is("<root/>");
		is.exceptions(ios_base::badbit | ios_base::failbit);
		XMLStreamParser p(is, "test");

		is.setstate(ios_base::badbit);
		p.next();
		poco_assert(false);
	} catch (const ios_base::failure&)
	{
	}

	// Test the nextExpect() functionality.
	//
	{
		istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		p.nextExpect(XMLStreamParser::EndElement);
	}

	try
	{
		istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EndElement);
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root1");
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test nextExpect() with content setting.
	//
	{
		istringstream is("<root>  </root>");
		XMLStreamParser p(is, "empty");

		p.nextExpect(XMLStreamParser::StartElement, "root", Content::Empty);
		p.nextExpect(XMLStreamParser::EndElement);
		p.nextExpect(XMLStreamParser::Eof);
	}

	// Test namespace declarations.
	//
	{
		// Followup end element event that should be precedeeded by end
		// namespace declaration.
		//
		istringstream is("<root xmlns:a='a'/>");
		XMLStreamParser p(is, "test", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		p.nextExpect(XMLStreamParser::StartElement, "root");
		p.nextExpect(XMLStreamParser::StartNamespaceDecl);
		p.nextExpect(XMLStreamParser::EndNamespaceDecl);
		p.nextExpect(XMLStreamParser::EndElement);
	}

	// Test value extraction.
	//
	{
		istringstream is("<root>123</root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		p.nextExpect(XMLStreamParser::Characters);
		poco_assert(p.value<int>() == 123);
		p.nextExpect(XMLStreamParser::EndElement);
	}

	// Test attribute maps.
	//
	{
		istringstream is("<root a='a' b='b' d='123' t='true'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");

		poco_assert(p.attribute("a") == "a");
		poco_assert(p.attribute("b", "B") == "b");
		poco_assert(p.attribute("c", "C") == "C");
		poco_assert(p.attribute<int>("d") == 123);
		poco_assert(p.attribute<bool>("t") == true);
		poco_assert(p.attribute("f", false) == false);

		p.nextExpect(XMLStreamParser::EndElement);
	}

	{
		istringstream is("<root a='a'><nested a='A'><inner/></nested></root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		poco_assert(p.attribute("a") == "a");
		poco_assert(p.peek() == XMLStreamParser::StartElement && p.name() == "nested");
		poco_assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::StartElement, "nested");
		poco_assert(p.attribute("a") == "A");
		p.nextExpect(XMLStreamParser::StartElement, "inner");
		poco_assert(p.attribute("a", "") == "");
		p.nextExpect(XMLStreamParser::EndElement);
		poco_assert(p.attribute("a") == "A");
		poco_assert(p.peek() == XMLStreamParser::EndElement);
		poco_assert(p.attribute("a") == "A"); // Still valid.
		p.nextExpect(XMLStreamParser::EndElement);
		poco_assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::EndElement);
		poco_assert(p.attribute("a", "") == "");
	}

	try
	{
		istringstream is("<root a='a' b='b'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		poco_assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::EndElement);
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		istringstream is("<root a='abc'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		p.attribute<int>("a");
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test peeking and getting the current event.
	//
	{
		istringstream is("<root x='x'>x<nested/></root>");
		XMLStreamParser p(is, "peek", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		poco_assert(p.event() == XMLStreamParser::Eof);

		poco_assert(p.peek() == XMLStreamParser::StartElement);
		poco_assert(p.next() == XMLStreamParser::StartElement);
		poco_assert(p.event() == XMLStreamParser::StartElement);

		poco_assert(p.peek() == XMLStreamParser::StartAttribute);
		poco_assert(p.event() == XMLStreamParser::StartAttribute);
		poco_assert(p.next() == XMLStreamParser::StartAttribute);

		poco_assert(p.peek() == XMLStreamParser::Characters && p.value() == "x");
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == "x");
		poco_assert(p.event() == XMLStreamParser::Characters && p.value() == "x");

		poco_assert(p.peek() == XMLStreamParser::EndAttribute);
		poco_assert(p.event() == XMLStreamParser::EndAttribute);
		poco_assert(p.next() == XMLStreamParser::EndAttribute);

		poco_assert(p.peek() == XMLStreamParser::Characters && p.value() == "x");
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == "x");
		poco_assert(p.event() == XMLStreamParser::Characters && p.value() == "x");

		poco_assert(p.peek() == XMLStreamParser::StartElement);
		poco_assert(p.next() == XMLStreamParser::StartElement);
		poco_assert(p.event() == XMLStreamParser::StartElement);

		poco_assert(p.peek() == XMLStreamParser::EndElement);
		poco_assert(p.next() == XMLStreamParser::EndElement);
		poco_assert(p.event() == XMLStreamParser::EndElement);

		poco_assert(p.peek() == XMLStreamParser::EndElement);
		poco_assert(p.next() == XMLStreamParser::EndElement);
		poco_assert(p.event() == XMLStreamParser::EndElement);

		poco_assert(p.peek() == XMLStreamParser::Eof);
		poco_assert(p.next() == XMLStreamParser::Eof);
		poco_assert(p.event() == XMLStreamParser::Eof);
	}

	// Test content processing.
	//

	// empty
	//
	{
		istringstream is("<root x=' x '>  \n\t </root>");
		XMLStreamParser p(is, "empty", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.content(Content::Empty);
		poco_assert(p.next() == XMLStreamParser::StartAttribute);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == " x ");
		poco_assert(p.next() == XMLStreamParser::EndAttribute);
		poco_assert(p.next() == XMLStreamParser::EndElement);
		poco_assert(p.next() == XMLStreamParser::Eof);
	}

	try
	{
		istringstream is("<root>  \n &amp; X \t </root>");
		XMLStreamParser p(is, "empty");

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.content(Content::Empty);
		p.next();
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// simple
	//
	{
		istringstream is("<root> X </root>");
		XMLStreamParser p(is, "simple");

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.content(Content::Simple);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == " X ");
		poco_assert(p.next() == XMLStreamParser::EndElement);
		poco_assert(p.next() == XMLStreamParser::Eof);
	}

	try
	{
		istringstream is("<root> ? <nested/></root>");
		XMLStreamParser p(is, "simple");

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.content(Content::Simple);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == " ? ");
		p.next();
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	{
		// Test content accumulation in simple content.
		//
		istringstream is("<root xmlns:a='a'>1&#x32;3</root>");
		XMLStreamParser p(is, "simple", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.nextExpect(XMLStreamParser::StartNamespaceDecl);
		p.content(Content::Simple);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == "123");
		p.nextExpect(XMLStreamParser::EndNamespaceDecl);
		poco_assert(p.next() == XMLStreamParser::EndElement);
		poco_assert(p.next() == XMLStreamParser::Eof);
	}

	try
	{
		// Test error handling in accumulation in simple content.
		//
		istringstream is("<root xmlns:a='a'>1&#x32;<nested/>3</root>");
		XMLStreamParser p(is, "simple", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.nextExpect(XMLStreamParser::StartNamespaceDecl);
		p.content(Content::Simple);
		p.next();
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// complex
	//
	{
		istringstream is("<root x=' x '>\n"
				"  <nested>\n"
				"    <inner/>\n"
				"    <inner> X </inner>\n"
				"  </nested>\n"
				"</root>\n");
		XMLStreamParser p(is, "complex", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		poco_assert(p.next() == XMLStreamParser::StartElement); // root
		p.content(Content::Complex);

		poco_assert(p.next() == XMLStreamParser::StartAttribute);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == " x ");
		poco_assert(p.next() == XMLStreamParser::EndAttribute);

		poco_assert(p.next() == XMLStreamParser::StartElement); // nested
		p.content(Content::Complex);

		poco_assert(p.next() == XMLStreamParser::StartElement); // inner
		p.content(Content::Empty);
		poco_assert(p.next() == XMLStreamParser::EndElement);   // inner

		poco_assert(p.next() == XMLStreamParser::StartElement); // inner
		p.content(Content::Simple);
		poco_assert(p.next() == XMLStreamParser::Characters && p.value() == " X ");
		poco_assert(p.next() == XMLStreamParser::EndElement);   // inner

		poco_assert(p.next() == XMLStreamParser::EndElement);   // nested
		poco_assert(p.next() == XMLStreamParser::EndElement);   // root
		poco_assert(p.next() == XMLStreamParser::Eof);
	}

	try
	{
		istringstream is("<root> \n<n/> X <n> X </n>  </root>");
		XMLStreamParser p(is, "complex");

		poco_assert(p.next() == XMLStreamParser::StartElement);
		p.content(Content::Complex);
		poco_assert(p.next() == XMLStreamParser::StartElement);
		poco_assert(p.next() == XMLStreamParser::EndElement);
		p.next();
		poco_assert(false);
	} catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test element with simple content helpers.
	//
	{
		istringstream is("<root>"
				"  <nested>X</nested>"
				"  <nested/>"
				"  <nested>123</nested>"
				"  <nested>Y</nested>"
				"  <t:nested xmlns:t='test'>Z</t:nested>"
				"  <nested>234</nested>"
				"  <t:nested xmlns:t='test'>345</t:nested>"
				"  <nested>A</nested>"
				"  <t:nested xmlns:t='test'>B</t:nested>"
				"  <nested1>A</nested1>"
				"  <t:nested1 xmlns:t='test'>B</t:nested1>"
				"  <nested>1</nested>"
				"  <t:nested xmlns:t='test'>2</t:nested>"
				"  <nested1>1</nested1>"
				"  <t:nested1 xmlns:t='test'>2</t:nested1>"
				"</root>");
		XMLStreamParser p(is, "element");

		p.nextExpect(XMLStreamParser::StartElement, "root", Content::Complex);

		p.nextExpect(XMLStreamParser::StartElement, "nested");
		poco_assert(p.element() == "X");

		p.nextExpect(XMLStreamParser::StartElement, "nested");
		poco_assert(p.element() == "");

		p.nextExpect(XMLStreamParser::StartElement, "nested");
		poco_assert(p.element<unsigned int>() == 123);

		poco_assert(p.element("nested") == "Y");
		poco_assert(p.element(QName("test", "nested")) == "Z");

		poco_assert(p.element<unsigned int>("nested") == 234);
		poco_assert(p.element<unsigned int>(QName("test", "nested")) == 345);

		poco_assert(p.element("nested", "a") == "A");
		poco_assert(p.element(QName("test", "nested"), "b") == "B");

		poco_assert(p.element("nested", "a") == "a" && p.element("nested1") == "A");
		poco_assert(p.element(QName("test", "nested"), "b") == "b" && p.element(QName("test", "nested1")) == "B");

		poco_assert(p.element<unsigned int>("nested", 10) == 1);
		poco_assert(p.element<unsigned int>(QName("test", "nested"), 20) == 2);

		poco_assert(p.element<unsigned int>("nested", 10) == 10 && p.element<unsigned int>("nested1") == 1);
		poco_assert(p.element<unsigned int>(QName("test", "nested"), 20) == 20 && p.element<unsigned int>(QName("test", "nested1")) == 2);

		p.nextExpect(XMLStreamParser::EndElement);
	}

	// Test the iterator interface.
	//
	{
		istringstream is("<root><nested>X</nested></root>");
		XMLStreamParser p(is, "iterator");

		vector<XMLStreamParser::EventType> v;

		for (XMLStreamParser::Iterator i(p.begin()); i != p.end(); ++i)
			v.push_back(*i);

		//for (XMLStreamParser::EventType e: p)
		//  v.push_back (e);

		poco_assert(v.size() == 5);
		poco_assert(v[0] == XMLStreamParser::StartElement);
		poco_assert(v[1] == XMLStreamParser::StartElement);
		poco_assert(v[2] == XMLStreamParser::Characters);
		poco_assert(v[3] == XMLStreamParser::EndElement);
		poco_assert(v[4] == XMLStreamParser::EndElement);
	}

	// Test space extraction into the std::string value.
	//
	{
		istringstream is("<root a=' a '> b </root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::StartElement, "root");
		poco_assert(p.attribute<std::string>("a") == " a ");
		p.nextExpect(XMLStreamParser::Characters);
		poco_assert(p.value<std::string>() == " b ");
		p.nextExpect(XMLStreamParser::EndElement);
	}
}

void XMLStreamParserTestSuite::setUp()
{
}

void XMLStreamParserTestSuite::tearDown()
{
}

CppUnit::Test* XMLStreamParserTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XMLStreamParserTestSuite");

	CppUnit_addTest(pSuite, XMLStreamParserTestSuite, testParser);

	return pSuite;
}
