//
// XSDParserTest.h
//
// Definition of the XSDParserTest class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDParserTest_INCLUDED
#define XSDParserTest_INCLUDED


#include "CppUnit/TestCase.h"


class XSDParserTest: public CppUnit::TestCase
{
public:
	XSDParserTest(const std::string& name);
	~XSDParserTest();

	void testAnnotation();
	void testAnnotation2();
	void testAttribute();
	void testAttribute2();
	void testAttribute3();
	void testElement();
	void testElementInline();
	void testComplexType();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XSDParserTest_INCLUDED
