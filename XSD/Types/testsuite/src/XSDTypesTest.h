//
// XSDTypesTest.h
//
// Definition of the XSDTypesTest class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypesTest_INCLUDED
#define XSDTypesTest_INCLUDED


#include "CppUnit/TestCase.h"


class XSDTypesTest: public CppUnit::TestCase
{
public:
	XSDTypesTest(const std::string& name);
	~XSDTypesTest();

	void testEmptySequence();
	void testSequenceIterator();
	void testSequenceIterator2();
	void testSequenceIterator3();
	void testSequenceIterator4();
	void testSequenceIterator5();
	void testSequenceIterator6();
	void testSequenceIterator7();
	void testSequenceIterator8();
	void testSequenceIterator9();
	void testSequenceIterator10();

	void testEmptyChoice();
	void testChoiceIterator();
	void testChoiceIterator2();
	void testChoiceIterator3();
	void testChoiceIterator4();
	void testChoiceIterator5();
	void testChoiceIterator6();
	void testChoiceIterator7();
	void testChoiceIterator8();
	void testChoiceIterator9();
	void testChoiceIterator10();
	void testChoiceIterator11();
	void testChoiceIterator12();
	void testChoiceIterator13();

	void testEmptyAll();
	void testAllIterator();
	void testAllIterator2();
	void testAllIterator3();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XSDTypesTest_INCLUDED
