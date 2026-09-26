//
// XSDParserTestSuite.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "XSDParserTestSuite.h"
#include "XSDParserTest.h"


CppUnit::Test* XSDParserTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDParserTestSuite");

	pSuite->addTest(XSDParserTest::suite());

	return pSuite;
}
