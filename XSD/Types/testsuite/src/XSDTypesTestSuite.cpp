//
// XSDTypesTestSuite.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "XSDTypesTestSuite.h"
#include "XSDTypesTest.h"


CppUnit::Test* XSDTypesTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDTypesTestSuite");

	pSuite->addTest(XSDTypesTest::suite());

	return pSuite;
}
