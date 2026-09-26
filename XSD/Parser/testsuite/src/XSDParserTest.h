//
// XSDParserTest.h
//
// Definition of the XSDParserTest class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
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
	void testElementWithTypeAndInlineType();
	void testRestrictionWithoutBase();
	void testTopLevelElementRef();
	void testSoapHeaderFault();
	void testSameNamespaceDocuments();
	void testFailedLoadIsRevoked();
	void testEraseSchemaRemovesLocation();
	void testAttributeWithTypeAndInlineType();
	void testRestrictionWithBaseAndInlineType();
	void testRestrictionWithTwoInlineTypes();
	void testContentAfterSchemaEndMergesNothing();
	void testEraseSchemaRemovesMergedLocations();
	void testIncludeAfterImport();
	void testIncludeTwice();
	void testImportSecondDocument();
	void testIncludeChameleon();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XSDParserTest_INCLUDED
