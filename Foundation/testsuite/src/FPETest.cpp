//
// FPETest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FPETest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/FPEnvironment.h"


using Poco::FPE;


FPETest::FPETest(const std::string& rName): CppUnit::TestCase(rName)
{
}


FPETest::~FPETest()
{
}


#ifdef POCO_OS_FAMILY_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4723) // potential divide by 0
#endif
void FPETest::testClassify()
{
	{
		float a = 0.0f;
		float b = 0.0f;
		float nan = a/b;
		float inf = 1.0f/b;

		assertTrue (FPE::isNaN(nan));
		assertTrue (!FPE::isNaN(a));
		assertTrue (FPE::isInfinite(inf));
		assertTrue (!FPE::isInfinite(a));
	}
	{
		double a = 0;
		double b = 0;
		double nan = a/b;
		double inf = 1.0/b;

		assertTrue (FPE::isNaN(nan));
		assertTrue (!FPE::isNaN(a));
		assertTrue (FPE::isInfinite(inf));
		assertTrue (!FPE::isInfinite(a));
	}
}
#ifdef POCO_OS_FAMILY_WINDOWS
#pragma warning(pop)
#endif


#if defined(__HP_aCC)
	#pragma OPTIMIZE OFF
#elif defined(_MSC_VER)
	#pragma optimize("", off)
#elif defined(__APPLE__) && defined(POCO_COMPILER_GCC)
	#pragma GCC optimization_level 0
#endif


double mult(double a, double b)
{
	return a*b;
}


double div(double a, double b)
{
	return a/b;
}


void FPETest::testFlags()
{
	FPE::clearFlags();

	// some compilers are intelligent enough to optimize the calculations below away.
	// unfortunately this leads to a failing test, so we have to trick out the
	// compiler's optimizer a little bit by doing something with the results.
	volatile double a = 10;
	volatile double b = 0;
	volatile double c = div(a, b);

#if !defined(POCO_NO_FPENVIRONMENT)	
	assertTrue (FPE::isFlag(FPE::FP_DIVIDE_BY_ZERO));
#endif
	assertTrue (FPE::isInfinite(c));

	FPE::clearFlags();
	a = 1.23456789e210;
	b = 9.87654321e210;
	c = mult(a, b);
#if !defined(POCO_NO_FPENVIRONMENT)	
	assertTrue (FPE::isFlag(FPE::FP_OVERFLOW));
#endif
	assertEqualDelta(c, c, 0);

	FPE::clearFlags();
	a = 1.23456789e-99;
	b = 9.87654321e210;
	c = div(a, b);	
#if !defined(POCO_NO_FPENVIRONMENT)	
	assertTrue (FPE::isFlag(FPE::FP_UNDERFLOW));
#endif
	assertEqualDelta(c, c, 0);
}


#if defined(__HP_aCC)
	#pragma OPTIMIZE ON
#elif defined(_MSC_VER)
	#pragma optimize("", on)
#elif defined(__APPLE__) && defined(POCO_COMPILER_GCC)
	#pragma GCC optimization_level reset
#endif


void FPETest::testRound()
{
#if !defined(__osf__) && !defined(__VMS) && !defined(POCO_NO_FPENVIRONMENT)

	FPE::setRoundingMode(FPE::FP_ROUND_TONEAREST);			
	assertTrue (FPE::getRoundingMode() == FPE::FP_ROUND_TONEAREST);
	{
		FPE env(FPE::FP_ROUND_TOWARDZERO);
		assertTrue (FPE::getRoundingMode() == FPE::FP_ROUND_TOWARDZERO);
	}
	assertTrue (FPE::getRoundingMode() == FPE::FP_ROUND_TONEAREST);	
#endif
}


void FPETest::setUp()
{
}


void FPETest::tearDown()
{
}


CppUnit::Test* FPETest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FPETest");

	CppUnit_addTest(pSuite, FPETest, testClassify);
	CppUnit_addTest(pSuite, FPETest, testFlags);
	CppUnit_addTest(pSuite, FPETest, testRound);

	return pSuite;
}
