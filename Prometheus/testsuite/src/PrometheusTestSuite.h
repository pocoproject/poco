//
// PrometheusTestSuite.h
//
// Definition of the PrometheusTestSuite class.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PrometheusTestSuite_INCLUDED
#define PrometheusTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class PrometheusTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // PrometheusTestSuite_INCLUDED
