//
// PrometheusTestSuite.cpp
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PrometheusTestSuite.h"
#include "CounterTest.h"
#include "GaugeTest.h"
#include "IntCounterTest.h"
#include "IntGaugeTest.h"
#include "CallbackMetricTest.h"
#include "HistogramTest.h"


CppUnit::Test* PrometheusTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PrometheusTestSuite");

	pSuite->addTest(CounterTest::suite());
	pSuite->addTest(GaugeTest::suite());
	pSuite->addTest(IntCounterTest::suite());
	pSuite->addTest(IntGaugeTest::suite());
	pSuite->addTest(CallbackMetricTest::suite());
	pSuite->addTest(HistogramTest::suite());

	return pSuite;
}
