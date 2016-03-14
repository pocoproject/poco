//
// PDFTestSuite.h
//
// $Id: //poco/Main/PDF/testsuite/src/PDFTestSuite.h#1 $
//
// Definition of the PDFTestSuite class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDFTestSuite_INCLUDED
#define PDFTestSuite_INCLUDED


#include "Poco/CppUnit/TestSuite.h"


class PDFTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // PDFTestSuite_INCLUDED
