//
// TestSuite.cpp
//


#include "CppUnit/TestSuite.h"
#include "CppUnit/TestResult.h"


namespace CppUnit {


// Deletes all tests in the suite.
void TestSuite::deleteContents()
{
	for (auto* _test : _tests)
		delete _test;
}


// Runs the tests and collects their result in a TestResult.
void TestSuite::run(TestResult *result, const Test::Callback& callback)
{
	for (auto* test : _tests)
	{
		if (result->shouldStop())
			break;

		if (!setup().empty())
			test->addSetup(setup());

		test->run(result, callback);
	}
}


// Counts the number of test cases that will be run by this test.
int TestSuite::countTestCases() const
{
	int count = 0;

	for (auto* _test : _tests)
		count += _test->countTestCases();

	return count;
}


} // namespace CppUnit
