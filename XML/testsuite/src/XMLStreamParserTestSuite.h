//
// XMLStreamParserTestSuite.h
//
// $Id: //poco/1.4/XML/testsuite/src/XMLStreamParserTestSuite.h#2 $
//
// Definition of the XMLStreamParserTestSuite class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XMLStreamParserTestSuite_INCLUDED
#define XMLStreamParserTestSuite_INCLUDED


#include "Poco/XML/XML.h"
#include "CppUnit/TestCase.h"


class XMLStreamParserTestSuite: public CppUnit::TestCase
{
public:
	XMLStreamParserTestSuite(const std::string& name);
	~XMLStreamParserTestSuite();

	void testParser();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XMLStreamParserTestSuite_INCLUDED
