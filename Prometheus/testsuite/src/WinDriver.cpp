//
// WinDriver.cpp
//
// Windows test driver for Poco Prometheus.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WinTestRunner/WinTestRunner.h"
#include "PrometheusTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(PrometheusTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
