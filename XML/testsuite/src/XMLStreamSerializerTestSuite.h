//
// XMLStreamSerializerTestSuite.h
//
// $Id: //poco/1.4/XML/testsuite/src/XMLStreamSerializerTestSuite.h#2 $
//
// Definition of the XMLStreamSerializerTestSuite class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XMLStreamSerializerTestSuite_INCLUDED
#define XMLStreamSerializerTestSuite_INCLUDED


#include "Poco/XML/XML.h"
#include "CppUnit/TestCase.h"


class XMLStreamSerializerTestSuite: public CppUnit::TestCase
{
public:
	XMLStreamSerializerTestSuite(const std::string& name);
	~XMLStreamSerializerTestSuite();

	void testSerialize();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XMLStreamSerializerTestSuite_INCLUDED
