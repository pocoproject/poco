module;

#ifdef ENABLE_CPPUNIT
#include "CppUnit/CppUnit.h"
#include "CppUnit/CppUnitException.h"
#include "CppUnit/Guards.h"
#include "CppUnit/Orthodox.h"
#include "CppUnit/RepeatedTest.h"
#include "CppUnit/Test.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestCase.h"
#include "CppUnit/TestDecorator.h"
#include "CppUnit/TestFailure.h"
#include "CppUnit/TestResult.h"
#include "CppUnit/TestRunner.h"
#include "CppUnit/TestSetup.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TextTestResult.h"
#include "CppUnit/estring.h"
#endif

export module CppUnit;

export namespace CppUnit {
	#ifdef ENABLE_CPPUNIT
	using CppUnit::CppUnitException;
	using CppUnit::Orthodox;
	using CppUnit::RepeatedTest;
	using CppUnit::Test;
	using CppUnit::TestCaller;
	using CppUnit::TestCase;
	using CppUnit::TestDecorator;
	using CppUnit::TestFailure;
	using CppUnit::TestResult;
	using CppUnit::TestRunner;
	using CppUnit::TestSetup;
	using CppUnit::TestSuite;
	using CppUnit::TextTestResult;
	using CppUnit::estring;
	#endif
}
