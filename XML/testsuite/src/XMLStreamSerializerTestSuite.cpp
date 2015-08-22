//
// XMLStreamSerializerTestSuite.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/XMLStreamSerializerTestSuite.cpp#4 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "XMLStreamSerializerTestSuite.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XML/XMLStreamSerializer.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace Poco::XML;
using namespace std;

XMLStreamSerializerTestSuite::XMLStreamSerializerTestSuite(const std::string& name)
	: CppUnit::TestCase(name)
{
}

XMLStreamSerializerTestSuite::~XMLStreamSerializerTestSuite()
{
}

void XMLStreamSerializerTestSuite::testSerialize()
{
	  try
	  {
	    ostringstream os;
	    XMLStreamSerializer s (os, "test");

	    s.attribute ("foo", "bar");
	    assert (false);
	  }
	  catch (const Poco::Exception&)
	  {
	    // cerr << e.what () << endl;
	  }

	  try
	  {
	    ostringstream os;
	    os.exceptions (ios_base::badbit | ios_base::failbit);
	    XMLStreamSerializer s (os, "test");

	    s.startElement ("root");
	    s.characters ("one");
	    os.setstate (ios_base::badbit);
	    s.characters ("two");
	    assert (false);
	  }
	  catch (const ios_base::failure&)
	  {
	  }

	  // Test value serialization.
	  //
	  {
	    ostringstream os;
	    XMLStreamSerializer s (os, "test", 0);

	    s.startElement ("root");
	    s.attribute ("version", 123);
	    s.characters (true);
	    s.endElement ();

	    assert (os.str () == "<root version=\"123\">true</root>\n");
	  }

	  // Test helpers for serializing elements with simple content.
	  //
	  {
	    ostringstream os;
	    XMLStreamSerializer s (os, "element", 0);

	    s.startElement ("root");

	    s.startElement ("nested");
	    s.element ("X");

	    s.startElement ("nested");
	    s.element (123);

	    s.element ("nested", "X");
	    s.element ("nested", 123);
	    s.element ("test", "nested", "X");
	    s.element ("test", "nested", 123);
	    s.element (QName ("test", "nested"), "X");
	    s.element (QName ("test", "nested"), 123);

	    s.endElement (); // root

	    assert (os.str () ==
	            "<root>"
	            "<nested>X</nested>"
	            "<nested>123</nested>"
	            "<nested>X</nested>"
	            "<nested>123</nested>"
	            "<g1:nested xmlns:g1=\"test\">X</g1:nested>"
	            "<g1:nested xmlns:g1=\"test\">123</g1:nested>"
	            "<g1:nested xmlns:g1=\"test\">X</g1:nested>"
	            "<g1:nested xmlns:g1=\"test\">123</g1:nested>"
	            "</root>\n");
	  }
}

void XMLStreamSerializerTestSuite::setUp()
{
}

void XMLStreamSerializerTestSuite::tearDown()
{
}

CppUnit::Test* XMLStreamSerializerTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XMLStreamSerializerTestSuite");

	CppUnit_addTest(pSuite, XMLStreamSerializerTestSuite, testSerialize);

	return pSuite;
}
