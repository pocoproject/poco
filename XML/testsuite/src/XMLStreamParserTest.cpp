//
// XMLStreamParserTest.cpp
//
// $Id$
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "XMLStreamParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XML/XMLStreamParser.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>


using namespace Poco::XML;


XMLStreamParserTest::XMLStreamParserTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


XMLStreamParserTest::~XMLStreamParserTest()
{
}


void XMLStreamParserTest::testParser()
{
	// Test error handling.
	//
	try
	{
		std::istringstream is("<root><nested>X</nasted></root>");
		XMLStreamParser p(is, "test");

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == "X");
		p.next();
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		std::istringstream is("<root/>");
		is.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		XMLStreamParser p(is, "test");

		is.setstate(std::ios_base::badbit);
		p.next();
		assert(false);
	} 
	catch (const std::ios_base::failure&)
	{
	}

	// Test the nextExpect() functionality.
	//
	{
		std::istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}

	try
	{
		std::istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		std::istringstream is("<root/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root1");
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test nextExpect() with content setting.
	//
	{
		std::istringstream is("<root>  </root>");
		XMLStreamParser p(is, "empty");

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root", Content::Empty);
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		p.nextExpect(XMLStreamParser::EV_EOF);
	}

	// Test namespace declarations.
	//
	{
		// Followup end element event that should be precedeeded by end
		// namespace declaration.
		//
		std::istringstream is("<root xmlns:a='a'/>");
		XMLStreamParser p(is, "test", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		p.nextExpect(XMLStreamParser::EV_START_NAMESPACE_DECL);
		p.nextExpect(XMLStreamParser::EV_END_NAMESPACE_DECL);
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}

	// Test value extraction.
	//
	{
		std::istringstream is("<root>123</root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		p.nextExpect(XMLStreamParser::EV_CHARACTERS);
		assert(p.value<int>() == 123);
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}

	// Test attribute maps.
	//
	{
		std::istringstream is("<root a='a' b='b' d='123' t='true'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");

		assert(p.attribute("a") == "a");
		assert(p.attribute("b", "B") == "b");
		assert(p.attribute("c", "C") == "C");
		assert(p.attribute<int>("d") == 123);
		assert(p.attribute<bool>("t") == true);
		assert(p.attribute("f", false) == false);

		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}

	{
		std::istringstream is("<root a='a'><nested a='A'><inner/></nested></root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		assert(p.attribute("a") == "a");
		assert(p.peek() == XMLStreamParser::EV_START_ELEMENT && p.localName() == "nested");
		assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "nested");
		assert(p.attribute("a") == "A");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "inner");
		assert(p.attribute("a", "") == "");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		assert(p.attribute("a") == "A");
		assert(p.peek() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.attribute("a") == "A"); // Still valid.
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		assert(p.attribute("a", "") == "");
	}

	try
	{
		std::istringstream is("<root a='a' b='b'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		assert(p.attribute("a") == "a");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	try
	{
		std::istringstream is("<root a='abc'/>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		p.attribute<int>("a");
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test peeking and getting the current event.
	//
	{
		std::istringstream is("<root x='x'>x<nested/></root>");
		XMLStreamParser p(is, "peek", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		assert(p.event() == XMLStreamParser::EV_EOF);

		assert(p.peek() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.event() == XMLStreamParser::EV_START_ELEMENT);

		assert(p.peek() == XMLStreamParser::EV_START_ATTRIBUTE);
		assert(p.event() == XMLStreamParser::EV_START_ATTRIBUTE);
		assert(p.next() == XMLStreamParser::EV_START_ATTRIBUTE);

		assert(p.peek() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");
		assert(p.event() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");

		assert(p.peek() == XMLStreamParser::EV_END_ATTRIBUTE);
		assert(p.event() == XMLStreamParser::EV_END_ATTRIBUTE);
		assert(p.next() == XMLStreamParser::EV_END_ATTRIBUTE);

		assert(p.peek() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");
		assert(p.event() == XMLStreamParser::EV_CHARACTERS && p.value() == "x");

		assert(p.peek() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.event() == XMLStreamParser::EV_START_ELEMENT);

		assert(p.peek() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.event() == XMLStreamParser::EV_END_ELEMENT);

		assert(p.peek() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.event() == XMLStreamParser::EV_END_ELEMENT);

		assert(p.peek() == XMLStreamParser::EV_EOF);
		assert(p.next() == XMLStreamParser::EV_EOF);
		assert(p.event() == XMLStreamParser::EV_EOF);
	}

	// Test content processing.
	//

	// empty
	//
	{
		std::istringstream is("<root x=' x '>  \n\t </root>");
		XMLStreamParser p(is, "empty", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.content(Content::Empty);
		assert(p.next() == XMLStreamParser::EV_START_ATTRIBUTE);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == " x ");
		assert(p.next() == XMLStreamParser::EV_END_ATTRIBUTE);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_EOF);
	}

	try
	{
		std::istringstream is("<root>  \n &amp; X \t </root>");
		XMLStreamParser p(is, "empty");

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.content(Content::Empty);
		p.next();
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// simple
	//
	{
		std::istringstream is("<root> X </root>");
		XMLStreamParser p(is, "simple");

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.content(Content::Simple);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == " X ");
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_EOF);
	}

	try
	{
		std::istringstream is("<root> ? <nested/></root>");
		XMLStreamParser p(is, "simple");

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.content(Content::Simple);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == " ? ");
		p.next();
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	{
		// Test content accumulation in simple content.
		//
		std::istringstream is("<root xmlns:a='a'>1&#x32;3</root>");
		XMLStreamParser p(is, "simple", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.nextExpect(XMLStreamParser::EV_START_NAMESPACE_DECL);
		p.content(Content::Simple);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == "123");
		p.nextExpect(XMLStreamParser::EV_END_NAMESPACE_DECL);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_EOF);
	}

	try
	{
		// Test error handling in accumulation in simple content.
		//
		std::istringstream is("<root xmlns:a='a'>1&#x32;<nested/>3</root>");
		XMLStreamParser p(is, "simple", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.nextExpect(XMLStreamParser::EV_START_NAMESPACE_DECL);
		p.content(Content::Simple);
		p.next();
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// complex
	//
	{
		std::istringstream is("<root x=' x '>\n"
				"  <nested>\n"
				"    <inner/>\n"
				"    <inner> X </inner>\n"
				"  </nested>\n"
				"</root>\n");
		XMLStreamParser p(is, "complex", XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT); // root
		p.content(Content::Complex);

		assert(p.next() == XMLStreamParser::EV_START_ATTRIBUTE);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == " x ");
		assert(p.next() == XMLStreamParser::EV_END_ATTRIBUTE);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT); // nested
		p.content(Content::Complex);

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT); // inner
		p.content(Content::Empty);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);   // inner

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT); // inner
		p.content(Content::Simple);
		assert(p.next() == XMLStreamParser::EV_CHARACTERS && p.value() == " X ");
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);   // inner

		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);   // nested
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);   // root
		assert(p.next() == XMLStreamParser::EV_EOF);
	}

	try
	{
		std::istringstream is("<root> \n<n/> X <n> X </n>  </root>");
		XMLStreamParser p(is, "complex");

		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		p.content(Content::Complex);
		assert(p.next() == XMLStreamParser::EV_START_ELEMENT);
		assert(p.next() == XMLStreamParser::EV_END_ELEMENT);
		p.next();
		assert(false);
	} 
	catch (const Poco::Exception&)
	{
		// cerr << e.what () << endl;
	}

	// Test element with simple content helpers.
	//
	{
		std::istringstream is("<root>"
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

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root", Content::Complex);

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "nested");
		assert(p.element() == "X");

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "nested");
		assert(p.element() == "");

		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "nested");
		assert(p.element<unsigned int>() == 123);

		assert(p.element("nested") == "Y");
		assert(p.element(QName("test", "nested")) == "Z");

		assert(p.element<unsigned int>("nested") == 234);
		assert(p.element<unsigned int>(QName("test", "nested")) == 345);

		assert(p.element("nested", "a") == "A");
		assert(p.element(QName("test", "nested"), "b") == "B");

		assert(p.element("nested", "a") == "a" && p.element("nested1") == "A");
		assert(p.element(QName("test", "nested"), "b") == "b" && p.element(QName("test", "nested1")) == "B");

		assert(p.element<unsigned int>("nested", 10) == 1);
		assert(p.element<unsigned int>(QName("test", "nested"), 20) == 2);

		assert(p.element<unsigned int>("nested", 10) == 10 && p.element<unsigned int>("nested1") == 1);
		assert(p.element<unsigned int>(QName("test", "nested"), 20) == 20 && p.element<unsigned int>(QName("test", "nested1")) == 2);

		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}

	// Test the iterator interface.
	//
	{
		std::istringstream is("<root><nested>X</nested></root>");
		XMLStreamParser p(is, "iterator");

		std::vector<XMLStreamParser::EventType> v;

		for (XMLStreamParser::Iterator i(p.begin()); i != p.end(); ++i)
			v.push_back(*i);

		//for (XMLStreamParser::EventType e: p)
		//  v.push_back (e);

		assert(v.size() == 5);
		assert(v[0] == XMLStreamParser::EV_START_ELEMENT);
		assert(v[1] == XMLStreamParser::EV_START_ELEMENT);
		assert(v[2] == XMLStreamParser::EV_CHARACTERS);
		assert(v[3] == XMLStreamParser::EV_END_ELEMENT);
		assert(v[4] == XMLStreamParser::EV_END_ELEMENT);
	}

	// Test space extraction into the std::string value.
	//
	{
		std::istringstream is("<root a=' a '> b </root>");
		XMLStreamParser p(is, "test");
		p.nextExpect(XMLStreamParser::EV_START_ELEMENT, "root");
		assert(p.attribute<std::string>("a") == " a ");
		p.nextExpect(XMLStreamParser::EV_CHARACTERS);
		assert(p.value<std::string>() == " b ");
		p.nextExpect(XMLStreamParser::EV_END_ELEMENT);
	}
}


void XMLStreamParserTest::setUp()
{
}


void XMLStreamParserTest::tearDown()
{
}


CppUnit::Test* XMLStreamParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XMLStreamParserTest");

	CppUnit_addTest(pSuite, XMLStreamParserTest, testParser);

	return pSuite;
}
