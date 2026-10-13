//
// GeoTestSuite.cpp
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "GeoTestSuite.h"
#include "AngleTest.h"
#include "LatLonTest.h"


CppUnit::Test* GeoTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("GeoTestSuite");

	pSuite->addTest(AngleTest::suite());
	pSuite->addTest(LatLonTest::suite());

	return pSuite;
}
