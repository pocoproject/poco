//
// PDFTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PDFTestSuite.h"
#include "PDFTest.h"
#include "XMLTemplateTest.h"


CppUnit::Test* PDFTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PDFTestSuite");

	pSuite->addTest(PDFTest::suite());
	pSuite->addTest(XMLTemplateTest::suite());

	return pSuite;
}
