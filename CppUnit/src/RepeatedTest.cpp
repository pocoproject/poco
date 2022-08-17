//
// RepeatedTest.cpp
//


#include "CppUnit/RepeatedTest.h"
#include "CppUnit/TestResult.h"


namespace CppUnit {

// Counts the number of test cases that will be run by this test.
int RepeatedTest::countTestCases()
{
	return TestDecorator::countTestCases() * _timesRepeat;
}


// Returns the name of the test instance.
std::string RepeatedTest::toString()
{
	return TestDecorator::toString() + " (repeated)";
}


// Runs a repeated test
void RepeatedTest::run(TestResult* result, const Test::Callback& callback)
{
	for (int n = 0; n < _timesRepeat; n++)
	{
		if (result->shouldStop())
			break;

		TestDecorator::run(result);
	}
}

} // namespace CppUnit
