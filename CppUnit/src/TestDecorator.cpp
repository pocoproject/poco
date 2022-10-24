//
// TestDecorator.cpp
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


int TestDecorator::countTestCases() const
{
	return _test->countTestCases();
}


void TestDecorator::run(TestResult* result, const Test::Callback& callback)
{
	_test->run(result);
}


std::string TestDecorator::toString() const
{
	return _test->toString();
}


} // namespace CppUnit
