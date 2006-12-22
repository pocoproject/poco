//
// TestDecorator.cpp
//
// $Id: //poco/Main/CppUnit/src/TestDecorator.cpp#6 $
//


#include "CppUnit/TestDecorator.h"


namespace CppUnit {


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


} // namespace CppUnit
