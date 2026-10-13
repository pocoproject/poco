//
// WinDriver.cpp
//
// Windows test driver for Poco Geo.
//
// Copyright (c) 2011-2015, Günter Obiltschnig.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WinTestRunner/WinTestRunner.h"
#include "GeoTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(GeoTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
