//
// PDFTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PDFTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


PDFTest::PDFTest(const std::string& name): CppUnit::TestCase(name)
{
}


PDFTest::~PDFTest()
{
}


void PDFTest::testDocument()
{
	warn("not implemented");
}


void PDFTest::testPage()
{
	warn("not implemented");
}


void PDFTest::testImage()
{
	warn("not implemented");
}


void PDFTest::testFont()
{
	warn("not implemented");
}


void PDFTest::testEncoding()
{
	warn("not implemented");
}


void PDFTest::testOutline()
{
	warn("not implemented");
}


void PDFTest::testDestination()
{
	warn("not implemented");
}


void PDFTest::testAnnotation()
{
	warn("not implemented");
}


void PDFTest::setUp()
{
}


void PDFTest::tearDown()
{
}


CppUnit::Test* PDFTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PDFTest");

	CppUnit_addTest(pSuite, PDFTest, testDocument);
	CppUnit_addTest(pSuite, PDFTest, testPage);
	CppUnit_addTest(pSuite, PDFTest, testImage);
	CppUnit_addTest(pSuite, PDFTest, testFont);
	CppUnit_addTest(pSuite, PDFTest, testEncoding);
	CppUnit_addTest(pSuite, PDFTest, testOutline);
	CppUnit_addTest(pSuite, PDFTest, testDestination);
	CppUnit_addTest(pSuite, PDFTest, testAnnotation);

	return pSuite;
}
