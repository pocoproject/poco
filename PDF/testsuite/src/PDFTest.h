//
// PDFTest.h
//
// $Id: //poco/Main/PDF/testsuite/src/PDFTest.h#6 $
//
// Definition of the PDFTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDFTest_INCLUDED
#define PDFTest_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/CppUnit/TestCase.h"


class PDFTest: public CppUnit::TestCase
{
public:
	PDFTest(const std::string& name);
	~PDFTest();

	void testDocument();
	void testPage();
	void testImage();
	void testFont();
	void testEncoding();
	void testOutline();
	void testDestination();
	void testAnnotation();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // PDFTest_INCLUDED
