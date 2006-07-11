//
// TestDecorator.cpp
//
// $Id: //poco/1.1.0/CppUnit/src/TestDecorator.cpp#1 $
//


#include "CppUnit/TestDecorator.h"


CppUnit_BEGIN


TestDecorator::TestDecorator(Test* test)
{
	_test = test;
}


TestDecorator::~TestDecorator()
{
}


int TestDecorator::countTestCases()
{
	return _test->countTestCases();
}


void TestDecorator::run(TestResult* result)
{
	_test->run(result);
} 


std::string TestDecorator::toString()
{
	return _test->toString();
}


CppUnit_END
